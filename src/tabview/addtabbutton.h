#pragma once

#include <QPushButton>

class AddTabButton : public QPushButton
{
public:
    explicit AddTabButton(QWidget *parent = nullptr);
    void setSide(int side);
};
