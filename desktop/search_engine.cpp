#include "browser.h"
#include "search_engine.h"

#include <QDialog>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>

SearchModel::SearchModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    m_engines << SearchEngine { QStringLiteral("Ecosia"), QStringLiteral("The search engine that plants trees"), QStringLiteral("https://www.ecosia.org/search?tt=crusta&q={searchTerms}"), QStringLiteral("https://ac.ecosia.org/autocomplete?tt=crusta&q={searchTerms}") };
    m_engines << SearchEngine { QStringLiteral("Google"), QStringLiteral("Google Search"), QStringLiteral("https://www.google.com/search?q={searchTerms}"), QStringLiteral("https://www.google.com/complete/search?q={searchTerms}") };
    m_engines << SearchEngine { QStringLiteral("DuckDuckGo"), QStringLiteral("Search DuckDuckGo"), QStringLiteral("https://duckduckgo.com/?q={searchTerms}"), QStringLiteral("https://duckduckgo.com/ac/?q={searchTerms}&amp;type=list") };

    qRegisterMetaType<SearchEngine>();
    qRegisterMetaTypeStreamOperators<SearchEngine>();

    QSettings settings;
    const QList<QVariant> engines = settings.value(QStringLiteral("search_engines")).toList();
    for (const QVariant &var : engines) {
        m_engines << var.value<SearchEngine>();
    }

    m_default_index = settings.value(QStringLiteral("default_search_engine"), 0).toInt();
}

int SearchModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_engines.count();
}

int SearchModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : 2;
}

QVariant SearchModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 && index.row() >= m_engines.count())
        return QVariant();

    SearchEngine engine = m_engines.at(index.row());

    switch (role) {
    case Qt::DisplayRole:
        switch (index.column()) {
        case 0: return engine.name;
        case 1: return engine.description;
        default: break;
        }
    case Qt::FontRole:
        if (index.row() == m_default_index) {
            QFont font;
            font.setBold(true);
            return font;
        }
    }

    return QVariant();
}

QVariant SearchModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0: return QStringLiteral("Name");
        case 1: return QStringLiteral("Description");
        }
    }

    return QAbstractTableModel::headerData(section, orientation, role);
}

SearchEngine SearchModel::default_engine() const
{
    return m_engines.at(m_default_index);
}

void SearchModel::set_default(int index)
{
    m_default_index = index;
    QSettings settings;
    settings.setValue(QStringLiteral("default_search_engine"), index);
}

void SearchModel::add_engine(SearchEngine engine)
{
    int row = m_engines.count();
    beginInsertRows(QModelIndex(), row, row);
    m_engines.append(engine);
    endInsertRows();

    QSettings settings;
    QList<QVariant> engines = settings.value(QStringLiteral("search_engines")).toList();
    QVariant var;
    var.setValue(engine);
    engines << var;
    settings.setValue(QStringLiteral("search_engines"), engines);
}

void SearchModel::remove_engine(QModelIndex index)
{
    int row = index.row();

    SearchEngine engine = m_engines.at(row);

    beginRemoveRows(QModelIndex(), row, row);
    m_engines.remove(row);
    endRemoveRows();

    QSettings settings;
    QList<QVariant> engines = settings.value(QStringLiteral("searchengines")).toList();
    QMutableListIterator<QVariant> it(engines);
    while (it.hasNext()) {
        QVariant var = it.next();
        SearchEngine e = var.value<SearchEngine>();
        if (e == engine) {
            it.remove();
            break;
        }
    }

    settings.setValue(QStringLiteral("searchengines"), engines);
}

SearchWidget::SearchWidget(QWidget *parent)
    : QWidget(parent)
{
    m_tree_view = new QTreeView;
    m_tree_view->setModel(browser->search_model());

    QPushButton *add_button = new QPushButton(QStringLiteral("Add New"));
    QPushButton *remove_button = new QPushButton(QStringLiteral("Remove"));
    QPushButton *make_default_button = new QPushButton(QStringLiteral("Make Default"));

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(add_button);
    vbox->addWidget(remove_button);
    vbox->addWidget(make_default_button);
    vbox->addWidget(new QWidget);

    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->setContentsMargins(0, 0, 0, 0);
    setLayout(hbox);
    hbox->addWidget(m_tree_view);
    hbox->addLayout(vbox);

    connect(add_button, &QPushButton::clicked, [this] {
        QDialog *sd = new QDialog(this);
        sd->setAttribute(Qt::WA_DeleteOnClose);
        sd->setModal(true);

        QVBoxLayout *vbox = new QVBoxLayout;
        sd->setLayout(vbox);

        QLineEdit *name = new QLineEdit;
        name->setPlaceholderText(QStringLiteral("Name"));
        vbox->addWidget(name);

        QLineEdit *description = new QLineEdit;
        description->setPlaceholderText(QStringLiteral("Description"));
        vbox->addWidget(description);

        QLineEdit *query_url = new QLineEdit;
        query_url->setPlaceholderText(QStringLiteral("Query url in opensearch format"));
        vbox->addWidget(query_url);

        QLineEdit *suggestion_url = new QLineEdit;
        suggestion_url->setPlaceholderText(QStringLiteral("Autocomplete url in opensearch format"));
        vbox->addWidget(suggestion_url);

        QPushButton *save = new QPushButton(QStringLiteral("Add"));
        QPushButton *cancel = new QPushButton(QStringLiteral("Cancel"));

        QHBoxLayout *hbox = new QHBoxLayout;
        hbox->addWidget(new QWidget, 1);
        hbox->addWidget(save);
        hbox->addWidget(cancel);

        vbox->addLayout(hbox);

        sd->open();

        connect(save, &QPushButton::clicked, [=] {
            SearchEngine engine = { name->text(), description->text(), query_url->text(), suggestion_url->text() };
            browser->search_model()->add_engine(engine);
            sd->close();
        });

        connect(cancel, &QPushButton::clicked, sd, &QDialog::close);
    });

    connect(remove_button, &QPushButton::clicked, [this] {
        QModelIndex index = m_tree_view->currentIndex();
        if (!index.isValid() || index.row() < 3)
            return ;

        if (QMessageBox::Yes != QMessageBox::question(this, QStringLiteral("Confirm delete?"), QStringLiteral("Confirm remove this search engine?")))
            return;

        browser->search_model()->remove_engine(index);
    });

    connect(make_default_button, &QPushButton::clicked, [this] {
        QModelIndex index = m_tree_view->currentIndex();
        if (!index.isValid())
            return ;

        browser->search_model()->set_default(index.row());
    });
}
