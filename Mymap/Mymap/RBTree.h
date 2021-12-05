#pragma once
#include<iostream>
#include<assert.h>

using namespace std;

enum Colour		//红黑树颜色枚举
{
	RED,
	BLACK,
};

template<class K, class V>
struct RBTreeNode					//节点结构体
{
	RBTreeNode<K, V>* _left;		//左子树
	RBTreeNode<K, V>* _right;		//右子树
	RBTreeNode<K, V>* _parent;		//父节点

	pair<K, V> _kv;

	Colour _col;

	RBTreeNode(const pair<K, V>& kv)	//构造函数
		: _left(nullptr)
		, _right(nullptr)
		, _parent(nullptr)
		, _kv(kv)
		, _col(RED)
	{}
};

template<class K, class V>
class RBTree
{
	typedef RBTreeNode<K, V> Node;
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
	RBTree()
		:_root(nullptr)
	{}

	~RBTree()
	{
		_Destory(_root);
		_root = nullptr;
	}

	Node* Find(const K& key)
	{
		Node* cur = _root;
		while (cur)
		{
			if (cur->_kv.first > key)
			{
				cur = cur->_left;
			}
			else if (cur->_kv < key)
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

	pair<Node*, bool> Insert(const pair<K, V>& kv)
	{
		if (_root == nullptr)
		{
			_root = new Node(kv);
			_root->_col = BLACK;
			return make_pair(_root, true);
		}

		Node* parent = nullptr;
		Node* cur = _root;
		while (cur)
		{
			if (cur->_kv.first > kv.first)
			{
				parent = cur;
				cur = cur->_left;
			}
			else if (cur->_kv.first < kv.first)
			{
				parent = cur;
				cur = cur->_right;
			}
			else
			{
				return make_pair(cur, false);
			}
		}

		Node* newNode = new Node(kv);
		newNode->_col = RED;
		if (parent->_kv.first > kv.first)
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

		return make_pair(newNode, true);
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
