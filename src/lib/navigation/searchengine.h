#ifndef CR_SEARCH_ENGINE_H
#define CR_SEARCH_ENGINE_H

#include <QObject>

class SearchEngine : public QObject
{
    Q_OBJECT
public:
    explicit SearchEngine(QObject *parent = nullptr);
};

#endif
