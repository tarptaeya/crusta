#include "browser.h"
#include "cookies.h"

#include <QDateTime>
#include <QGridLayout>
#include <QHeaderView>
#include <QString>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QWebEngineCookieStore>
#include <QWebEngineProfile>

#include <iostream>

Cookies::Cookies(QObject *parent)
    : QObject (parent)
{
    m_widget = new QWidget;
    m_treeWidget = new QTreeWidget;
    m_searchBox = new QLineEdit;
    m_infoWidget = new QWidget;

    m_treeWidget->header()->hide();
    m_searchBox->setPlaceholderText(QStringLiteral("Search cookies..."));

    setupInfoWidget();

    QVBoxLayout *vboxLayout = new QVBoxLayout;
    vboxLayout->setContentsMargins(0, 0, 0, 0);
    m_widget->setLayout(vboxLayout);

    vboxLayout->addWidget(m_searchBox);
    vboxLayout->addWidget(m_treeWidget);
    vboxLayout->addWidget(m_infoWidget);

    connect(Browser::instance()->profile()->cookieStore(), &QWebEngineCookieStore::cookieAdded, this, [this] (const QNetworkCookie &cookie) {
        const QString &domain = cookie.domain();

        QTreeWidgetItem *parent = m_domains.value(domain, nullptr);
        if (!parent) {
            parent = new QTreeWidgetItem;
            parent->setText(0, domain);
            m_treeWidget->addTopLevelItem(parent);
            m_domains.insert(domain, parent);
        }

        QTreeWidgetItem *item = new QTreeWidgetItem;
        item->setText(0, cookie.name());
        m_items.insert(item, cookie);
        m_cookies.insert(cookie, item);
        parent->addChild(item);
    });

    connect(Browser::instance()->profile()->cookieStore(), &QWebEngineCookieStore::cookieRemoved, this, [this] (const QNetworkCookie &cookie) {
        QTreeWidgetItem *item = m_cookies.value(cookie, nullptr);
        m_cookies.remove(cookie);
        if (!item) {
            std::cerr << "[!] treeitem associated with cookie is null, please report it!" << std::endl;
        }
        m_items.remove(item);
        delete item;
    });

    connect(m_treeWidget, &QTreeWidget::itemClicked, this, [this] (QTreeWidgetItem *item, int column) {
        Q_UNUSED(column);

        if (!item->parent()) {
            clearInfo();
            return ;
        }

        const QNetworkCookie cookie = m_items.value(item);
        showInfo(cookie);
    });

    connect(m_searchBox, &QLineEdit::returnPressed, this, [this] {
        const QString text = m_searchBox->text();
        QList<QTreeWidgetItem *> allItem = m_treeWidget->findItems(QStringLiteral(""), Qt::MatchContains);
        for (QTreeWidgetItem *item : allItem) {
            item->setHidden(true);
        }

        QList<QTreeWidgetItem *> items = m_treeWidget->findItems(text, Qt::MatchContains);
        for (QTreeWidgetItem *item : items) {
            item->setHidden(false);
        }

    });
}

QWidget *Cookies::cookiesWidget()
{
    return m_widget;
}

void Cookies::setupInfoWidget()
{
    m_name = new QLineEdit;
    m_value = new QLineEdit;
    m_expiration = new QLineEdit;
    m_http = new QCheckBox;
    m_session = new QCheckBox;
    m_secure = new QCheckBox;
    m_deleteButton = new QPushButton;

    m_name->setReadOnly(true);
    m_value->setReadOnly(true);
    m_expiration->setReadOnly(true);
    m_http->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_http->setFocusPolicy(Qt::NoFocus);
    m_session->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_session->setFocusPolicy(Qt::NoFocus);
    m_secure->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_secure->setFocusPolicy(Qt::NoFocus);
    m_deleteButton->setText(QStringLiteral("Delete"));

    QGridLayout *grid = new QGridLayout;
    m_infoWidget->setLayout(grid);

    grid->setContentsMargins(0, 0, 0, 0);
    grid->addWidget(new QLabel(QStringLiteral("<b>Name</b>")), 1, 1);
    grid->addWidget(m_name, 1, 2);
    grid->addWidget(new QLabel(QStringLiteral("<b>Value</b>")), 2, 1);
    grid->addWidget(m_value, 2, 2);
    grid->addWidget(new QLabel(QStringLiteral("<b>Expiration Date</b>")), 3, 1);
    grid->addWidget(m_expiration, 3, 2);
    grid->addWidget(new QLabel(QStringLiteral("<b>HTTP Only</b>")), 4, 1);
    grid->addWidget(m_http, 4, 2);
    grid->addWidget(new QLabel(QStringLiteral("<b>Session</b>")), 5, 1);
    grid->addWidget(m_session, 5, 2);
    grid->addWidget(new QLabel(QStringLiteral("<b>Secure</b>")), 6, 1);
    grid->addWidget(m_secure, 6, 2);
    grid->addWidget(m_deleteButton, 7, 2);

    connect(m_deleteButton, &QPushButton::clicked, this, [this] {
        QTreeWidgetItem *item = m_treeWidget->currentItem();
        if (!item) {
            return ;
        }

        QNetworkCookie cookie = m_items.value(item);
        Browser::instance()->profile()->cookieStore()->deleteCookie(cookie);
        clearInfo();
    });
}

void Cookies::showInfo(const QNetworkCookie &cookie)
{
    QString name = cookie.name();
    QString value = cookie.value();
    QString expDate = cookie.expirationDate().toString();

    m_name->setText(name);
    m_value->setText(value);
    m_expiration->setText(expDate);
    m_http->setChecked(cookie.isHttpOnly());
    m_session->setChecked(cookie.isSessionCookie());
    m_secure->setChecked(cookie.isSecure());

    m_name->setCursorPosition(0);
    m_value->setCursorPosition(0);
    m_expiration->setCursorPosition(0);

    m_deleteButton->setEnabled(true);
}

void Cookies::clearInfo()
{
    m_name->clear();
    m_value->clear();
    m_expiration->clear();
    m_http->setChecked(false);
    m_session->setChecked(false);
    m_secure->setChecked(false);

    m_deleteButton->setDisabled(true);
}
