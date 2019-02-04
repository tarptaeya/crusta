#include "common-defs.h"
#include "qmlwebpage.h"

#include <QQmlEngine>

QmlWebPage::QmlWebPage(QWebEnginePage *page, QObject *parent)
    : QObject (parent)
    , m_page(page)
{
    qmlEngine(this)->setObjectOwnership(this, QQmlEngine::JavaScriptOwnership);
}

void QmlWebPage::load(const QString &url)
{
    if (!m_page) {
        return;
    }

    m_page->load(QUrl(url));
}

QString QmlWebPage::title() const
{
    if (!m_page) {
        return QSL("");
    }

    return m_page->title();
}

QString QmlWebPage::url() const
{
    if (!m_page) {
        return QSL("");
    }

    return QString::fromUtf8(m_page->url().toEncoded());
}
