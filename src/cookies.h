#pragma once

#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QNetworkCookie>
#include <QObject>
#include <QPushButton>
#include <QTreeWidget>
#include <QWidget>

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
    QMap<QTreeWidgetItem *, QNetworkCookie> m_items;
    QMap<QNetworkCookie, QTreeWidgetItem *> m_cookies;

    void setupInfoWidget();
    void showInfo(const QNetworkCookie &cookie);
    void clearInfo();
};

inline bool operator<(const QNetworkCookie &a, const QNetworkCookie b)
{
    return QString(a.domain() + a.name()) < QString(b.domain() + b.name());
}
