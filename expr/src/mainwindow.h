#pragma once

#include <QMainWindow>
#include <QSplitter>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

    void createWindow();
    void closeEvent(QCloseEvent *event) override;

Q_SIGNALS:
    void mainWindowWillClose();

private:
    QSplitter *m_centralWidget = nullptr;
};
