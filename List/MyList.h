#include <iostream>
#include <list>
#include <assert.h>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

namespace Zht
{
  template <class T>
  
  struct _list_node     //list本身和list的节点是不同的结构，所以需要分开设计，这里是list节点的结构
  {
      T val;              //数据
      _list_node<T>* _next;       //下一个节点指针
      _list_node<T>* _prev;       //上一个
      
      _list_node(const T& val = T())      //构造节点
        :val(val)                         //传的参
        ,_prev(nullptr)
        ,_next(nullptr)
      {
          
      }
  };

  template <class T, class Ref, class Ptr>
  struct _list_iterator 
  {
    typedef _list_node<T> node;
    typedef _list_iterator<T, Ref, Ptr> self;

    node* _pnode;      //迭代器本质上是指针

    _list_iterator(node* pnode)     //构造函数
      :_pnode(pnode)
    {
    }

    //这里，成员都是只需浅拷贝，所以拷贝构造，析构 ，重载 = 都可以使用默认
   
    Ref operator*()       //重载*,通过Ref灵活的调整const和普通。
    {
      return _pnode->val;
    }

    bool operator!=(const self& s) const 
    {
      return _pnode != s._pnode;
    }

    bool operator==(const self& s) const 
    {
      return _pnode == s._pnode;
    }

    self& operator++()            //++就是指向下一个节点
    {
      _pnode = _pnode->_next;

      return *this;
    }

    self operator++(int)          //C++规定后缀调用需要有一个int型参数作为区分前缀与后缀调用的区别
    {
      self tmp (*this);
      ++*this;
      return tmp;                 //*this++后++
    }

    self& operator--()
    {
      _pnode = _pnode->_prev;

      return *this;
    }

    self operator--(int)
    {
      self tmp (*this);
      --*this;
      return tmp;
    }

    Ptr operator->()
    {
      return &(operator*());      
    }
  };

  template <class T>
  class list
  {
      typedef _list_node<T> node;
  public:
      typedef _list_iterator<T, T&, T*> iterator;
      typedef _list_iterator<T,const T&, const T*> const_iterator;


      list()      //构造函数，构造哨兵位
      {
          _head = new node;           //开一个节点
          _head->_next = _head;       //初始化节点
          _head->_prev = _head;
      }

      template <class Iterator>
      list(Iterator first, Iterator last)
      {
        _head = new node;
        _head->_next = _head;
        _head->_prev = _head;

        while(first != last)
        {
          push_back(*first);
          first++;
        }
      }

      list(const list<T>& It)
      {
        _head = new node;                   //构造哨兵节点
        _head->_next = _head;     
        _head->_prev = _head;

        for(const auto& e : It)             //逐个尾插
        {
          push_back(e);
        }
      }

      list<T>& operator=(list<T> It)
      {
        swap(_head, It._head);

        return *this;
      }

      ~list()
      {
        clear();
        delete _head;
        _head = nullptr;
      }

      iterator begin()
      {
        return iterator(_head->_next);
      }

      const_iterator begin() const            //调用const的迭代器，返回一个用_head->next构造的迭代器对象
      {
        return const_iterator(_head->_next); 
      }

      iterator end()                          
      {
        return iterator(_head);
      }

      const_iterator end() const 
      {
        return const_iterator(_head);
      }

      void push_back(const T& x)        //只有哨兵位的也可以通用
      {
         /* node* newnode = new node(x);    //创建新节点
          node* tail = _head->_prev;      //当前的最后一个节点

          tail->_next = newnode;        
          newnode->_next = _head;
          newnode->_prev = tail;
          _head->_prev = newnode;*/

        insert(iterator(end()), x);     //前一个就是尾
      }
      
      void push_front(const T& x)
      {
        insert(iterator(begin),x);
      }

      void pop_back()
      {
        erase(iterator(--end()));
      }

      void pop_front()
      {
        erase(iterator(begin()));
      }
      
      iterator insert(iterator pos, const T& val)       //pos位置前插入
      {
        node* newnode = new node(val);
        node* tail = pos._pnode;

        newnode->_next = tail;
        newnode->_prev = tail->_prev;
        newnode->_prev->_next = newnode;              //还要让前一个指向自己
        pos._pnode->_prev = newnode;

        return iterator(newnode);                     //需要返回迭代器；
      }

      iterator erase(iterator pos)    //删除pos位置        
      {
        assert(pos._pnode);
        assert(pos != end());

        node* tail = pos._pnode;
        node* ret = tail->_next;
        tail->_prev->_next = tail->_next;
        tail->_next->_prev = tail->_prev;

        delete tail;

        return iterator(ret);       //返回下一个
      }

      bool empty()
      {
        return begin() == end();
      }

      size_t size()
      {
        size_t sz = 0;
        iterator it = begin();

        while(it != end())
        {
          sz++;
          it++;
        }

        return sz;
      }

      void clear()
      {
        iterator it = begin();

        while(it != end())
        {
          erase(it);
          it++;
        }
      }

      

  private:
      node* _head;
  };
  
  void PrintList(const list<int>& It)
  {
    list<int>::const_iterator it = It.begin();

    while(it != It.end())
    {
      cout << *it <<endl;
      ++it;
    }
    
    cout << endl;
  }

  void test1()
  {
    list<int> lt;
		lt.push_back(1);
		lt.push_back(2);
		lt.push_back(3);
		lt.push_back(4);

		list<int>::iterator it = lt.begin();
		while (it != lt.end())
		{
			*it += 1;
			cout << *it << " ";
			++it;
		}
		cout << endl;

		for (auto e : lt)
		{
			cout << e << " ";
		}
		cout << endl;

		PrintList(lt);
  }
}
