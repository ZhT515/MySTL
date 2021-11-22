#include"AVLTree.h"
#include<string>

void TestAVLTree()
{
	//int a[] = { 1, 3, 5, 7, 6 };
	//int a[] = { 16, 3, 7, 11, 9, 26, 18, 14, 15 };
	int a[] = { 4, 2, 6, 1, 3, 5, 15, 7, 16, 14 };
	AVLTree<int, int> t;
	for (auto e : a)
	{
		t.Insert(make_pair(e, e));
		cout << "" << e << "->" << t.IsAVLTree() << endl;
	}

	t.InOrder();
	cout << t.IsAVLTree() << endl;
	t.InOrder();
	t[3] *= 10;
	t[4] *= 10;
	t[5] *= 10;
	t.InOrder();

	AVLTree<string, string> dict;
	dict.Insert(make_pair("sort", ""));
	dict.Insert(make_pair("left", ""));
	dict.InOrder();

	dict["left"] = "+";
	dict["string"] = "ַ";
	dict.InOrder();
}

int main()
{
	TestAVLTree();
}