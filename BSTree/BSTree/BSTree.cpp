#include"BSTree.h"
#include<string>

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

void Test2()
{
	K::BSTree<int> t, t1;
	int arr[] = { 5, 3, 4, 6, 9, 7, 1, 2, 8, 0 };

	for (auto e : arr)
	{
		t.InsertR(e);
	}

	t1 = t;

	t1.InOrder();

	t1.EraseR(5);
	t1.InOrder();

	t1.EraseR(0);
	t1.InOrder();
}

void Test3()
{
	KV::BSTree<string, string> dict;
	dict.InsertR("string", "字符串");
	dict.InsertR("tree", "树");
	dict.InsertR("left", "左边、剩余");
	dict.InsertR("right", "右边");
	dict.InsertR("sort", "排序");
	// ...插入词库中所有单词
	string str;
	while (cin >> str)
	{
		KV::BSTreeNode<string, string>* ret = dict.FindR(str);
		if (ret == nullptr)
		{
			cout << "单词拼写错误，词库中没有这个单词:" << str << endl;
		}
		else
		{
			cout << str << "中文翻译:" << ret->_val << endl;
		}
	}
}

void Test4()
{
	// 统计水果出现的次数
	string arr[] = { "苹果", "西瓜", "苹果", "西瓜", "苹果", "苹果", "西瓜", "苹果", "香蕉", "苹果", "香蕉" };
	KV::BSTree<string, int> countTree;
	for (const auto& str : arr)
	{
		// 先查找水果在不在搜索树中
		// 1、不在，说明水果第一次出现，则插入<水果, 1>
		//KV::BSTreeNode<string, int>* ret = countTree.FindR(str);
		auto ret = countTree.FindR(str);
		if (ret == NULL)
		{
			countTree.InsertR(str, 1);
		}
		else
		{
			ret->_val++;
		}
	}

	countTree.InOrder();
}

int main()
{
	Test4();

	return 0;
}