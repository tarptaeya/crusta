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

#include <QWidget>

class OmniBar;
class ToolBarButton;
class ProgressBar;

class ToolBar : public QWidget
{
    Q_OBJECT
public:
    explicit ToolBar(QWidget *parent = nullptr);
    void setIsCanGoBack(bool canGoBack);
    void setIsCanGoForward(bool canGoForward);
    void setIsLoading(bool isLoading);
    void setAddress(const QUrl &url);
    void setProgress(int progress);
Q_SIGNALS:
    void backRequested();
    void forwardRequested();
    void stopReloadRequested();
private:
    ToolBarButton *m_backButton = nullptr;
    ToolBarButton *m_forwardButton = nullptr;
    ToolBarButton *m_stopReloadButton = nullptr;
    ToolBarButton *m_favouritesButton = nullptr;
    OmniBar *m_omniBar = nullptr;
    ToolBarButton *m_shieldButton = nullptr;
    ToolBarButton *m_downloadsButton = nullptr;
    ProgressBar *m_progressBar = nullptr;

    QWidget *spacerWidget(int width);
};
