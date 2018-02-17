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

#include "webpage.h"
#include "../speeddial/speeddial.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <QDir>
#include <QWebEngineProfile>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QComboBox>
#include <QSettings>
#include <QGroupBox>
#include <QRadioButton>
#include <QColorDialog>


Profile::Profile()
{
    setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);
}



QStringList WebPage::chooseFiles(FileSelectionMode mode, const QStringList &oldFiles, const QStringList &acceptedMimeTypes)
{
    QFileDialog *f = new QFileDialog();

    //f->setOption(QFileDialog::DontUseNativeDialog,true);
    switch (mode) {
    case (QWebEnginePage::FileSelectOpen): {
        QString name = f->getOpenFileName(nullptr, QString(tr("Crusta : Upload File")), QDir::homePath(), QString(), nullptr, f->options());
        QStringList list;
        list.append(name);
        return list;
    }
    break;

    case (QWebEnginePage::FileSelectOpenMultiple): {
        QStringList names = f->getOpenFileNames(nullptr, QString(tr("Crusta : Upload Multiple Files")), QDir::homePath(), QString(), nullptr, f->options());
        return names;
    }
    break;
    }

    return QStringList();
}

void WebPage::loadUAString()
{
    QFile inputFile(QDir::homePath() + "/.crusta_db/settings.txt");

    if (inputFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&inputFile);
        in.setCodec("UTF-8");

        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList data = line.split(">>>>>");

            if (data[0] == "UA String") {
                agent = data[1];
                break;
            }
        }

        inputFile.close();
    }

    profile()->setHttpUserAgent(agent);
}

WebPage::WebPage()
{
    preventAlert = false;
    alertCount = 0;
    key = "ifRiVOjEJzLc2MQ9h3xT";
    loadUAString();
}

