#include<iostream>
#include<cstdio>
#include<cstring>

using namespace std;

class CMyString
{
public:
	CMyString(const char* pdata = nullptr)
	{
		if (pdata == nullptr)//防止赋空值给一个新的对象
		{
			m_pdata = new char[1];
			m_pdata[0] = '\0';//一个String类型的对象并不是空，应该是空字符串
		}
		else
		{
			int _size = strlen(pdata);
			m_pdata = new char[_size + 1];
			strcpy(m_pdata, pdata);	
		}
		cout << "CMyString(const char* pdata)" << endl;
	}
	CMyString(const CMyString&);
	CMyString(CMyString&& str);//提供带右值引用参数的拷贝构造和赋值运算符重载，以达到对临时对象的无开销利用。

	CMyString& operator = (const CMyString& );
	CMyString& operator= (CMyString&& str);

	bool operator >(const CMyString&str)const
	{
		//return strlen(m_pdata) > strlen(str.m_pdata);//×，比较两个字符串是否相等，不仅仅是长度，还有内容。
		return strcmp(m_pdata, str.m_pdata) > 0;
	}
	bool operator <(const CMyString& str)const
	{
		return strcmp(m_pdata, str.m_pdata) < 0;
	}
	bool operator ==(const CMyString& str)const
	{
		return  strcmp(m_pdata, str.m_pdata) == 0;
	}
	char& operator[](int index);
	void Print();
	int length() const
	{
		return strlen(m_pdata);
	}
	const char* c_str()const
	{
		return m_pdata;
	}
	~CMyString()
	{
		delete[]m_pdata;
		m_pdata = nullptr;
		cout << "~CMyString()" << endl;
	}

	//String 作为近容器的存在，库中也实现了string类的迭代器。 一般将迭代器实现为类的嵌套类
	class iterator
	{
	private:
		char* _ptr;

	public:
		iterator(char* src =nullptr)
		{
			_ptr = src;
		}
		void operator ++()
		{
			_ptr++;
		}
		char& operator* ()
		{
			return *_ptr;
		}
		bool operator!= (const iterator& it2)
		{
			return _ptr != it2._ptr;
		}
	};
	iterator begin()
	{
		return iterator(m_pdata);
	}
	iterator end()
	{
		return iterator(m_pdata+strlen(m_pdata));
	}

	//MyString::iterator it = str1.begin()   ++;!=;*;  begin() ,end();
private:
		char* m_pdata;
		friend CMyString operator+(const CMyString& lstr, const CMyString& rstr);
		friend ostream& operator<<(ostream& out, const CMyString& str);
};

//拷贝构造函数
CMyString::CMyString(const CMyString& str)  //用一个已存在的对象来构造一个新的对象
{
		int length = strlen(str.m_pdata);
		m_pdata = new char[length+1];
		strcpy(m_pdata, str.m_pdata);
		cout << "CMyString(CMyString& str) " << endl;
}
//提供带右值引用参数的拷贝构造函数，以解决 因为生成临时对象而造成的效率低下的问题。  
CMyString::CMyString(CMyString&& str)  //用一个已存在的对象来构造一个新的对象
{
	m_pdata = str.m_pdata;
	str.m_pdata = nullptr;
}
CMyString& CMyString::operator = (const CMyString& str)
{
	if (this == &str)
		return *this;

	delete[]m_pdata;

	m_pdata = new char[strlen(str.m_pdata) + 1];
	strcpy(m_pdata, str.m_pdata);
	cout << "operator=(const CMyString& str) " << endl;
	return *this;
	/*
	更为advanced写法是
	if (this != &str)
	{
		CMyString tmp(str);
		char* p_tmp = tmp.m_pdata;

		tmp.m_pdata = m_pdata;
		m_pdata = p_tmp;
	}     //利用局部对象出作用域会析构的特点，把delete的事情都交给析构函数去做。然后把申请空间之类的事情交给拷贝构造函数去做     
	return *this;*/
}
CMyString& CMyString::operator = (CMyString&& str)
{
	m_pdata = str.m_pdata;
	str.m_pdata = nullptr;

	return *this;
}

