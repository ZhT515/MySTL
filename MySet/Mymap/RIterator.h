#pragma once

// 反向迭代器--迭代器适配器
template<class Iterator>
struct ReverseIterator
{
	typedef typename Iterator::reference Ref;
	typedef typename Iterator::pointer Ptr;
	typedef ReverseIterator<Iterator> Self;

	Iterator _it;

	ReverseIterator(Iterator it)
		:_it(it)
	{}

	Ref operator*()		//*返回引用
	{
		return *_it;
	}

	Ptr operator->()		//->返回指针 可以直接用
	{
		return _it.operator->();
	}

	bool operator != (const Self& s) const
	{
		return _it != s._it;
	}

	bool operator == (const Self& s) const
	{
		return _it == s._it;
	}

	Self operator++()
	{
		--_it;
		return *this;
	}

	Self operator--()
	{
		++_it;
		return *this;
	}
	
};
