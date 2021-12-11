#include"RBTree.h"
#include <stdlib.h>
#include <time.h>

void TestRBTree()
{
	RBTree<int, int> t;
	int n = 10000;
	srand(time(0));
	for (int i = 0; i < n; ++i)
	{
		int e = rand();
		t.Insert(make_pair(e, e));
	}

	//t.InOrder();

	cout << t.CheckBlance() << endl;
	
}

int main()
{
	TestRBTree();

	return 0;
}