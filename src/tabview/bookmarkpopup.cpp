#include "bookmarkpopup.h"
#include "appmanager.h"
#include "bookmarksitem.h"
#include "tab.h"
#include "webview.h"
#include "tools.h"
#include "database.h"
#include "omnibar.h"
#include "bookmarksitem.h"
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

    BookmarksItem item = appManager->database()->isBookmarked(tab->urlString());

    m_titleEntry = new QLineEdit;
    m_folder = new QComboBox;
    m_saveBtn = new QPushButton(tr("Save"));
    m_otherBtn = new QPushButton(item.url().isEmpty() ? tr("Add to speeddial") : tr("Delete"));
    m_cancelBtn = new QPushButton(tr("Cancel"));

    if (!item.folder().isEmpty()) {
        m_folder->addItem(item.folder());
    }
    m_folder->addItem(QSL("Others"));
    m_folder->addItem(QSL("Bookmark toolbar"));

    QVBoxLayout *vBoxLayout = new QVBoxLayout;
    vBoxLayout->addWidget(m_titleEntry);
    vBoxLayout->addWidget(m_folder);
    QHBoxLayout *h1BoxLayout = new QHBoxLayout;
    h1BoxLayout->addWidget(m_cancelBtn);
    h1BoxLayout->addWidget(m_otherBtn);
    h1BoxLayout->addWidget(m_saveBtn);
    m_saveBtn->setDefault(true);
    vBoxLayout->addLayout(h1BoxLayout);
    setLayout(vBoxLayout);

    m_titleEntry->setText(tab->title());

    connect(m_cancelBtn, &QPushButton::clicked, this, &BookmarkPopup::hide);
    connect(m_otherBtn, &QPushButton::clicked, this, [this, item] {
        if (!item.url().isEmpty()) {
            appManager->database()->removeBookmarksEntry(item.url());
            (static_cast<OmniBar *>(parentAction()->parentWidget()))->updateBookmarksIcon(false);
        } else {

        }

        hide();
    });
    connect(m_saveBtn, &QPushButton::clicked, this, [this, tab]{
        BookmarksItem item;
        item.setTitle(m_titleEntry->text());
        item.setUrl(tab->urlString());
        item.setFavicon(convertIconToByteArray(tab->webView()->icon()));
        bool isBookmarked = appManager->database()->addBookmarksEntry(item);
        (static_cast<OmniBar *>(parentAction()->parentWidget()))->updateBookmarksIcon(isBookmarked);
        hide();
    });    
}
