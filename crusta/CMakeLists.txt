# ============================================================
# Crusta - Qt5 webengine browser
# Copyright (C) 2017-2018 Anmol Gautam <anmol@crustabrowser.com>
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


project( crusta )
set(CMAKE_PREFIX_PATH $ENV{HOME}/Qt/5.10.0/gcc_64)
cmake_minimum_required( VERSION 3.0 )
set ( CMAKE_BUILD_TYPE Release )
add_definitions ( -Wall )
find_package ( Qt5Core REQUIRED )
find_package ( Qt5Widgets REQUIRED )

include_directories (
	${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
	${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR} ${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR} ${QT_QTCHARTS_INCLUDE_DIR} ${QT_QTLOCATION_INCLUDE_DIR} ${QT_QTWEBENGINEWIDGETS_INCLUDE_DIR} ${QT_QTWEBENGINE_INCLUDE_DIR} ${QT_QTTEXTTOSPEECH_INCLUDE_DIR} ${QT_QTMULTIMEDIA_INCLUDE_DIR}
	)
set ( EXECUTABLE_OUTPUT_PATH $$PWD/../out )
set ( crusta_HDRS
	
	)

set ( crusta_SRCS
	main.cpp
	mainview.cpp
	privatemainview.cpp
	lib/utils/codeeditor.cpp
	lib/webview/webpage.cpp
	lib/webview/webview.cpp
	lib/utils/jseditor.cpp
	lib/webview/popup.cpp
	lib/webview/privatewebview.cpp
	lib/utils/timenotifier.cpp
	lib/utils/themeeditor.cpp
	lib/utils/crustaspeech.cpp
	lib/utils/siteinfo.cpp
	lib/utils/fullscreennotifier.cpp
	lib/utils/permissions.cpp
	lib/managers/historymanager.cpp
	lib/managers/downloadmanager.cpp
	lib/managers/downloadwidget.cpp
	lib/managers/bookmarkmanager.cpp
	lib/utils/presentationmodenotifier.cpp
	lib/tabwidget/tabwindow.cpp
	lib/tabwidget/privatetabwindow.cpp
	lib/tabwidget/addresslineedit.cpp
	lib/tabwidget/privateaddresslineedit.cpp
	lib/speeddial/speeddial.cpp
	lib/sidepanel/sidepane.cpp
    lib/helpers/EnvironmentInfo.cpp)

set ( crusta_RSCS
	resource.qrc
	)
QT5_ADD_RESOURCES(RSCS ${crusta_RSCS})

set ( crusta_MOCS
	mainview.h
	privatemainview.h
	lib/utils/codeeditor.h
	lib/webview/webpage.h
	lib/webview/webview.h
	lib/utils/jseditor.h
	lib/webview/popup.h
	lib/webview/privatewebview.h
	lib/utils/timenotifier.h
	lib/utils/themeeditor.h
	lib/utils/crustaspeech.h
	lib/utils/siteinfo.h
	lib/utils/fullscreennotifier.h
	lib/utils/permissions.h
	lib/managers/historymanager.h
	lib/managers/downloadmanager.h
	lib/managers/downloadwidget.h
	lib/managers/bookmarkmanager.h
	lib/utils/presentationmodenotifier.h
	lib/tabwidget/tabwindow.h
	lib/tabwidget/privatetabwindow.h
	lib/tabwidget/addresslineedit.h
	lib/tabwidget/privateaddresslineedit.h
	lib/speeddial/speeddial.h
	lib/sidepanel/sidepane.h
    lib/helpers/EnvironmentInfo.h
    )
QT5_WRAP_CPP(MOCS ${crusta_MOCS})

add_executable ( crusta ${crusta_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} )
target_link_libraries ( crusta  ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTCHARTS_LIBRARY} ${QT_QTLOCATION_LIBRARY} ${QT_QTWEBENGINEWIDGETS_LIBRARY} ${QT_QTWEBENGINE_LIBRARY} ${QT_QTTEXTTOSPEECH_LIBRARY} ${QT_QTMULTIMEDIA_LIBRARY} )
qt5_use_modules (crusta Widgets WebEngineWidgets WebEngine Multimedia Location Core Gui TextToSpeech)