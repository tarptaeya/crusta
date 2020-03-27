#include "bookmarks.h"
#include "browser.h"
#include "browser_window.h"
#include "tab.h"
#include "webview.h"

#include <QBuffer>
#include <QDebug>
#include <QDir>
#include <QDataStream>
#include <QFile>
#include <QMenu>
#include <QMimeData>
#include <QStandardPaths>
#include <QStyleFactory>
#include <QVBoxLayout>

BookmarkTreeNode::BookmarkTreeNode(BookmarkTreeNode::Type type, BookmarkTreeNode *parent)
    : parent(parent)
    , type(type)
{
    if (parent)
        parent->insert(this);
}

BookmarkTreeNode::~BookmarkTreeNode()
{
    if (parent)
        parent->remove(this);
    parent = nullptr;
    qDeleteAll(children);
}

void BookmarkTreeNode::insert(BookmarkTreeNode *child, int index)
{
    if (child->parent)
        child->parent->remove(child);
    child->parent = this;

    if (index == -1)
        index = children.count();
    children.insert(index, child);
}

void BookmarkTreeNode::remove(BookmarkTreeNode *child)
{
    child->parent = nullptr;
    children.removeAll(child);
}

void BookmarkModel::save()
{
    QDir standardLocation(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    const QString path = standardLocation.absoluteFilePath(QStringLiteral("bookmarks.xbel"));
    XbelWriter writer;
    writer.write(path, m_root_node);
}

BookmarkModel::BookmarkModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    QDir standardLocation(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    const QString path = standardLocation.absoluteFilePath(QStringLiteral("bookmarks.xbel"));
    if (QFile::exists(path)) {
        XbelReader reader;
        m_root_node = reader.read(path);
        if (reader.error() != QXmlStreamReader::NoError) {
            qDebug() << "Error loading bookmarks file" << reader.errorString();
        }
    } else {
        m_root_node = new BookmarkTreeNode(BookmarkTreeNode::Root);
    }
}

BookmarkModel::~BookmarkModel()
{
    if (m_is_dirty)
        save();
    delete m_root_node;
}

QVariant BookmarkModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0: return QStringLiteral("Title");
        case 1: return QStringLiteral("Address");
        case 2: return QStringLiteral("Description");
        }
    }

    return QAbstractItemModel::headerData(section, orientation, role);
}

QVariant BookmarkModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    BookmarkTreeNode *bookmark_node = tree_node(index);

    switch (role) {
    case Qt::DisplayRole:
        switch (index.column()) {
        case 0: return bookmark_node->title;
        case 1: return bookmark_node->address;
        case 2: return bookmark_node->desc;
        }
        break;
    case Qt::DecorationRole:
        if (index.column() == 0) {
            if (bookmark_node->type == BookmarkTreeNode::Folder)
                return QIcon::fromTheme(QStringLiteral("folder"));
            return QIcon::fromTheme(QStringLiteral("text-html"));
        }
        break;
    }

    return QVariant();
}

int BookmarkModel::columnCount(const QModelIndex &parent) const
{
    return parent.column() > 0 ? 0 : 3;
}

int BookmarkModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        return m_root_node->children.count();

    BookmarkTreeNode *node = static_cast<BookmarkTreeNode *>(parent.internalPointer());
    return node->children.count();
}

QModelIndex BookmarkModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    BookmarkTreeNode *parent_node;
    if (!parent.isValid())
        parent_node = m_root_node;
    else
        parent_node = tree_node(parent);

    if (row >= parent_node->children.count())
        return QModelIndex();
    return createIndex(row, column, parent_node->children.at(row));
}

QModelIndex BookmarkModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    BookmarkTreeNode *child_node = tree_node(child);
    BookmarkTreeNode *parent_node = child_node->parent;

    if (!parent_node || parent_node == m_root_node)
        return QModelIndex();

    int row = parent_node->parent->children.indexOf(parent_node);
    return createIndex(row, 0, parent_node);
}

Qt::ItemFlags BookmarkModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return QAbstractItemModel::flags(index);

    BookmarkTreeNode *node = tree_node(index);
    if (node->type == BookmarkTreeNode::Address && index.column() != 2)
        return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | QAbstractItemModel::flags(index);

    return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

Qt::DropActions BookmarkModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

QStringList BookmarkModel::mimeTypes() const
{
    QStringList types;
    types << BOOKMARK_MIMETYPE;
    return types;
}

QMimeData *BookmarkModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mime_data = new QMimeData;
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    for (const QModelIndex &index : indexes) {
        if (index.column() != 0 || !index.isValid() || indexes.contains(index.parent()))
            continue;

        QByteArray encoded_data;
        QBuffer buffer(&encoded_data);
        buffer.open(QBuffer::ReadWrite);
        stream << index.row() << (qintptr) index.internalPointer();
    }

    mime_data->setData(BOOKMARK_MIMETYPE, data);
    return mime_data;
}

