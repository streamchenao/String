#include<iostream>
using namespace std;

/*
����Ҫ��ȷmalloc��new������
1.malloc�ǰ��ֽڿ����ڴ�Ĵ�Сʹ��ʱmalloc(sizeof(int)*n)��(��Ҳ����˵malloc����ȥ��Ҫ��ʲô���͵����ݣ�ֻ��Ҫ����ָ����С�Ŀռ����)
new��ֱ�Ӱ���ָ������,�Լ���С������new��������غ���������ֵ������ָ�����Ǹ����͡�����new int[10]����new char[5]
2.mallocֻ���ռ䣬new�������ռ仹��Կռ���г�ʼ��
3.���ڴ治��ʱ��malloc��newʧ�ܵķ�Ӧ�ǲ�һ���ġ�malloc���ؿգ�new����һ���쳣��������Ҫ��newд��try��catch����
�����free��delete������
delete�ĵ������Ȼ����ָ����ָ������͵�����������Ȼ���ٽ����ڴ�free����//����delete []p;�����p��һ����������ָ�� String *p=new String[100];
*/

#if 0
//1.�ȵ�operator new�����ڴ� 2.�ٵ��ö���Ĺ��캯�����г�ʼ��
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
//1.�ȵ�pָ��Ķ��������   2.�ٵ���operator delete�ͷ��ڴ�ռ�
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
