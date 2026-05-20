// вариант 4
// StackArray checkBalanceBrackets QueueVector

#include "Queue.h"
#include "QueueVector.h"
#include "Stack.h"
#include "StackArray.h"
#include <cassert>

void stack_basic_behvior() {
    {
        StackArray<int> s(4);
        s.push(3);

        assert(s.top() == 3);
        assert(s.pop() == 3);
        assert(s.isEmpty());
    }

    {   // copy ctor
        StackArray<int> s1(3);
        s1.push(3);
        s1.push(2);
        s1.push(1);

        assert(!s1.isEmpty());

        StackArray<int> s2(s1);
        assert(s2.top() == 1 && s1.top() == 1);

        s2.pop();
        assert(s1.top() == 1 && s2.top() == 2);
    }

    {   // move ctor
        StackArray<int> s1(3);
        s1.push(3);
        s1.push(2);
        s1.push(1);

        assert(!s1.isEmpty());

        StackArray<int> s2(std::move(s1));
        assert(s2.top() == 1 && s1.isEmpty());

        s2.pop();
        assert(s2.top() == 2);
    }

    {   // copy assignment
        StackArray<int> s1(3);
        s1.push(3);
        s1.push(2);
        s1.push(1);

        assert(!s1.isEmpty());

        StackArray<int> s2 = s1;
        assert(s2.top() == 1 && s1.top() == 1);

        s2.pop();
        assert(s1.top() == 1 && s2.top() == 2);
    }

    {   // move assignment
        StackArray<int> s1(3);
        s1.push(3);
        s1.push(2);
        s1.push(1);

        assert(!s1.isEmpty());

        StackArray<int> s2 = std::move(s1);
        assert(s2.top() == 1 && s1.isEmpty());

        s2.pop();
        assert(s2.top() == 2);
    }
}

void stack_exceptions() {
    {
        try {
            StackArray<int> s(0);
        } catch (const WrongStackSize& ex) {

        }
    }

    {
        try {
            StackArray<int> s(2);
            s.push(3);
            s.push(4);
            s.push(5);
        } catch(const StackOverflow& ex) {

        }
    }

    {
        try {
            StackArray<int> s(2);
            s.pop();
        } catch(const StackUnderflow& ex) {

        }
    }

    {
        try {
            StackArray<int> s(2);
            s.top();
        } catch(const StackUnderflow& ex) {

        }
    }
}

void stack_edge_cases() {
    {
        StackArray<double> s(2);
        s.push(1);
        assert(s.top() == 1 && s.size() == 1);
    }

    {
        StackArray<int> s(1);
        assert(s.isEmpty());
    }

    {
        StackArray<int> s(1);
        s.push(1000);
        assert(s.pop() == 1000);
        assert(s.isEmpty());
    }

    {
        StackArray<int> s(1);
        s.push(1);

        s = s;
    }
}

void queue_basic_behavior() {
    {
        QueueVector<double> q(2);
        q.enQueue(3.3);
        q.enQueue(5.5);

        assert(q.size() == 2);

        q.enQueue(6.6);
        assert(q.size() == 3 && q.capacity() == 4);

        assert(q.front() == 3.3);

        q.enQueue(7.6);

        q.deQueue();
        assert(q.size() == 3);

        assert(q.front() == 5.5);

        q.enQueue(8.6);
        q.enQueue(9.6);

        q.deQueue();
        q.deQueue();
        q.deQueue();
        q.deQueue();
        assert(q.front() == 9.6);
        q.deQueue();
    }

    {   // copy ctor
        QueueVector<int> q1(3);
        q1.enQueue(3);
        q1.enQueue(5);
        q1.enQueue(6);

        QueueVector<int> q2(q1);

        assert(q2.size() == 3 && (!q1.isEmpty()));

        q2.deQueue();
        assert(q2.front() == 5 && q1.front() == 3);
    }

    {   // move ctor
        QueueVector<int> q1(3);
        q1.enQueue(3);
        q1.enQueue(5);
        q1.enQueue(6);

        QueueVector<int> q2(std::move(q1));

        assert(q2.size() == 3 && q1.isEmpty());
    }

    {   // copy assignment
        QueueVector<int> q1(3);
        q1.enQueue(3);
        q1.enQueue(5);
        q1.enQueue(6);

        QueueVector<int> q2;
        q2.enQueue(1);
        q2.enQueue(2);

        q2 = q1;

        assert(q2.size() == 3 && (!q1.isEmpty()));
    }

    {   // move assignment
        QueueVector<int> q1(3);
        q1.enQueue(3);
        q1.enQueue(5);
        q1.enQueue(6);

        QueueVector<int> q2;
        q2.enQueue(1);
        q2.enQueue(2);

        q2 = std::move(q1);

        assert(q2.size() == 3 && q1.isEmpty());
    }
}

