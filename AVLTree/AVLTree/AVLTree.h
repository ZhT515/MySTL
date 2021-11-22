#pragma once
#include <iostream>
#include <assert.h>

using namespace std;

template<class K, class V>
struct AVLTreeNode
{
	AVLTreeNode<K, V>* _left;
	AVLTreeNode<K, V>* _right;
	AVLTreeNode<K, V>* _parent;

	int _bf; //平衡因子

	pair<K, V> _kv;		//使用结构模板将两个数据打包为一个数据

	AVLTreeNode(const pair<K, V>& kv)
		:_left(nullptr)
		, _right(nullptr)
		,_parent(nullptr)
		,_bf(0)
		,_kv(kv)
	{}
};


template<class K, class V>
class AVLTree
{
	typedef AVLTreeNode<K, V> Node;
private:
	Node* _root;

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

		// 根据调整后的结构更新部分节点的平衡因子
		subL->_bf = parent->_bf = 0;
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

		subR->_bf = parent->_bf = 0;
	}

	void RotateLR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		int bf = subLR->_bf;

		RotateL(parent->_left);
		RotateR(parent);

		if (bf == -1)				//情况一：
		{
			subL->_bf = 0;
			parent->_bf = 1;
			subLR->_bf = 0;
		}
		else if (bf == 1)				//情况二：
		{
			parent->_bf = 0;
			subL->_bf = -1;
			subLR->_bf = 0;
		}
		else if (bf == 0)			//情况三：
		{
			parent->_bf = 0;
			subL->_bf = 0;
			subLR->_bf = 0;
		}
		else
		{
			assert(false);
		}
	}

	void RotateRL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		int bf = subRL->_bf;	//旋转之前，保存SubLR的平衡因子，旋转完成之后，需要根据该平衡因子来调整其他节点的平衡因子

		RotateR(parent->_right);		//先右
		RotateL(parent);

		//更新平衡因子
		if (bf == 1)				//情况一：
		{
			subR->_bf = 0;
			parent->_bf = -1;
			subRL->_bf = 0;
		}
		else if (bf == -1)				//情况二：
		{
			parent->_bf = 0;
			subR->_bf = 1;
			subRL->_bf = 0;
		}
		else if (bf == 0)			//情况三：
		{
			parent->_bf = 0;
			subR->_bf = 0;
			subRL->_bf = 0;
		}
		else
		{
			assert(false);
		}
	}

	void _InOrder(Node* root)
	{
		if (root == nullptr)
		{
			return;
		}

		_InOrder(root->_left);
		cout << root->_kv.first << ":" << root->_kv.second << endl;
		_InOrder(root->_right);
	}

	int _Height(Node* root)
	{
		if (root == nullptr)
		{
			return 0;
		}

		int left = _Height(root->_left);
		int right = _Height(root->_right);

		return right > left ? right + 1 : left + 1;
	}

	bool _IsBalance(Node* root)
	{
		if (root == nullptr)
		{
			return true;
		}

		int leftHeight = _Height(root->_left);
		int rightHeight = _Height(root->_right);

		// 检查一下平衡因子是否正确
		if (rightHeight - leftHeight != root->_bf)
		{
			cout << "平衡因子异常:" << root->_kv.first << endl;
			return false;
		}

		return abs(rightHeight - leftHeight) < 2
			&& _IsBalance(root->_left)
			&& _IsBalance(root->_right);
	}

public:
	AVLTree()
		:_root(nullptr)
	{}

	~AVLTree()
	{
		_Destory(_root);

		_root = nullptr;
	}

	V& operator[](const K& key)
	{
		pair<Node*, bool> ret = Insert(make_pair(key, V()));
		return ret.first->_kv.second;
	}

	pair<Node*, bool> Insert(const pair<K, V> kv)
	{
		if (_root == nullptr)
		{
			_root = new Node(kv);

			return make_pair(_root, true);
		}


		//1.找到目标位置并插入新节点
		Node* parent = _root,* cur = _root;
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
				return make_pair(cur, true);
			}
		}

		cur = new Node(kv);
		Node* newnode = cur;
		if (parent->_kv.first < kv.first)
		{
			parent->_right = cur;
			cur->_parent = parent;
		}
		else
		{
			parent->_left = cur;
			cur->_parent = parent;
		}

		//控制平衡
		//1.更新平衡因子
		//2.不平衡则旋转
		while (cur != _root)
		{
			if (parent->_left == cur) {			//处理
				parent->_bf--;	
			}
			else {
				parent->_bf++;
			}

			if (parent->_bf == 0) {
				break;						//原本 = -1 / 1，且加到了另一侧空的地方，高度没有变化
			}
			else if(parent->_bf == 1 || parent->_bf == -1)
			{
				//高度发生变化，需要继续改变父节点
				cur = parent;
				parent = parent->_parent;
			}
			else if (parent->_bf == 2 || parent->_bf == -2)
			{
				//需要旋转
				if (parent->_bf == -2)			//左边高
				{
					if (parent->_bf == -1)
					{
						//左边高，右单旋
						RotateR(parent);
					}
					else
					{
						RotateLR(parent);
					}
				}
				else
				{
					if (parent->_bf == 1)
					{
						//左边高，右单旋
						RotateL(parent);
					}
					else
					{
						RotateRL(parent);
					}
				}
			}
			else
			{
				assert(false);
			}
		}

		return make_pair(newnode, true);
	}

	Node* Find(const K& key)
	{
		Node* cur = _root;
		while (cur)
		{
			if (cur->_kv.first < key)
			{
				cur = cur->_right;
			}
			else if (cur->_kv.first > key)
			{
				cur = cur->_left;
			}
			else
			{
				return cur;
			}
		}

		return nullptr;
	}

	void InOrder()
	{
		_InOrder(_root);
	}

	bool IsAVLTree()
	{
		return _IsBalance(_root);
	}

	bool Erase(const K& key)
	{
		return false;
	}
};

