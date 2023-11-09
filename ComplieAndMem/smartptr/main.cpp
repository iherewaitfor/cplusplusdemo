#include <iostream>
using namespace std; // 采用 4 字节对齐

//#pragma pack(4)
class A
{
public:
    int a;
};

class B : virtual public A
{
public:
    int b;
    void bPrintf() {
        std::cout << "This is class B" << "\n";
    }
};

class C : virtual public A
{
public:
    int c;
    void cPrintf() {
        std::cout << "This is class C" << "\n";
    }
};

class D : public B, public C
{
public:
    int d;
    void dPrintf() {
        std::cout << "This is class D" << "\n";
    }
};

#include<type_traits>
int main() {
    A a;
    B b;
    C c;
    D d;
    cout << sizeof(a) << endl;
    cout << sizeof(b) << endl;
    cout << sizeof(c) << endl;
    cout << sizeof(d) << endl;
    A& aa = a;
    int i = 1;
    int& j = i;
    std::move<int>(j);
    return 0;
}
/*
4
16
16
32
*/
