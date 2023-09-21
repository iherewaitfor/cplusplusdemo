#include <iostream>
#include <string>
#include <memory>
#include <atomic>
using namespace std;
template <class T>
class myshared_ptr {
private:
    T* m_ptr;
    atomic_int* m_ref_count;

public:
    // 空构造。
    myshared_ptr() :m_ptr(nullptr), m_ref_count(nullptr) {}
    // 构造函数
    myshared_ptr(T* t) {
        m_ptr = t;
        // 初始化引用计数
        m_ref_count = new atomic_int(1);
        cout << "myshared_ptr construct num " << *m_ref_count << endl;
    }
    //拷贝构造函数
    myshared_ptr(const myshared_ptr& ptr) {
        m_ptr = ptr.m_ptr;
        // 引用计数加1
        m_ref_count = ptr.m_ref_count;
        if (m_ref_count) {
            (*m_ref_count)++;
        }
        cout << "myshared_ptr copy construct num " << *m_ref_count << endl;
    }
    //to do 移动构造函数
    myshared_ptr( myshared_ptr&& ptr) {
        m_ptr = ptr.m_ptr;
        ptr.m_ptr = nullptr;
        m_ref_count = ptr.m_ref_count;
        ptr.m_ref_count = nullptr;
        cout << "myshared_ptr move construct num " << *m_ref_count << endl;
    }

    // 重载operator=，然后将新的对象引用次数加一。
    // 赋值操作符是两个现有对象的操作。把一个对象给另一个对象。
    myshared_ptr& operator =(const myshared_ptr& ptr) {
        //先处理本myshared_ptr原来管理的指针的逻辑。
        if (m_ptr) {
            //交出原来指针的管理权。若自己是最后的管理者，则释放指针。
            if (m_ref_count && --(*m_ref_count) == 0) {
                delete m_ptr;
                m_ptr = nullptr;
            }
        }
        //处理新管理的指针。
        m_ptr = ptr.m_ptr;

        // 引用计数加1
        m_ref_count = ptr.m_ref_count;
        if (m_ref_count) {
            (*m_ref_count)++;
        }
        cout<< "myshared_ptr operator = num " << *m_ref_count << endl;
        return *this;
    }
    // 析构
    ~myshared_ptr()
    {
        if (m_ref_count)
        {
            cout << "~myshared_ptr num " << *m_ref_count << endl;
            if (--(*m_ref_count) == 0)
            {
                cout << "~myshared_ptr delete " << *m_ref_count << endl;
                delete m_ref_count; //释放计数器
                if (m_ptr) {
                    delete m_ptr; //释放被管理对象
                }
            }
            else {
                cout << "~myshared_ptr delete " << *m_ref_count << endl;
            }
        }
    }
    // 重载 operator*和operator-> 实现指针功能
    T operator *() { return *m_ptr; }
    T* operator ->() { return m_ptr; }
    // 获取原指针
    T* get() { return m_ptr; }
    // 获取计数
    int use_count() { 
        if (m_ref_count) {
            return *m_ref_count;
        }
        return 0;
    }
};

class B;
class A
{
public:
    myshared_ptr<B> m_b;
    A() {}
    ~A() { cout << "~A->B num " << m_b.use_count() << endl;
    }
};

class B
{
public:
    myshared_ptr<A> m_a;
    B() {}
    ~B() { cout << "~B->A num " << m_a.use_count() << endl;
    }
};

void test()
{
    myshared_ptr<A> a(new A);
    myshared_ptr<B> b(new B);
    cout << "A num " << a.use_count() << endl;
    cout << "B num " << b.use_count() << endl;
    a->m_b = b;
    //b->m_a = a;
    cout << "A num " << a.use_count() << endl;
    cout << "B num " << b.use_count() << endl;
}
void testoperatorequal() {
    myshared_ptr<B> pb1(new B);
    myshared_ptr<B> pb2(new B);
    pb1 = pb2;
    cout << "pb2 num " << pb2.use_count() << endl;
}
void testcopy() {
    myshared_ptr<B> pb1(new B);
    myshared_ptr<B> pb2(pb1);
    cout << "pb2 num " << pb2.use_count() << endl;
}
void testnullequal() {
    myshared_ptr<B> pb2;
    myshared_ptr<B> pb1(new B);
    pb2 = pb1;
    cout << "pb2 num " << pb2.use_count() << endl;
}

int main(int argc, char* argv[])
{
    //test();
    //testoperatorequal();
    testnullequal();
    //testcopy();
}

