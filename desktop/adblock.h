#pragma once

#include <QHash>
#include <QUrl>

struct Trie
{
    bool is_end = false;
    QHash<QChar, Trie *> children;

    void insert(const QString &string);
    bool contains(const QString &string);
};

class Adblock
{
    Trie *m_root = nullptr;
    void parse_hosts_file(const QString &path);
public:
    Adblock();
    bool allow_url(const QUrl &url);
};
