#include <iostream>
#include <vector>

using namespace std;

namespace Zht
{
  template <class T, class Container = vector<T> >    //stack是容器适配器，以现有的容器作为底部结构，将其改造为符合栈需求的接口。
  class stack 
  {
  public:
    void push(const T& x)
    {
      _c.push_back(x);
    }

    void pop()
    {
      _c.pop_back();
    }

    T top()
    {
      return _c.back();
    }

    size_t size()
    {
      return _c.size();
    }

    bool empty()
    {
      return _c.empty();
    }
  private:
    Container _c;
  };
}
