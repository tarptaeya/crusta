#ifndef CR_SEARCH_ENGINE_H
#define CR_SEARCH_ENGINE_H

#include <QObject>

class WebPage;

class SearchEngine : public QObject
{
    Q_OBJECT
public:
    explicit SearchEngine(WebPage *page, QObject *parent = nullptr);

    Q_INVOKABLE void openSearchData(const QString &name, const QString &queryUrl, const QString &suggestionUrl);

private:
    WebPage *m_page = nullptr;
};

#endif
