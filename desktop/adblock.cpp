#include "adblock.h"

#include <QFile>
#include <QTextStream>

void Trie::insert(const QString &string)
{
    Trie *node = this;
    for (const QChar &c : string) {
        if (!node->children.contains(c))
            node->children.insert(c, new Trie);
        node = node->children[c];
    }
    node->is_end = true;
}

bool Trie::contains(const QString &string)
{
    Trie *node = this;
    for (const QChar &c : string) {
        if (!node->children.contains(c))
            return false;
        node = node->children[c];
    }

    return node->is_end;
}

void Adblock::parse_hosts_file(const QString &path)
{
    QFile file(path);
    if (!file.open(QFile::ReadOnly))
        return;

    QTextStream stream(&file);
    while (!stream.atEnd()) {
        const QString line = stream.readLine();
        if (line.startsWith(QStringLiteral("#")) || line.isEmpty())
            continue;

        const QString domain = line.split(QStringLiteral(" ")).at(1);
        m_root->insert(domain);
    }
}

Adblock::Adblock()
{
    m_root = new Trie;
    parse_hosts_file(QStringLiteral(":assets/adblock/hosts"));
}

bool Adblock::has_match(const QUrl &url)
{
    const QString domain = url.host();
    if (domain.isEmpty())
        return false;

    return m_root->contains(domain);
}
