#pragma once

#include <QWidget>

class WebView;

class Tab : public QWidget
{
    Q_OBJECT
public:
    enum DisplayMode {
        Desktop,
        IPhone,
        Android,
        Custom
    };
    explicit Tab(QWidget *parent = nullptr);
    QString title() const;
private:
    QString m_title;
    WebView *m_webView;
};
