#include "bookmarkitem.h"
#include "bookmarksmanager.h"
#include "common-defs.h"
#include "database.h"
#include "mainapplication.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

BookmarksManager::BookmarksManager(QWidget *parent)
    : QWidget (parent)
{
    m_folderCombo = new QComboBox;
    m_folderCombo->addItems(appManager->dataBase()->bookmarkFolders());

    m_treeWidget = new QTreeWidget;
    QStringList headerLabels;
    headerLabels << QSL("Title") << QSL("Address");
    m_treeWidget->setHeaderLabels(headerLabels);

    QVBoxLayout *vboxLayout = new QVBoxLayout;

    QHBoxLayout *hboxLayout = new QHBoxLayout;
    QLabel *folderLabel = new QLabel(QSL("Folder : "));
    hboxLayout->addWidget(folderLabel, 0);
    hboxLayout->addWidget(m_folderCombo);

    vboxLayout->addLayout(hboxLayout);
    vboxLayout->addWidget(m_treeWidget);

    setLayout(vboxLayout);

    updateEntries();

    connect(m_folderCombo, &QComboBox::currentTextChanged, this, &BookmarksManager::updateEntries);
}

void BookmarksManager::updateEntries()
{
    m_treeWidget->clear();

    QString folderName = m_folderCombo->currentText();
    const QList<BookmarkItem> bookmarks = appManager->dataBase()->bookmarks(folderName);
    for (const BookmarkItem &bookmark : qAsConst(bookmarks)) {
        QTreeWidgetItem *item = new QTreeWidgetItem;
        item->setText(0, bookmark.title);
        item->setText(1, bookmark.address);
        m_treeWidget->addTopLevelItem(item);
    }
}