CMyString operator+(const CMyString& ls,const CMyString& rs)
{
	int len = ls.length() + rs.length();
	char* ptr = new char[len +1];
     //不能先strcat(m_pdata,str.m_pdata),m_pdata指向的内存长度根本不够。
	 strcpy(ptr, ls.m_pdata);
	 strcat(ptr, rs.m_pdata);
	 //但是如果这样直接return 一个临时对象的话，会导致ptr指向的堆内存无法释放，而导致内存泄露问题
	 CMyString tmp(ptr);
	 delete[]ptr;   // 但是这样做效率实在是太低了。   tmp会调用一次构造，将ptr指向的内容拷贝一份。然后再删除ptr指向的堆内存，之后再将tmp，return回去。然后tmp出了作用域又会析构。所以对于那一块同样的内容，new了2次，delete了2次
	 return tmp;//为了将tmp这个局部对象带出去，会产生一个临时对象，使用tmp拷贝构造一个临时对象返回回去。
	 // return *this;    C5 = C1 + A1;    C1.operator+(A1);       返回值如果是*this ×错误的写法
	 /*
	 方法二：
	 CMyString tmp;
	 tmp.m_pdata = new char[length1 + length2 + 1];
	 strcpy(tmp.m_pdata, m_pdata);
	 strcat(tmp.m_pdata, str.m_pdata);
	 return tmp;
	 */
}

ostream& operator<<(ostream& out, const CMyString& str)//能传引用就传引用
{
	out << str.m_pdata;
	return out;
}
char& CMyString::operator[](int index)
{
	return m_pdata[index];
}

void CMyString::Print()//返回值类型+作用域+函数名
{
	cout << m_pdata << endl;
}

//函数的设计是根据用户的调用点的具体使用情况来调用的。
/*对于String这个类而言，用户希望这样用
   String a="123";
   String b("456");     String a(a);
   String c="123456789";
   String b(a,5,3);
   String d=a+c;
   String str1="abc"+str0;
	String str2=str1+"abc";
   //String a="";  String a("");String a=nullptr;

   String b=a;
   String c=d=b;
   char *text="I love MANSNOW！"
   String e=text;
*/

CMyString GetString(CMyString& str)
{
	const char* pstr = str.c_str();
	CMyString tmpStr(pstr);
	return tmpStr;
}

int main()
{
	/*const char* p = "123";
	CMyString C1(p);
	C1.Print();
	CMyString A1("456");
	A1.Print();
	CMyString C5;
	C5 = C1 + A1;
	C5.Print();*/

	CMyString str1 = "abc";
	//CMyString str2 ="abc";
	CMyString str3;
	CMyString str4;
	str3 = str4 = str1;

	CMyString str5("dsfhkslkadhf");
	char* pbuf = new char[str5.length() + 1];
	strcpy(pbuf, str5.c_str()); // c_str()  将string类型转成const char*
	cout << pbuf << endl;

	for (int i = 0; i < str5.length(); ++i)
	{
		cout << str5[i] << " ";
	}
	cout << endl;

	CMyString::iterator it = str5.begin();
	for (; it != str5.end(); ++it)
	{
		cout << *it;
	}
	cout << endl;
	/*const int& a = 10;
	int&& b = 20;*/

	


	/*
	if (str1 > str2)
	{
		cout << str1 << endl;
	}
	if (str2 <= str3)
	{
		cout << str3 << endl;
	}
	if (str3 == str4)
	{
		cout << "true" << endl;
	}
	for (int i = 0; i < str4.length(); ++i)
	{
		// char& operator[](int index)
		// str6[i] = 'a';
		cout << str4[i] << " ";
	}
	cout << endl;

	// string str = "wiyuuoiiuo"    char* => string
	char* pbuf = new char[str4.length() + 1];
	strcpy(pbuf, str4.c_str()); // c_str()  将string类型转成const char*
	cout << pbuf << endl;*/


	return 0;
}
