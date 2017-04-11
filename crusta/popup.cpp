#include "popup.h"

#include <QLineEdit>
#include <QVBoxLayout>
#include <QWebEngineView>
#include <QWidget>
#include <QIcon>
#include <QPixmap>
#include <QPushButton>



void PopUp::setWebView(QWebEngineView * view){
    webview=view;
}

void PopUp::createView(){
    widget->setLayout(vbox);
    btn->setParent(lineedit);
    btn->setFlat(true);
    btn->setFixedWidth(20);
    vbox->setContentsMargins(0,0,0,0);
    vbox->addWidget(lineedit);
    vbox->addWidget(webview);
    connect(webview,&QWebEngineView::urlChanged,this,&PopUp::updateURL);
    connect(webview,&QWebEngineView::iconChanged,this,&PopUp::updateFavicon);
    connect(webview,&QWebEngineView::titleChanged,this,&PopUp::updateTitle);
}

void PopUp::updateURL(){
    lineedit->setTextMargins(20,0,0,0);
    lineedit->setText(webview->url().toString());
    lineedit->setCursorPosition(0);
    lineedit->setReadOnly(true);
}

void PopUp::updateFavicon(QIcon icon){
    btn->setIcon(icon);
}

void PopUp::updateTitle(){
    widget->setWindowTitle(webview->title());
}

void PopUp::close(){
    widget->deleteLater();
}
