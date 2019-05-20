#include "qmlwindows.h"

#include "src/browser.h"
#include "src/tab.h"
#include "src/webview.h"

#include <QQmlEngine>

QmlWindows::QmlWindows(QObject *parent)
    : QObject (parent)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}

QmlWindow *QmlWindows::create(QJsonObject param)
{
    Tab *tab = Browser::instance()->createMainWindow();
    QmlWindow *window = new QmlWindow;

    QString url = param.value(QStringLiteral("url")).toString();
    if (!url.isEmpty()) {
        tab->webView()->load(url);
        window->setUrl(url);
    }

    return window;
}

QmlWindow::QmlWindow(QObject *parent)
    : QObject (parent)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}

QString QmlWindow::url() const
{
    return m_url;
}

void QmlWindow::setUrl(const QString &url)
{
    m_url = url;
}
