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
    // �չ��졣
    myshared_ptr() :m_ptr(NULL), m_ref_count(NULL) {}
    // ���캯��
    myshared_ptr(T* t) {
        m_ptr = t;
        // ��ʼ�����ü���
        m_ref_count = new atomic_int(1);
        cout << "myshared_ptr num " << *m_ref_count << endl;
    }
    //�������캯��
    myshared_ptr(const myshared_ptr& ptr) {
        m_ptr = ptr.m_ptr;
        // ���ü�����1
        m_ref_count = ptr.m_ref_count;
        (*m_ref_count)++;
        cout << "myshared_ptr num " << *m_ref_count << endl;
    }
    //to do �ƶ����캯��
    myshared_ptr( myshared_ptr&& ptr) {
        m_ptr = ptr.m_ptr;
        ptr.m_ptr = nullptr;
        // ���ü�����1
        m_ref_count = ptr.m_ref_count;
        ptr.m_ref_count = nullptr;
        cout << "myshared_ptr num " << *m_ref_count << endl;
    }

    // ����operator=��Ȼ���µĶ������ô�����һ��
    myshared_ptr& operator =(const myshared_ptr& ptr) {
        m_ptr = ptr.m_ptr;
        // ���ü�����1
        m_ref_count = ptr.m_ref_count;
        (*m_ref_count)++;
        cout<< "myshared_ptr num " << *m_ref_count << endl;
        return *this;
    }
    // ����
    ~myshared_ptr()
    {
        if (m_ref_count)
        {
            cout << "~myshared_ptr num " << *m_ref_count << endl;
            if (--(*m_ref_count) == 0)
            {
                cout << "~myshared_ptr delete " << *m_ref_count << endl;
                delete m_ref_count;
                delete m_ptr;
            }
            else {
                cout << "~myshared_ptr delete " << *m_ref_count << endl;
            }
        }
    }
    // ���� operator*��operator-> ʵ��ָ�빦��
    T operator *() { return *m_ptr; }
    T* operator ->() { return m_ptr; }
    // ��ȡԭָ��
    T* get() { return m_ptr; }
    // ��ȡ����
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

int main(int argc, char* argv[])
{
    test();
}

