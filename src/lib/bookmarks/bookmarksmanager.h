#ifndef CR_BOOKMARKS_MANAGER_H
#define CR_BOOKMARKS_MANAGER_H

#include <QComboBox>
#include <QTreeWidget>
#include <QWidget>

class BookmarksManager : public QWidget
{
public:
    explicit BookmarksManager(QWidget *parent = nullptr);

    void updateEntries();

private:
    QComboBox *m_folderCombo = nullptr;
    QTreeWidget *m_treeWidget = nullptr;
};

#endif
