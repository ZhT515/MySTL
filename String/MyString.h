#include<string.h>
#include<assert.h>
#include<iostream>
#include<string>

using std::cout;
using std::endl;

namespace zht
{

  class string
  {
    
  public:

    typedef char* iterator; //容器迭代器本质上是指针，通过typedef给char*重定义关键字
    typedef const char* const_iterator;//迭代器需要提供const型，const 迭代器与普通迭代器在编译器处理时会进行修饰，构成了函数重载

    friend std::ostream& operator<<(std::ostream& out, const string& s);  //为了方便内部引用，所以要设置为友元
    friend std::istream& operator>>(std::istream& in, string& s);

    iterator begin()  // 开始
    {
      return _str;
    }

    const_iterator begin() const //需要提供const类型迭代器，权限只能缩小不能放大，所以在处理const类型的问题时需要使用const类型的迭代器
    {
      return _str;
    }

    iterator end()    //结束
    {
      return _str + _size;  //迭代器结束实在空间的最后一位的后一个
    }

    const_iterator end() const
    {
      return _str + _size;
    }

   // operator& 
    
    string(const char* str = "")      //构造函数，现代写法，减少创建的临时对象的个数
      :_str(new char[strlen(str) + 1])
    {
      _size = strlen(str);
      _capacity = _size;
      strcpy(_str,str);
    }

    //void swap(string& s)
    //{
     // ::swap(_str,s._str);
      //::swap(_size,s._size);
     //::swap(_capacity,s._capacity);
    //}
    
    //开空间
    void reserve(std::size_t n)
    {
      if(n > _capacity) //当N大于最大容量时扩容
      {
        char* tmp = new char[n + 1];   //创建N+1个空间，需要保存\0.
        strncpy(tmp, _str, _size + 1); //将原空间中的数据拷贝到新的中
        delete []_str;

        _str = tmp;               //更新
        _capacity = n;
      }
    }



    //开空间 + 初始化，重置capacity
    void resize(std::size_t n, char ch = '\0')
    {
      //三情况，1.小于当前的字符串长度，2.大于字符串长度但是小于空间大小；3.大于空间大小
      
      if(n < _size)  //1.直接在n处加\0
      {
          _size = n;
          _str[n] = '\0';
      
      }

      else
      {
          if(n > _capacity)     //3.扩容，然后与2.合并
           {
              reserve(n);
           }

          for(std::size_t i = _size; i < _capacity; i++)  //从当前字符串向后覆盖
          {
              _str[i] = ch;
          }

          _str[_capacity] = '\0';
          _size = n;
      }

    }

    void swap(string& s)
		{
			std::swap(_str, s._str);
			std::swap(_size, s._size);
			std::swap(_capacity, s._capacity);
		}

    string(const string& s) //拷贝构造函数，现代写法，通过创建一个新对象，交换，达到拷贝构造的目的
      :_str(NULL)
      ,_size(0)
      ,_capacity(0)
    {
      string tmp(s._str);
      swap(tmp);
    }

    //binstring& operator+= (char ch)
    //{
      
    //}

    string& operator=(string s)   // = 运算符重载
    {

      swap(s);

      return *this;
    
    }
    
    ~string()
    {
      delete [] _str;
      _str = NULL;
      _size = 0;
      _capacity = 0;
    }
    
    void clear()
    {
      _size = 0;
      _str[0] = '\0';
    
    }
    
    //可读可写
    char& operator[](std::size_t i)
    {
        assert(i < _size);    //\0,所以闭区间

        return _str[i];
    }

    //只读
    const char& operator[](std::size_t i) const
    {
        assert(i < _size);

        return _str[i];
    } 
    ///返回对象中的字符串，用const
    const char* c_str() const
    {
      return _str;
    }

    //pos位置插入
    string& insert(std::size_t pos, char ch)
    {
      assert(pos <= _size);   //可以尾插，所以可以等于

      //先判断是否需要扩容
      if(_size == _capacity)
      {
          reserve(_capacity == 0 ? 4 : _capacity * 2);
      }

      //将数据后移
      char* end = _size + _str;  //从\0开始挪

      while (end >= _str + pos)//pos位需要挪
      {
          *(end + 1) = *end;  //end向后挪也就是end-1
          --end;              //再向前
      }

      *(_str + pos) = ch;
      _size++;

      return *this;
    }

