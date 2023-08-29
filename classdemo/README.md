- [classlayout](#classlayout)
- [普通多继承的内存布局](#普通多继承的内存布局)
- [带虚继承的内存布局](#带虚继承的内存布局)

# classlayout
查看一个文件的所有类的对象内存布局

```C++
cl [filename] /d1reportAllClassLayout
```

# 普通多继承的内存布局


# 带虚继承的内存布局

源码如下
```C++
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
```


可使用命令查看类的对象的内存布局。
```
cl [filename] /d1reportSingleClassLayout[classname]
```
如
```
D:\srccode\cplusplusdemo\classdemo\build>cl ..\classlayout\main.cpp /d1reportSingleClassLayoutD > layout.txt
```
D类对象d的内存布局如下。

```
class D	size(36):
	+---
 0	| +--- (base class B)
 0	| | {vfptr}
 4	| | {vbptr}
 8	| | b
	| +---
12	| +--- (base class C)
12	| | {vfptr}
16	| | {vbptr}
20	| | c
	| +---
24	| d
	+---
	+--- (virtual base A)
28	| {vfptr}
32	| a
	+---
```

A的内存布局
```
class A	size(8):
	+---
 0	| {vfptr}
 4	| a
	+---
```

B的内存布局
```
class B	size(20):
	+---
 0	| {vfptr}
 4	| {vbptr}
 8	| b
	+---
	+--- (virtual base A)
12	| {vfptr}
16	| a
	+---
```
C的内存布局

```
class C	size(20):
	+---
 0	| {vfptr}
 4	| {vbptr}
 8	| c
	+---
	+--- (virtual base A)
12	| {vfptr}
16	| a
	+---
```