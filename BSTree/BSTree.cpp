#include"BSTree.h"

void Test1()
{
	K::BSTree<int> t;
	int arr[] = {5, 3, 4, 6, 9, 7, 1, 2, 8, 0};

	for (auto e : arr) 
	{
		t.InsertR(e);
	}

	t.InOrder();

	t.EraseR(5);
	t.InOrder();

	t.EraseR(0);
	t.InOrder();
}

int main()
{
	Test1();

	return 0;
}