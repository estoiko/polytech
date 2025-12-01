#include <cassert>
#include <cstddef>
#include <string>
#include <sstream>
#include <utility>
#include "String.h"
#include <iostream>
#include "Strutils.h"

int main() {
    String s1;
    assert(s1.length() == 0 && "Тест 1: конструктор по умолчанию, длина должна быть 0");
    assert(s1.empty() && "Тест 1: конструктор по умолчанию, строка должна быть пустой");

    String s2a;
    String s2b;
    String s2c;
    s2a = "a";
    s2b = "bb";
    s2c = "ccc";
    assert(s2a.length() == 1 && "Тест 2: независимые строки, длина s2a должна быть 1");
    assert(s2b.length() == 2 && "Тест 2: независимые строки, длина s2b должна быть 2");
    assert(s2c.length() == 3 && "Тест 2: независимые строки, длина s2c должна быть 3");

    String s3("");
    assert(s3.length() == 0 && "Тест 3: конструктор из пустой C-строки, длина должна быть 0");
    assert(s3.empty() && "Тест 3: конструктор из пустой C-строки, строка должна быть пустой");

    String s4("Hello");
    assert(s4.length() == 5 && "Тест 4: конструктор из C-строки, длина должна быть 5");

    std::string longStr5(1500, 'x');
    String s5(longStr5.c_str());
    assert(s5.length() == 1500 && "Тест 5: конструктор из длинной C-строки, длина должна быть 1500");

    String a6("abc");
    String b6(a6);
    assert(b6 == a6 && "Тест 6: копирующий конструктор, строки должны быть равны");

    String a7;
    String b7(a7);
    assert(b7.empty() && "Тест 7: копирующий конструктор пустой строки, строка должна быть пустой");

    String b8(String("abc"));
    assert(b8 == String("abc") && "Тест 8: перемещающий конструктор от временного объекта, строка должна быть 'abc'");

    String a9("abc");
    String b9(std::move(a9));
    assert(b9 == String("abc") && "Тест 9: перемещающий конструктор, целевая строка должна быть 'abc'");
    assert(a9.length() == 0 && "Тест 9: перемещающий конструктор, исходная строка должна стать пустой");

    String s10;
    assert(s10.length() == 0 && "Тест 10: length() для пустой строки должно быть 0");

    String s11("Test");
    assert(s11.length() == 4 && "Тест 11: length() для 'Test' должно быть 4");

    String s12;
    assert(s12.empty() && "Тест 12: empty() для пустой строки должно быть true");

    String s13("a");
    assert(!s13.empty() && "Тест 13: empty() для непустой строки должно быть false");

    String a14("abc");
    String b14("x");
    b14 = a14;
    assert(b14 == a14 && "Тест 14: оператор присваивания String=String, строки должны быть равны");

    String a15("abc");
    a15 = a15;
    assert(a15 == String("abc") && "Тест 15: самоприсваивание String=String, строка должна сохраниться");

    String a16("abc");
    String b16("def");
    b16 = std::move(a16);
    assert(b16 == String("abc") && "Тест 16: оператор присваивания String=String&&, целевая строка должна быть 'abc'");

    String s17("hello");
    s17 = "";
    assert(s17.empty() && "Тест 17: оператор присваивания из пустой C-строки, строка должна стать пустой");

    String s18("a");
    std::string long18(1000, 'b');
    s18 = long18.c_str();
    assert(s18.length() == 1000 && "Тест 18: оператор присваивания из длинной C-строки, длина должна быть 1000");

    String s19("abc");
    s19[0] = 'z';
    assert(s19 == String("zbc") && "Тест 19: operator[] изменение первого символа, строка должна быть 'zbc'");

    String s20("abc");
    s20[s20.length() - 1] = 'z';
    assert(s20 == String("abz") && "Тест 20: operator[] изменение последнего символа, строка должна быть 'abz'");

    const String s21("abc");
    char c21 = s21[1];
    assert(c21 == 'b' && "Тест 21: const operator[] чтение второго символа, должно быть 'b'");

    String a22("ab");
    String b22("cd");
    String c22 = a22 + b22;
    assert(c22 == String("abcd") && "Тест 22: operator+ для двух непустых строк, результат должен быть 'abcd'");

    String a23;
    String b23("abc");
    String c23 = a23 + b23;
    assert(c23 == String("abc") && "Тест 23: operator+ когда левая строка пустая, результат должен быть 'abc'");

    String a24("abc");
    String b24;
    String c24 = a24 + b24;
    assert(c24 == String("abc") && "Тест 24: operator+ когда правая строка пустая, результат должен быть 'abc'");

    String a25("ab");
    String b25("cd");
    a25 += b25;
    assert(a25 == String("abcd") && "Тест 25: operator+= (String), результат должен быть 'abcd'");

    String a26("ab");
    String b26;
    a26 += b26;
    assert(a26 == String("ab") && "Тест 26: operator+= (пустой String), строка не должна измениться");

    String a27("ab");
    a27 += "cde";
    assert(a27 == String("abcde") && "Тест 27: operator+= (const char*), результат должен быть 'abcde'");

    String s28("abc");
    s28.clear();
    assert(s28.empty() && "Тест 28: clear() для непустой строки, строка должна стать пустой");

    String s29("abc");
    s29.clear();
    s29.clear();
    assert(s29.empty() && "Тест 29: повторный вызов clear(), строка должна оставаться пустой");

    String a30("abc");
    String b30("abc");
    assert(a30 == b30 && "Тест 30: operator== для равных строк должен возвращать true");

    String a31("abc");
    String b31("abd");
    assert(!(a31 == b31) && "Тест 31: operator== для разных строк должен возвращать false");

    String a32("abc");
    String b32("abd");
    assert(a32 != b32 && "Тест 32: operator!= для разных строк должен возвращать true");

    String a33("abc");
    String b33("abd");
    assert(a33 < b33 && "Тест 33: operator< 'abc' < 'abd' должен быть true");

    String a34("ab");
    String b34("abc");
    assert(a34 < b34 && "Тест 34: operator< для префикса 'ab' < 'abc' должен быть true");

    String a35("abc");
    String b35("abc");
    assert(a35 <= b35 && "Тест 35: operator<= для равных строк должен быть true");
    assert(a35 >= b35 && "Тест 35: operator>= для равных строк должен быть true");

    std::istringstream iss36("hello\n");
    String s36;
    iss36 >> s36;
    assert(s36 == String("hello") && "Тест 36: operator>> ввод одного слова, строка должна быть 'hello'");

    std::istringstream iss37("hello world\n");
    String s37;
    iss37 >> s37;
    assert(s37 == String("hello") && "Тест 37: operator>> ввод до пробела, строка должна быть 'hello'");

    std::istringstream iss38("");
    String s38;
    iss38 >> s38;
    assert(s38.length() == 0 && "Тест 38: operator>> пустой ввод, строка должна остаться пустой");

    std::ostringstream oss39;
    String s39("abc");
    oss39 << s39;
    assert(oss39.str() == "abc" && "Тест 39: operator<< вывод непустой строки, результат должен быть 'abc'");

    std::ostringstream oss40;
    String s40;
    oss40 << s40;
    assert(oss40.str() == "" && "Тест 40: operator<< вывод пустой строки, результат должен быть пустым");

    String s41("AbC1!");
    String l41 = s41.lower();
    assert(l41 == String("abc1!") && "Тест 41: lower() для смешанного регистра, результат должен быть 'abc1!'");

    String s42("123_+-");
    String l42 = s42.lower();
    assert(l42 == s42 && "Тест 42: lower() без латинских букв, строка не должна измениться");

    String s43("abc");
    String l43 = s43.lower();
    assert(l43 == s43 && "Тест 43: lower() для уже нижнего регистра, строка не должна измениться");

    const char* p44 = "";
    std::size_t len44 = my::strlen(p44);
    assert(len44 == 0 && "Тест 44: my::strlen для пустой строки должно вернуть 0");

    const char* p45 = "abcd";
    std::size_t len45 = my::strlen(p45);
    assert(len45 == 4 && "Тест 45: my::strlen для 'abcd' должно вернуть 4");

    char dest46[10];
    my::strcpy(dest46, "abc");
    assert(dest46[0] == 'a' && dest46[1] == 'b' && dest46[2] == 'c' && dest46[3] == '\0' && "Тест 46: my::strcpy копирование 'abc', результат должен быть 'abc\\0'");

    char dest47[10];
    my::strcpy(dest47, "");
    assert(dest47[0] == '\0' && "Тест 47: my::strcpy копирование пустой строки, первый символ должен быть '\\0'");

    char buf48[10] = "abc";
    my::strcpy(buf48, buf48);
    assert(buf48[0] == 'a' && buf48[1] == 'b' && buf48[2] == 'c' && buf48[3] == '\0' && "Тест 48: my::strcpy самокопирование 'abc', строка должна остаться 'abc\\0'");

    String s49;
    std::cin >> s49;
    std::cout << s49 << "\n";
    return 0;
}