bool WebPage::certificateError(const QWebEngineCertificateError &error)
{
    if (error.isOverridable()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(error.errorDescription());
        msgBox.setInformativeText(tr("If you wish so, you may continue with an unverified certificate. "
                                     "Accepting an unverified certificate means "
                                     "you may not be connected with the host you tried to connect to.\n"
                                     "Do you wish to override the security check and continue?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        return msgBox.exec() == QMessageBox::Yes;
    }

    QMessageBox::critical(view(), tr("Certificate Error"), error.errorDescription(), QMessageBox::Ok, QMessageBox::NoButton);
    return false;
}

void WebPage::javaScriptAlert(const QUrl &securityOrigin, const QString &msg)
{
    if (preventAlert) {
        return;
    }

    QDialog *jad = new QDialog();
    jad->setWindowFlag(Qt::FramelessWindowHint);
    QVBoxLayout *vbox = new QVBoxLayout();
    jad->setLayout(vbox);
    QLabel *murl = new QLabel(tr("Page at ") + securityOrigin.toString() + tr(" says:"));
    murl->setStyleSheet("font-size: 14px; font: italics");
    vbox->addWidget(murl);
    QLabel *mlbl = new QLabel(msg);
    mlbl->setStyleSheet("margin-top: 15px; margin-bottom: 15px;");
    vbox->addWidget(mlbl);

    if (alertCount >= 1) {
        QCheckBox *prvnt_btn = new QCheckBox(tr("Prevent this page from additional dialogs"));
        vbox->addWidget(prvnt_btn);
        connect(prvnt_btn, &QCheckBox::clicked, this, [this] {
            preventAlert = true;
        });
    }

    QPushButton *okbtn = new QPushButton(tr("OK"));
    okbtn->setFixedWidth(80);
    connect(okbtn, &QPushButton::clicked, jad, &QDialog::accept);
    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(new QLabel());
    hbox->addWidget(okbtn);
    vbox->addLayout(hbox);
    jad->setObjectName("dialog");
    jad->setStyleSheet("#dialog{border: 1px solid #00b0e3}");
    alertCount++;
    jad->move(view()->mapToGlobal(QPoint((view()->width() - jad->width() / 2) / 2, 0)));
    jad->exec();
}

bool WebPage::javaScriptPrompt(const QUrl &securityOrigin, const QString &msg, const QString &defaultValue, QString *result)
{
    if (msg == key) {
        QDialog *sd = new QDialog();
        sd->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
        QVBoxLayout *vbox = new QVBoxLayout();
        sd->setLayout(vbox);
        QLabel *hlbl = new QLabel(tr("Add Speed Dial"));
        hlbl->setStyleSheet("font-size: 16px");
        QHBoxLayout *h0box = new QHBoxLayout();
        h0box->addWidget(new QLabel());
        h0box->addWidget(hlbl);
        h0box->addWidget(new QLabel());
        vbox->addLayout(h0box);
        QLineEdit *input_t = new QLineEdit();
        input_t->setPlaceholderText("Unique personalised title without space");
        vbox->addWidget(input_t);
        QLineEdit *input_u = new QLineEdit();
        input_u->setPlaceholderText("http://example.com");
        vbox->addWidget(input_u);
        QPushButton *ok = new QPushButton(tr("Add"));
        QPushButton *cncl = new QPushButton(tr("Cancel"));
        ok->setDefault(true);
        QHBoxLayout *h1box = new QHBoxLayout();
        h1box->addWidget(ok);
        h1box->addWidget(cncl);
        vbox->addLayout(h1box);
        connect(ok, &QPushButton::clicked, sd, &QDialog::accept);
        connect(cncl, &QPushButton::clicked, sd, &QDialog::reject);
        sd->setObjectName("dialog");
        sd->setStyleSheet("#dialog{border: 1px solid #00b0e3}");
        sd->setFixedWidth(300);

        if (sd->exec() == QDialog::Accepted) {
            *result = input_t->text();
            SpeedDial().add(input_t->text(), input_u->text());
            this->load(this->url());
            return true;
        } else {
            return false;
        }
    } else {
        QDialog *jpd = new QDialog();
        jpd->setWindowFlag(Qt::FramelessWindowHint);
        QVBoxLayout *vbox = new QVBoxLayout();
        jpd->setLayout(vbox);
        QLabel *murl = new QLabel(tr("Page at ") + securityOrigin.toString() + tr(" says:"));
        murl->setStyleSheet("font-size: 14px; font: italics");
        vbox->addWidget(murl);
        QLabel *mlbl = new QLabel(msg);
        mlbl->setStyleSheet("margin-top: 15px; margin-bottom: 15px;");
        vbox->addWidget(mlbl);
        QLineEdit *input = new QLineEdit();
        input->setText(defaultValue);
        input->selectAll();
        vbox->addWidget(input);
        QPushButton *ok = new QPushButton(tr("OK"));
        QPushButton *cncl = new QPushButton(tr("Cancel"));
        ok->setFixedWidth(80);
        cncl->setFixedWidth(80);
        ok->setDefault(true);
        QHBoxLayout *hbox = new QHBoxLayout();
        hbox->addWidget(new QLabel());
        hbox->addWidget(ok);
        hbox->addWidget(cncl);
        vbox->addLayout(hbox);
        connect(ok, &QPushButton::clicked, jpd, &QDialog::accept);
        connect(cncl, &QPushButton::clicked, jpd, &QDialog::reject);
        jpd->setObjectName("dialog");
        jpd->setStyleSheet("#dialog{border: 1px solid #00b0e3}");
        jpd->move(view()->mapToGlobal(QPoint((view()->width() - jpd->width() / 2) / 2, 0)));

        if (jpd->exec() == QDialog::Accepted) {
            *result = input->text();
            return true;
        } else {
            return false;
        }
    }
}

bool WebPage::javaScriptConfirm(const QUrl &securityOrigin, const QString &msg)
{
    if (msg.split(" ")[0] == key) {
        QDialog *sd = new QDialog();
        sd->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
        QVBoxLayout *vbox = new QVBoxLayout();
        sd->setLayout(vbox);
        QLabel *murl = new QLabel(tr("Do you want to remove ") + msg.split(" ")[1] + " ?");
        murl->setStyleSheet("font-size: 14px; font: italics");
        vbox->addWidget(murl);
        QPushButton *ok = new QPushButton(tr("OK"));
        QPushButton *cncl = new QPushButton(tr("Cancel"));
        ok->setDefault(true);
        QHBoxLayout *hbox = new QHBoxLayout();
        hbox->addWidget(ok);
        hbox->addWidget(cncl);
        vbox->addLayout(hbox);
        connect(ok, &QPushButton::clicked, sd, &QDialog::accept);
        connect(cncl, &QPushButton::clicked, sd, &QDialog::reject);
        sd->setObjectName("dialog");
        sd->setStyleSheet("#dialog{border: 1px solid #00b0e3; background-color: #fff}");

        if (sd->exec() == QDialog::Accepted) {
            SpeedDial().remove(msg.split(" ")[1]);
            this->load(this->url());
            return true;
        } else {
            return false;
        }
    } else if (msg == "ifRiVOjJzQWe2MQ9h3xT_filter_btn") { // filter button key of speed-dial
        QDialog *fd = new QDialog();
        fd->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
        fd->setObjectName("dialog");
        fd->setStyleSheet("#dialog{border: 1px solid #00b0e3; background-color: #fff}");
        QVBoxLayout *fdvbox = new QVBoxLayout();
        fdvbox->setSpacing(5);
        QLabel *title = new QLabel(tr("Speed-dial settings"));
        title->setStyleSheet("font-size: 16px");
        fdvbox->addWidget(title);
        QGroupBox *gbox1 = new QGroupBox();
        QVBoxLayout *gvbox = new QVBoxLayout();
        gbox1->setLayout(gvbox);
        gbox1->setTitle(tr("Search Engine"));
        QComboBox *src_en_text = new QComboBox();
        src_en_text->insertItem(0, QIcon(":/res/fav/web.png"), tr("Other"));
        src_en_text->insertItem(0, QIcon(":/res/fav/yandex.png"), tr("Yandex"));
        src_en_text->insertItem(0, QIcon(":/res/fav/ecosia.png"), tr("Ecosia"));
        QString currentSpeeddialSearchEngine = QSettings("Tarptaeya", "Crusta").value("speeddial_srch_engine").toString();
        gvbox->addWidget(src_en_text);
        QLineEdit *oth_src_en_text = new QLineEdit();
        oth_src_en_text->setPlaceholderText("http://your-favourite-search-string");
        gvbox->addWidget(oth_src_en_text);
        oth_src_en_text->hide();

        if (currentSpeeddialSearchEngine == "Yandex" || currentSpeeddialSearchEngine == "Ecosia") {
            src_en_text->setCurrentText(currentSpeeddialSearchEngine);
        } else {
            src_en_text->setCurrentText(currentSpeeddialSearchEngine);
            oth_src_en_text->setText(currentSpeeddialSearchEngine);

            if (oth_src_en_text->isHidden()) {
                oth_src_en_text->show();
            }
        }

        connect(src_en_text, &QComboBox::currentTextChanged, this, [this, oth_src_en_text](const QString s) {
            if (s == "Other") {
                if (!oth_src_en_text->isVisible()) {
                    oth_src_en_text->show();
                }
            } else {
                if (oth_src_en_text->isVisible()) {
                    oth_src_en_text->hide();
                }
            }
        });
        fdvbox->addWidget(gbox1);
        QGroupBox *gbox2 = new QGroupBox();
        gbox2->setTitle(tr("Background"));
        QVBoxLayout *gvbox2 = new QVBoxLayout();
        gbox2->setLayout(gvbox2);
        QRadioButton *solidOption = new QRadioButton(tr("Use Solid Color"));
        QRadioButton *imageOption = new QRadioButton(tr("Use Image"));
        gvbox2->addWidget(solidOption);
        gvbox2->addWidget(imageOption);
        QPushButton *hidden = new QPushButton(); // hidden button to fix the default button
        hidden->setDefault(true);
        gvbox2->addWidget(hidden);
        hidden->hide();
        QPushButton *bg_sld = new QPushButton(tr("Choose Solid Color"));
        connect(bg_sld, &QPushButton::clicked, this, [this] {
            QColorDialog *f = new QColorDialog();
            sdBgimage = f->getColor().name();
        });
        gvbox2->addWidget(bg_sld);
        QPushButton *bg_img = new QPushButton(tr("Choose Image"));
        connect(bg_img, &QPushButton::clicked, this, [this] {
            QFileDialog *f = new QFileDialog();
            sdBgimage = f->getOpenFileUrl(nullptr, tr("Speed Dial : Background"), QDir::homePath(), tr("Image Files (*.png *.jpg *.jpeg)")).toString();
        });
        gvbox2->addWidget(bg_img);
        QString backgrounOption = QSettings("Tarptaeya", "Crusta").value("background_option").toString();

        if (backgrounOption == tr("solid")) {
            solidOption->setChecked(true);
            bg_sld->setEnabled(true);
            bg_img->setEnabled(false);
        } else {
            imageOption->setChecked(true);
            bg_sld->setDisabled(true);
            bg_img->setEnabled(true);
        }

        connect(solidOption, &QRadioButton::toggled, this, [this, bg_sld, bg_img, solidOption] {
            if (solidOption->isChecked())
            {
                bg_sld->setEnabled(true);
                bg_img->setEnabled(false);
            } else
            {
                bg_sld->setDisabled(true);
                bg_img->setEnabled(true);
            }
        });
        fdvbox->addWidget(gbox2);
        fd->setFixedSize(300, 300);
        fd->setLayout(fdvbox);
        fd->move(view()->mapToGlobal(QPoint(view()->width() - 320, 20)));
        fd->exec();

        if (imageOption->isChecked()) {
            QSettings("Tarptaeya", "Crusta").setValue("background_option", "image");
        } else {
            QSettings("Tarptaeya", "Crusta").setValue("background_option", "solid");
        }

        if (sdBgimage.isEmpty()) {
            sdBgimage = QSettings("Tarptaeya", "Crusta").value("speeddial_bgimage").toString();
        } else {
            QSettings("Tarptaeya", "Crusta").setValue("speeddial_bgimage", sdBgimage);
        }

        if (src_en_text->currentText() == "Other") {
            currentSpeeddialSearchEngine = oth_src_en_text->text();
        } else {
            currentSpeeddialSearchEngine = src_en_text->currentText();
        }

        QSettings("Tarptaeya", "Crusta").setValue("speeddial_srch_engine", currentSpeeddialSearchEngine);
        SpeedDial().save(sdBgimage, currentSpeeddialSearchEngine);
        this->load(this->url());
        return true;
    } else {
        QDialog *jcd = new QDialog();
        jcd->setWindowFlag(Qt::FramelessWindowHint);
        QVBoxLayout *vbox = new QVBoxLayout();
        jcd->setLayout(vbox);
        QLabel *murl = new QLabel(tr("Page at ") + securityOrigin.toString() + tr(" says:"));
        murl->setStyleSheet("font-size: 14px; font: italics");
        vbox->addWidget(murl);
        QLabel *mlbl = new QLabel(msg);
        mlbl->setStyleSheet("margin-top: 15px; margin-bottom: 15px;");
        vbox->addWidget(mlbl);
        QPushButton *ok = new QPushButton(tr("OK"));
        QPushButton *cncl = new QPushButton(tr("Cancel"));
        ok->setFixedWidth(80);
        cncl->setFixedWidth(80);
        ok->setDefault(true);
        QHBoxLayout *hbox = new QHBoxLayout();
        hbox->addWidget(new QLabel());
        hbox->addWidget(ok);
        hbox->addWidget(cncl);
        vbox->addLayout(hbox);
        connect(ok, &QPushButton::clicked, jcd, &QDialog::accept);
        connect(cncl, &QPushButton::clicked, jcd, &QDialog::reject);
        jcd->setObjectName("dialog");
        jcd->setStyleSheet("#dialog{border: 1px solid #00b0e3}");
        jcd->move(view()->mapToGlobal(QPoint((view()->width() - jcd->width() / 2) / 2, 0)));

        if (jcd->exec() == QDialog::Accepted) {
            return true;
        } else {
            return false;
        }
    }
}



QStringList PrivateWebPage::chooseFiles(FileSelectionMode mode, const QStringList &oldFiles, const QStringList &acceptedMimeTypes)
{
    QFileDialog *f = new QFileDialog();

    //f->setOption(QFileDialog::DontUseNativeDialog,true);
    switch (mode) {
    case (QWebEnginePage::FileSelectOpen): {
        QString name = f->getOpenFileName(nullptr, QString(tr("Crusta : Upload File")), QDir::homePath(), QString(), nullptr, f->options());
        QStringList list;
        list.append(name);
        return list;
    }
    break;

    case (QWebEnginePage::FileSelectOpenMultiple): {
        QStringList names = f->getOpenFileNames(nullptr, QString(tr("Crusta : Upload Multiple Files")), QDir::homePath(), QString(), nullptr, f->options());
        return names;
    }
    break;
    }

    return QStringList();
}

void PrivateWebPage::loadUAString()
{
    QFile inputFile(QDir::homePath() + "/.crusta_db/settings.txt");

    if (inputFile.open(QIODevice::ReadOnly)) {
        QTextStream in(&inputFile);
        in.setCodec("UTF-8");

        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList data = line.split(">>>>>");

            if (data[0] == "UA String") {
                agent = data[1];
                break;
            }
        }

        inputFile.close();
    }

    profile()->setHttpUserAgent(agent);
}

PrivateWebPage::PrivateWebPage(QWebEngineProfile *profile, QObject *parent): QWebEnginePage(profile, parent)
{
    loadUAString();
    alertCount = 0;
    preventAlert = false;
    key = "ifRiVOjEJzLc2MQ9h3xT";
}

bool PrivateWebPage::certificateError(const QWebEngineCertificateError &error)
{
    if (error.isOverridable()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(error.errorDescription());
        msgBox.setInformativeText(tr("If you wish so, you may continue with an unverified certificate. "
                                     "Accepting an unverified certificate means "
                                     "you may not be connected with the host you tried to connect to.\n"
                                     "Do you wish to override the security check and continue?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        return msgBox.exec() == QMessageBox::Yes;
    }

    QMessageBox::critical(view(), tr("Certificate Error"), error.errorDescription(), QMessageBox::Ok, QMessageBox::NoButton);
    return false;
}

void PrivateWebPage::javaScriptAlert(const QUrl &securityOrigin, const QString &msg)
{
    if (preventAlert) {
        return;
    }

    QDialog *jad = new QDialog();
    jad->setWindowFlag(Qt::FramelessWindowHint);
    QVBoxLayout *vbox = new QVBoxLayout();
    jad->setLayout(vbox);
    QLabel *murl = new QLabel(tr("Page at ") + securityOrigin.toString() + tr(" says:"));
    murl->setStyleSheet("font-size: 14px; font: italics");
    vbox->addWidget(murl);
    QLabel *mlbl = new QLabel(msg);
    mlbl->setStyleSheet("margin-top: 15px; margin-bottom: 15px;");
    vbox->addWidget(mlbl);

    if (alertCount >= 1) {
        QCheckBox *prvnt_btn = new QCheckBox(tr("Prevent this page from additional dialogs"));
        vbox->addWidget(prvnt_btn);
        connect(prvnt_btn, &QCheckBox::clicked, this, [this] {
            preventAlert = true;
        });
    }

    QPushButton *okbtn = new QPushButton(tr("OK"));
    okbtn->setFixedWidth(80);
    connect(okbtn, &QPushButton::clicked, jad, &QDialog::accept);
    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(new QLabel());
    hbox->addWidget(okbtn);
    vbox->addLayout(hbox);
    jad->setObjectName("dialog");
    jad->setStyleSheet("#dialog{border: 1px solid #00b0e3}");
    alertCount++;
    jad->move(view()->mapToGlobal(QPoint((view()->width() - jad->width() / 2) / 2, 0)));
    jad->exec();
}

bool PrivateWebPage::javaScriptPrompt(const QUrl &securityOrigin, const QString &msg, const QString &defaultValue, QString *result)
{
    if (msg == key) {
        QDialog *sd = new QDialog();
        sd->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
        QVBoxLayout *vbox = new QVBoxLayout();
        sd->setLayout(vbox);
        QLabel *hlbl = new QLabel(tr("Add Speed Dial"));
        hlbl->setStyleSheet("font-size: 16px");
        QHBoxLayout *h0box = new QHBoxLayout();
        h0box->addWidget(new QLabel());
        h0box->addWidget(hlbl);
        h0box->addWidget(new QLabel());
        vbox->addLayout(h0box);
        QLineEdit *input_t = new QLineEdit();
        input_t->setPlaceholderText("Unique Title");
        vbox->addWidget(input_t);
        QLineEdit *input_u = new QLineEdit();
        input_u->setPlaceholderText("http://example.com");
        vbox->addWidget(input_u);
        QPushButton *ok = new QPushButton(tr("Add"));
        QPushButton *cncl = new QPushButton(tr("Cancel"));
        ok->setDefault(true);
        QHBoxLayout *h1box = new QHBoxLayout();
        h1box->addWidget(ok);
        h1box->addWidget(cncl);
        vbox->addLayout(h1box);
        connect(ok, &QPushButton::clicked, sd, &QDialog::accept);
        connect(cncl, &QPushButton::clicked, sd, &QDialog::reject);
        sd->setObjectName("dialog");
        sd->setStyleSheet("#dialog{border: 1px solid #00b0e3}");
        sd->setFixedWidth(300);

        if (sd->exec() == QDialog::Accepted) {
            *result = input_t->text();
            SpeedDial().add(input_t->text(), input_u->text());
            this->load(this->url());
            return true;
        } else {
            return false;
        }
    } else {
        QDialog *jpd = new QDialog();
        jpd->setWindowFlag(Qt::FramelessWindowHint);
        QVBoxLayout *vbox = new QVBoxLayout();
        jpd->setLayout(vbox);
        QLabel *murl = new QLabel(tr("Page at ") + securityOrigin.toString() + tr(" says:"));
        murl->setStyleSheet("font-size: 14px; font: italics");
        vbox->addWidget(murl);
        QLabel *mlbl = new QLabel(msg);
        mlbl->setStyleSheet("margin-top: 15px; margin-bottom: 15px;");
        vbox->addWidget(mlbl);
        QLineEdit *input = new QLineEdit();
        input->setText(defaultValue);
        input->selectAll();
        vbox->addWidget(input);
        QPushButton *ok = new QPushButton(tr("OK"));
        QPushButton *cncl = new QPushButton(tr("Cancel"));
        ok->setFixedWidth(80);
        cncl->setFixedWidth(80);
        ok->setDefault(true);
        QHBoxLayout *hbox = new QHBoxLayout();
        hbox->addWidget(new QLabel());
        hbox->addWidget(ok);
        hbox->addWidget(cncl);
        vbox->addLayout(hbox);
        connect(ok, &QPushButton::clicked, jpd, &QDialog::accept);
        connect(cncl, &QPushButton::clicked, jpd, &QDialog::reject);
        jpd->setObjectName("dialog");
        jpd->setStyleSheet("#dialog{border: 1px solid #00b0e3}");
        jpd->move(view()->mapToGlobal(QPoint((view()->width() - jpd->width() / 2) / 2, 0)));

        if (jpd->exec() == QDialog::Accepted) {
            *result = input->text();
            return true;
        } else {
            return false;
        }
    }
}

bool PrivateWebPage::javaScriptConfirm(const QUrl &securityOrigin, const QString &msg)
{
    if (msg.split(" ")[0] == key) {
        QDialog *sd = new QDialog();
        sd->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
        QVBoxLayout *vbox = new QVBoxLayout();
        sd->setLayout(vbox);
        QLabel *murl = new QLabel(tr("Do you want to remove ") + msg.split(" ")[1] + " ?");
        murl->setStyleSheet("font-size: 14px; font: italics");
        vbox->addWidget(murl);
        QPushButton *ok = new QPushButton(tr("OK"));
        QPushButton *cncl = new QPushButton(tr("Cancel"));
        ok->setDefault(true);
        QHBoxLayout *hbox = new QHBoxLayout();
        hbox->addWidget(ok);
        hbox->addWidget(cncl);
        vbox->addLayout(hbox);
        connect(ok, &QPushButton::clicked, sd, &QDialog::accept);
        connect(cncl, &QPushButton::clicked, sd, &QDialog::reject);
        sd->setObjectName("dialog");
        sd->setStyleSheet("#dialog{border: 1px solid #00b0e3; background-color: #fff}");

        if (sd->exec() == QDialog::Accepted) {
            SpeedDial().remove(msg.split(" ")[1]);
            this->load(this->url());
            return true;
        } else {
            return false;
        }
    } else if (msg == "ifRiVOjJzQWe2MQ9h3xT_filter_btn") { // filter button key of speed-dial
        QDialog *fd = new QDialog();
        fd->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);
        fd->setObjectName("dialog");
        fd->setStyleSheet("#dialog{border: 1px solid #00b0e3; background-color: #fff}");
        QVBoxLayout *fdvbox = new QVBoxLayout();
        fdvbox->setSpacing(5);
        QLabel *title = new QLabel(tr("Speed-dial settings"));
        title->setStyleSheet("font-size: 16px");
        fdvbox->addWidget(title);
        QGroupBox *gbox1 = new QGroupBox();
        QVBoxLayout *gvbox = new QVBoxLayout();
        gbox1->setLayout(gvbox);
        gbox1->setTitle(tr("Search Engine"));
        QComboBox *src_en_text = new QComboBox();
        src_en_text->insertItem(0, QIcon(":/res/fav/web.png"), tr("Other"));
        src_en_text->insertItem(0, QIcon(":/res/fav/yandex.png"), tr("Yandex"));
        src_en_text->insertItem(0, QIcon(":/res/fav/ecosia.png"), tr("Ecosia"));
        QString currentSpeeddialSearchEngine = QSettings("Tarptaeya", "Crusta").value("speeddial_srch_engine").toString();
        gvbox->addWidget(src_en_text);
        QLineEdit *oth_src_en_text = new QLineEdit();
        oth_src_en_text->setPlaceholderText("http://your-favourite-search-string");
        gvbox->addWidget(oth_src_en_text);
        oth_src_en_text->hide();

        if (currentSpeeddialSearchEngine == "Yandex" || currentSpeeddialSearchEngine == "Ecosia") {
            src_en_text->setCurrentText(currentSpeeddialSearchEngine);
        } else {
            src_en_text->setCurrentText(currentSpeeddialSearchEngine);
            oth_src_en_text->setText(currentSpeeddialSearchEngine);

            if (oth_src_en_text->isHidden()) {
                oth_src_en_text->show();
            }
        }

        connect(src_en_text, &QComboBox::currentTextChanged, this, [this, oth_src_en_text](const QString s) {
            if (s == "Other") {
                if (!oth_src_en_text->isVisible()) {
                    oth_src_en_text->show();
                }
            } else {
                if (oth_src_en_text->isVisible()) {
                    oth_src_en_text->hide();
                }
            }
        });
        fdvbox->addWidget(gbox1);
        QGroupBox *gbox2 = new QGroupBox();
        gbox2->setTitle(tr("Background"));
        QVBoxLayout *gvbox2 = new QVBoxLayout();
        gbox2->setLayout(gvbox2);
        QRadioButton *solidOption = new QRadioButton(tr("Use Solid Color"));
        QRadioButton *imageOption = new QRadioButton(tr("Use Image"));
        gvbox2->addWidget(solidOption);
        gvbox2->addWidget(imageOption);
        QPushButton *hidden = new QPushButton(); // hidden button to fix the default button
        hidden->setDefault(true);
        gvbox2->addWidget(hidden);
        hidden->hide();
        QPushButton *bg_sld = new QPushButton(tr("Choose Solid Color"));
        connect(bg_sld, &QPushButton::clicked, this, [this] {
            QColorDialog *f = new QColorDialog();
            sdBgimage = f->getColor().name();
        });
        gvbox2->addWidget(bg_sld);
        QPushButton *bg_img = new QPushButton(tr("Choose Image"));
        connect(bg_img, &QPushButton::clicked, this, [this] {
            QFileDialog *f = new QFileDialog();
            sdBgimage = f->getOpenFileUrl(nullptr, tr("Speed Dial : Background"), QDir::homePath(), tr("Image Files (*.png *.jpg *.jpeg)")).toString();
        });
        gvbox2->addWidget(bg_img);
        QString backgrounOption = QSettings("Tarptaeya", "Crusta").value("background_option").toString();

        if (backgrounOption == tr("solid")) {
            solidOption->setChecked(true);
            bg_sld->setEnabled(true);
            bg_img->setEnabled(false);
        } else {
            imageOption->setChecked(true);
            bg_sld->setDisabled(true);
            bg_img->setEnabled(true);
        }

        connect(solidOption, &QRadioButton::toggled, this, [this, bg_sld, bg_img, solidOption] {
            if (solidOption->isChecked())
            {
                bg_sld->setEnabled(true);
                bg_img->setEnabled(false);
            } else
            {
                bg_sld->setDisabled(true);
                bg_img->setEnabled(true);
            }
        });
        fdvbox->addWidget(gbox2);
        fd->setFixedSize(300, 300);
        fd->setLayout(fdvbox);
        fd->move(view()->mapToGlobal(QPoint(view()->width() - 320, 20)));
        fd->exec();

        if (imageOption->isChecked()) {
            QSettings("Tarptaeya", "Crusta").setValue("background_option", "image");
        } else {
            QSettings("Tarptaeya", "Crusta").setValue("background_option", "solid");
        }

        if (sdBgimage.isEmpty()) {
            sdBgimage = QSettings("Tarptaeya", "Crusta").value("speeddial_bgimage").toString();
        } else {
            QSettings("Tarptaeya", "Crusta").setValue("speeddial_bgimage", sdBgimage);
        }

        if (src_en_text->currentText() == "Other") {
            currentSpeeddialSearchEngine = oth_src_en_text->text();
        } else {
            currentSpeeddialSearchEngine = src_en_text->currentText();
        }

        QSettings("Tarptaeya", "Crusta").setValue("speeddial_srch_engine", currentSpeeddialSearchEngine);
        SpeedDial().save(sdBgimage, currentSpeeddialSearchEngine);
        this->load(this->url());
        return true;
    } else {
        QDialog *jcd = new QDialog();
        jcd->setWindowFlag(Qt::FramelessWindowHint);
        QVBoxLayout *vbox = new QVBoxLayout();
        jcd->setLayout(vbox);
        QLabel *murl = new QLabel(tr("Page at ") + securityOrigin.toString() + tr(" says:"));
        murl->setStyleSheet("font-size: 14px; font: italics");
        vbox->addWidget(murl);
        QLabel *mlbl = new QLabel(msg);
        mlbl->setStyleSheet("margin-top: 15px; margin-bottom: 15px;");
        vbox->addWidget(mlbl);
        QPushButton *ok = new QPushButton(tr("OK"));
        QPushButton *cncl = new QPushButton(tr("Cancel"));
        ok->setFixedWidth(80);
        cncl->setFixedWidth(80);
        ok->setDefault(true);
        QHBoxLayout *hbox = new QHBoxLayout();
        hbox->addWidget(new QLabel());
        hbox->addWidget(ok);
        hbox->addWidget(cncl);
        vbox->addLayout(hbox);
        connect(ok, &QPushButton::clicked, jcd, &QDialog::accept);
        connect(cncl, &QPushButton::clicked, jcd, &QDialog::reject);
        jcd->setObjectName("dialog");
        jcd->setStyleSheet("#dialog{border: 1px solid #00b0e3}");
        jcd->move(view()->mapToGlobal(QPoint((view()->width() - jcd->width() / 2) / 2, 0)));

        if (jcd->exec() == QDialog::Accepted) {
            return true;
        } else {
            return false;
        }
    }
}