bool BookmarkModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    Q_UNUSED(row)

    if (action == Qt::IgnoreAction)
        return true;

    if (!data->hasFormat(BOOKMARK_MIMETYPE) || column > 0)
        return false;

    BookmarkTreeNode *parent_node = tree_node(parent);
    if (parent_node->type != BookmarkTreeNode::Folder)
        return false;

    QByteArray ba = data->data(BOOKMARK_MIMETYPE);
    QDataStream stream(&ba, QIODevice::ReadOnly);
    if (stream.atEnd())
        return false;

    while (!stream.atEnd()) {
        int row;
        qintptr ptr;
        stream >> row >> ptr;

        QModelIndex index = createIndex(row, 0, (void *) ptr);
        BookmarkTreeNode *node = tree_node(index);
        add_bookmark(tree_node(parent), node, row);
    }

    return true;
}

bool BookmarkModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    const QString val = value.toString();
    if (val.isEmpty())
        return false;

    BookmarkTreeNode *node = tree_node(index);

    switch (role) {
    case Qt::EditRole:
    case Qt::DisplayRole:
        if (index.column() == 0) {
            node->title = val;
            break;
        } else if (index.column() == 1) {
            node->address = val;
            break;
        } else if (index.column() == 2) {
            node->desc = val;
            break;
        }
    default:
        return false;
    }

    m_is_dirty = true;
    return true;
}

void BookmarkModel::add_bookmark(BookmarkTreeNode *parent, BookmarkTreeNode *node, int row)
{
    if (!parent)
        parent = m_root_node;

    if (row == -1)
        row = parent->children.count();

    if (node->parent) {
        BookmarkTreeNode *parent = node->parent;
        int row = parent->children.indexOf(node);
        beginRemoveRows(QModelIndex(), row, row);
        parent->remove(node);
        endRemoveRows();
    }

    beginInsertRows(QModelIndex(), row, row);
    parent->insert(node);
    endInsertRows();

    m_is_dirty = true;
}

void BookmarkModel::remove_bookmark(BookmarkTreeNode *node)
{
    if (!node->parent)
        return;

    BookmarkTreeNode *parent = node->parent;
    int row = parent->children.indexOf(node);
    beginRemoveRows(QModelIndex(), row, row);
    parent->remove(node);
    endRemoveRows();

    delete node;

    m_is_dirty = true;
}

BookmarkTreeNode *BookmarkModel::tree_node(const QModelIndex &index) const
{
    return static_cast<BookmarkTreeNode *>(index.internalPointer());
}

void BookmarkWidget::show_context_menu(const QPoint &pos)
{
    QModelIndex index = m_tree_view->indexAt(pos);

    QMenu *menu = new QMenu;

    if (index.isValid()) {
        BookmarkTreeNode *node = browser->bookmark_model()->tree_node(index);

        if (node->type == BookmarkTreeNode::Address) {
            QAction *open_in_tab = menu->addAction(QStringLiteral("Open in new tab"));
            QAction *open_in_window = menu->addAction(QStringLiteral("Open in new window"));

            menu->addSeparator();

            connect(open_in_tab, &QAction::triggered, [this, index] {
                open_in_new_tab(index);
            });

            connect(open_in_window, &QAction::triggered, [node] {
                BrowserWindow *window = browser->create_browser_window();
                WebTab *tab = dynamic_cast<WebTab *>(window->tabs().at(0));
                if (!tab) return ;

                tab->webview()->load(node->address);
            });
        } else {
            QAction *add_folder = menu->addAction(QStringLiteral("Add folder"));
            connect(add_folder, &QAction::triggered, [node] {
                BookmarkTreeNode *folder = new BookmarkTreeNode(BookmarkTreeNode::Folder);
                folder->title = QStringLiteral("New Folder");
                browser->bookmark_model()->add_bookmark(node, folder);
            });
        }

        menu->addSeparator();

        QAction *remove = menu->addAction(QStringLiteral("Remove"));
        connect(remove, &QAction::triggered, [node] {
            browser->bookmark_model()->remove_bookmark(node);
        });
    } else {
        QAction *add_folder = menu->addAction(QStringLiteral("Add folder"));
        connect(add_folder, &QAction::triggered, [] {
            BookmarkTreeNode *folder = new BookmarkTreeNode(BookmarkTreeNode::Folder);
            folder->title = QStringLiteral("New Folder");
            browser->bookmark_model()->add_bookmark(nullptr, folder);
        });
    }


#ifdef Q_OS_MACOS
    menu->setStyle(QStyleFactory::create(QStringLiteral("macintosh")));
#endif
    menu->exec(m_tree_view->mapToGlobal(pos));
}

void BookmarkWidget::open_in_new_tab(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    BookmarkTreeNode *node = browser->bookmark_model()->tree_node(index);
    if (node->type != BookmarkTreeNode::Address || index.column() == 2) return;

    QWidget *parent_widget = this;
    while (parent_widget->parentWidget()) {
        parent_widget = parent_widget->parentWidget();
    }

    BrowserWindow *window = dynamic_cast<BrowserWindow *>(parent_widget);
    if (!window) return ;

    WebTab *tab = dynamic_cast<WebTab *>(window->add_new_tab());
    if (!tab) return;

    tab->webview()->load(node->address);
}

