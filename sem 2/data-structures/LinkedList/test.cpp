#include <cassert>
#include <stdexcept>
#include <string>
#include "SinglyOrderedList.h"

void test_essential() {

    {   // Constructors
       SinglyOrderedList<int> l1{};

       SinglyOrderedList<int> l2{3};
       assert(!l2.isEmpty() && l2.head() == 3 && l2.tail() == 3);

       SinglyOrderedList<int> l3(l2);
       assert(l3 == l2);
    }

    {   // Copy & Move
        SinglyOrderedList<double> l1{4};

        SinglyOrderedList<double> l2 = l1;
        assert(!l2.isEmpty() && l2.head() == 4 && l2.tail() == 4);

        SinglyOrderedList<double> l3 = std::move(l1);
        assert(l1.isEmpty());
        assert(!l3.isEmpty() && l3.head() == 4 && l3.tail() == 4);
    }

    {   // Copy & Move Assignment
        SinglyOrderedList<int> l1;
        l1.insert(1);
        l1.insert(2);
        l1.insert(3);

        SinglyOrderedList<int> l2;
        l2.insert(4);
        l2.insert(5);
        l2.insert(6);

        l2 = std::move(l1);
        assert(l1.isEmpty() && !(l2.isEmpty()));

        SinglyOrderedList<int> l3;
        l3.insert(4);
        l3.insert(5);
        l3.insert(6);

        l2 = l3;
        assert(l2 == l3);
    }

    {   // clear()
        SinglyOrderedList<bool> l1{true};
        l1.insert(false);
        l1.clear();
        assert(l1.isEmpty());
    }

}

void test_search() {

    {   // Search
        SinglyOrderedList<std::string> l1{"line"};
        l1.insert("add");
        assert(l1.search("line") && l1.search("add") && !l1.search("two"));
    }

    {   // Empty list
        SinglyOrderedList<std::string> l1{};
        assert(!l1.search("a"));
    }

}

void test_remove() {

    {   // Remove a from b
        SinglyOrderedList<int> l1{};
        l1.insert(1);
        l1.insert(2);
        l1.insert(3);

        SinglyOrderedList<int> l2{};
        l2.insert(1);
        l2.insert(5);
        l2.insert(3);
        l2.insert(6);

        l2.remove(l1);
    }

    {   // Remove head
        SinglyOrderedList<int> l1{4};
        l1.insert(99);
        l1.removeHead();
        assert(l1.size() == 1 && l1.head() == 99);
    }

    {   // Remove head on empty list
        SinglyOrderedList<int> l1{};
        try {
            l1.removeHead();
        } catch (std::runtime_error &ex) {
            assert(ex.what());
        }
    }

    {   // Remove key
        SinglyOrderedList<int> l1{};
        l1.insert(4);
        l1.insert(5);
        l1.insert(59);
        l1.removeKey(5);
        assert(l1.size() == 2 && !l1.search(5));

        SinglyOrderedList<int> l2{};
        l2.insert(4);
        l2.removeKey(4);
        assert(l2.isEmpty());
    }

}

void test_getIntersection() {

    {   // Basic
        SinglyOrderedList<int> l1{};
        l1.insert(1);
        l1.insert(3);
        l1.insert(4);
        l1.insert(6);
        l1.insert(7);
        l1.insert(9);

        SinglyOrderedList<int> l2{};
        l2.insert(1);
        l2.insert(4);
        l2.insert(7);
        l2.insert(8);
        l2.insert(9);

        SinglyOrderedList<int> l3 = getIntersection(l1, l2);
        SinglyOrderedList<int> _FINE{};
        _FINE.insert(1);
        _FINE.insert(4);
        _FINE.insert(7);
        _FINE.insert(9);

        assert(l3 == _FINE);
    }

    {   // Empty lists
        SinglyOrderedList<int> l1, l2;
        SinglyOrderedList<int> l3 = getIntersection(l1, l2);
        assert(l3.isEmpty());
    }

    {   // Empty intersection
        SinglyOrderedList<int> l1, l2;

        l1.insert(1);
        l1.insert(2);
        l1.insert(3);

        l2.insert(4);
        l2.insert(7);
        l2.insert(8);

        SinglyOrderedList<int> l3 = getIntersection(l1, l2);
        assert(l3.isEmpty());
    }

}

int main() {
    test_essential();
    test_search();
    test_remove();
    test_getIntersection();
}
