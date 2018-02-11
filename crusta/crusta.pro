# ============================================================
# Crusta - Qt5 webengine browser
# Copyright (C) 2017 Anmol Gautam <anmol@crustabrowser.com>
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
# ============================================================ */

#-------------------------------------------------
#
# Project created by QtCreator 2017-03-22T20:08:06
#
#-------------------------------------------------

QT       += core gui charts
QT       += location webenginewidgets webengine texttospeech multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = crusta
TEMPLATE = app


SOURCES += main.cpp\
    mainview.cpp \
    privatemainview.cpp \
    lib/utils/codeeditor.cpp \
    lib/webview/webpage.cpp \
    lib/webview/webview.cpp \
    lib/utils/jseditor.cpp \
    lib/webview/popup.cpp \
    lib/webview/privatewebview.cpp \
    lib/utils/timenotifier.cpp \
    lib/utils/themeeditor.cpp \
    lib/utils/crustaspeech.cpp \
    lib/utils/siteinfo.cpp \
    lib/utils/fullscreennotifier.cpp \
    lib/utils/permissions.cpp \
    lib/managers/historymanager.cpp \
    lib/managers/downloadmanager.cpp \
    lib/managers/downloadwidget.cpp \
    lib/managers/bookmarkmanager.cpp \
    lib/utils/presentationmodenotifier.cpp \
    lib/tabwidget/tabwindow.cpp \
    lib/tabwidget/privatetabwindow.cpp \
    lib/tabwidget/addresslineedit.cpp \
    lib/tabwidget/privateaddresslineedit.cpp \
    lib/speeddial/speeddial.cpp \
    lib/sidepanel/sidepane.cpp

HEADERS  += \
    mainview.h \
    privatemainview.h \
    lib/utils/codeeditor.h \
    lib/webview/webpage.h \
    lib/webview/webview.h \
    lib/utils/jseditor.h \
    lib/webview/popup.h \
    lib/webview/privatewebview.h \
    lib/utils/timenotifier.h \
    lib/utils/themeeditor.h \
    lib/utils/crustaspeech.h \
    lib/utils/siteinfo.h \
    lib/utils/fullscreennotifier.h \
    lib/utils/permissions.h \
    lib/managers/historymanager.h \
    lib/managers/downloadmanager.h \
    lib/managers/downloadwidget.h \
    lib/managers/bookmarkmanager.h \
    lib/utils/presentationmodenotifier.h \
    lib/tabwidget/tabwindow.h \
    lib/tabwidget/privatetabwindow.h \
    lib/tabwidget/addresslineedit.h \
    lib/tabwidget/privateaddresslineedit.h \
    lib/speeddial/speeddial.h \
    lib/sidepanel/sidepane.h

RESOURCES += \
    resource.qrc

RC_ICONS += crusta.ico
