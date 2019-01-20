#include <QFile>

QString readFile(const QString &path)
{
    QString fileContents;
    QFile file(path);
    if (!file.open(QFile::ReadOnly)) {
        return fileContents;
    }
    fileContents = QString::fromUtf8(file.readAll());
    file.close();
    return fileContents;
}
