#include "preferences.h"
#include "utils.h"

#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QSettings>
#include <QSpinBox>
#include <QTabWidget>
#include <QTextCodec>
#include <QVBoxLayout>
#include <QWebEngineSettings>

Preferences::Preferences(QWidget *parent)
    : QWidget (parent)
{
    m_tabWidget = new QTabWidget;

    QPushButton *doneButton = new QPushButton(QStringLiteral("Done"));

    QVBoxLayout *vboxLayout = new QVBoxLayout;
    setLayout(vboxLayout);

    vboxLayout->addWidget(m_tabWidget);

    QHBoxLayout *hboxLayout = new QHBoxLayout;
    hboxLayout->addWidget(new QWidget, 1);
    hboxLayout->addWidget(doneButton, 0);
    vboxLayout->addLayout(hboxLayout);

    m_tabWidget->setDocumentMode(true);
    m_tabWidget->tabBar()->setExpanding(true);
    doneButton->setObjectName(QStringLiteral("default"));

    installWidget(QStringLiteral("Appearance"), createAppearanceTab());
    installWidget(QStringLiteral("Browsing"), createBrowsingTab());
    installWidget(QStringLiteral("Search Engine"), createSearchEngineTab());
    installWidget(QStringLiteral("Web Engine"), createWebEngineTab());
    installWidget(QStringLiteral("Privacy"), createPrivacyTab());

    setAttribute(Qt::WA_DeleteOnClose);
#ifdef __APPLE__
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
#else
    setWindowTitle(QStringLiteral("Crusta:Preferences"));
    setWindowFlags(Qt::WindowStaysOnTopHint);
#endif

    Utils::setMovableByBackground(winId());

    setMinimumWidth(100 * m_tabWidget->count());
    connect(doneButton, &QPushButton::clicked, this, &QWidget::close);
}

void Preferences::installWidget(const QString &name, QWidget *widget)
{
    m_tabWidget->addTab(widget, name);
}

QWidget *Preferences::createAppearanceTab()
{
    QWidget *widget = new QWidget;
    QVBoxLayout *vboxLayout = new QVBoxLayout;
    widget->setLayout(vboxLayout);

    QGroupBox *themeBox = new QGroupBox(QStringLiteral("Theme"));
    {
        QVBoxLayout *vboxLayout = new QVBoxLayout;
        themeBox->setLayout(vboxLayout);

        QComboBox *themeName = new QComboBox;

        QGridLayout *grid = new QGridLayout;
        grid->addWidget(new QLabel(QStringLiteral("Theme name")), 1, 1);
        grid->addWidget(themeName, 1, 2);

        vboxLayout->addLayout(grid);
    }

    QGroupBox *generalBox = new QGroupBox(QStringLiteral("General"));
    {
        QGridLayout *grid = new QGridLayout;
        generalBox->setLayout(grid);

        QComboBox *tabsPosition = new QComboBox;
        tabsPosition->addItems(QStringList() << QStringLiteral("Top") << QStringLiteral("Bottom"));

        QComboBox *toolbarPosition = new QComboBox;
        toolbarPosition->addItems(QStringList() << QStringLiteral("Top") << QStringLiteral("Bottom"));

        QCheckBox *statusBarVisibility = new QCheckBox(QStringLiteral("visible"));

        grid->addWidget(new QLabel(QStringLiteral("Tabs position")), 1, 1);
        grid->addWidget(tabsPosition, 1, 2);
        grid->addWidget(new QLabel(QStringLiteral("Tool bar position")), 2, 1);
        grid->addWidget(toolbarPosition, 2, 2);
        grid->addWidget(new QLabel(QStringLiteral("Status bar")), 3, 1);
        grid->addWidget(statusBarVisibility, 3, 2);
    }

    vboxLayout->addWidget(themeBox, 0);
    vboxLayout->addWidget(generalBox, 0);
    vboxLayout->addWidget(new QWidget, 1);

    return widget;
}

