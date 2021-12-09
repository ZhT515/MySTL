#pragma once
#include<iostream>
#include"RIterator.h"

using namespace std;

enum Colour		//�������ɫö��
{
	RED,
	BLACK,
};

template<class T>				//pair���͵�T
struct RBTreeNode					//�ڵ�ṹ��
{
	RBTreeNode<T>* _left;		//������
	RBTreeNode<T>* _right;		//������
	RBTreeNode<T>* _parent;		//���ڵ�

	T _data;

	Colour _col;

	RBTreeNode(const T& x)	//���캯��
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

	Ref operator*()		//*��������
	{
		return _node->_data;
	}

	Ptr operator->()		//->����ָ�� ����ֱ����
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

template<class K, class T, class KeyOfT>		//ͨ���º�����ȡkey��ֵ���бȽ�
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

			cout << "��ɫ�ڵ�����������" << endl;
			return false;
		}

		if (root->_col == RED && root->_parent->_col == RED)
		{
			cout << "���������ĺ�ɫ�ڵ�" << endl;
			return false;
		}

		if (root->_col == BLACK)
			count++;
		return _CheckBlance(root->_left, BlackNum, count)					//�ݹ鴦��ÿ����
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
		while (right && right->_right)			//����
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
		while (left && left->_left)			//����
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

		while (parent && parent->_col == RED)		//Υ��������
		{
			Node* grandfather = parent->_parent;

			if (parent == grandfather->_left)			//����
			{
				Node* uncle = grandfather->_right;

				if (uncle && uncle->_col == RED)		//���һ
				{
					uncle->_col = BLACK;
					grandfather->_col = RED;
					parent->_col = BLACK;

					cur = grandfather;			//����
					parent = cur->_parent;
				}
				else							//���2.3
				{
					if (cur == parent->_left)		//����
					{
						RotateR(grandfather);

						grandfather->_col = RED;
						parent->_col = BLACK;
					}
					else					//��
					{
						RotateL(parent);
						RotateR(grandfather);

						cur->_col = BLACK;
						grandfather->_col = RED;
					}

					break;		//��ɫ�����ޱ仯������Ҫ����
				}
			}
			else         // parent == grandfather->_right
			{
				Node* uncle = grandfather->_left;
				if (uncle && uncle->_col == RED)		//���һ
				{
					uncle->_col = BLACK;
					grandfather->_col = RED;
					parent->_col = BLACK;

					cur = grandfather;			//����
					parent = cur->_parent;
				}
				else							//���2.3
				{
					if (cur == parent->_right)		//����
					{
						RotateL(grandfather);

						grandfather->_col = RED;
						parent->_col = BLACK;
					}
					else					//��
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

		_root->_col = BLACK;		//��������ٴν�����Ϊ��

		return make_pair(iterator(newNode), true);
	}

	bool CheckBlance()			//����Ƿ�Ϊ�������ÿ��·���ĺ�ɫ�ڵ�����ȣ�
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
		Node* left = _root;		//������·���ĺڽڵ������ȷ����׼
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
