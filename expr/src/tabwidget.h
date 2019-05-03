#pragma once

#include <QToolButton>
#include <QTabWidget>

class Tab;

class TabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit TabWidget(QWidget *parent = nullptr);

    int addTab(Tab *tab, const QString &label);
    void back();
    void forward();

Q_SIGNALS:
    void urlChanged(const QUrl &url);
    void windowCloseRequested();

private:
    QToolButton *m_newTabButton = nullptr;
};
