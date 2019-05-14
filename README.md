![Banner](https://github.com/CrustaBrowser/CrustaBrowser/blob/master/banner-transparent.png) 

[![Build Status](https://travis-ci.org/Crusta/CrustaBrowser.svg?branch=master)](https://travis-ci.org/Crusta/CrustaBrowser)
[![Download Crusta Browser](https://img.shields.io/sourceforge/dm/crustabrowser.svg)](https://sourceforge.net/projects/crustabrowser/files/latest/download)
[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

Crusta is a feature rich, modern web-browser, based on powerful Qt5 framework. Crusta aims to be fast, lightweight and powerful browser. Crusta is available for Mac, Linux and Windows.

![Screenshot](https://github.com/Crusta/CrustaBrowser/blob/master/ScreenShots/Screenshot%202019-05-13%20at%202.37.34%20PM.png)

## Features
- Split mode
- Cookie manager
- History and bookmarks manager
- Private browsing
- Extensible sidebar

## Building
**Prerequisite:** Please make sure that Qt5 (>= 5.11) is installed on your system.

After checking that Qt5 is installed on your system, clone the repository using `git clone https://github.com/crusta/crustabrowser` and then from the project root directory execute the following commands  
```console
mkdir -p build
cd build
qmake ..
make
make install
```

## License
Crusta is licensed under the GNU GPLv3 License - see the [LICENSE](https://github.com/Crusta/CrustaBrowser/blob/readme-patch/LICENSE) file for details
