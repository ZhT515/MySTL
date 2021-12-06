#include "MyStack.h"

int main()
{
  Zht::stack<int> c;
  c.push(1);
  c.push(2);
  c.push(3);
  c.push(4);
  c.push(5);

  while(!c.empty())
  {
    cout << c.top() << " ";
    c.pop();
  }

  cout << endl;
}
