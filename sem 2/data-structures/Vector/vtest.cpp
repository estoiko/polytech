#include "Vector.h"
#include <cstddef>
#include <iostream>
#include <cassert>
#include <string>

using ll = long long;
using ld = long double;

class Tracer {
private:
    int id_;

public:
    std::string prev_action = "none";

    Tracer(int id = 0) : id_(id), prev_action("constructed") {
        // std::cout << "  Tracer[" << id_ << "] DEFAULT constructed\n";
    }

    Tracer(const Tracer& other) : id_(other.id_), prev_action("copied") {
        // std::cout << "  Tracer[" << id_ << "] COPY constructed\n";
    }

    Tracer(Tracer&& other) noexcept : id_(other.id_), prev_action("moved") {
        // std::cout << "  Tracer[" << id_ << "] MOVE constructed\n";
        other.id_ = -1;
    }

    ~Tracer() {
        if (id_ != -1) {
            // std::cout << "  Tracer[" << id_ << "] destructed\n";
        }
    }

    Tracer& operator=(const Tracer& other) {
        // std::cout << "  Tracer[" << id_ << "] COPY assigned from Tracer[" << other.id_ << "]\n";
        id_ = other.id_;
        prev_action = "copy=";
        return *this;
    }

    Tracer& operator=(Tracer&& other) noexcept {
        // std::cout << "  Tracer[" << id_ << "] MOVE assigned from Tracer[" << other.id_ << "]\n";
        id_ = other.id_;
        prev_action = "move=";
        other.id_ = -1;
        return *this;
    }

    int id() const { return id_; }

    bool operator==(const Tracer& other) const { return id_ == other.id_; }
    bool operator!=(const Tracer& other) const { return !(*this == other); }
    bool operator<(const Tracer& other) const { return id_ < other.id_; }
};

class NoDefault {
public:
    int value;
    std::string str;
    NoDefault(int v, std::string s): value(v), str(s) {}
};

template<typename T>
void print(const Vector<T>& v) {
    for (std::size_t i = 0; i < v.size(); ++i) {
        std::cout << v[i] << " ";
    }
    std::cout << "\n";
}

void test_conctructors_mem() {
    std::cout << "CONSTRUCTORS & MEMORY TEST :: ";

    Vector<int> v1{};
    assert(v1.empty() == true);

    Vector<NoDefault> v2;
    v2.push_back(NoDefault(1, "1"));
    v2.push_back(NoDefault(2, "2"));
    assert(v2.size() == 2);
    assert(v2[0].value == 1 && v2[0].str == "1");
    assert(v2[1].value == 2 && v2[1].str == "2");

    v2.insert(1, NoDefault(3, "3"));
    assert(v2.size() == 3);
    assert(v2[1].value == 3 && v2[1].str == "3");
    assert(v2[2].value == 2 && v2[2].str == "2");

    v2 = v2;

    std::cout << "PASSED\n";
}

void test_copy_move() {
    std::cout << "COPY & MOVE :: ";

    Vector<Tracer> v1;
    v1.reserve(10);

    Tracer t1(1);
    v1.push_back(t1);
    assert(v1[0].prev_action == "copied");

    v1.push_back(Tracer(2));
    assert(v1[1].prev_action == "moved");

    Tracer t3(3);
    v1.push_back(std::move(t3));
    assert(v1[2].prev_action == "moved");

    Tracer t4(4);
    v1.insert(1, t4);
    assert(v1.size() == 4 && v1[1] == t4 && v1[1].prev_action == "copy=");

    v1.insert(2, Tracer(5));
    assert(v1.size() == 5 && v1[2].id() == 5 && v1[2].prev_action == "move=");

    std::cout << "PASSED\n";
}

void test_all_methods() {
    std::cout << "ALL METHODS :: ";

    Vector<std::string> v1(3, "c");
    assert(v1[0] == "c" && v1[2] == "c");
    assert(v1.size() == 3);

    v1.push_back("v");
    assert(v1.capacity() == 6);
    v1.push_back("v");
    v1.push_back("v");
    v1.push_back("v");
    assert(v1.capacity() == 12 && v1.size() == 7);

    v1.erase(2, 4);
    assert(v1.size() == 5 && v1.capacity() == 12);

    Vector<int> v2(5);
    assert(v2.size() == 5);

    Vector<int> v3 = v2;
    assert(v3 == v2);

    Vector<int> v4 = std::move(v3);
    assert(v4.size() == 5);
    assert(v3.empty());

    v2 = std::move(v4);
    assert(v4.empty());

    v4.resize(100);
    v4.resize(3);
    assert(v4.size() == 3);
    v4.resize(0);
    assert(v4.empty());

    v4.resize(10, 5);
    assert(v4[8] == 5);

    v4.reserve(1000);
    assert(v4.capacity() == 1000);
    v4.shrink_to_fit();
    assert(v4.capacity() == v4.size() && v4.capacity() == 10);

    v4.pop_back();
    v4.clear();
    assert(v4.capacity() == 10);

    v2.insert(3, 77);
    assert(v2[3] == 77);

    v2.erase(2, 4);
    assert(v2[3] != 77);

    Vector<double> v5, v5_2, v6;
    v5.push_back(5);
    v5_2.push_back(5);
    v6.push_back(6);

    assert(v5 != v6);
    assert(v5 == v5 && v5 == v5_2);
    assert(v5 < v6);
    assert(v5 <= v5);
    assert(v6 > v5);
    assert(v6 >= v5);

    assert(v6.at(0) == 6);
    assert(v6.front() == 6 && v6.back() == 6);

    std::cout << "PASSED\n";
}

