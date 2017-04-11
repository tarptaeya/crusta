#ifndef POPUP_H
#define POPUP_H

#include <QLineEdit>
#include <QVBoxLayout>
#include <QWebEngineView>
#include <QWidget>
#include <QIcon>
#include <QPushButton>




class PopUp:public QObject{
public:
    QWidget* widget=new QWidget();
    QPushButton* btn=new QPushButton();
    QLineEdit* lineedit=new QLineEdit();
    QVBoxLayout* vbox=new QVBoxLayout();
    QWebEngineView* webview =new QWebEngineView();
    void createView();
    void updateURL();
    void updateFavicon(QIcon icon);
    void updateTitle();
    void close();
    void setWebView(QWebEngineView*);
};




#endif // POPUP_H
