#pragma once

#include <QAbstractTableModel>
#include <QTreeView>
#include <QWidget>

struct SearchEngine
{
    QString name;
    QString description;
    QString query_url;
    QString suggestion_url;

    bool operator==(const SearchEngine &other)
    {
        return name == other.name
                && description == other.description
                && query_url == other.query_url
                && suggestion_url == other.suggestion_url;
    }

    friend QDataStream &operator<<(QDataStream &out, const SearchEngine &engine)
    {
        out << engine.name << engine.description << engine.query_url << engine.suggestion_url;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, SearchEngine &engine)
    {
        in >> engine.name >> engine.description >> engine.query_url >> engine.suggestion_url;
        return in;
    }
};

Q_DECLARE_METATYPE(SearchEngine)

class SearchModel : public QAbstractTableModel
{
    QVector<SearchEngine> m_engines;
    int m_default_index = 0;
public:
    explicit SearchModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    SearchEngine default_engine() const;
    void set_default(int index);

    void add_engine(SearchEngine engine);
    void remove_engine(QModelIndex index);
};

class SearchWidget : public QWidget
{
    QTreeView *m_tree_view = nullptr;
public:
    explicit SearchWidget(QWidget *parent = nullptr);
};
