#pragma once

#include <QLineEdit>

class OmniBar : public QLineEdit
{
    Q_OBJECT
public:
    explicit OmniBar(QWidget *parent = nullptr);
    void setUrl(const QUrl &url);
};
