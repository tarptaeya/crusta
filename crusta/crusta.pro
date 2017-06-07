# ============================================================
# Crusta - Qt5 webengine browser
# Copyright (C) 2017 Anmol Gautam <tarptaeya@gmail.com>
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

QT       += core gui
QT       += webenginewidgets webengine

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = crusta
TEMPLATE = app


SOURCES += main.cpp\
    fullscreennotifier.cpp \
    addresslineedit.cpp \
    presentationmodenotifier.cpp \
    webview.cpp \
    tabwindow.cpp \
    mainview.cpp \
    timenotifier.cpp \
    popup.cpp \
    historymanager.cpp \
    privatewebview.cpp \
    downloadmanager.cpp \
    privatetabwindow.cpp \
    jseditor.cpp \
    codeeditor.cpp \
    downloadwidget.cpp \
    themeeditor.cpp \
    webpage.cpp \
    bookmarkmanager.cpp \
    siteinfo.cpp \
    privateaddresslineedit.cpp \
    privatemainview.cpp

HEADERS  += \
    fullscreennotifier.h \
    addresslineedit.h \
    presentationmodenotifier.h \
    webview.h \
    tabwindow.h \
    mainview.h \
    timenotifier.h \
    popup.h \
    historymanager.h \
    privatewebview.h \
    downloadmanager.h \
    privatetabwindow.h \
    jseditor.h \
    codeeditor.h \
    downloadwidget.h \
    themeeditor.h \
    webpage.h \
    bookmarkmanager.h \
    siteinfo.h \
    privateaddresslineedit.h \
    privatemainview.h

RESOURCES += \
    resource.qrc
