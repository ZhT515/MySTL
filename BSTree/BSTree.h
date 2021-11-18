#pragma once
#include<iostream>

using namespace std;

namespace K
{
	template<class K>
	struct BSTreeNode					//节点类
	{
		BSTreeNode<K>* _left;			//左子树指针
		BSTreeNode<K>* _right;			//右子树指针

		K _key;				//值

		BSTreeNode(const K& key)
			:_left(nullptr)
			,_right(nullptr)
			,_key(key)
		{}
	};

	template<class K>
	class BSTree {
		typedef BSTreeNode<K> Node;

	private:
		Node* _root;		//根节点


		void _Destory(Node* root)
		{
			if (root == nullptr){
				return;
			}

			_Destory(root->_left);
			_Destory(root->_right);

			delete root;
		}

		Node* _Copy(Node* root)					//深拷贝
		{
			if (root == nullptr) {					
				return;
			}

			Node* copyNode = new Node(root->_key);
			copyNode->_left = _Copy(root->_left);
			copyNode->_right = _Copy(root->_right);

			return copyNode;
		}

		void _InOrder(Node* root) 
		{
			if (root == nullptr) {
				return;
			}

			_InOrder(root->_left);			//中序遍历
			cout << root->_key << ' ';
			_InOrder(root->_right);
		}

		bool _InsertR(Node*& root, const K& key)			//引用，可以直接改变父节点的左右值
		{
			if (root == nullptr)
			{
				root = new Node(key);
				return true;
			}

			if (root->_key < key) {
				return _InsertR(root->_right, key);
			}
			else if (root->_key > key) {
				return _InsertR(root->_left, key);
			}
			else
				return false;
		}

		bool _EraseR(Node*& root, const K& key)
		{
			if (root == nullptr)
			{
				return false;
			}

			if (root->_key < key) {
				return _EraseR(root->_right, key);
			}
			else if (root->_key > key) {
				return _EraseR(root->_left, key);
			}
			else
			{
				// 找root就是要删除的节点
				if (root->_left == nullptr)
				{
					Node* del = root;
					root = root->_right;
					delete del;
				}
				else if (root->_right == nullptr)
				{
					Node* del = root;
					root = root->_left;
					delete del;
				}
				else
				{
					Node* minRight = root->_right;

					while (minRight->_left) {
						minRight = minRight->_left;
					}

					K min = minRight->_key;

					_EraseR(root->_right, min);			//调用自己删除找到的

					root->_key = min;
				}
			}
		}

		Node* _FindR(Node* root, const K& key)
		{
			if (root == nullptr)
				return nullptr;

			if (root->_key < key) {
				return _FindR(_root->_right, key);
			}
			else if (root->_key > key) {
				return _FindR(_root->_left, key);
			}
			else 
				return root;
		}

	public:
		BSTree()				//构造函数
			:_root(nullptr)
		{}

		BSTree(const BSTree<K>& t)
		{
			_root = _Copy(t._root);
		}

		~BSTree()
		{
			_Destory(_root);
			_root = nullptr;
		}

		bool Insert(const K& key)			//非递归插入
		{
			if (_root == nullptr){
				_root = new Node(key);
				return true;
			}

			Node* parent = nullptr;
			Node* cur = _root;
			while (cur) {					//找位置

				if (cur->_key < key) {
					parent = cur;
					cur = cur->_right;
				}
				else if (cur->_key > key) {
					parent = cur;
					cur = cur->_left;
				}
				else {
					return false;
				}
			}

			cur = new Node(key);
			if (parent->_key > key) {
				parent->_left = cur;
			}
			else {
				parent->_right = cur;
			}

			return true;
		}

		bool Erase(const K& key)
		{
			Node* parent = nullptr;
			Node* cur = _root;

			while (cur)
			{
				if (cur->_key > key) {
					parent = cur;
					cur = cur->_left;
				}
				else if (cur->_key < key) {
					parent = cur;
					cur = cur->_right;
				}
				else 
				{
					//找到

					//1.左为空，把孩子交给父亲管理，删除自己
					if (cur->_left == nullptr) 
					{
						if (cur == _root) {					
							cur = cur->_right;
						}
						else
						{
							if (parent->_right == cur)				//确定在父节点那一侧
								parent->_right = cur->_right;
							else 
								parent->_left = cur->_right;
						}

						delete cur;
					}
					else if (cur->_right == nullptr)
					{
						if (cur == _root) {
							cur = cur->_left;
						}
						else
						{
							if (parent->_right == cur)				//确定在父节点那一侧
								parent->_right = cur->_left;
							else
								parent->_left = cur->_left;
						}

						delete cur;
					}
					else
					{
						//左右都不为空，需要找左子树的最大值，或者右子树的最小值进行替代

						Node* minRight = cur->_right;
						Node* minParent = cur;

						while (minRight->_left) {
							minParent = minRight;
							minRight = minRight->_left;
						}

						cur->_key = minRight->_key;

						if (minParent->_left == minRight)
							minParent->_left = minRight->_right;		//只可能有右子树
						else
							minParent->_right = minRight->_right;		//存在可能，此时minParent就是cur,此时minParent->_right == minRight，则需要minParent->_right接管。

						delete minRight;


						/*
						//方法2：调用自己去删除最小节点
						Node* minRight = cur->_right;

						while (minRight->_left) {
							minRight = minRight->_left;
						}

						K min = minRight->_key;

						this->Erase(min);			//调用自己删除找到的
						
						cur->_key = min;*/
					}

					return true;
				}
			}

			return false;
		}

		void InOrder()					//中序遍历
		{
			_InOrder(_root);			//递归实现
			cout << endl;
		}

		Node* Find(const K& key)
		{
			Node* cur = _root;
			while (cur) {					//找位置

				if (cur->_key < key) {
					cur = cur->_right;
				}
				else if (cur->_key > key) {
					cur = cur->_left;
				}
				else {
					return cur;
				}
			}

			return nullptr;
		}

		bool InsertR(const K& key)
		{
			return _InsertR(_root, key);
		}

		bool EraseR(const K& key)
		{
			return _EraseR(_root, key);
		}

		Node* FindR(const K& key)
		{
			return _FindR(_root, key);
		}
	};
}