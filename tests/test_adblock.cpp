#include "test_adblock.h"
#include "adblock.h"

void TestAdblock::test_trie()
{
    Trie *trie = new Trie;
    trie->insert("aabb");
    trie->insert("aaabb");
    trie->insert("bbb");

    QVERIFY(trie->contains("aaab") == false);
    QVERIFY(trie->contains("bbb") == true);
    QVERIFY(trie->contains("aabb") == true);
}

QTEST_MAIN(TestAdblock)
