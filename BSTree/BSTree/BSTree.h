#pragma once
#include<iostream>

using namespace std;

namespace K
{
	template<class K>
	struct BSTreeNode					//�ڵ���
	{
		BSTreeNode<K>* _left;			//������ָ��
		BSTreeNode<K>* _right;			//������ָ��

		K _key;				//ֵ

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
		Node* _root;		//���ڵ�


		void _Destory(Node* root)
		{
			if (root == nullptr){
				return;
			}

			_Destory(root->_left);
			_Destory(root->_right);

			delete root;
		}

		Node* _Copy(Node* root)					//���
		{
			if (root == nullptr) {					
				return nullptr;
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

			_InOrder(root->_left);			//�������
			cout << root->_key << ' ';
			_InOrder(root->_right);
		}

		bool _InsertR(Node*& root, const K& key)			//���ã�����ֱ�Ӹı丸�ڵ������ֵ
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
				// ��root����Ҫɾ���Ľڵ�
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

					_EraseR(root->_right, min);			//�����Լ�ɾ���ҵ���

					root->_key = min;
				}
			}
		}

		Node* _FindR(Node* root, const K& key)
		{
			if (root == nullptr)
				return nullptr;

			if (root->_key < key) {
				return _FindR(root->_right, key);
			}
			else if (root->_key > key) {
				return _FindR(root->_left, key);
			}
			else 
				return root;
		}

	public:
		BSTree()				//���캯��
			:_root(nullptr)
		{}

		BSTree(const BSTree<K>& t)
		{
			_root = _Copy(t._root);
		}

		BSTree<K> operator=(BSTree<K> t)
		{
			swap(_root, t._root);
			return *this;
		}

		~BSTree()
		{
			_Destory(_root);
			_root = nullptr;
		}

		bool Insert(const K& key)			//�ǵݹ����
		{
			if (_root == nullptr){
				_root = new Node(key);
				return true;
			}

			Node* parent = nullptr;
			Node* cur = _root;
			while (cur) {					//��λ��

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
					//�ҵ�

					//1.��Ϊ�գ��Ѻ��ӽ������׹���ɾ���Լ�
					if (cur->_left == nullptr) 
					{
						if (cur == _root) {					
							cur = cur->_right;
						}
						else
						{
							if (parent->_right == cur)				//ȷ���ڸ��ڵ���һ��
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
							if (parent->_right == cur)				//ȷ���ڸ��ڵ���һ��
								parent->_right = cur->_left;
							else
								parent->_left = cur->_left;
						}

						delete cur;
					}
					else
					{
						//���Ҷ���Ϊ�գ���Ҫ�������������ֵ����������������Сֵ�������

						Node* minRight = cur->_right;
						Node* minParent = cur;

						while (minRight->_left) {
							minParent = minRight;
							minRight = minRight->_left;
						}

						cur->_key = minRight->_key;

						if (minParent->_left == minRight)
							minParent->_left = minRight->_right;		//ֻ������������
						else
							minParent->_right = minRight->_right;		//���ڿ��ܣ���ʱminParent����cur,��ʱminParent->_right == minRight������ҪminParent->_right�ӹܡ�

						delete minRight;


						/*
						//����2�������Լ�ȥɾ����С�ڵ�
						Node* minRight = cur->_right;

						while (minRight->_left) {
							minRight = minRight->_left;
						}

						K min = minRight->_key;

						this->Erase(min);			//�����Լ�ɾ���ҵ���
						
						cur->_key = min;*/
					}

					return true;
				}
			}

			return false;
		}

		void InOrder()					//�������
		{
			_InOrder(_root);			//�ݹ�ʵ��
			cout << endl;
		}

		Node* Find(const K& key)
		{
			Node* cur = _root;
			while (cur) {					//��λ��

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

namespace KV						//k-v
{
	template<class K, class V>
	struct BSTreeNode					//�ڵ���
	{
		BSTreeNode<K, V>* _left;			//������ָ��
		BSTreeNode<K, V>* _right;			//������ָ��

		K _key;				//ֵ
		V _val;				//������PYTHON�е��ֵ�

		BSTreeNode(const K& key, const V& val)
			:_left(nullptr)
			, _right(nullptr)
			, _key(key)
			, _val(val)
		{}
	};

	template<class K, class V>
	class BSTree {
		typedef BSTreeNode<K, V> Node;

	private:
		Node* _root;		//���ڵ�


		void _Destory(Node* root)
		{
			if (root == nullptr) {
				return;
			}

			_Destory(root->_left);
			_Destory(root->_right);

			delete root;
		}

		Node* _Copy(Node* root)					//���
		{
			if (root == nullptr) {
				return nullptr;
			}

			Node* copyNode = new Node(root->_key, root->_val);
			copyNode->_left = _Copy(root->_left);
			copyNode->_right = _Copy(root->_right);

			return copyNode;
		}

		void _InOrder(Node* root)
		{
			if (root == nullptr) {
				return;
			}

			_InOrder(root->_left);			//�������
			cout << root->_key << ':' << root->_val << endl;
			_InOrder(root->_right);
		}

		bool _InsertR(Node*& root, const K& key, const V& val)			//���ã�����ֱ�Ӹı丸�ڵ������ֵ
		{
			if (root == nullptr)
			{
				root = new Node(key, val);
				return true;
			}

			if (root->_key < key) {
				return _InsertR(root->_right, key, val);
			}
			else if (root->_key > key) {
				return _InsertR(root->_left, key, val);
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
				// ��root����Ҫɾ���Ľڵ�
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
					K minV = minRight->_val;

					_EraseR(root->_right, min);			//�����Լ�ɾ���ҵ���

					root->_key = min;
					root->_val = minV;
				}
			}
		}

		Node* _FindR(Node* root, const K& key)
		{
			if (root == nullptr)
				return nullptr;

			if (root->_key < key) {
				return _FindR(root->_right, key);
			}
			else if (root->_key > key) {
				return _FindR(root->_left, key);
			}
			else
				return root;
		}

	public:
		BSTree()				//���캯��
			:_root(nullptr)
		{}

		BSTree(const BSTree<K, V>& t)
		{
			_root = _Copy(t._root);
		}

		BSTree<K, V> operator=(BSTree<K, V> t)
		{
			swap(_root, t._root);
			return *this;
		}

		~BSTree()
		{
			_Destory(_root);
			_root = nullptr;
		}

		void InOrder()					//�������
		{
			_InOrder(_root);			//�ݹ�ʵ��
			cout << endl;
		}

		bool InsertR(const K& key, const V& val)
		{
			return _InsertR(_root, key, val);
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