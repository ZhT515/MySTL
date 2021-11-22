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

	int _bf; //ƽ������

	pair<K, V> _kv;		//ʹ�ýṹģ�彫�������ݴ��Ϊһ������

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

		if (subLR)							//�����������������ӵ�������
			subLR->_parent = parent;		
		
		parent->_left = subLR;
		subL->_right = parent;
		parent->_parent = subL;

		// ���parent�Ǹ��ڵ㣬����ָ����ڵ��ָ��
		if (parent == _root)
		{
			_root = subL;
			subL->_parent = nullptr;
		}
		else
		{
			// ���parent����������������˫�׵���������Ҳ������������
			if (parentP->_left == parent)
				parentP->_left = subL;
			else
				parentP->_right = subL;

			subL->_parent = parentP;
		}

		// ���ݵ�����Ľṹ���²��ֽڵ��ƽ������
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

		// ���parent�Ǹ��ڵ㣬����ָ����ڵ��ָ��
		if (parent == _root)
		{
			_root = subR;
			subR->_parent = nullptr;
		}
		else
		{
			// ���parent����������������˫�׵���������Ҳ������������
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

		if (bf == -1)				//���һ��
		{
			subL->_bf = 0;
			parent->_bf = 1;
			subLR->_bf = 0;
		}
		else if (bf == 1)				//�������
		{
			parent->_bf = 0;
			subL->_bf = -1;
			subLR->_bf = 0;
		}
		else if (bf == 0)			//�������
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
		int bf = subRL->_bf;	//��ת֮ǰ������SubLR��ƽ�����ӣ���ת���֮����Ҫ���ݸ�ƽ�����������������ڵ��ƽ������

		RotateR(parent->_right);		//����
		RotateL(parent);

		//����ƽ������
		if (bf == 1)				//���һ��
		{
			subR->_bf = 0;
			parent->_bf = -1;
			subRL->_bf = 0;
		}
		else if (bf == -1)				//�������
		{
			parent->_bf = 0;
			subR->_bf = 1;
			subRL->_bf = 0;
		}
		else if (bf == 0)			//�������
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

		// ���һ��ƽ�������Ƿ���ȷ
		if (rightHeight - leftHeight != root->_bf)
		{
			cout << "ƽ�������쳣:" << root->_kv.first << endl;
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


		//1.�ҵ�Ŀ��λ�ò������½ڵ�
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

		//����ƽ��
		//1.����ƽ������
		//2.��ƽ������ת
		while (cur != _root)
		{
			if (parent->_left == cur) {			//����
				parent->_bf--;	
			}
			else {
				parent->_bf++;
			}

			if (parent->_bf == 0) {
				break;						//ԭ�� = -1 / 1���Ҽӵ�����һ��յĵط����߶�û�б仯
			}
			else if(parent->_bf == 1 || parent->_bf == -1)
			{
				//�߶ȷ����仯����Ҫ�����ı丸�ڵ�
				cur = parent;
				parent = parent->_parent;
			}
			else if (parent->_bf == 2 || parent->_bf == -2)
			{
				//��Ҫ��ת
				if (parent->_bf == -2)			//��߸�
				{
					if (parent->_bf == -1)
					{
						//��߸ߣ��ҵ���
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
						//��߸ߣ��ҵ���
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

