#ifndef CR_QML_WEBPAGE_H
#define CR_QML_WEBPAGE_H

#include <QWebEnginePage>

class QmlWebPage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title CONSTANT)
    Q_PROPERTY(QString url READ url CONSTANT)

public:
    explicit QmlWebPage(QWebEnginePage *page, QObject *parent = nullptr);

    Q_INVOKABLE void load(const QString &url);

private:
    QString title() const;
    QString url() const;

    QWebEnginePage *m_page = nullptr;
};

#endif
