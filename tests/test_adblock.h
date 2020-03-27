#pragma once

#include <QtTest>

class TestAdblock : public QObject
{
    Q_OBJECT
private slots:
    void test_trie();
};
