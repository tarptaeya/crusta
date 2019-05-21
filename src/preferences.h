#pragma once

#include "searchengine.h"

#include <QListWidgetItem>
#include <QTabWidget>
#include <QWidget>

class Preferences : public QWidget
{
    Q_OBJECT
public:
    explicit Preferences(QWidget *parent = nullptr);
    void installWidget(const QString &name, QWidget *widget);

Q_SIGNALS:
    void browsingSaveRequested();
    void browsingRestoreRequested();

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
