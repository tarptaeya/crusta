#pragma once
#include <QString>
#include <QFile>
#include <QBuffer>
#include <QIcon>

QString readFile(const QString &fileName);
QByteArray convertIconToByteArray(const QIcon &icon);
