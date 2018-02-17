/* ============================================================
* Crusta - Qt5 webengine browser
* Copyright (C) 2017-2018 Anmol Gautam <anmol@crustabrowser.com>
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

#include "addresslineedit.h"

#include <QDialog>
#include <QDir>
#include <QFile>
#include <QHBoxLayout>
#include <QIODevice>
#include <QLabel>
#include <QMenu>
#include <QString>
#include <QStringListModel>
#include <QTextStream>
#include <QVBoxLayout>
#include <QWebEngineProfile>
#include <QGroupBox>
#include <QRadioButton>
#include <QComboBox>


void AddressLineEdit::createAddressLineEdit()
{
    siteinfo_btn->move(3, 3);
    siteinfo_btn->setFixedSize(20, 20);
    siteinfo_btn->setIcon(QIcon(":/res/drawables/normal_site.svg"));
    siteinfo_btn->setStyleSheet("border: 1px solid #00b0e3");
    siteinfo_btn->setToolTip(tr("Site Info"));
    setTextMargins(22, 0, 7, 0);
    setFixedHeight(26);
    loadSearchString();
}

QLineEdit *AddressLineEdit::initialize()
{
    return this;
}

void AddressLineEdit::showContextMenu(const QPoint &pos)
{
    QMenu *contextMenu = new QMenu();
    QAction *undo = new QAction(tr("Undo"));
    connect(undo, &QAction::triggered, this, &QLineEdit::undo);
    contextMenu->addAction(undo);
    QAction *redo = new QAction(tr("Redo"));
    connect(redo, &QAction::triggered, this, &QLineEdit::redo);
    contextMenu->addAction(redo);
    contextMenu->addSeparator();
    QAction *cut = new QAction(tr("Cut"));
    connect(cut, &QAction::triggered, this, &QLineEdit::cut);
    contextMenu->addAction(cut);
    QAction *copy = new QAction(tr("Copy"));
    connect(copy, &QAction::triggered, this, &QLineEdit::copy);
    contextMenu->addAction(copy);
    QAction *paste = new QAction(tr("Paste"));
    connect(paste, &QAction::triggered, this, &QLineEdit::paste);
    contextMenu->addAction(paste);
    contextMenu->addSeparator();
    contextMenu->addAction(default_search);
    contextMenu->addAction(changeUAstring);
    contextMenu->exec(this->mapToGlobal(pos));
}

AddressLineEdit::AddressLineEdit()
{
    setPlaceholderText("Search or enter address");
    createAddressLineEdit();
    setCompleter(cmpleter);
    cmpleter->setCompletionMode(QCompleter::PopupCompletion);
    createCompleter();
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &AddressLineEdit::customContextMenuRequested, this, &AddressLineEdit::showContextMenu);
    connect(this, &AddressLineEdit::returnPressed, this, &AddressLineEdit::updateCompleter);
    connect(default_search, &QAction::triggered, this, &AddressLineEdit::setDefaultSearch);
    connect(changeUAstring, &QAction::triggered, this, &AddressLineEdit::setUAString);
}

void AddressLineEdit::createCompleter()
{
    QStringListModel *model = new QStringListModel();
    cmpleter->setModel(model);
    cmpleter->setCaseSensitivity(Qt::CaseInsensitive);
    cmpleter->setFilterMode(Qt::MatchContains);
    QFile inputFile(QDir::homePath() + "/.crusta_db/completer.txt");

    if (inputFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&inputFile);
        in.setCodec("UTF-8");

        while (!in.atEnd()) {
            QString line = in.readLine();
            list.append(line);
        }

        inputFile.close();
    }

    model->setStringList(list);
}

void AddressLineEdit::updateCompleter()
{
    QString s = this->text();
    QFile inputFile(QDir::homePath() + "/.crusta_db/completer.txt");

    if (inputFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&inputFile);
        in.setCodec("UTF-8");

        while (!in.atEnd()) {
            QString line = in.readLine();

            if (line.toLower() == s.toLower()) {
                return;
            }
        }

        inputFile.close();
    }

    QFile file(QDir::homePath() + "/.crusta_db/completer.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << s.toUtf8() + "\n";
    file.close();
}

void AddressLineEdit::loadSearchString()
{
    QFile inputFile(QDir::homePath() + "/.crusta_db/settings.txt");

    if (inputFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&inputFile);
        in.setCodec("UTF-8");

        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList data = line.split(">>>>>");

            if (data[0] == "engine") {
                this->defaultSearch = data[1];

                if (this->defaultSearch.isEmpty()) {
                    this->defaultSearch = QString("https://www.ecosia.org/search?tt=crusta&q=");
                }

                inputFile.close();
                return;
            }
        }

        inputFile.close();
    }
}

void AddressLineEdit::setDefaultSearch()
{
    QDialog *w = new QDialog();
    QVBoxLayout *vbox = new QVBoxLayout();
    QGroupBox *gbox = new QGroupBox();
    gbox->setTitle(tr("Search Engine Settings"));
    QVBoxLayout *gvbox = new QVBoxLayout();
    gbox->setLayout(gvbox);
    QRadioButton *defaultOption = new QRadioButton(tr("Use default search engines"));
    QRadioButton *customOption = new QRadioButton(tr("Use custom search string"));
    gvbox->addWidget(defaultOption);
    gvbox->addWidget(customOption);
    QComboBox *srch_cmb = new QComboBox();
    srch_cmb->addItem(QIcon(":/res/fav/ecosia.png"), tr("Ecosia"));
    srch_cmb->addItem(QIcon(":/res/fav/yandex.png"), tr("Yandex"));
    gvbox->addWidget(srch_cmb);
    QLineEdit *cstm_srch = new QLineEdit();
    cstm_srch->setText(defaultSearch);
    cstm_srch->setPlaceholderText(tr("http://your-favourite-search-engine-string"));
    gvbox->addWidget(cstm_srch);

    if (defaultSearch == "http://www.yandex.ru/?clid=2308389&q=" || defaultSearch == "https://www.ecosia.org/search?tt=crusta&q=") {
        defaultOption->setChecked(true);
        cstm_srch->hide();

        if (defaultSearch.contains("yandex")) {
            srch_cmb->setCurrentIndex(1);
        } else if (defaultSearch.contains("ecosia")) {
            srch_cmb->setCurrentIndex(0);
        }
    } else {
        srch_cmb->setDisabled(true);
        cstm_srch->show();
        customOption->setChecked(true);
    }

    connect(customOption, &QRadioButton::toggled, this, [this, customOption, cstm_srch, srch_cmb] {
        if (customOption->isChecked())
        {
            cstm_srch->show();
            srch_cmb->setDisabled(true);
        } else
        {
            cstm_srch->hide();
            srch_cmb->setEnabled(true);
        }
    });
    vbox->addWidget(gbox);
    QHBoxLayout *h1box = new QHBoxLayout();
    QPushButton *cncl = new QPushButton(tr("Cancel"));
    QPushButton *ok = new QPushButton(tr("Save"));
    h1box->addWidget(new QLabel());
    h1box->addWidget(cncl);
    h1box->addWidget(ok);
    ok->setDefault(true);
    cncl->setFixedWidth(100);
    ok->setFixedWidth(100);
    vbox->addLayout(h1box);
    w->setLayout(vbox);
    w->setFixedWidth(500);
    w->setWindowTitle("Crusta : Set Default Search Engine");
    connect(cncl, &QPushButton::clicked, w, &QDialog::reject);
    connect(ok, &QPushButton::clicked, w, &QDialog::accept);

    if (w->exec() != QDialog::Accepted) {
        return;
    }

    QString new_string;

    if (defaultOption->isChecked()) {
        if (srch_cmb->currentText() == "Ecosia") {
            new_string = "https://www.ecosia.org/search?tt=crusta&q=";
        } else if (srch_cmb->currentText() == "Yandex") {
            new_string = "http://www.yandex.ru/?clid=2308389&q=";
        }
    } else {
        new_string = cstm_srch->text();
    }

    defaultSearch = new_string;
    QFile f(QDir::homePath() + "/.crusta_db/settings.txt");

    if (f.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QString s;
        QTextStream t(&f);
        t.setCodec("UTF-8");

        while (!t.atEnd()) {
            QString line = t.readLine();
            QStringList data = line.split(">>>>>");

            if (data[0] == "engine") {
                s.append(data[0] + ">>>>>" + new_string + "\n");
            } else {
                s.append(line + "\n");
            }
        }

        f.resize(0);
        t << s;
        f.close();
    }
}

void AddressLineEdit::setUAString()
{
    QDialog *w = new QDialog();
    QLabel *lbl = new QLabel(tr("HTTP USER AGENT"));
    QLineEdit *ua = new QLineEdit();
    QString http;
    QFile inputFile(QDir::homePath() + "/.crusta_db/settings.txt");

    if (inputFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&inputFile);
        in.setCodec("UTF-8");

        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList data = line.split(">>>>>");

            if (data[0] == "UA String") {
                http = data[1];
                break;
            }
        }

        inputFile.close();
    }

    ua->setText(http);
    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(lbl);
    hbox->addWidget(ua);
    QHBoxLayout *h1box = new QHBoxLayout();
    QPushButton *restore = new QPushButton(tr("Restore"));
    QPushButton *cncl = new QPushButton(tr("Cancel"));
    QPushButton *ok = new QPushButton(tr("Save"));
    h1box->addWidget(restore);
    h1box->addWidget(new QLabel());
    h1box->addWidget(cncl);
    h1box->addWidget(ok);
    cncl->setFixedWidth(100);
    ok->setFixedWidth(100);
    ok->setDefault(true);
    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addLayout(hbox);
    vbox->addLayout(h1box);
    w->setLayout(vbox);
    w->setFixedWidth(500);
    w->setWindowTitle("Crusta : Edit HTTP User Agent String");
    connect(cncl, &QPushButton::clicked, w, &QDialog::reject);
    connect(ok, &QPushButton::clicked, w, &QDialog::accept);
    connect(ua, &QLineEdit::returnPressed, w, &QDialog::accept);
    connect(restore, &QPushButton::clicked, this, [this, w] {restoreUAString(); w->reject();});

    if (w->exec() != QDialog::Accepted) {
        return;
    }

    if (ua->text() == "") {
        return;
    }

    QString new_string = ua->text();
    QFile f(QDir::homePath() + "/.crusta_db/settings.txt");

    if (f.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QString s;
        QTextStream t(&f);
        t.setCodec("UTF-8");

        while (!t.atEnd()) {
            QString line = t.readLine();
            QStringList data = line.split(">>>>>");

            if (data[0] == "UA String") {
                s.append(data[0] + ">>>>>" + new_string + "\n");
            } else {
                s.append(line + "\n");
            }
        }

        f.resize(0);
        t << s;
        f.close();
    }
}

void AddressLineEdit::restoreUAString()
{
    QWebEngineProfile p;
    p.setHttpUserAgent("");
    QStringList ua = p.httpUserAgent().split(" ");
    QString new_string = "";
    int len = ua.length();

    for (int i = 0; i < len - 1; i++) {
        new_string += ua[i] + " ";
    }

    new_string += "Crusta/1.4.2 " + ua[len - 1];
    QFile f(QDir::homePath() + "/.crusta_db/settings.txt");

    if (f.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QString s;
        QTextStream t(&f);
        t.setCodec("UTF-8");

        while (!t.atEnd()) {
            QString line = t.readLine();
            QStringList data = line.split(">>>>>");

            if (data[0] == "UA String") {
                s.append(data[0] + ">>>>>" + new_string + "\n");
            } else {
                s.append(line + "\n");
            }
        }

        f.resize(0);
        t << s;
        f.close();
    }
}