void test_sort() {
    {
        Vector<int> v;
        v.resize(5);
        v[0] = 5;
        v[1] = 4;
        v[2] = 1;
        v[3] = 2;
        v[4] = 3;

        print(v);
        selectionSort(v);
        print(v);
    }

    {
        Vector<int> one;
        one.push_back(5);
        selectionSort(one);
        assert(one[0] == 5);
    }

    {
        Vector<int> two;
        two.push_back(2);
        two.push_back(1);
        selectionSort(two);
        assert(two[0] == 1 && two[1] == 2);
    }

    {
        Vector<int> v1;
        v1.push_back(2);
        v1.push_back(1);
        v1.push_back(3);
        v1.push_back(3);
        selectionSort(v1);
        assert(v1[0] == 1 && v1[1] == 2 && v1[2] == 3 && v1[3] == 3);
    }

    {
        Vector<int> same(5, 7);
        selectionSort(same);
        for (size_t i = 0; i < 5; ++i) assert(same[i] == 7);
    }

    {
        Vector<int> sorted;
        for (int i = 1; i <= 10; ++i) sorted.push_back(i);
        selectionSort(sorted);
        for (size_t i = 0; i < 10; ++i) assert(sorted[i] == (int)i + 1);
    }

    {
        Vector<int> reverse;
        for (int i = 10; i >= 1; --i) reverse.push_back(i);
        selectionSort(reverse);
        for (size_t i = 0; i < 10; ++i) assert(reverse[i] == (int)i + 1);
    }
}

void test_edge_cases() {
    std::cout << "EDGE CASES :: ";
    // пустой вектор
    {
        Vector<int> empty;

        empty.pop_back();
        empty.clear();
        empty.shrink_to_fit();
        assert(empty.capacity() == 0);

        empty.push_back(1);
        empty.erase(0, 0);
        assert(empty.size() == 1);

        empty.resize(0);
        assert(empty.empty() && empty.capacity() > 0);
    }

    // вектор с одним элементом
    {
        Vector<int> v(1, 42);
        assert(v.front() == v.back());

        v.pop_back();
        assert(v.empty());

        v.insert(0, 100);
        assert(v.size() == 1 && v[0] == 100);

        v.erase(0);
        assert(v.empty());
    }

    // insert
    {
        Vector<int> v;

        v.insert(0, 1);
        v.insert(v.size(), 2);
        v.insert(0, 0);
        assert(v[0] == 0 && v[1] == 1 && v[2] == 2);

        v.reserve(3);
        v.insert(1, 100);
        assert(v[1] == 100 && v.capacity() > 3);

        v.insert(2, std::move(999));
        assert(v[2] == 999);
    }

    // erase
    {
        Vector<int> v;
        for (int i = 0; i < 10; ++i) v.push_back(i);

        v.erase(0);
        assert(v[0] == 1);

        v.erase(v.size() - 1);
        assert(v.back() == 8);

        v.erase(3);
        assert(v[3] == 5);

        v.erase(0, 2);
        v.erase(v.size() - 2, v.size());
        v.erase(0, v.size());
        assert(v.empty());

        v.push_back(1);
        v.push_back(2);
        v.erase(1, 1);
        assert(v.size() == 2);
    }

    // exceptions
    {
        Vector<int> v;
        v.push_back(1);
        v.push_back(2);

        bool caught = false;

        try { v.at(10); }
        catch (const std::out_of_range&) { caught = true; }
        assert(caught);

        caught = false;
        try { v.insert(100, 42); }
        catch (const std::out_of_range&) { caught = true; }
        assert(caught);

        caught = false;
        try { v.erase(5, 10); }
        catch (const std::out_of_range&) { caught = true; }
        assert(caught);

        caught = false;
        try { v.erase(1, 0); }
        catch (const std::out_of_range&) { caught = true; }
        assert(caught);

        caught = false;
        try { v.erase(100); }
        catch (const std::out_of_range&) { caught = true; }
        assert(caught);
    }

    std::cout << "PASSED\n";
}

int main() {
    test_conctructors_mem();
    test_copy_move();
    test_all_methods();
    test_edge_cases();
    test_sort();
}
