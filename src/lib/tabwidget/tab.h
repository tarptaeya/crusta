#ifndef CR_TAB_H
#define CR_TAB_H

#include "common-defs.h"

#include <QVBoxLayout>
#include <QIcon>
#include <QWidget>

class TabWidget;
class ToolBar;
class WebView;

class Tab : public QWidget
{
public:
    explicit Tab(const QString &address = QSL(""), QWidget *parent = nullptr);

    void setTabWidget(TabWidget *tabWidget);
    int index();

    QString title() const;
    QIcon icon() const;
    WebView *webView();

private:
    QVBoxLayout *m_vboxLayout = nullptr;

    TabWidget *m_tabWidget = nullptr;
    ToolBar *m_toolBar = nullptr;
    WebView *m_webView = nullptr;
};

#endif
