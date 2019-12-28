#include<iostream>
#include<cstdio>
#include<cstring>

using namespace std;

class CMyString
{
public:
	CMyString(const char* pdata = nullptr)
	{
		if (pdata == nullptr)//��ֹ����ֵ��һ���µĶ���
		{
			m_pdata = new char[1];
			m_pdata[0] = '\0';//һ��String���͵Ķ��󲢲��ǿգ�Ӧ���ǿ��ַ���
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
	CMyString(CMyString&& str);//�ṩ����ֵ���ò����Ŀ�������͸�ֵ��������أ��Դﵽ����ʱ������޿������á�

	CMyString& operator = (const CMyString& );
	CMyString& operator= (CMyString&& str);

	bool operator >(const CMyString&str)const
	{
		//return strlen(m_pdata) > strlen(str.m_pdata);//�����Ƚ������ַ����Ƿ���ȣ��������ǳ��ȣ��������ݡ�
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

	//String ��Ϊ�������Ĵ��ڣ�����Ҳʵ����string��ĵ������� һ�㽫������ʵ��Ϊ���Ƕ����
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

//�������캯��
CMyString::CMyString(const CMyString& str)  //��һ���Ѵ��ڵĶ���������һ���µĶ���
{
		int length = strlen(str.m_pdata);
		m_pdata = new char[length+1];
		strcpy(m_pdata, str.m_pdata);
		cout << "CMyString(CMyString& str) " << endl;
}
//�ṩ����ֵ���ò����Ŀ������캯�����Խ�� ��Ϊ������ʱ�������ɵ�Ч�ʵ��µ����⡣  
CMyString::CMyString(CMyString&& str)  //��һ���Ѵ��ڵĶ���������һ���µĶ���
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
	��Ϊadvancedд����
	if (this != &str)
	{
		CMyString tmp(str);
		char* p_tmp = tmp.m_pdata;

		tmp.m_pdata = m_pdata;
		m_pdata = p_tmp;
	}     //���þֲ��������������������ص㣬��delete�����鶼������������ȥ����Ȼ�������ռ�֮������齻���������캯��ȥ��     
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
     //������strcat(m_pdata,str.m_pdata),m_pdataָ����ڴ泤�ȸ���������
	 strcpy(ptr, ls.m_pdata);
	 strcat(ptr, rs.m_pdata);
	 //�����������ֱ��return һ����ʱ����Ļ����ᵼ��ptrָ��Ķ��ڴ��޷��ͷţ��������ڴ�й¶����
	 CMyString tmp(ptr);
	 delete[]ptr;   // ����������Ч��ʵ����̫���ˡ�   tmp�����һ�ι��죬��ptrָ������ݿ���һ�ݡ�Ȼ����ɾ��ptrָ��Ķ��ڴ棬֮���ٽ�tmp��return��ȥ��Ȼ��tmp�����������ֻ����������Զ�����һ��ͬ�������ݣ�new��2�Σ�delete��2��
	 return tmp;//Ϊ�˽�tmp����ֲ��������ȥ�������һ����ʱ����ʹ��tmp��������һ����ʱ���󷵻ػ�ȥ��
	 // return *this;    C5 = C1 + A1;    C1.operator+(A1);       ����ֵ�����*this �������д��
	 /*
	 ��������
	 CMyString tmp;
	 tmp.m_pdata = new char[length1 + length2 + 1];
	 strcpy(tmp.m_pdata, m_pdata);
	 strcat(tmp.m_pdata, str.m_pdata);
	 return tmp;
	 */
}

ostream& operator<<(ostream& out, const CMyString& str)//�ܴ����þʹ�����
{
	out << str.m_pdata;
	return out;
}
char& CMyString::operator[](int index)
{
	return m_pdata[index];
}

void CMyString::Print()//����ֵ����+������+������
{
	cout << m_pdata << endl;
}

//����������Ǹ����û��ĵ��õ�ľ���ʹ����������õġ�
/*����String�������ԣ��û�ϣ��������
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
   char *text="I love MANSNOW��"
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
	strcpy(pbuf, str5.c_str()); // c_str()  ��string����ת��const char*
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
	strcpy(pbuf, str4.c_str()); // c_str()  ��string����ת��const char*
	cout << pbuf << endl;*/


	return 0;
}
