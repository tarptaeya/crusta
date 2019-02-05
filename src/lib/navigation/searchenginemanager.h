#ifndef CR_SEARCH_ENGINE_MANAGER_H
#define CR_SEARCH_ENGINE_MANAGER_H

#include <QWidget>

class SearchEngineManager : public QWidget
{
public:
    explicit SearchEngineManager(QWidget *parent = nullptr);

    QStringList engines() const;
    void addSearchEngine(const QStringList &engine);
    QString getQuery(const QString &query) const;

private:
    QStringList m_engines;
};

#endif
