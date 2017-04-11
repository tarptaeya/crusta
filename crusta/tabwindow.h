#ifndef TABWINDOW_H
#define TABWINDOW_H

#include "addresslineedit.h"
#include "searchlineedit.h"
#include "webview.h"

#include <QObject>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>



class TabWindow:public QObject{
    void viewHome();
    void updateAddrBar();
public:
    QVBoxLayout* vbox=new QVBoxLayout();
    QWidget* tab=new QWidget();
    QPushButton* back_btn=new QPushButton();
    QPushButton* fwd_btn=new QPushButton();
    QPushButton* load_btn=new QPushButton();
    QPushButton* home_btn=new QPushButton();
    QPushButton* bookmark_btn=new QPushButton();
    QPushButton* tool_btn=new QPushButton();
    QPushButton* options_btn=new QPushButton();
    AddressLineEdit* addr_bar=new AddressLineEdit();
    SearchLineEdit* search_bar=new SearchLineEdit();
    WebView* view=new WebView();
    void createControls();
    QWidget* returnTab();
    TabWindow* returnThis();
    void setWebView(WebView*);
};



#endif // TABWINDOW_H
