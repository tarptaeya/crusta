#pragma once

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QNetworkCookie>
#include <QObject>
#include <QPushButton>
#include <QTreeWidget>
#include <QWidget>

class CookieTreeItem : public QTreeWidgetItem
{
public:
    QNetworkCookie cookie;
};

class Cookies : public QObject
{
public:
    explicit Cookies(QObject *parent = nullptr);

    QWidget *cookiesWidget();
private:
    QWidget *m_widget = nullptr;
    QTreeWidget *m_treeWidget = nullptr;
    QLineEdit *m_searchBox = nullptr;
    QWidget *m_infoWidget = nullptr;
    QLineEdit *m_name = nullptr;
    QLineEdit *m_value = nullptr;
    QLineEdit *m_expiration = nullptr;
    QCheckBox *m_http = nullptr;
    QCheckBox *m_session = nullptr;
    QCheckBox *m_secure = nullptr;
    QPushButton *m_deleteButton = nullptr;

    QMap<QString, QTreeWidgetItem *> m_domains;

    static QList<QNetworkCookie> s_cookies;

    void addCookie(const QNetworkCookie &cookie);
    void removeCookie(const QNetworkCookie &cookie);

    void setupInfoWidget();
    void showInfo(const QNetworkCookie &cookie);
    void clearInfo();
};
