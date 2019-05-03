#pragma once

#include <QLineEdit>
#include <QToolBar>
#include <QToolButton>

class ToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit ToolBar(QWidget *parent = nullptr);

    void setUrl(const QUrl &url);
    void loadStarted();
    void loadFinished();

Q_SIGNALS:
    void backRequested();
    void forwardRequested();

private:
    QToolButton *m_backButton = nullptr;
    QToolButton *m_forwardButton = nullptr;
    QToolButton *m_refreshButton = nullptr;
    QToolButton *m_homeButton = nullptr;
    QToolButton *m_bookmarkButton = nullptr;
    QToolButton *m_menuButton = nullptr;

    QLineEdit *m_addressBar = nullptr;
};
