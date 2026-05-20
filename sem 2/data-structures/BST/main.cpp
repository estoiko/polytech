// var 3

#include "BinarySearchTree.h"
#include <cassert>

void basic_behavior() {
    {
        BinarySearchTree<int> bst(8);
        bst.insert(3);
        bst.insert(10);
        bst.insert(14);
        bst.insert(6);
        bst.insert(1);
        bst.insert(4);
        bst.insert(7);
        bst.insert(13);

        bst.output(std::cout);
        bst.inorderWalk();
        bst.inorderWalkIterative();
        bst.walkByLevels();
        assert(bst.getHeight() == 3);
        assert(bst.getNumberOfNodes() == 9);

        assert(bst.findKSmallest(3) == 4);
        bst.output(std::cout);
    }

    {
        BinarySearchTree<int> bst;
        bst.insert(1);
        bst.insert(2);
        bst.insert(3);
        bst.insert(4);
        assert(bst.getHeight() == 3);
        assert(bst.getNumberOfNodes() == 4);

        bst.remove(4);
        assert(bst.getHeight() == 2);
        assert(bst.getNumberOfNodes() == 3);
        assert(!bst.searchIterative(4));

        bst.remove(2);
        assert(bst.getHeight() == 1);
        assert(bst.getNumberOfNodes() == 2);
        assert(!bst.searchIterative(2));

        bst.remove(1);
        assert(bst.getHeight() == 0);
        assert(bst.getNumberOfNodes() == 1);
        assert(!bst.searchIterative(1));

        bst.remove(3);
        assert(bst.getHeight() == -1);
        assert(bst.getNumberOfNodes() == 0);
        assert(!bst.searchIterative(3));
    }

    {
        BinarySearchTree<int> bst;
        assert(bst.insert(5));
        assert(!bst.insert(5));
        assert(bst.getNumberOfNodes() == 1);
    }

    {
        BinarySearchTree<int> bst;
        bst.insert(5);
        bst.insert(4);
        bst.insert(3);
        bst.insert(2);
        bst.insert(1);

        assert(bst.getHeight() == 4);
    }
}

void memory_management() {
    {   // move ctor
        BinarySearchTree<double> bst1;
        bst1.insert(4.0);
        bst1.insert(2.0);
        bst1.insert(3.0);
        bst1.insert(1.0);

        BinarySearchTree<double> bst2(std::move(bst1));

        assert(bst2.searchIterative(1));
        assert(bst2.searchIterative(2));

        assert(bst1.getNumberOfNodes() == 0);
    }

    {   // move assignment
        BinarySearchTree<double> bst1;
        bst1.insert(4.0);
        bst1.insert(2.0);
        bst1.insert(3.0);
        bst1.insert(1.0);

        BinarySearchTree<double> bst2;
        bst2.insert(11.0);
        bst2.insert(22.0);
        bst2 = std::move(bst1);

        assert(!bst2.searchIterative(11.0));
        assert(!bst2.searchIterative(22.0));
        assert(bst2.searchIterative(1.0));

        assert(bst1.getNumberOfNodes() == 0);
    }
}

void edge_cases() {
    {   // remove
        BinarySearchTree<int> bst(8);
        bst.insert(3);
        bst.insert(10);
        bst.insert(14);
        bst.insert(6);
        bst.insert(1);
        bst.insert(4);
        bst.insert(7);
        bst.insert(13);

        bst.remove(13);
        assert(!bst.searchIterative(13));
        bst.insert(13);
        bst.output(std::cout);

        bst.remove(14);
        assert(!bst.searchIterative(14) && bst.searchIterative(13));
        bst.output(std::cout);

        bst.remove(3);
        assert(!bst.searchIterative(3));
        bst.output(std::cout);
    }

    {
        BinarySearchTree<int> bst;
        bst.insert(5);
        bst.remove(5);
        assert(bst.getNumberOfNodes() == 0);
    }

    {
        BinarySearchTree<int> bst;
        bst.insert(5);
        bst.insert(3);
        bst.remove(5);
        assert(bst.searchIterative(3));
        assert(bst.getNumberOfNodes() == 1);
    }

    {
        BinarySearchTree<int> bst;
        bst.insert(5);
        bst.insert(3);
        bst.insert(7);
        bst.insert(6);

        bst.remove(5);

        assert(!bst.searchIterative(5));
        assert(bst.searchIterative(6));
        assert(bst.searchIterative(3));
        assert(bst.searchIterative(7));
    }

    {
        BinarySearchTree<int> bst;
        bst.insert(1);
        bst.insert(2);

        assert(!bst.remove(3));
        assert(bst.getNumberOfNodes() == 2);
    }

    {
        BinarySearchTree<int> bst;
        bst.insert(1);
        bst.insert(2);

        bst.remove(1);
        bst.remove(2);

        bst.insert(10);
        assert(bst.searchIterative(10));
    }

    {
        BinarySearchTree<int> bst;
        bst.insert(10);
        bst.insert(5);
        bst.insert(15);
        bst.insert(12);
        bst.insert(13);
        bst.insert(20);

        assert(bst.remove(10));

        assert(!bst.searchIterative(10));
        assert(bst.searchIterative(5));
        assert(bst.searchIterative(12));
        assert(bst.searchIterative(13));
        assert(bst.searchIterative(15));
        assert(bst.searchIterative(20));
        assert(bst.getNumberOfNodes() == 5);

        bst.inorderWalkIterative();
        // "5 12 13 15 20 \n"
    }

    {   // empty tree
        BinarySearchTree<bool> bst;
        assert(!bst.searchIterative(1));
        assert(!bst.remove(1));
    }
}

void kth_edge_cases() {
    {
        BinarySearchTree<int> bst;

        try {
            bst.findKSmallest(1);
            assert(false);
        } catch (...) {}
    }

    {
        BinarySearchTree<int> bst;
        bst.insert(2);
        bst.insert(1);
        bst.insert(3);

        try {
            bst.findKSmallest(0);
            assert(false);
        } catch (...) {}

        try {
            bst.findKSmallest(4);
            assert(false);
        } catch (...) {}

        assert(bst.findKSmallest(1) == 1);
        assert(bst.findKSmallest(2) == 2);
        assert(bst.findKSmallest(3) == 3);
    }
}



int main() {
    std::cout << "BASIC_BEHAVIOR_TEST\n";
    basic_behavior();
    memory_management();
    std::cout << "EDGE_CASES_TEST\n";
    edge_cases();
    kth_edge_cases();

    std::cout << "\n-----\n";

    BinarySearchTree<int> bst;
    bst.insert(15);
    bst.insert(10);
    bst.insert(25);
    bst.insert(3);
    bst.insert(12);
    bst.insert(20);
    bst.insert(8);

    bst.output(std::cout);
    bst.inorderWalkIterative();

    bst.remove(8);
    bst.output(std::cout);

    bst.remove(10);
    bst.output(std::cout);

    bst.remove(15);
    bst.output(std::cout);
}
