#include "browser_window.h"

BrowserWindow::BrowserWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setAttribute(Qt::WA_DeleteOnClose, true);
}
