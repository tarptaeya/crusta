/* ============================================================
* Crusta - Qt5 webengine browser
* Copyright (C) 2017 Anmol Gautam <tarptaeya@gmail.com>
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

#ifndef MAINVIEW_H
#define MAINVIEW_H

#include "fullscreennotifier.h"
#include "webview.h"
#include "searchlineedit.h"
#include "addresslineedit.h"
#include "tabwindow.h"
#include "presentationmodenotifier.h"
#include "manager.h"
#include "downloadwidget.h"
#include "jseditor.h"

#include <QObject>
#include <QPoint>
#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QPushButton>
#include <QAction>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>




class MainView:public QMainWindow{
    void closeTab(int);
    void zoomIn();
    void zoomOut();
    void resetZoom();
    void fullScreen();
    void tabBarContext(QPoint);
    void FindText();
    void findFindWidget();
    void hideFindWidget();
    void selectAllText();
    void quit();
    void enterPresentationMode();
    void undoPageAction();
    void redoPageAction();
    void cutPageAction();
    void copyPageAction();
    void pastePageAction();
public:
    QWidget* window=new QWidget();
    QTabWidget* tabWindow=new QTabWidget();
    QVBoxLayout* box=new QVBoxLayout();
    QMenuBar* menubar=new QMenuBar();
    QPushButton* add_btn=new QPushButton();
    QMenu* file_menu=new QMenu();
    QMenu* edit_menu=new QMenu();
    QMenu* view_menu=new QMenu();
    QMenu* history_menu=new QMenu();
    QMenu* bookmark_menu=new QMenu();
    QMenu* download_menu=new QMenu();
    QMenu* tool_menu=new QMenu();
    QMenu* help_menu=new QMenu();
    QMenu* devTools=new QMenu();
    QAction* new_tab_action=new QAction();
    QAction* new_private_tab_action=new QAction();
    QAction* new_window_action=new QAction();
    QAction* open_file=new QAction();
    QAction* save_as_pdf=new QAction();
    QAction* save_page=new QAction();
    QAction* capture_screenshot=new QAction();
    QAction* exit_action=new QAction();
    QAction* undo_action=new QAction();
    QAction* redo_action=new QAction();
    QAction* cut_action=new QAction();
    QAction* copy_action=new QAction();
    QAction* paste_action=new QAction();
    QAction* selectall_action=new QAction();
    QAction* find_action=new QAction();
    QAction* view_page_source_action=new QAction();
    QAction* zoom_in_action=new QAction();
    QAction* zoom_out_action=new QAction();
    QAction* reset_zoom_action=new QAction();
    QAction* presentation_action=new QAction();
    QAction* show_all_history=new QAction();
    QAction* web_inspector_action=new QAction();
    QAction* download_bar=new QAction();
    QAction* runJsCode=new QAction();
    PresentationModeNotifier* p_notifier=new PresentationModeNotifier();
    QAction* fullscreen_action=new QAction();
    int start_findwidget;
    QPushButton* close_findwidget=new QPushButton();
    QWidget* findwidget=new QWidget();
    QHBoxLayout* hbox=new QHBoxLayout();
    QCheckBox* match_case_btn=new QCheckBox();
    QLabel* label=new QLabel();
    QLineEdit* text=new QLineEdit();
    Manager* manager=new Manager();
    DownloadWidget* downloadWidget=new DownloadWidget();
    JsEditor* jsEditor=new JsEditor();
    QPushButton* newtabbtn=new QPushButton();
    MainView();

    void createView();
    void showView();
    void newWindow();
    void createMenuBar();
    void createTabWindow();
    void addNormalTab();
    void addPrivateTab();
    void viewPageSource();
    void saveAsPdf();
    void savePage();
    void showManager();
    void showDownloadBar();
    void showJsCodeEditor();
    void openLocalFile();
    void screenShot();
    void tabAreaDoubleClicked(int index);
    void addNewTabButton();
};




#endif // MAINVIEW_H
