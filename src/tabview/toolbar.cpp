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
#include "toolbar.h"
#include "omnibar.h"
#include "toolbarbutton.h"
#include "progressbar.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#define SPACER_WIDGET_WIDTH 75
#define QSL QStringLiteral

ToolBar::ToolBar(QWidget *parent)
    : QWidget(parent)
{
    setObjectName("tool-bar");
    setAttribute(Qt::WA_StyledBackground, true);

    QHBoxLayout *hBoxLayout = new QHBoxLayout;
    hBoxLayout->setContentsMargins(5, 5, 5, 5);
    hBoxLayout->setSpacing(0);

    m_backButton = new ToolBarButton(this);
    m_backButton->setIconFromFileName(QSL(":/icons/back.svg"));
    m_forwardButton = new ToolBarButton(this);
    m_forwardButton->setIconFromFileName(QSL(":/icons/forward.svg"));
    m_stopReloadButton = new ToolBarButton(this);
    m_favouritesButton = new ToolBarButton(this);
    m_favouritesButton->setIconFromFileName(QSL(":/icons/widgets.svg"));
    m_omniBar = new OmniBar(this);
    m_shieldButton = new ToolBarButton(this);
    m_shieldButton->setIconFromFileName(QSL(":/icons/shield.svg"));
    m_optionsButton = new ToolBarButton(this);
    m_optionsButton->setIconFromFileName(QSL(":/icons/options.svg"));
    m_optionsButton->setMenu(createOptionsMenu());

    hBoxLayout->addWidget(m_backButton);
    hBoxLayout->addWidget(m_forwardButton);
    hBoxLayout->addWidget(m_stopReloadButton);
    hBoxLayout->addWidget(m_favouritesButton);
    hBoxLayout->addWidget(spacerWidget(SPACER_WIDGET_WIDTH));
    hBoxLayout->addWidget(m_omniBar);
    hBoxLayout->addWidget(spacerWidget(SPACER_WIDGET_WIDTH));
    hBoxLayout->addWidget(m_shieldButton);
    hBoxLayout->addWidget(m_optionsButton);

    QVBoxLayout *vBoxLayout = new QVBoxLayout;
    vBoxLayout->setContentsMargins(0, 0, 0, 0);
    vBoxLayout->setSpacing(0);
    vBoxLayout->addLayout(hBoxLayout);
    setLayout(vBoxLayout);

    m_progressBar = new ProgressBar(this);
    vBoxLayout->addWidget(m_progressBar);

    connect(m_backButton, &ToolBarButton::clicked, this, [this]{ emit backRequested(); });
    connect(m_forwardButton, &ToolBarButton::clicked, this, [this]{ emit forwardRequested(); });
    connect(m_stopReloadButton, &ToolBarButton::clicked, this, [this]{ emit stopReloadRequested(); });
    connect(m_optionsButton, &QToolButton::clicked, m_optionsButton, &ToolBarButton::showMenu);
}

void ToolBar::setIsCanGoBack(bool canGoBack)
{
    m_backButton->setEnabled(canGoBack);
}

void ToolBar::setIsCanGoForward(bool canGoForward)
{
    m_forwardButton->setEnabled(canGoForward);
}

void ToolBar::setIsLoading(bool isLoading)
{
    if (isLoading) {
        m_stopReloadButton->setIconFromFileName(QSL(":/icons/close.svg"));
    } else {
        m_stopReloadButton->setIconFromFileName(QSL(":/icons/refresh.svg"));
    }
}

void ToolBar::setAddress(const QUrl &url)
{
    m_omniBar->setAddress(url);
}

void ToolBar::setProgress(int progress)
{
    m_progressBar->setValue(progress % 100);
}

QWidget *ToolBar::spacerWidget(int width)
{
    QWidget *widget = new QWidget(this);
    widget->setMinimumWidth(width);
    return widget;
}

QMenu *ToolBar::createOptionsMenu()
{
    QMenu *menu = new QMenu;

    QMenu *file = new QMenu(tr("File"));
    QAction *newTab = new QAction(tr("New tab"));
    QAction *newWindow = new QAction(tr("New window"));
    QAction *newPrivateWindow = new QAction(tr("New incognito window"));
    QAction *open = new QAction(tr("Open file"));
    QAction *save = new QAction(tr("Save page"));
    QAction *savePdf = new QAction(tr("Print as PDF"));
    QAction *exit = new QAction(tr("Exit"));
    file->addAction(newTab);
    file->addAction(newWindow);
    file->addAction(newPrivateWindow);
    file->addSeparator();
    file->addAction(open);
    file->addAction(save);
    file->addAction(savePdf);
    file->addSeparator();
    file->addAction(exit);

    QMenu *edit = new QMenu(tr("Edit"));
    QAction *cut = new QAction(tr("Cut"));
    QAction *copy = new QAction(tr("Copy"));
    QAction *paste = new QAction(tr("Paste"));
    QAction *undo = new QAction(tr("Undo"));
    QAction *redo = new QAction(tr("Redo"));
    QAction *find = new QAction(tr("Find"));
    edit->addAction(cut);
    edit->addAction(copy);
    edit->addAction(paste);
    edit->addSeparator();
    edit->addAction(undo);
    edit->addAction(redo);
    edit->addSeparator();
    edit->addAction(find);

    QMenu *view = new QMenu(tr("View"));
    QAction *bookmarks = new QAction(tr("Show bookmarks"));
    QAction *downloads = new QAction(tr("Show downloads"));
    QAction *history = new QAction(tr("Show history"));
    QAction *sidebar = new QAction(tr("Show sidebar"));
    QAction *zoomIn = new QAction(tr("Zoom in"));
    QAction *zoomOut = new QAction(tr("Zoom out"));
    QAction *resetZoom = new QAction(tr("Reset zoom"));
    QAction *pageSource = new QAction(tr("Page source"));
    QAction *fullScreen = new QAction(tr("Full screen"));
    view->addAction(bookmarks);
    view->addAction(downloads);
    view->addAction(history);
    view->addAction(sidebar);
    view->addSeparator();
    view->addAction(zoomIn);
    view->addAction(zoomOut);
    view->addAction(resetZoom);
    view->addSeparator();
    view->addAction(pageSource);
    view->addAction(fullScreen);

    QMenu *tools = new QMenu(tr("Tools"));
    QMenu *help = new QMenu(tr("Help"));

    menu->addMenu(file);
    menu->addMenu(edit);
    menu->addMenu(view);
    menu->addMenu(tools);
    menu->addMenu(help);

    return menu;
}
