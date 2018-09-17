/* ============================================================
* Crusta - Qt5 webengine browser
* Copyright (C) 2017-2018 Anmol Gautam <tarptaeya@gmail.com>
*
* THIS FILE IS A PART OF CRUSTA
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
* ============================================================ */
#include "speeddial.h"
#include "../../bootstrap/appmanager.h"
#include "../../bootstrap/database.h"
#include "../../data/speeddialitem.h"
#include <QDialog>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QUrl>

QList<SpeeddialItem> Speeddial::m_speeddialItems = QList<SpeeddialItem>();

Speeddial::Speeddial(QObject *parent, QWebEnginePage *page)
    : QObject(parent)
{
    m_page = page;
}

void Speeddial::addDial()
{
    QDialog dialog;
#ifdef Q_OS_MAC
    dialog.setParent(m_page->view());
    dialog.setWindowFlag(Qt::Sheet);
#else
    dialog.setWindowTitle(tr("Add new dial"));
#endif
    QVBoxLayout vBoxLayout;

    QLineEdit titleLineEdit;
    titleLineEdit.setPlaceholderText(tr("Enter title of new dial"));
    vBoxLayout.addWidget(&titleLineEdit);

    QLineEdit urlLineEdit;
    urlLineEdit.setPlaceholderText(tr("Enter URL of new dial"));
    vBoxLayout.addWidget(&urlLineEdit);

    QLabel errorLabel;
    QPalette palette;
    palette.setColor(errorLabel.foregroundRole(), Qt::red);
    errorLabel.setPalette(palette);
    vBoxLayout.addWidget(&errorLabel);

    QWidget spacer;
    QPushButton okButton(tr("Ok"));
    okButton.setDefault(true);
    QPushButton cancelButton(tr("Cancel"));
    QHBoxLayout hBoxLayout;
    hBoxLayout.addWidget(&spacer);
    hBoxLayout.addWidget(&okButton);
    hBoxLayout.addWidget(&cancelButton);
    hBoxLayout.setStretch(0, 1);
    hBoxLayout.setStretch(1, 0);
    hBoxLayout.setStretch(2, 0);
    hBoxLayout.setContentsMargins(0, 0, 0, 0);
    vBoxLayout.addLayout(&hBoxLayout);

    dialog.setLayout(&vBoxLayout);

    connect(&okButton, &QPushButton::clicked, this, [&dialog, &urlLineEdit, &errorLabel]{
        if (QUrl::fromUserInput(urlLineEdit.text()).isValid()) {
            dialog.accept();
        } else {
            errorLabel.setText(tr("Please enter a valid URL"));
        }
    });

    connect(&cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    dialog.setMinimumWidth(300);
    if (dialog.exec() == QDialog::Accepted) {
        const QString title = titleLineEdit.text();
        const QString url = urlLineEdit.text();
        saveDialToDatabase(title, url);

        QVariantMap map;
        map.insert(QStringLiteral("title"), title);
        map.insert(QStringLiteral("url"), url);
        emit dialAdded(map);
    }
}

void Speeddial::removeDial(const QString &url)
{
    appManager->database()->removeSpeeddialEntry(url);
    for (int i = 0; i < m_speeddialItems.count(); i++) {
        SpeeddialItem item = m_speeddialItems.at(i);
        if (item.url() == url) {
            m_speeddialItems.removeAt(i);
            break;
        }
    }
}

void Speeddial::loadDialsFromDatabase()
{
    m_speeddialItems.clear();

    QList<SpeeddialItem> speeddialItems = appManager->database()->loadSpeeddialEntries();
    QVariantList dials;
    for (SpeeddialItem item : qAsConst(speeddialItems)) {
        QVariantMap map;
        map.insert(QStringLiteral("image"), item.image().toBase64());
        map.insert(QStringLiteral("title"), item.title());
        map.insert(QStringLiteral("url"), item.url());
        dials << map;

        m_speeddialItems.append(item);
    }
    emit dialsAdded(dials);
}

QList<SpeeddialItem> Speeddial::speeddialItems()
{
    return m_speeddialItems;
}

void Speeddial::saveDialToDatabase(const QString &title, const QString &url)
{
    SpeeddialItem item;
    item.setTitle(title);
    item.setUrl(url);
    appManager->database()->addSpeeddialEntry(item);
}
