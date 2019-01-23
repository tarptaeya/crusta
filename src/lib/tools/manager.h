#ifndef CR_MANAGER_H
#define CR_MANAGER_H

#include <QTabWidget>
#include <QWidget>

class BookmarksManager;
class DownloadsManager;
class HistoryManager;

class Manager : public QWidget
{
public:
    explicit Manager(QWidget *parent = nullptr);

    DownloadsManager *downloadsManager();

    void showHistoryManager();
    void showBookmarksManager();
    void showDownloadsManager();

private:
    QTabWidget *m_tabWidget = nullptr;

    HistoryManager *m_historyManager = nullptr;
    BookmarksManager *m_bookmarksManager = nullptr;
    DownloadsManager *m_downloadsManager = nullptr;
};

#endif
