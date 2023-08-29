#include <iostream>
using namespace std; // 采用 4 字节对齐

#pragma pack(4)
class A
{
public:
    int a;
    virtual void aPrintf() {
        std::cout << "This is class B" << "\n";
    }
};

class B : virtual public A
{
public:
    int b;
    virtual void bPrintf() {
        std::cout << "This is class B" << "\n";
    }
};

class C : virtual public A
{
public:
    int c;
    virtual void cPrintf() {
        std::cout << "This is class C" << "\n";
    }
};

class D : public B, public C
{
public:
    int d;
    virtual void dPrintf() {
        std::cout << "This is class D" << "\n";
    }
    virtual void cPrintf() {
        std::cout << "This is class A" << "\n";
    }
};

int main() {
    A a;
    B b;
    C c;
    D d;
    cout << sizeof(a) << endl;
    cout << sizeof(b) << endl;
    cout << sizeof(c) << endl;
    cout << sizeof(d) << endl;
    return 0;
}