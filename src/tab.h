#ifndef TAB_H
#define TAB_H

#include <QWidget>

class ToolBar;
class WebView;

class Tab : public QWidget
{
    ToolBar *m_toolBar = nullptr;
    WebView *m_webView = nullptr;
public:
    Tab(QWidget *parent = nullptr);
};

#endif // TAB_H
