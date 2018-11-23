#pragma once

#include "toolbarpopup.h"
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>

class BookmarkPopup : public ToolBarPopup
{
public:
    explicit BookmarkPopup(QWidget *parent = nullptr);
private:
    QLineEdit *m_titleEntry = nullptr;
    QComboBox *m_folder = nullptr;
    QPushButton *m_saveBtn = nullptr;
    QPushButton *m_otherBtn = nullptr;
    QPushButton *m_cancelBtn = nullptr;
};
