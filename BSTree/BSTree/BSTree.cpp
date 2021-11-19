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
	dict.InsertR("string", "�ַ���");
	dict.InsertR("tree", "��");
	dict.InsertR("left", "��ߡ�ʣ��");
	dict.InsertR("right", "�ұ�");
	dict.InsertR("sort", "����");
	// ...����ʿ������е���
	string str;
	while (cin >> str)
	{
		KV::BSTreeNode<string, string>* ret = dict.FindR(str);
		if (ret == nullptr)
		{
			cout << "����ƴд���󣬴ʿ���û���������:" << str << endl;
		}
		else
		{
			cout << str << "���ķ���:" << ret->_val << endl;
		}
	}
}

void Test4()
{
	// ͳ��ˮ�����ֵĴ���
	string arr[] = { "ƻ��", "����", "ƻ��", "����", "ƻ��", "ƻ��", "����", "ƻ��", "�㽶", "ƻ��", "�㽶" };
	KV::BSTree<string, int> countTree;
	for (const auto& str : arr)
	{
		// �Ȳ���ˮ���ڲ�����������
		// 1�����ڣ�˵��ˮ����һ�γ��֣������<ˮ��, 1>
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