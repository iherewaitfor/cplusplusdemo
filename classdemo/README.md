- [classlayout](#classlayout)
- [基类无虚函数的内存布局](#基类无虚函数的内存布局)
- [普通多继承的内存布局](#普通多继承的内存布局)
  - [虚函数表（无虚继承）](#虚函数表无虚继承)
- [带虚继承的内存布局](#带虚继承的内存布局)
  - [虚函数表](#虚函数表)
  - [虚基表](#虚基表)
- [深拷贝（拷贝构造函数）](#深拷贝拷贝构造函数)

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
## 虚函数表（无虚继承）
可以看出来，有两个表
- D::$vftable@B@:
  - 包含了父类B类的所有虚函数（包括B类从A类继承来的）
  - 然后是D类自己的虚函数
- D::$vftable@C@:
  - 包含了父类C类的所有虚函数（包括D类从A类继承来的），但有被重写的虚函数
  - 可以看到D类重写了C类的cPrintf函数。
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
D::$vftable@B@:
	| &D_meta
	|  0
 0	| &A::aPrintf
 1	| &B::bPrintf
 2	| &D::dPrintf
D::$vftable@C@:
	| -12
 0	| &A::aPrintf
 1	| &D::cPrintf
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

## 虚函数表
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

D::$vftable@B@:
	| &D_meta
	|  0
 0	| &B::bPrintf 
 1	| &D::dPrintf 

D::$vftable@C@:
	| -12
 0	| &D::cPrintf 

D::$vbtable@B@:
 0	| -4
 1	| 24 (Dd(B+4)A)

D::$vbtable@C@:
 0	| -4
 1	| 12 (Dd(C+4)A)

D::$vftable@A@:
	| -28
 0	| &A::aPrintf 
```

特别摘出来。
可以看出，其实有3个虚函数表。具体偏移对照类的内存布局
- D::$vftable@B@:
    - 该表的指针位置在类内存偏移为0
    - 可以看出D类没有重写父类B的bPrintf函数
    - D类包括了所有的B类的虚函数，然后是D类自己定义的虚函数
    - 注意这里不含B类虚继承的父类A的虚函数。A的虚函数在别外一张单独的表。
- D::$vftable@C@:
    - 该表的指针位置在类内存偏移为12
    - 在这里可以看到，D类重写了父类C的cPrintf函数。
    - 注意这里不含C类虚继承的父类A的虚函数。A的虚函数在别外一张单独的表。
- D::$vftable@A@:
    - 该表的指针位置在类内存偏移28
    - 包含了所有的A的虚函数
    - D类没有重写A的虚函数
```
D::$vftable@B@:
	| &D_meta
	|  0
 0	| &B::bPrintf 
 1	| &D::dPrintf 

D::$vftable@C@:
	| -12
 0	| &D::cPrintf 

D::$vbtable@B@:
 0	| -4
 1	| 24 (Dd(B+4)A)

D::$vbtable@C@:
 0	| -4
 1	| 12 (Dd(C+4)A)

D::$vftable@A@:
	| -28
 0	| &A::aPrintf 
```

## 虚基表

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

D::$vftable@B@:
	| &D_meta
	|  0
 0	| &B::bPrintf 
 1	| &D::dPrintf 

D::$vftable@C@:
	| -12
 0	| &D::cPrintf 

D::$vbtable@B@:
 0	| -4
 1	| 24 (Dd(B+4)A)

D::$vbtable@C@:
 0	| -4
 1	| 12 (Dd(C+4)A)

D::$vftable@A@:
	| -28
 0	| &A::aPrintf 
```

单独摘出来。可以看出来有两个虚基表
- D::$vbtable@B@:
  - 该表指针的位置，位于基类B的位置偏移4
  - 虚基类A的偏移为该vbptr指针后的24
- D::$vbtable@C@:
  - 该表指针的位置，位于基类D的位置偏移4
  - 虚基类A的偏移为该vbptr指针后的12
```
D::$vbtable@B@:
 0	| -4
 1	| 24 (Dd(B+4)A)

D::$vbtable@C@:
 0	| -4
 1	| 12 (Dd(C+4)A)
```

# 深拷贝（拷贝构造函数）
A(const A & a)