#include "HashTable.h"
#include <cassert>
#include <iostream>

void basic() {
    std::cout << "\n --- BASIC --- \n";

    {
        HashTable<int> ht(4);
        ht.insert("cat", 3);
        ht.insert("cat", 3);
        ht.insert("dog", 2);
        ht.insert("dog", 3);
        ht.insert("god", 3);
        ht.insert("man", 1);
        ht.insert("aaaaaaaa", -112);
        ht.insert("bob", 1);

        ht.print(std::cout);

        assert(ht.contains("cat"));
        assert(ht.contains("dog"));
        assert(!ht.contains("zzz"));
        assert(ht.getCollisionsNumber() >= 1);

        assert(ht.remove("dog"));
        assert(!ht.contains("dog"));
        assert(!ht.remove("dog"));
    }

    {
        HashTable<int> ht(100);
        ht.insert("cat", 3);
        ht.insert("cat", 3);
        ht.insert("dog", 2);
        ht.insert("dog", 3);
        ht.insert("god", 3);
        ht.insert("man", 1);
        ht.insert("aaaaaaaa", -112);
        ht.insert("bob", 1);

        assert(ht.getCollisionsNumber() == 0);
    }
}

void insert_remove() {
    std::cout << "\n --- INSERT / REMOVE --- \n";

    {
        HashTable<int> ht(4);
        ht.insert("cat", 3);
        ht.insert("dog", 2);
        ht.insert("man", 1);
        ht.insert("bob", 1);

        ht.remove("dog");
        ht.print(std::cout);
    }

    {
        HashTable<int> ht(4);
        ht.insert("cat", 3);
        ht.insert("dog", 2);
        ht.insert("man", 1);
        ht.insert("bob", 1);

        assert(ht.remove("dog") == true);
        assert(ht.remove("dog") == false);
    }
}

void hash() {
    std::cout << "\n --- HASH --- \n";

    {
        HashTable<int> ht(1);

        ht.insert("a", 1);
        ht.insert("b", 2);
        ht.insert("c", 3);
        ht.insert("d", 4);
        ht.insert("d", 5);

        assert(ht.getCollisionsNumber() == 3);
    }

    {
        HashTable<int> ht(17);

        char a1[] = "hello";
        char a2[] = "hello";

        assert(ht.getHash(a1) == ht.getHash(a2));
    }
}

void move() {
    std::cout << "\n --- MOVE --- \n";

    {
        HashTable<int> a(8);
        a.insert("x", 10);
        a.insert("y", 20);

        HashTable<int> b(std::move(a));
        assert(b.contains("x"));
        assert(b.contains("y"));

        HashTable<int> c(4);
        c.insert("temp", 1);
        c = std::move(b);

        assert(c.contains("x"));
        assert(c.contains("y"));
        assert(!c.contains("temp"));
    }
}

void edge_cases() {
    std::cout << "\n --- EDGE CASES --- \n";

    {
        HashTable<int> ht(8);

        ht.insert("cat", 3);
        ht.insert("cat", 999);

        const int* v = ht.search("cat");
        assert(v != nullptr);
        assert(*v == 3);
    }

    {
        HashTable<int> ht(8);

        char key[] = "dog";
        ht.insert(key, 10);

        key[0] = 'l';

        assert(ht.contains("dog"));
        assert(!ht.contains("log"));
    }

    {
        bool thrown = false;
        try {
            HashTable<int> bad(0);
        } catch (const std::invalid_argument&) {
            thrown = true;
        }
        assert(thrown);
    }

    {
        HashTable<int> ht(8);

        bool thrown = false;
        try {
            ht.insert(nullptr, 1);
        } catch (const std::invalid_argument&) {
            thrown = true;
        }
        assert(thrown);

        assert(ht.search(nullptr) == nullptr);
        assert(ht.contains(nullptr) == false);
        assert(ht.remove(nullptr) == false);
    }
}

int main() {
    basic();
    insert_remove();
    hash();
    move();
    edge_cases();
}
