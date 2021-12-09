#pragma once
#include<iostream>
#include"RIterator.h"

using namespace std;

enum Colour		//红黑树颜色枚举
{
	RED,
	BLACK,
};

template<class T>				//pair类型的T
struct RBTreeNode					//节点结构体
{
	RBTreeNode<T>* _left;		//左子树
	RBTreeNode<T>* _right;		//右子树
	RBTreeNode<T>* _parent;		//父节点

	T _data;

	Colour _col;

	RBTreeNode(const T& x)	//构造函数
		: _left(nullptr)
		, _right(nullptr)
		, _parent(nullptr)
		, _data(x)
		, _col(RED)
	{}
};

template<class T, class Ref, class Ptr>
struct __TreeIterator
{
	typedef Ref reference;
	typedef Ptr pointer;

	typedef RBTreeNode<T> Node;
	typedef __TreeIterator<T, Ref, Ptr> Self; 

	Node* _node;

	__TreeIterator(Node* node)
		:_node(node)
	{}

	Ref operator*()		//*返回引用
	{
		return _node->_data;
	}

	Ptr operator->()		//->返回指针 可以直接用
	{
		return &_node->_data;
	}

	bool operator != (const Self& s) const
	{
		return _node != s._node;
	}

	bool operator == (const Self& s) const
	{
		return _node == s._node;
	}

	Self operator++()
	{
		if (_node->_right)
		{
			Node* left = _node->_right;
			while (left->_left)
			{
				left = left->_left;
			}

			_node = left;
		}
		else
		{
			Node* cur = _node;
			Node* parent = _node->_parent;
			
			while (parent&& cur == parent->_right)
			{
				cur = cur->_parent;
				parent = parent->_parent;
			}

			_node = parent;
		}

		return *this;
	}

	Self operator--()
	{
		if (_node->_left)
		{
			Node* right = _node->_left;
			while (right->_left)
			{
				right = right->_left;
			}

			_node = right;
		}
		else
		{
			Node* cur = _node;
			Node* parent = _node->_parent;

			while (parent && cur == parent->_left)
			{
				cur = cur->_parent;
				parent = parent->_parent;
			}

			_node = parent;
		}

		return *this;
	}
};

template<class K, class T, class KeyOfT>		//通过仿函数来取key的值进行比较
class RBTree
{
	typedef RBTreeNode<T> Node;
private:
	Node* _root;

	void RotateR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		Node* parentP = parent->_parent;

		if (subLR)							//左子树的右子树连接到父的右
			subLR->_parent = parent;

		parent->_left = subLR;
		subL->_right = parent;
		parent->_parent = subL;

		// 如果parent是根节点，根新指向根节点的指针
		if (parent == _root)
		{
			_root = subL;
			subL->_parent = nullptr;
		}
		else
		{
			// 如果parent是子树，可能是其双亲的左子树，也可能是右子树
			if (parentP->_left == parent)
				parentP->_left = subL;
			else
				parentP->_right = subL;

			subL->_parent = parentP;
		}
	}

	void RotateL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		Node* parentP = parent->_parent;

		if (subRL)
			subRL->_parent = parent;

		parent->_right = subRL;
		subR->_left = parent;
		parent->_parent = subR;

		// 如果parent是根节点，根新指向根节点的指针
		if (parent == _root)
		{
			_root = subR;
			subR->_parent = nullptr;
		}
		else
		{
			// 如果parent是子树，可能是其双亲的左子树，也可能是右子树
			if (parentP->_left == parent)
				parentP->_left = subR;
			else
				parentP->_right = subR;

			subR->_parent = parentP;
		}
	}

	void _InOrder(Node* root)
	{
		if (root == nullptr)
		{
			return;
		}

		_InOrder(root->_left);
		cout << root->_kv.first << ':' << root->_kv.second << endl;
		_InOrder(root->_right);
	}

	bool _CheckBlance(Node* root, int BlackNum, int count)
	{
		if (root == nullptr)
		{
			if (count == BlackNum)
				return true;

			cout << "黑色节点的数量不相等" << endl;
			return false;
		}

		if (root->_col == RED && root->_parent->_col == RED)
		{
			cout << "存在连续的红色节点" << endl;
			return false;
		}

		if (root->_col == BLACK)
			count++;
		return _CheckBlance(root->_left, BlackNum, count)					//递归处理每个边
			&& _CheckBlance(root->_right, BlackNum, count);
	}

	void _Destory(Node* root)
	{
		if (root == nullptr)
		{
			return;
		}

		_Destory(root->_left);
		_Destory(root->_right);

		delete root;
	}

