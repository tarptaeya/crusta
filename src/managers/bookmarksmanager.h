#pragma once

#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMenu>
#include <QAction>
#include <QClipboard>
#include <QApplication>

class BookmarksManager : public QObject
{
    Q_OBJECT
public:
    static void showBookmarksManager(QWidget *baseWidget);
};
