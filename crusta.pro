# ============================================================
# Crusta - Qt5 webengine browser
# Copyright (C) 2017-2018 Anmol Gautam <tarptaeya@gmail.com>
#
# THIS FILE IS A PART OF CRUSTA
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
# ============================================================

QT += core gui widgets webengine webenginewidgets webchannel sql

DEFINES += QT_DEPRECIATED_WARNINGS

SOURCES += \
    src/main.cpp \
    src/bootstrap/appmanager.cpp \
    src/bootstrap/settings.cpp \
    src/bootstrap/database.cpp \
    src/tabview/tabwidget.cpp \
    src/tabview/tab.cpp \
    src/tabview/tabbar.cpp \
    src/tabview/stackedwidget.cpp \
    src/tabview/tabbarbutton.cpp \
    src/tabview/tablist.cpp \
    src/tabview/tablistitem.cpp \
    src/tabview/omnibar/omnibar.cpp \
    src/window/window.cpp \
    src/window/centralwidget.cpp \
    src/window/menubar.cpp \
    src/window/statusbar.cpp \
    src/webview/webview.cpp \
    src/webview/webpage.cpp \
    src/sidebar/sidebar.cpp \
    src/sidebar/sidebarbutton.cpp \
    src/sidebar/sidebarpopup.cpp \
    src/utils/dimensions.cpp \
    src/utils/strings.cpp \
    src/utils/layouts.cpp \
    src/pages/externaljsobject.cpp \
    src/pages/scripts.cpp \
    src/pages/speeddial/speeddial.cpp \
    src/extra/qclickablelabel.cpp \
    src/data/historyitem.cpp \
    src/data/speeddialitem.cpp \
    src/data/abstractdataitem.cpp

HEADERS += \
    src/bootstrap/appmanager.h \
    src/bootstrap/settings.h \
    src/bootstrap/database.h \
    src/tabview/tabwidget.h \
    src/tabview/tab.h \
    src/tabview/tabbar.h \
    src/tabview/stackedwidget.h \
    src/tabview/tabbarbutton.h \
    src/tabview/tablist.h \
    src/tabview/tablistitem.h \
    src/tabview/omnibar/omnibar.h \
    src/window/window.h \
    src/window/centralwidget.h \
    src/window/menubar.h \
    src/window/statusbar.h \
    src/webview/webview.h \
    src/webview/webpage.h \
    src/sidebar/sidebar.h \
    src/sidebar/sidebarbutton.h \
    src/sidebar/sidebarpopup.h \
    src/utils/dimensions.h \
    src/utils/strings.h \
    src/utils/layouts.h \
    src/pages/externaljsobject.h \
    src/pages/scripts.h \
    src/pages/speeddial/speeddial.h \
    src/extra/qclickablelabel.h \
    src/data/historyitem.h \
    src/data/speeddialitem.h \
    src/data/abstractdataitem.h

RESOURCES += \
    assets/assets.qrc \

TEMPLATE = app
TARGET = crusta

unix:!macx {
target.path = /usr/local/bin
desktop.path = /usr/share/applications
desktop.files += crusta.desktop
INSTALLS += target desktop
}