QWidget *Preferences::createBrowsingTab()
{
    QWidget *widget = new QWidget;
    QVBoxLayout *vboxLayout = new QVBoxLayout;
    widget->setLayout(vboxLayout);

    QGroupBox *generalBox = new QGroupBox(QStringLiteral("General"));
    {
        QGridLayout *grid = new QGridLayout;
        generalBox->setLayout(grid);

        QLineEdit *homePage = new QLineEdit;
        QLineEdit *startPage = new QLineEdit;
        QLineEdit *newTabPage = new QLineEdit;

        grid->addWidget(new QLabel(QStringLiteral("Home page")), 1, 1);
        grid->addWidget(homePage, 1, 2);
        grid->addWidget(new QLabel(QStringLiteral("Start page")), 2, 1);
        grid->addWidget(startPage, 2, 2);
        grid->addWidget(new QLabel(QStringLiteral("New tab page")), 3, 1);
        grid->addWidget(newTabPage, 3, 2);

        QSettings settings;
        QString homePageUrl = settings.value(QStringLiteral("browsing/homepage"), QStringLiteral("crusta:startpage")).toString();
        QString startPageUrl = settings.value(QStringLiteral("browsing/startpage"), QStringLiteral("crusta:startpage")).toString();
        QString newTabPageUrl = settings.value(QStringLiteral("browsing/newtabpage"), QStringLiteral("crusta:startpage")).toString();

        homePage->setText(homePageUrl);
        startPage->setText(startPageUrl);
        newTabPage->setText(newTabPageUrl);

        connect(this, &Preferences::browsingSaveRequested, [homePage, startPage, newTabPage] {
            QSettings settings;
            settings.setValue(QStringLiteral("browsing/homepage"), homePage->text());
            settings.setValue(QStringLiteral("browsing/startpage"), startPage->text());
            settings.setValue(QStringLiteral("browsing/newtabpage"), newTabPage->text());
        });

        connect(this, &Preferences::browsingRestoreRequested, [homePage, startPage, newTabPage] {
            QSettings settings;
            settings.setValue(QStringLiteral("browsing/homepage"), QStringLiteral("crusta:startpage"));
            settings.setValue(QStringLiteral("browsing/startpage"), QStringLiteral("crusta:startpage"));
            settings.setValue(QStringLiteral("browsing/newtabpage"), QStringLiteral("crusta:startpage"));

            homePage->setText(QStringLiteral("crusta:startpage"));
            startPage->setText(QStringLiteral("crusta:startpage"));
            newTabPage->setText(QStringLiteral("crusta:startpage"));
        });
    }

    QGroupBox *fontGroup = new QGroupBox(QStringLiteral("Font"));
    {
        QGridLayout *grid = new QGridLayout;
        fontGroup->setLayout(grid);

        QSpinBox *minFont = new QSpinBox;
        QSpinBox *minLogicalFont = new QSpinBox;
        QSpinBox *defaultFont = new QSpinBox;
        QSpinBox *defaultFixedFont = new QSpinBox;

        grid->addWidget(new QLabel(QStringLiteral("Minimum font size")), 1, 1);
        grid->addWidget(minFont, 1, 2);
        grid->addWidget(new QLabel(QStringLiteral("Minimum logical font size")), 2, 1);
        grid->addWidget(minLogicalFont, 2, 2);
        grid->addWidget(new QLabel(QStringLiteral("Default font size")), 3, 1);
        grid->addWidget(defaultFont, 3, 2);
        grid->addWidget(new QLabel(QStringLiteral("Default fixed font size")), 4, 1);
        grid->addWidget(defaultFixedFont, 4, 2);
    }

    vboxLayout->addWidget(generalBox, 0);
    vboxLayout->addWidget(fontGroup, 0);
    vboxLayout->addWidget(new QWidget, 1);
    QHBoxLayout *hboxLayout = new QHBoxLayout;
    QPushButton *restoreButton = new QPushButton(QStringLiteral("Restore to defaults"));
    QPushButton *saveButton = new QPushButton(QStringLiteral("Save"));
    hboxLayout->addWidget(new QWidget, 1);
    hboxLayout->addWidget(restoreButton, 0);
    hboxLayout->addWidget(saveButton, 0);
    vboxLayout->addLayout(hboxLayout);

    connect(saveButton, &QPushButton::clicked, this, [this] { emit browsingSaveRequested(); });
    connect(restoreButton, &QPushButton::clicked, this, [this] { emit browsingRestoreRequested(); });

    return widget;
}

QWidget *Preferences::createSearchEngineTab()
{
    QWidget *widget = new QWidget;
    QVBoxLayout *vboxLayout = new QVBoxLayout;
    widget->setLayout(vboxLayout);

    QListWidget *list = new QListWidget;
    QList<Engine> knownEngines = SearchEngine::knownEngines();
    for (const Engine &engine : knownEngines) {
        QWidget *widget = new QWidget;

        QWidget *iconLabel = Utils::webIconLabel(QUrl(engine.url).resolved(engine.favicon));
        QLabel *nameLabel = new QLabel(QStringLiteral("<b>%1</b>").arg(engine.name));
        QLabel *descriptionLabel = new QLabel(engine.description);

        descriptionLabel->setWordWrap(true);

        QGridLayout *grid = new QGridLayout;
        grid->addWidget(iconLabel, 1, 1);
        grid->addWidget(nameLabel, 1, 2);
        grid->addWidget(descriptionLabel, 2, 2);

        widget->setLayout(grid);

        EngineListWidgetItem *item = new EngineListWidgetItem;
        item->engine = engine;
        item->setSizeHint(widget->sizeHint());
        item->setSelected(true);

        list->addItem(item);
        list->setItemWidget(item, widget);
    }

    QLabel *currentDefault = new QLabel;
    QPushButton *makeDefault = new QPushButton;

    vboxLayout->addWidget(list);
    QHBoxLayout *hboxLayout = new QHBoxLayout;
    hboxLayout->addWidget(currentDefault, 1);
    hboxLayout->addWidget(makeDefault, 0);
    vboxLayout->addLayout(hboxLayout);

    currentDefault->setText(QStringLiteral("%1 is set as default").arg(SearchEngine::defaultEngine().name));
    makeDefault->hide();

    connect(list, &QListWidget::currentItemChanged, [makeDefault] (QListWidgetItem *item) {
        EngineListWidgetItem *engineItem = dynamic_cast<EngineListWidgetItem *>(item);
        if (!engineItem) {
            return ;
        }

        makeDefault->show();
        makeDefault->setText(QStringLiteral("Use %1 as default").arg(engineItem->engine.name));
    });

    connect(makeDefault, &QPushButton::clicked, [list, currentDefault] {
        QListWidgetItem *item = list->currentItem();
        EngineListWidgetItem *engineItem = dynamic_cast<EngineListWidgetItem *>(item);
        if (!engineItem) {
            return ;
        }

        SearchEngine::makeDefault(engineItem->engine);
        currentDefault->setText(QStringLiteral("%1 is set as default").arg(engineItem->engine.name));
    });
    return widget;
}

QWidget *Preferences::createWebEngineTab()
{
    QWidget *widget = new QWidget;
    QVBoxLayout *vboxLayout = new QVBoxLayout;
    widget->setLayout(vboxLayout);
    return widget;
}

QWidget *Preferences::createPrivacyTab()
{
    QWidget *widget = new QWidget;
    QVBoxLayout *vboxLayout = new QVBoxLayout;
    widget->setLayout(vboxLayout);
    return widget;
}
