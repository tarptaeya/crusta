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
#include "sidebarpopup.h"

SideBarPopup::SideBarPopup(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *hbox = new  QHBoxLayout;
    hbox->setContentsMargins(10, 10, 10, 10);
    setLayout(hbox);

    QLineEdit *addressLine = new QLineEdit;
    addressLine->setPlaceholderText("https://");
    addressLine->setMinimumWidth(200);
    hbox->addWidget(addressLine, 1);

    QPushButton *addButton = new QPushButton;
    addButton->setIcon(QIcon(":/icons/plus.svg"));
    addButton->setFlat(true);
    hbox->addWidget(addButton);

    connect(addressLine, &QLineEdit::returnPressed, addButton, &QPushButton::click);

    connect(addButton, &QPushButton::clicked, this, [this, addressLine]{
        const QString urlString = addressLine->text();
        if (!QUrl::fromUserInput(urlString).isValid()) {
            return;
        }
        emit addPanelRequested(urlString);
        close();
    });
}
