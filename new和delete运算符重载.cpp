#include<iostream>
using namespace std;

/*
首先要明确malloc和new的区别
1.malloc是按字节开辟内存的大小使用时malloc(sizeof(int)*n)。(这也就是说malloc不会去管要放什么类型的数据，只需要开辟指定大小的空间就行)
new是直接按所指定类型,以及大小，调用new运算符重载函数。返回值就是所指定的那个类型。例如new int[10]或者new char[5]
2.malloc只开空间，new不仅开空间还会对空间进行初始化
3.当内存不足时，malloc和new失败的反应是不一样的。malloc返回空，new返回一个异常，常常需要将new写在try，catch块中
其次是free和delete的区别
delete的调用首先会调用指针所指向的类型的析构函数，然后再将堆内存free掉。//比如delete []p;这里的p是一个对象数组指针 String *p=new String[100];
*/

#if 0
//1.先调operator new开辟内存 2.再调用对象的构造函数进行初始化
void* operator new(size_t size)
{
	void* p = malloc(size);
	if (p == nullptr)
	{
		throw bad_alloc();
	}
	return p;
}
//delete p
//1.先调p指向的对象的析构   2.再调用operator delete释放内存空间
void operator delete(void* ptr)
{
	free(ptr);
}

void* operator new[](size_t size)
{
	void* p = malloc(size);
	if (p == nullptr)
	{
		throw bad_alloc();
	}
	return p;
}
void operator delete[](void* ptr)
{
	free(ptr);
}

class MyTest 
{
public:
	MyTest(int data = 10) :ptr(new int[data]() ) {}
	~MyTest()
	{
		delete[]ptr;
		ptr = nullptr;
		cout << "~MyTest" << endl;
	}
private:
	int* ptr;
};


int main()
{
	MyTest* p = new MyTest();
	delete p;
	/*try
	{
		int* p = new int;
		delete p;
	}
	catch (const bad_alloc&err)
	{
		cerr << err.what() << endl;
	}*/
	

	return 0;
}
#endif
