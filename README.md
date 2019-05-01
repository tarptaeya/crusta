![Banner](https://github.com/CrustaBrowser/CrustaBrowser/blob/master/banner.png) 

[![Build Status](https://travis-ci.org/Crusta/CrustaBrowser.svg?branch=master)](https://travis-ci.org/Crusta/CrustaBrowser)
[![Download Crusta Browser](https://img.shields.io/sourceforge/dm/crustabrowser.svg)](https://sourceforge.net/projects/crustabrowser/files/latest/download)
[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

Chat: `#crusta` on freenode

## About

Crusta is a feature rich web-browser, based on Qt5 and is licenced under GNU GPLv3. Crusta is available for all major desktop platforms (Linux, Mac and Windows) and will soon be available for mobile platforms.

**Note** The code for version 1 is available in the v1 branch of this repo, version 2 will be complete redesign so that code is removed from the master branch.

## Some interesting features
(as in v1, v2 is under development)
- Split tabs
- Text to speech
- Side panel
- ~~Web PDF viewer (using pdf.js)~~ (QWebEngine 5.12.3 supports chromium's pdf viewer)

## Try Crusta
- Clone the repository `git clone https://github.com/crusta/crustabrowser/`.
- Make build dir and change current dir to build `mkdir build && cd build`.
- Run CMake `cmake -DCMAKE_GENERATOR_PLATFORM=x64 ..`.
- Build using `make` if unix else `msbuild ALL_BUILD.vcxproj`.
- Run using `./Crusta` if unix else `Debug\Crusta.exe`.
