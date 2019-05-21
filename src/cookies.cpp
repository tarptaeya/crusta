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

QList<QNetworkCookie> Cookies::s_cookies = QList<QNetworkCookie>();

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
        s_cookies.append(cookie);
        addCookie(cookie);
    });

    connect(Browser::instance()->profile()->cookieStore(), &QWebEngineCookieStore::cookieRemoved, this, [this] (const QNetworkCookie &cookie) {
        s_cookies.removeAll(cookie);
        removeCookie(cookie);
    });

    connect(m_treeWidget, &QTreeWidget::itemClicked, this, [this] (QTreeWidgetItem *item, int column) {
        Q_UNUSED(column);

        if (!item->parent()) {
            clearInfo();
            return ;
        }

        CookieTreeItem *cookieItem = dynamic_cast<CookieTreeItem *>(item);
        if (!cookieItem) {
            std::cerr << "[!] " << __FUNCTION__ << " cannot cast QTreeWidgetItem * to CookieTreeItem *: please report it!" << std::endl;
            return;
        }

        const QNetworkCookie cookie = QNetworkCookie(cookieItem->cookie);
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

void Cookies::addCookie(const QNetworkCookie &cookie)
{
    const QString &domain = cookie.domain();

    QTreeWidgetItem *parent = m_domains.value(domain, nullptr);
    if (!parent) {
        parent = new QTreeWidgetItem;
        parent->setText(0, domain);
        m_treeWidget->addTopLevelItem(parent);
        m_domains.insert(domain, parent);
    }

    CookieTreeItem *item = new CookieTreeItem;
    item->setText(0, cookie.name());
    item->cookie = cookie;
    parent->addChild(item);
    if (parent->isHidden()) {
        parent->setHidden(false);
    }
}

void Cookies::removeCookie(const QNetworkCookie &cookie)
{
    QList<QTreeWidgetItem *> domainItems = m_treeWidget->findItems(cookie.domain(), Qt::MatchExactly);

    for (QTreeWidgetItem *parent : domainItems) {
        for (int i = 0; i < parent->childCount(); i++) {
            CookieTreeItem *cookieItem = dynamic_cast<CookieTreeItem *>(parent->child(i));
            if (!cookieItem) {
                std::cerr << "[!] " << __FUNCTION__ << " cannot cast QTreeWidgetItem * to CookieTreeItem *: please report it!" << std::endl;
                continue;
            }

            if (cookieItem->cookie.hasSameIdentifier(cookie)) {
                parent->removeChild(cookieItem);
                if (!parent->childCount()) {
                    parent->setHidden(true);
                }
                delete cookieItem;
                return;
            }
        }
    }
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

        CookieTreeItem *cookieItem = dynamic_cast<CookieTreeItem *>(item);
        if (!cookieItem) {
            std::cerr << "[!] " << __FUNCTION__ << " cannot cast QTreeWidgetItem * to CookieTreeItem *: please report it!" << std::endl;
            return;
        }

        QNetworkCookie cookie = QNetworkCookie(cookieItem->cookie);
        Browser::instance()->profile()->cookieStore()->deleteCookie(cookie);
        clearInfo();
    });

    for (const QNetworkCookie &cookie : s_cookies) {
        addCookie(cookie);
    }
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
