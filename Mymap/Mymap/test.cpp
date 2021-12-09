#include"MyMap.h"

int main()
{
	zht::map<int, int> m;
	m.insert(make_pair(1, 1));
	m.insert(make_pair(3, 3));
	m.insert(make_pair(0, 0));
	m.insert(make_pair(9, 9));

	zht::map<string, string> dict;
	dict["sort"];
	dict["left"] = "���";
	dict["left"] = "ʣ��";

	for (auto& e : dict)
	{
		cout << e.first << ":" << e.second << endl;
	}
	dict["sort"] = "����";

	for (auto& e : dict)
	{
		cout << e.first << ":" << e.second << endl;
	}

	return 0;
}