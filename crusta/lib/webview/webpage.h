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

#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <QWebEnginePage>
#include <QObject>
#include <QString>
#include <QWebEngineProfile>
#include <QMessageBox>
#include <QWebEngineCertificateError>


class Profile:public QWebEngineProfile{
public:
    Profile();
};

class WebPage:public QWebEnginePage{
    Q_OBJECT
    QString key;
protected:
    QStringList chooseFiles(FileSelectionMode mode, const QStringList &oldFiles, const QStringList &acceptedMimeTypes);
    virtual bool certificateError(const QWebEngineCertificateError &error);
    void javaScriptAlert(const QUrl &securityOrigin, const QString &msg);
    bool javaScriptPrompt(const QUrl &securityOrigin, const QString &msg, const QString &defaultValue, QString *result);
    bool javaScriptConfirm(const QUrl &securityOrigin, const QString &msg);
    //virtual bool acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame);
public:
    int alertCount;
    bool preventAlert;
    QString agent;
    QString sdBgimage;
    void loadUAString();
    WebPage();
};



class PrivateWebPage:public QWebEnginePage{
    QString  key;
protected:
    QStringList chooseFiles(FileSelectionMode mode, const QStringList &oldFiles, const QStringList &acceptedMimeTypes);
    virtual bool certificateError(const QWebEngineCertificateError &error);
    void javaScriptAlert(const QUrl &securityOrigin, const QString &msg);
    bool javaScriptPrompt(const QUrl &securityOrigin, const QString &msg, const QString &defaultValue, QString *result);
    bool javaScriptConfirm(const QUrl &securityOrigin, const QString &msg);
public:
    int alertCount;
    bool preventAlert;
    QString agent;
    void loadUAString();
    QString sdBgimage;
    PrivateWebPage(QWebEngineProfile* profile,QObject* parent=0);
};

#endif // WEBPAGE_H