BookmarkWidget::BookmarkWidget(QWidget *parent)
    : QWidget(parent)
{
    m_tree_view = new QTreeView;
    m_tree_view->setModel(browser->bookmark_model());

    m_tree_view->setDragEnabled(true);
    m_tree_view->setAcceptDrops(true);
    m_tree_view->setDropIndicatorShown(true);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setContentsMargins(0, 0, 0, 0);
    setLayout(vbox);
    vbox->addWidget(m_tree_view);

    m_tree_view->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_tree_view, &QTreeView::customContextMenuRequested, this, &BookmarkWidget::show_context_menu);
    connect(m_tree_view, &QTreeView::doubleClicked, this, &BookmarkWidget::open_in_new_tab);
}

XbelReader::XbelReader()
{
}

BookmarkTreeNode *XbelReader::read(const QString &file_name)
{
    QFile file(file_name);
    if (!file.exists())
        return new BookmarkTreeNode(BookmarkTreeNode::Root);

    file.open(QFile::ReadOnly);
    return read(&file);
}

BookmarkTreeNode *XbelReader::read(QIODevice *device)
{
    BookmarkTreeNode *node = new BookmarkTreeNode(BookmarkTreeNode::Root);
    setDevice(device);
    if (readNextStartElement()) {
        QString version = attributes().value(QLatin1String("version")).toString();
        if (name() == QLatin1String("xbel") && (version.isEmpty() || version == QLatin1String("1.0"))) {
            readXBEL(node);
        } else {
            raiseError(QObject::tr("The file is not an XBEL version 1.0 file."));
        }
    }

    return node;
}

void XbelReader::readXBEL(BookmarkTreeNode *node)
{
    while (readNextStartElement()) {
        if (name() == QLatin1String("folder"))
            readFolder(node);
        else if (name() == QLatin1String("bookmark"))
            readBookmarkTreeNode(node);
        else
            skipCurrentElement();
    }
}

void XbelReader::readTitle(BookmarkTreeNode *node)
{
    node->title = readElementText();
}

void XbelReader::readFolder(BookmarkTreeNode *node)
{
    BookmarkTreeNode *folder = new BookmarkTreeNode(BookmarkTreeNode::Folder, node);
    while (readNextStartElement()) {
        if (name() == QLatin1String("title"))
            readTitle(folder);
        else if (name() == QLatin1String("folder"))
            readFolder(folder);
        else if (name() == QLatin1String("bookmark"))
            readBookmarkTreeNode(folder);
        else if (name() == QLatin1String("desc"))
            readDescription(folder);
        else
            skipCurrentElement();
    }
}

void XbelReader::readDescription(BookmarkTreeNode *node)
{
    node->desc = readElementText();
}

void XbelReader::readBookmarkTreeNode(BookmarkTreeNode *node)
{
    BookmarkTreeNode *bookmark = new BookmarkTreeNode(BookmarkTreeNode::Address, node);
    bookmark->address = attributes().value(QLatin1String("href")).toString();
    while (readNextStartElement()) {
        if (name() == QLatin1String("title"))
            readTitle(bookmark);
        else if (name() == QLatin1String("desc"))
            readDescription(bookmark);
        else
            skipCurrentElement();
    }

    if (bookmark->title.isEmpty())
        bookmark->title = QObject::tr("Unknown title");
}

XbelWriter::XbelWriter()
{
}

bool XbelWriter::write(const QString &file_name, BookmarkTreeNode *node)
{
    QFile file(file_name);
    if (!node || !file.open(QFile::WriteOnly))
        return false;
    return write(&file, node);
}

bool XbelWriter::write(QIODevice *device, BookmarkTreeNode *node)
{
    setDevice(device);

    writeStartDocument();
    writeDTD(QLatin1String("<!DOCTYPE xbel>"));
    writeStartElement(QLatin1String("xbel"));
    writeAttribute(QLatin1String("version"), QLatin1String("1.0"));
    if (node->type == BookmarkTreeNode::Root) {
        for (int i = 0; i < node->children.count(); i++) {
            write_item(node->children.at(i));
        }
    } else {
        write_item(node);
    }

    writeEndDocument();
    return true;
}

void XbelWriter::write_item(BookmarkTreeNode *node)
{
    switch (node->type) {
    case BookmarkTreeNode::Folder:
        writeStartElement(QLatin1String("folder"));
        writeTextElement(QLatin1String("title"), node->title);
        for (int i = 0; i < node->children.count(); i++) {
            write_item(node->children.at(i));
        }
        writeEndElement();
        break;
    case BookmarkTreeNode::Address:
        writeStartElement(QLatin1String("bookmark"));
        if (!node->address.isEmpty())
            writeAttribute(QLatin1String("href"), node->address);
        writeTextElement(QLatin1String("title"), node->title);
        writeTextElement(QLatin1String("desc"), node->desc);
        writeEndElement();
    default:
        break;
    }
}