public:
	typedef __TreeIterator < T, T&, T* > iterator;
	typedef __TreeIterator < T, const T&, const T* > const_iterator;
	typedef ReverseIterator<iterator> reverse_iterator;

	RBTree()
		:_root(nullptr)
	{}

	~RBTree()
	{
		_Destory(_root);
		_root = nullptr;
	}

	reverse_iterator rbegin()
	{
		Node* right= _root;
		while (right && right->_right)			//最左
		{
			right = right->_right;
		}

		return reverse_iterator(right);
	}

	reverse_iterator rend()
	{
		return reverse_iterator(nullptr);
	}

	iterator begin()
	{
		Node* left = _root;
		while (left && left->_left)			//最左
		{
			left = left->_left;
		}

		return iterator(left);
	}

	iterator end()
	{
		return iterator(nullptr);
	}

	Node* Find(const K& key)
	{
		KeyOfT kot;				//
		Node* cur = _root;
		while (cur)
		{
			if (kot(cur->_data) > key)
			{
				cur = cur->_left;
			}
			else if (kot(cur->_data) < key)
			{
				cur = cur->_right;
			}
			else
			{
				return cur;
			}
		}

		return nullptr;
	}

	pair<iterator, bool> Insert(const T& data)
	{
		if (_root == nullptr)
		{
			_root = new Node(data);
			_root->_col = BLACK;
			return make_pair(iterator(_root), true);
		}

		KeyOfT kot;

		Node* parent = nullptr;
		Node* cur = _root;
		while (cur)
		{
			if (kot(cur->_data) > kot(data))
			{
				parent = cur;
				cur = cur->_left;
			}
			else if (kot(cur->_data) < kot(data))
			{
				parent = cur;
				cur = cur->_right;
			}
			else
			{
				return make_pair(iterator(cur), false);
			}
		}

		Node* newNode = new Node(data);
		newNode->_col = RED;
		if (kot(parent->_data) > kot(data))
		{
			parent->_left = newNode;
			newNode->_parent = parent;
		}
		else
		{
			parent->_right = newNode;
			newNode->_parent = parent;
		}
		cur = newNode;

		while (parent && parent->_col == RED)		//违反规则三
		{
			Node* grandfather = parent->_parent;

			if (parent == grandfather->_left)			//左半边
			{
				Node* uncle = grandfather->_right;

				if (uncle && uncle->_col == RED)		//情况一
				{
					uncle->_col = BLACK;
					grandfather->_col = RED;
					parent->_col = BLACK;

					cur = grandfather;			//迭代
					parent = cur->_parent;
				}
				else							//情况2.3
				{
					if (cur == parent->_left)		//单侧
					{
						RotateR(grandfather);

						grandfather->_col = RED;
						parent->_col = BLACK;
					}
					else					//折
					{
						RotateL(parent);
						RotateR(grandfather);

						cur->_col = BLACK;
						grandfather->_col = RED;
					}

					break;		//黑色数量无变化，不需要向上
				}
			}
			else         // parent == grandfather->_right
			{
				Node* uncle = grandfather->_left;
				if (uncle && uncle->_col == RED)		//情况一
				{
					uncle->_col = BLACK;
					grandfather->_col = RED;
					parent->_col = BLACK;

					cur = grandfather;			//迭代
					parent = cur->_parent;
				}
				else							//情况2.3
				{
					if (cur == parent->_right)		//单侧
					{
						RotateL(grandfather);

						grandfather->_col = RED;
						parent->_col = BLACK;
					}
					else					//折
					{
						RotateR(parent);
						RotateL(grandfather);

						cur->_col = BLACK;
						grandfather->_col = RED;
					}

					break;
				}
			}
		}

		_root->_col = BLACK;		//插入结束再次将根变为黑

		return make_pair(iterator(newNode), true);
	}

	bool CheckBlance()			//检查是否为红黑树，每个路径的黑色节点数相等，
	{
		if (_root == nullptr)
		{
			return true;
		}

		if (_root->_col == RED)
		{
			return false;
		}

		int BlackNum = 0;
		Node* left = _root;		//找最左路径的黑节点个数，确定基准
		while (left)
		{
			if (left->_col == BLACK)
				BlackNum++;

			left = left->_left;
		}

		int count = 0;		
		return _CheckBlance(_root, BlackNum, count);
	}

	void InOrder()
	{
		_InOrder(_root);

		cout << endl;
	}
};
