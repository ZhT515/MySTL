#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <assert.h>
#include <list>
#include <algorithm>
using namespace std;

namespace Zht
{
  template <class T>      //template<typename 数据类型参数标识符> 

  class vector 
  {
    public:
      typedef T* iterator;
      typedef const T* const_iterator;

      vector()                  //无参构造函数
        :_start(nullptr)
        ,_finsh(nullptr)
        ,_endofstorage(nullptr)
      {
      }

      vector(int n, const T& val = T())
        :_start(nullptr)
        ,_finsh(nullptr)
        ,_endofstorage(nullptr)
      {
        reserve(n);
        while(n--)
        {
            push_back(val);
        }
      }

      vector(const vector<T>& v)          //拷贝构造
        :_start(nullptr)
        ,_finsh(nullptr)
        ,_endofstorage(nullptr)
      {
        reservr(v.capacity());

        iterator it = begin();
        const_iterator vit = v.cbegin();

        while(vit != v.cend())
        {
          *it = *vit;
          it++;
          vit++;
        }
      }

      void swap(vector<T>& v)
      {
        std::swap(_start, v._start);
        std::swap(_finsh, v._finsh);
        std::swap(_endofstorage, v._endofstorage);
      }

      vector<T>& operator=(vector<T> v)
      {
        swap(v);
        
        return *this;
      }

      ~vector()         //析构函数
      {
        delete[]_start;
        _start = _finsh = _endofstorage = nullptr;
      }

      iterator begin()
      {
        return _start;
      }

      iterator end()
      {
        return _finsh;
      }

      const_iterator cbegin() const
      {
        return _start;
      }

      const_iterator cend() const
      {
        return _finsh;
      }

      size_t capacity()           //空间大小
      {
        return _endofstorage - _start;
      }

      size_t size()               //数据长度
      {
        return _finsh - _start;
      }

      T& operator[](size_t i)     //运算符重载
      {
        assert(i < size());

        return _start[i]; 
      }
      
      const T& operator[](size_t i) const
      {
        assert(i < size());

        return _start[i];
      }
      void reserve(size_t n)        //要求向量容量至少足以包含 n 个元素。
      {
        if(n > capacity())
        {
            size_t sz = size();
      
            T* tmp = new T[n];      //开新空间

            if(_start)              //如果为空则说明是首次
            {
                cout << sz;
                memcpy(tmp, _start, sz * sizeof(T));
                cout<< tmp[2];
                delete[]_start;
            }

            _start = tmp;
            _finsh = _start + sz;
            _endofstorage = _start + n;
        }
      }

      void resize(size_t n, const T& val = T())
      {
        if(n <= size())     //小于当前的数据量
        {
            _finsh = _start + n;
            return;
        }

        if(n > capacity())    //扩容
        {
            reserve(n);
        }

        iterator it = begin();
        _finsh = _start + n;
        while(it != _finsh)
        {
            *it = val;
            it++;
        }
      }

      iterator insert(iterator pos, const T& x)
      {
        assert(pos <= _finsh);

        if(_finsh == _endofstorage)  //检查容量
        {
          size_t a = size();
          size_t newcapacity1 = capacity() == 0 ? 4 : capacity() * 2;
          reserve(newcapacity1);

          pos = _start + a;    //如果发生增容，则需要重置pos,因为发生增容则说明pos
        }
        
        iterator end = _finsh - 1;
        while(end >= pos)
        {
          *(end + 1) = *end;
          end--;
        }

        *pos = x;
        ++_finsh;

        return pos;
      }

      void push_back(const T& x)
      {
        if(_finsh == _endofstorage)
        {
            size_t newcapacity = capacity() == 0 ? 4 : capacity() * 2;
            reserve(newcapacity);
        }

        *_finsh = x;
        _finsh++;

      }

      iterator erase(iterator pos)      //删除pos位
      {
        assert(pos <= _finsh);
        
        iterator begin = pos + 1;
        while(begin != end())
        {
            *(begin - 1) = *begin;
            begin++;
        }

        _finsh--;

        return pos;

      }

      void pop_back()       //尾删
      {
          erase(--end());
      }

    private:
      iterator _start;      //数据块开始
      iterator _finsh;      //有效尾
      iterator _endofstorage;     //空间尾

  };

  void test1()
  {
    vector<int> v;
    
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    v.push_back(5);
    //v.push_back(6);

    vector<int>::iterator it = v.begin();
    
    while(it != v.end())
    {
        cout << *it <<" ";
        it++;
    }
    cout << endl;

    for(size_t i = 0; i < v.size(); i++)
    {
        cout << v[i] << " ";
    }
    cout << endl;

    for(auto e : v)
    {
        cout << e << " ";
    }
    cout << endl;
  }
}
