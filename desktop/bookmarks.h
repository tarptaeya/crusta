#pragma once

#include <QAbstractItemModel>
#include <QTreeView>
#include <QWidget>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#define BOOKMARK_MIMETYPE QStringLiteral("application/bookmark.crusta")

class BookmarkTreeNode
{
public:
    enum Type
    {
        Root,
        Folder,
        Address,
    };

    BookmarkTreeNode(Type type = Root, BookmarkTreeNode *parent = nullptr);
    ~BookmarkTreeNode();

    void insert(BookmarkTreeNode *child, int index = -1);
    void remove(BookmarkTreeNode *child);

    QString title;
    QString address;
    QString desc;

    BookmarkTreeNode *parent = nullptr;
    Type type;
    QVector<BookmarkTreeNode *> children;

    inline bool operator==(const BookmarkTreeNode &node) {
        if (type != node.type)
            return false;

        if (children.count() != node.children.count())
            return false;

        switch (type) {
        case Address:
            if (address != node.address)
                return false;
        case Root:
        case Folder:
            for (int i = 0; i < children.count(); i++) {
                if (!(children[i] == node.children[i]))
                    return false;
            }
        }

        return true;
    }
};

class BookmarkModel : public QAbstractItemModel
{
    bool m_is_dirty = false;
    BookmarkTreeNode *m_root_node = nullptr;
    void save();
public:
    explicit BookmarkModel(QObject *parent = nullptr);
    ~BookmarkModel();

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    Qt::DropActions supportedDropActions() const override;
    QStringList mimeTypes() const override;
    QMimeData * mimeData(const QModelIndexList &indexes) const override;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    void add_bookmark(BookmarkTreeNode *parent, BookmarkTreeNode *node, int row = -1);
    void remove_bookmark(BookmarkTreeNode *node);
    BookmarkTreeNode *tree_node(const QModelIndex &index) const;
};

class BookmarkWidget : public QWidget
{
    QTreeView *m_tree_view = nullptr;

    void show_context_menu(const QPoint &pos);
    void open_in_new_tab(const QModelIndex &index);

public:
    explicit BookmarkWidget(QWidget *parent = nullptr);    
};

class XbelReader : public QXmlStreamReader
{
public:
    explicit XbelReader();
    BookmarkTreeNode *read(const QString &file_name);
    BookmarkTreeNode *read(QIODevice *device);

private:
    void readXBEL(BookmarkTreeNode *node);
    void readTitle(BookmarkTreeNode *node);
    void readFolder(BookmarkTreeNode *node);
    void readDescription(BookmarkTreeNode *node);
    void readBookmarkTreeNode(BookmarkTreeNode *node);
};

class XbelWriter : public QXmlStreamWriter
{
public:
    explicit XbelWriter();
    bool write(const QString &file_name, BookmarkTreeNode *node);
    bool write(QIODevice *device, BookmarkTreeNode *node);

private:
    void write_item(BookmarkTreeNode *node);
};
