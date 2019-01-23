#ifndef CR_WEB_DIALOG_WINDOW_H
#define CR_WEB_DIALOG_WINDOW_H

#include <QMainWindow>

class WebView;

class WebDialogWindow : public QMainWindow
{
public:
    explicit WebDialogWindow(WebView *webView, QWidget *parent = nullptr);
};

#endif
