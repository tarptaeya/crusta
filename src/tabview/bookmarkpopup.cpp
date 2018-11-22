#include "bookmarkpopup.h"
#include "appmanager.h"
#include "bookmarksitem.h"
#include "tab.h"
#include "webview.h"
#include <QVBoxLayout>

#define QSL QStringLiteral

BookmarkPopup::BookmarkPopup(QWidget *parent)
    : ToolBarPopup(parent)
{
    position = Trailing;

    Tab *tab = appManager->getCurrentTab();
    if (!tab) {
        return;
    }

    m_titleEntry = new QLineEdit;
    m_folder = new QComboBox;
    m_saveBtn = new QPushButton(tr("Save"));
    m_addToSpeedDial = new QPushButton(tr("Add to startpage"));
    m_cancelBtn = new QPushButton(tr("Cancel"));

    m_folder->addItem(QSL("Others"));
    m_folder->addItem(QSL("Bookmark toolbar"));

    QVBoxLayout *vBoxLayout = new QVBoxLayout;
    vBoxLayout->addWidget(m_titleEntry);
    vBoxLayout->addWidget(m_folder);
    QHBoxLayout *h1BoxLayout = new QHBoxLayout;
    h1BoxLayout->addWidget(m_cancelBtn);
    h1BoxLayout->addWidget(m_addToSpeedDial);
    h1BoxLayout->addWidget(m_saveBtn);
    m_saveBtn->setDefault(true);
    vBoxLayout->addLayout(h1BoxLayout);
    setLayout(vBoxLayout);

    m_titleEntry->setText(tab->title());

    connect(m_cancelBtn, &QPushButton::clicked, this, &BookmarkPopup::hide);
    connect(m_saveBtn, &QPushButton::clicked, this, [this, tab]{
        BookmarksItem *item = new BookmarksItem();
        item->setTitle(m_titleEntry->text());
        item->setUrl(tab->urlString());
    });
}
