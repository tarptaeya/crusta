#pragma once

#include <QObject>
#include <QWidget>

class Bookmarks : public QObject
{
public:
    explicit Bookmarks(QObject *parent = nullptr);

    static QWidget *popupWidget();
};
