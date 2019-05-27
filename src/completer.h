#pragma once

#include <QObject>
#include <QLineEdit>

class Completer : public QObject
{
public:
    explicit Completer(QObject *parent = nullptr);

    static void attachCompleterTo(QLineEdit *lineEdit);
};
