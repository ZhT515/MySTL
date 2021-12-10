#include"MySet.h"

int main()
{
	zht::set<int, int> s;
	s.insert(1);
	s.insert(3);
	s.insert(5);
	s.insert(2);
	s.insert(13);
	s.insert(0);
	s.insert(15);
	s.insert(18);

	zht::set<int, int>::iterator sit = s.begin();
	while (sit != s.end())
	{
		cout << *sit << " ";
		++sit;
	}
	cout << endl;

	zht::set<int, int>::reverse_iterator rsit = s.rbegin();
	while (rsit != s.rend())
	{
		cout << *rsit << " ";
		++rsit;
	}
	cout << endl;

	return 0;
}