#pragma once
#include<iostream>
#include<assert.h>

using namespace std;

enum Colour		//�������ɫö��
{
	RED,
	BLACK,
};

template<class K, class V>
struct RBTreeNode					//�ڵ�ṹ��
{
	RBTreeNode<K, V>* _left;		//������
	RBTreeNode<K, V>* _right;		//������
	RBTreeNode<K, V>* _parent;		//���ڵ�

	pair<K, V> _kv;

	Colour _col;

	RBTreeNode(const pair<K, V>& kv)	//���캯��
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
};
