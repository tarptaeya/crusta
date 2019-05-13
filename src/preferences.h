#pragma once

#include <QTabWidget>
#include <QWidget>

class Preferences : public QWidget
{
public:
    explicit Preferences(QWidget *parent = nullptr);
    void installWidget(const QString &name, QWidget *widget);

private:
    QTabWidget *m_tabWidget = nullptr;

    QWidget *createAppearanceTab();
    QWidget *createBrowsingTab();
    QWidget *createSearchEngineTab();
    QWidget *createWebEngineTab();
};
