#ifndef CR_HISTORY_MANAGER_H
#define CR_HISTORY_MANAGER_H

#include <QTreeWidget>
#include <QWidget>

class HistoryManager : public QWidget
{
public:
    explicit HistoryManager(QWidget *parent = nullptr);

    void updateEntries();
private:
    void showContextMenu(const QPoint &pos);

    QTreeWidget *m_treeWidget = nullptr;
};

#endif
