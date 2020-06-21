#ifndef TAB_H
#define TAB_H

#include <QWidget>

class WebView;

class Tab : public QWidget
{
    WebView *m_webView = nullptr;
public:
    Tab(QWidget *parent = nullptr);
};

#endif // TAB_H