    //插入字符串
    string& insert(std::size_t pos,const char* str)
    {
      assert(pos <= _size);

      std::size_t len = strlen(str);
      
      if(_size + len >  _capacity)//可能会直接大于
      {
          reserve(_size + len);
      }

      char* end = _size + _str;
      while(end >= pos + _str)
      {
          *(end + len) = *end;
          --end;
      }

      strncpy(_str + pos, str, len);
      _size += len;

      return *this;
    }

    void push_back(char ch)           //尾插字符
    {
        insert(_size,ch);
    }

    void append(const char* str)      //尾插字符串
    {
        insert(_size, str);
    }

    string& operator+=(char ch)         //重载+=字符
    {
        push_back(ch);
        return *this;
    }

    string& operator+=(const char* str) //重载+=字符串
    {
        append(str);
        return *this;
    }

    string& erase(std::size_t pos,std::size_t len = -1)
    {
        assert(pos < _size);
        //两种情况：
        //1.剩余长度小于需要删除的
        //2.剩余长度大于需要删除的
        std::size_t LeftLen = _size - pos;

        if(LeftLen <=  len)   // 小于，全删除
        {
            _str[pos] = '\0';
            _size = pos;
        }
        else                  //大于，len位向前补。
        {
            strcpy(_str + pos, _str + pos + len);
            _size -= len;
        }

        return *this;
    }

    std::size_t find (char ch, std::size_t pos = 0)
    {
        assert(pos < _size);

        for(std::size_t i = pos; i < _size; ++i)
        {
            if(_str[i] == ch)
            {
                return i;
            }
        }

        return -1;
    }

    std::size_t find (const char* str, std::size_t pos = 0)
    {
        assert(pos < _size);

        const char* ret = strstr(_str + pos, str);    //函数返回在 haystack 中第一次出现 needle 字符串的位置，如果未找到则返回 null。
        if(ret)
        {
          return ret - _str;
        }
        else{
          return -1;
        }

    }

    std::size_t size() const
    {
        return _size;
    }
  private:

    char* _str;                   //字符串指针
    std::size_t _size;            //使用的空间大小
    std::size_t _capacity;        //空间大小

  };

  inline bool operator<(const string& s1, const string& s2)
  {
      return strcmp(s1.c_str(), s2.c_str()) < 0;        //strcmp(str1,str2)，若str1=str2，则返回零；若str1<str2，则返回负数；若str1>str2，则返回正数
  }

  inline bool operator==(const string& s1, const string& s2)
  {
      return strcmp(s1.c_str(), s2.c_str()) == 0;
  }

  inline bool operator<=(const string& s1, const string& s2)
  {
      return s1 < s2 || s1 == s2;
  }

  inline bool operator!=(const string& s1, const string& s2)
  {
      return !(s1 == s2);
  }

  inline bool operator>(const string& s1, const string& s2)
  {
      return !(s1 <= s2);
  }

  inline bool operator>=(const string& s1, const string& s2)
  {
      return !(s1 < s2);
  }
  

  std::ostream& operator<<(std::ostream& out, const string& s) 
    //因为cout的输出流对象和隐含的this指针在抢占第一个参数的位置。this指针默认是第一个参数也就是左操作数了。
    //但是实际使用中cout需要是第一个形参对象，才能正常使用。
    //友元函数可以访问
   {
     for(auto ch : s)    //使用范围for遍历字符串
      {
         out << ch;       //输出到输出流
      }

      return out;
   }

  std::istream& operator>>(std::istream& in,string& s)
  {
    s.clear();

    char ch;
    ch = in.get();
    while(ch != ' ' && ch != '\n')
    {
      s += ch;
      ch = in.get();
    }
    
    return in;
  }

  void test1()
  {
    string s1("hello world");
    string s2(s1);
    string s3 = s1;
    string s4 = "hi";

    std::cout << s1.c_str() << std::endl;
    std::cout << s2.c_str() << std::endl;
    std::cout << s3.c_str() << std::endl;
    std::cout << s4.c_str() << std::endl;
  }

}  
