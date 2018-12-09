#pragma once

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDateTime>
#include <QMenu>
#include <QAction>
#include <QClipboard>
#include <QApplication>

class HistoryManager : public QObject
{
    Q_OBJECT
public:
    static void showHistoryManager(QWidget *baseWidget);
};
