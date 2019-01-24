#include "common-defs.h"
#include "bookmarkdialog.h"
#include "bookmarkitem.h"
#include "database.h"
#include "mainapplication.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>

BookmarkDialog::BookmarkDialog(QWidget *parent)
    : QDialog (parent)
{
    QVBoxLayout *vboxLayout = new QVBoxLayout;
    vboxLayout->setContentsMargins(5, 5, 5, 5);

    m_titleLine = new QLineEdit;
    m_titleLine->setPlaceholderText(QSL("Bookmark Title"));
    m_titleLine->setToolTip(QSL("Bookmark Title"));
    m_folderCombo = new QComboBox;
    m_folderCombo->setEditable(true);
    m_folderCombo->setToolTip(QSL("Bookmark Folder"));

    m_folderCombo->addItems(appManager->dataBase()->bookmarkFolders());

    QHBoxLayout *hboxLayout = new QHBoxLayout;
    hboxLayout->setContentsMargins(0, 0, 0, 0);
    QPushButton *ok = new QPushButton(QSL("Ok"));
    QPushButton *cancel = new QPushButton(QSL("Cancel"));

    hboxLayout->addWidget(new QWidget);
    hboxLayout->addWidget(ok);
    hboxLayout->addWidget(cancel);

    vboxLayout->addWidget(m_titleLine);
    vboxLayout->addWidget(m_folderCombo);
    vboxLayout->addLayout(hboxLayout);

    setLayout(vboxLayout);

    connect(ok, &QPushButton::clicked, this, &BookmarkDialog::accept);
    connect(cancel, &QPushButton::clicked, this, &BookmarkDialog::reject);
}

void BookmarkDialog::setAddress(const QString &address)
{
    m_address = address;
}

void BookmarkDialog::setTitle(const QString &title)
{
    setWindowTitle(QSL("Bookmark %1").arg(title));
    m_titleLine->setText(title);
}

void BookmarkDialog::accept()
{
    BookmarkItem item;
    item.title = m_titleLine->text();
    item.folder = m_folderCombo->currentText();
    item.address = m_address;
    appManager->dataBase()->addBookmark(item);
    QDialog::accept();
}

void BookmarkDialog::reject()
{
    QDialog::reject();
}
