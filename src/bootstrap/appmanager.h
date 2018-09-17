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
#pragma once

#include <QObject>
#include <QWebEngineProfile>
#include <QWebEngineScriptCollection>
#include <QWebEngineScript>

#define appManager AppManager::instance()

class Window;
class Settings;
class ExternalJsObject;
class Database;

class AppManager : public QObject
{
    Q_OBJECT
    enum ApplicationMode {
        Development,
        Alpha,
        Beta,
        Release
    };
    struct Application {
        QString organisationName;
        QString organisationDomain;
        QString name;
        int majorVersion;
        int minorVersion;
        int patchVersion;
        ApplicationMode mode;
    };
public:
    explicit AppManager(QObject *parent = nullptr);
    ~AppManager();
    static AppManager *instance();

    QString organisationName();
    QString organisationDomain();
    QString applicationName();
    int majorVersion();
    int minorVersion();
    int patchVersion();
    QString applicationVersionString();

    void bootstrap();
    Settings *settings();
    QWebEngineProfile *webEngineProfile() const;
    Database *database() const;
private:
    Application m_application;
    QList<Window *> m_windowList;
    Settings *m_settings = nullptr;
    QWebEngineProfile *m_webEngineProfile = nullptr;
    Database *m_database;

    void setUpWebEngineProfile();
};
