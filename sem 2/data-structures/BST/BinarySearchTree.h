#ifndef _BINARY_SEARCH_TREE_H
#define _BINARY_SEARCH_TREE_H

#include <cstddef>
#include <ios>
#include <iosfwd>
#include <ostream>
#include <iostream>
#include <queue>
#include <stack>
#include <stdexcept>
#include <algorithm>

template<typename T>
class BinarySearchTree {
private:
    struct Node {
        T key_;
        Node* left_;
        Node* right_;
        Node* parent_;

        Node(T key, Node* left = nullptr, Node* right = nullptr, Node* parent = nullptr)
            : key_(key)
            , left_(left)
            , right_(right)
            , parent_(parent)
        {}
    };

    Node* root_;

    std::size_t getNumberOfNodes(const Node* node) const {
        if (node == nullptr) return 0;
        return 1 + getNumberOfNodes(node->left_) + getNumberOfNodes(node->right_);
    }

    int getHeight(const Node* node) const {
        if (node == nullptr) return -1;
        return 1 + std::max(getHeight(node->left_), getHeight(node->right_));
    }

    void inorderWalk(Node* node) const {
        if (node != nullptr) {
            inorderWalk(node->left_);
            std::cout << node->key_ << " ";
            inorderWalk(node->right_);
        }
    }

    void output(std::ostream& out, Node* node) const;

    void clear(Node* node) {
        if (node != nullptr) {
            clear(node->left_);
            clear(node->right_);
            delete node;
        }
    }

    void clear() {
        clear(root_);
        root_ = nullptr;
    }

    Node* search(const T& key) const {
        Node* current = root_;
        while (current != nullptr) {
            if (key == current->key_) {
                return current;
            } else if (key > current->key_) {
                current = current->right_;
            } else if (key < current->key_) {
                current = current->left_;
            }
        }
        return nullptr;
    }

public:
    BinarySearchTree()
        : root_(nullptr)
    {}

    BinarySearchTree(T key)
        : root_(new Node(key))
    {}

    virtual ~BinarySearchTree() {
        clear();
    };

    BinarySearchTree(const BinarySearchTree& other) = delete;
    BinarySearchTree& operator=(const BinarySearchTree& other) = delete;

    BinarySearchTree(BinarySearchTree&& other) noexcept
        : root_(other.root_)
    {
        other.root_ = nullptr;
    };

    BinarySearchTree& operator=(BinarySearchTree&& other) noexcept {
        if (this != &other) {
            clear();
            root_ = other.root_;
            other.root_ = nullptr;
        }
        return *this;
    }

    bool searchIterative(const T& key) const {
        if (search(key)) {
            return true;
        }

        return false;
    }

    bool insert(const T& key);
    bool remove(const T& key);

    void inorderWalkIterative() const;

    void inorderWalk() const {
        inorderWalk(root_);
        std::cout << "\n";
    }

    void walkByLevels() const;

    T findKSmallest(std::size_t k) const;

    void output(std::ostream& out) const {
        output(out, root_);
        out << "\n";
    }

    std::size_t getNumberOfNodes() const {
        return getNumberOfNodes(root_);
    }

    int getHeight() const {
        return getHeight(root_);
    }
};

template<typename T>
bool BinarySearchTree<T>::insert(const T& key) {
    Node* parent = nullptr;
    Node* current = root_;

    while (current != nullptr) {
        parent = current;

        if (key < current->key_) {
            current = current->left_;
        } else if (key > current->key_) {
            current = current->right_;
        } else {
            return false;
        }
    }

    Node* new_node = new Node(key);
    new_node->parent_ = parent;

    if (parent == nullptr) {
        root_ = new_node;
    } else if (key < parent->key_) {
        parent->left_ = new_node;
    } else {
        parent->right_ = new_node;
    }

    return true;
}

template<typename T>
bool BinarySearchTree<T>::remove(const T& key) {
    if (root_ == nullptr) return false;

    Node* to_remove = search(key);
    if (to_remove == nullptr) return false;

    Node* parent = to_remove->parent_;

    if (to_remove->left_ == nullptr && to_remove->right_ == nullptr) {
        if (to_remove == root_) {
            root_ = nullptr;
        } else {
            if (parent->left_ == to_remove)
                parent->left_ = nullptr;
            else
                parent->right_ = nullptr;
        }
        delete to_remove;
    } else if (to_remove->left_ == nullptr || to_remove->right_ == nullptr) {
        Node* child = (to_remove->left_ != nullptr) ? to_remove->left_ : to_remove->right_;

        if (to_remove == root_) {
            root_ = child;
            child->parent_ = nullptr;
        } else {
            if (parent->left_ == to_remove)
                parent->left_ = child;
            else
                parent->right_ = child;

            child->parent_ = parent;
        }

        delete to_remove;
    } else {
        Node* next_node = to_remove->right_;
        while (next_node->left_ != nullptr) {
            next_node = next_node->left_;
        }

        to_remove->key_ = next_node->key_;
        Node* next_parent = next_node->parent_;

        if (next_parent->left_ == next_node) {
            next_parent->left_ = next_node->right_;
        } else {
            next_parent->right_ = next_node->right_;
        }

        if (next_node ->right_ != nullptr)
            next_node->right_->parent_ = next_parent;

        delete next_node;
    }

    return true;
}

template<typename T>
void BinarySearchTree<T>::inorderWalkIterative() const {
    if (root_ == nullptr) return;

    std::stack<Node*> nodes;
    Node* current = root_;

    while (current != nullptr || !nodes.empty()) {
        while (current != nullptr) {
            nodes.push(current);
            current = current->left_;
        }

        current = nodes.top();
        nodes.pop();

        std::cout << current->key_ << " ";
        current = current->right_;
    }

    std::cout << "\n";
}

template<typename T>
void BinarySearchTree<T>::walkByLevels() const {
    if (root_ == nullptr) return;

    std::queue<Node*> q;
    q.push(root_);

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();
        std::cout << current->key_ << " ";

        if (current->left_ != nullptr)
            q.push(current->left_);
        if (current->right_ != nullptr)
            q.push(current->right_);
    }

    std::cout << "\n";
}

template<typename T>
T BinarySearchTree<T>::findKSmallest(std::size_t k) const {
    if (root_ == nullptr) {
        throw std::runtime_error("ERROR: the tree is empty!");
    }

    if (k == 0 || k > getNumberOfNodes()) {
        throw std::runtime_error("ERROR: k should be valid (from 1 to nodes number)!");
    }

    std::stack<Node*> nodes;
    Node* current = root_;
    Node* smallest = nullptr;

    while ((current != nullptr || !nodes.empty()) && k) {
        while (current != nullptr) {
            nodes.push(current);
            current = current->left_;
        }

        current = nodes.top();
        nodes.pop();

        --k;
        smallest = current;
        current = current->right_;
    }

    return smallest->key_;
}

template<typename T>
void BinarySearchTree<T>::output(std::ostream& out, Node* node) const {
    out << "(";
    if (node != nullptr) {
        out << node->key_;
        if (node->left_ != nullptr) {
            out << " ";
            output(out, node->left_);
            if (node->right_ == nullptr) {
                out << " ()";
            }
        }

        if (node->right_ != nullptr) {
            if (node->left_ == nullptr) {
                out << " ()";
            }
            out << " ";
            output(out, node->right_);
        }
    }
    out << ")";
}

#endif // _BINARY_SEARCH_TREE_H
