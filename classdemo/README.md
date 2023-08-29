- [classlayout](#classlayout)
- [基类无虚函数的内存布局](#基类无虚函数的内存布局)
- [普通多继承的内存布局](#普通多继承的内存布局)
- [带虚继承的内存布局](#带虚继承的内存布局)

# classlayout
查看一个文件的所有类的对象内存布局

```C++
cl [filename] /d1reportAllClassLayout
```
比如
```
D:\srccode\cplusplusdemo\classdemo\build>cl ..\classlayout\main.cpp /d1reportAllClassLayout > layout.txt
```
然后在layout.txt里搜索 对应的类。比如类名为D，则搜索"class D"。
# 基类无虚函数的内存布局
```C++
#include <iostream>
using namespace std; // 采用 4 字节对齐

#pragma pack(4)
class A
{
public:
    int a;
    void aPrintf() {
        std::cout << "This is class A" << "\n";
    }
};

class B : public A
{
public:
    int b;
    virtual void bPrintf() {
        std::cout << "This is class B" << "\n";
    }
};

class C : public A
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
输出
```
4
12
12
28
```

D类的内存布局
```
class D	size(28):
	+---
 0	| +--- (base class B)
 0	| | {vfptr}
 4	| | +--- (base class A)
 4	| | | a
	| | +---
 8	| | b
	| +---
12	| +--- (base class C)
12	| | {vfptr}
16	| | +--- (base class A)
16	| | | a
	| | +---
20	| | c
	| +---
24	| d
	+---
```

A类的内存布局
```
class A	size(4):
	+---
 0	| a
	+---
```

B类的内存布局
```
class B	size(12):
	+---
 0	| {vfptr}
 4	| +--- (base class A)
 4	| | a
	| +---
 8	| b
	+---
```

C类的内存布局
```
class C	size(12):
	+---
 0	| {vfptr}
 4	| +--- (base class A)
 4	| | a
	| +---
 8	| c
	+---
```

# 普通多继承的内存布局
```C++
#include <iostream>
using namespace std; // 采用 4 字节对齐

#pragma pack(4)
class A
{
public:
    int a;
    virtual void aPrintf() {
        std::cout << "This is class A" << "\n";
    }
};

class B : public A
{
public:
    int b;
    virtual void bPrintf() {
        std::cout << "This is class B" << "\n";
    }
};

class C : public A
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
输出为
```
8
12
12
28
```

D类的内存布局为。

```
class D	size(28):
	+---
 0	| +--- (base class B)
 0	| | +--- (base class A)
 0	| | | {vfptr}
 4	| | | a
	| | +---
 8	| | b
	| +---
12	| +--- (base class C)
12	| | +--- (base class A)
12	| | | {vfptr}
16	| | | a
	| | +---
20	| | c
	| +---
24	| d
	+---
```

A类的内存布局
```
class A	size(8):
	+---
 0	| {vfptr}
 4	| a
	+---
```
B类的内存布局
```
class B	size(12):
	+---
 0	| +--- (base class A)
 0	| | {vfptr}
 4	| | a
	| +---
 8	| b
	+---
```
C类的内存布局
```
class C	size(12):
	+---
 0	| +--- (base class A)
 0	| | {vfptr}
 4	| | a
	| +---
 8	| c
	+---
```

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
        std::cout << "This is class A" << "\n";
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
结果输出(在32位应用)
```
8
20
20
36
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
可以看到，相比不是虚继承
- 只有一份虚基类的内存
  - 非虚继承时，B、C均含一份A的数据
- 类B、C多了一vbptr
  - 在vfptr之后

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
可以看到，相比不是虚继承
- 把基类A的内存放到了最后
- 多了vbptr
  - 在vbptr之后
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