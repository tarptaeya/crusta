#pragma once

class BrowserWindow;

class Browser
{
public:
    int start(int argc, char **argv);

    BrowserWindow *create_browser_window() const;
};