void queue_exceptions() {
    {
        try {
            QueueVector<int> q(0);
        } catch(const WrongQueueSize& ex) {

        }
    }

    {
        try {
            QueueVector<int> q(3);
            q.deQueue();
        } catch(const QueueUnderflow& ex) {

        }
    }

    {
        try {
            QueueVector<int> q(3);
            q.front();
        } catch(const QueueUnderflow& ex) {

        }
    }
}

void queue_edge_cases() {
    {
        QueueVector<int> q(2);
        q.enQueue(1);
        q.enQueue(2);
        assert(q.front() == 1);
        assert(q.deQueue() == 1 && q.front() == 2);
    }

    {
        QueueVector<int> q(2);
        q.enQueue(1);
        q.enQueue(2);
        q.enQueue(3);

        q.deQueue();
        q.deQueue();
        q.deQueue();

        assert(q.capacity() == 4 && q.size() == 0 && q.isEmpty());
    }

    {
        QueueVector<int> q(1);
        q.enQueue(1);
        assert(q.front() == 1);
        assert(q.capacity() == 1);

        q.enQueue(2);
        assert(q.front() == 1);
        assert(q.capacity() == 2);

        q.enQueue(3);
        assert(q.front() == 1);
        assert(q.capacity() == 4);

        q.enQueue(4);
        q.enQueue(5);
        assert(q.front() == 1);
        assert(q.capacity() == 8);
    }

    {
        QueueVector<int> q(2);
        q.enQueue(4);

        q = q;
    }
}

/*
    1) Анализ правильности расстановки скобок.
    Функция должна возвращать True, если количество открывающих и
    закрывающих скобок одного типа совпадает, и они имеют правильную
    вложенность. Допускаются три вида скобок: круглые, квадратные и фигурные.
    Прототип функции:
    bool checkBalanceBrackets (const char* text, const int maxDeep);
    text - анализируемый текст, содержащий скобки
    maxDeep - максимально возможный уровень вложенности скобок
*/

bool isOpeningBracket(const char ch) {
    return ch == '(' || ch == '[' || ch == '{';
}

bool isClosingBracket(const char ch) {
    return ch == ')' || ch == ']' || ch == '}';
}

bool isMatchingBracket(const char open, const char close) {
    return (open == '(' && close == ')') ||
           (open == '[' && close == ']') ||
           (open == '{' && close == '}');
}

bool checkBalanceBrackets(const char* text, const int maxDeep) {
    if (maxDeep < 0 || text == nullptr) {
        return false;
    }

    try {
        StackArray<char> buf(maxDeep);

        for (int i = 0; text[i] != '\0'; ++i) {
            const char ch = text[i];

            if (isOpeningBracket(ch)) {
                buf.push(ch);
            } else if (isClosingBracket(ch)) {
                if (buf.isEmpty()) {
                    return false;
                }

                const char top = buf.pop();
                if (!isMatchingBracket(top, ch)) {
                    return false;
                }
            }
        }

        return buf.isEmpty();
    } catch (const StackOverflow&) {
        return false;
    } catch (const WrongStackSize&) {
        return false;
    }
}

void test_checkBalanceBrackets() {
    assert(checkBalanceBrackets("", 1) == true);
    assert(checkBalanceBrackets("(", 1) == false);
    assert(checkBalanceBrackets(")", 1) == false);
    assert(checkBalanceBrackets("()", 1) == true);
    assert(checkBalanceBrackets("[]", 1) == true);
    assert(checkBalanceBrackets("{}", 1) == true);
    assert(checkBalanceBrackets("{89}", 1) == true);
    assert(checkBalanceBrackets("([{}])", 3) == true);
    assert(checkBalanceBrackets("([)]", 3) == false);
    assert(checkBalanceBrackets("(((", 3) == false);
    assert(checkBalanceBrackets(")))", 3) == false);
    assert(checkBalanceBrackets("({[", 3) == false);
    assert(checkBalanceBrackets(")}]", 3) == false);
    assert(checkBalanceBrackets(")}]", 3) == false);
    assert(checkBalanceBrackets("text(){}text{tex{}{()}t}", 7) == true);
}

int main() {
    // StackArray
    stack_basic_behvior();
    stack_exceptions();
    stack_edge_cases();

    // checkBalanceBrackets
    test_checkBalanceBrackets();

    // QueueVector
    queue_basic_behavior();
    queue_exceptions();
    queue_edge_cases();

    return 0;
}
