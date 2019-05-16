#pragma once

#include "searchengine.h"

#include <QListWidgetItem>
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
    QWidget *createPrivacyTab();
};

class EngineListWidgetItem : public QListWidgetItem
{
public:
    Engine engine;
};
