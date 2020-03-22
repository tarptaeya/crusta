#pragma once

#include <QMainWindow>
#include <QToolBar>

class PreferencesWindow : public QMainWindow
{
    QToolBar *m_toolbar = nullptr;
    void setup_toolbar();
public:
    explicit PreferencesWindow(QWidget *parent = nullptr);
};
