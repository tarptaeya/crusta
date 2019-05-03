#pragma once

#include <QWebEngineHistory>
#include <QWebEngineView>

class WebPage;

class WebView : public QWebEngineView
{
    Q_OBJECT
public:
    explicit WebView(QWidget *parent = nullptr);

Q_SIGNALS:
    void historyChanged(QWebEngineHistory *history);

private:
    WebPage *m_webPage = nullptr;
};
