/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

template<
	class Key,
	class T,
	class Compare = std::less<Key>
> 
class map {
private:
	enum COLOR { RED, BLACK };
	/*template<
		class Key,
		class T,
		class Compare = std::less<Key>
	>*/
	struct RB_Node
	{
		COLOR RB_COLOR;
		RB_Node* right;
		RB_Node* left;
		RB_Node* parent;
		pair<const Key, T>* data;
		//size_t Count;
		RB_Node()
		{
			//RB_COLOR = BLACK;
			data = NULL;
			//Count = 1;
		}
		~RB_Node()
		{
			if(data!=NULL)
				delete data;
		}
	};
	
	size_t SIZE;							//�ڵ����
	void clear(RB_Node* node)
	{
		if (node == m_nullNode)
		{
			return;
		}
		else
		{
			clear(node->left);
			clear(node->right);
			delete node;
		}
	}

	//��������ʵ��    
	bool RotateLeft(RB_Node* node)
	{
		if (node == m_nullNode || node->right == m_nullNode)
		{
			return false;//can't rotate    
		}
		RB_Node* lower_right = node->right;
		lower_right->parent = node->parent;
		node->right = lower_right->left;
		if (lower_right->left != m_nullNode)
		{
			lower_right->left->parent = node;
		}
		if (node->parent == m_nullNode) //rotate node is root    
		{
			m_root = lower_right;
			m_nullNode->left = m_root;
			m_nullNode->right = m_root;
			//m_nullNode->parent = m_root;    
		}
		else
		{
			if (node == node->parent->left)
			{
				node->parent->left = lower_right;
			}
			else
			{
				node->parent->right = lower_right;
			}
		}
		node->parent = lower_right;
		lower_right->left = node;
		return true;
	}

	//��������ʵ��    
	bool RotateRight(RB_Node* node)
	{
		if (node == m_nullNode || node->left == m_nullNode)
		{
			return false;//can't rotate    
		}
		RB_Node* lower_left = node->left;
		node->left = lower_left->right;
		lower_left->parent = node->parent;
		if (lower_left->right != m_nullNode)
		{
			lower_left->right->parent = node;
		}
		if (node->parent == m_nullNode) //node is root    
		{
			m_root = lower_left;
			m_nullNode->left = m_root;
			m_nullNode->right = m_root;
			//m_nullNode->parent = m_root;    
		}
		else
		{
			if (node == node->parent->right)
			{
				node->parent->right = lower_left;
			}
			else
			{
				node->parent->left = lower_left;
			}
		}
		node->parent = lower_left;
		lower_left->right = node;
		return true;
	}
public:
	RB_Node* m_root;		//���ڵ�
	RB_Node* m_nullNode;	//�սڵ�
	Compare com;
	//Ѱ�����һ����㣬����end()--
	RB_Node* LAST()const
	{
		if (m_root == m_nullNode)
		{
			return m_root;
		}
		RB_Node* ans = m_root;
		while (ans->right != m_nullNode)
		{
			ans = ans->right;
		}
		return ans;
	}
	/**
	 * the internal type of data.
	 * it should have a default constructor, a copy constructor.
	 * You can use sjtu::map as value_type by typedef.
	 */
	typedef pair<const Key, T> value_type;
	/**
	 * see BidirectionalIterator at CppReference for help.
	 *
	 * if there is anything wrong throw invalid_iterator.
	 *     like it = map.begin(); --it;
	 *       or it = map.end(); ++end();
	 */
	
	class const_iterator;
	class iterator {
	private:
		/**
		 * TODO add data members
		 *   just add whatever you want.
		 */
	public:
		map * m;
		RB_Node* node;
		iterator(map*ma = NULL, RB_Node*no = NULL) {
			// TODO
			m = ma;
			node = no;
		}
		iterator(const iterator &other) {
			// TODO
			node = other.node;
			m = other.m;
		}
		/**
		 * return a new iterator which pointer n-next elements
		 *   even if there are not enough elements, just return the answer.
		 * as well as operator-
		 */
		/**
		 * TODO iter++
		 */
		iterator operator++(int)	//Ѱ�Һ�̣�����ֱ�ӵ���successor
		{
			iterator tmp = *this;
			if (node == m->m_nullNode)       //null node has no successor    
			{
				throw(invalid_iterator());
			}
			RB_Node* result = node->right;   //get node's right node    
			while (result != m->m_nullNode)        //try to find node's right subtree's left most node    
			{
				if (result->left != m->m_nullNode)
				{
					result = result->left;
				}
				else
				{
					break;
				}
			}                              //after while loop result==null or result's left child is null    
			if (result == m->m_nullNode)
			{
				RB_Node* index = node->parent;
				result = node;
				while (index != m->m_nullNode && result == index->right)
				{
					result = index;
					index = index->parent;
				}
				result = index;         //first parent's left or null    
			}
			node = result;
			return tmp;
			//RB_Node<Key, T, Compare> tmp = *this;
			//RB_Node<Key,T,Compare>* ans = node->right;
			//if (ans == NULL)	//���û���Ҷ���
			//{
			//	if (node->parent == NULL)
			//	{
			//		node = NULL;
			//		return tmp;
			//	}
			//	if (node == node->parent->left)	//����Ǹ��׵�����ӣ��ȸ���С��
			//	{
			//		node = node->parent;
			//		return tmp;
			//	}
			//	else if(node==node->parent->right) //����Ǹ��׵��Ҷ��ӣ��ȸ��״�
			//	{
			//		RB_Node<Key, T, Compare>* n_tmp = node->parent;
			//		while (n_tmp->parent!=NULL && n_tmp == n_tmp->parent->right)
			//			n_tmp = n_tmp->parent;
			//		node = n_tmp->parent;
			//		return tmp;
			//	}
			//}
			//else
			//{
			//	while (ans->left != NULL)
			//		ans = ans->left;
			//	node = ans;
			//}
			//return tmp;
		}
		/**
		 * TODO ++iter
		 */
		iterator & operator++() 
		{
			if (node == m->m_nullNode)       //null node has no successor    
			{
				throw(invalid_iterator());
			}
			RB_Node* result = node->right;   //get node's right node    
			while (result != m->m_nullNode)        //try to find node's right subtree's left most node    
			{
				if (result->left != m->m_nullNode)
				{
					result = result->left;
				}
				else
				{
					break;
				}
			}                              //after while loop result==null or result's left child is null    
			if (result == m->m_nullNode)
			{
				RB_Node* index = node->parent;
				result = node;
				while (index != m->m_nullNode && result == index->right)
				{
					result = index;
					index = index->parent;
				}
				result = index;         //first parent's left or null    
			}
			node = result;
			return *this;
		}
		/**
		 * TODO iter--
		 */
		iterator operator--(int) 
		{
			iterator tmp = *this;
			//if (node == m->m_root)       //null node has no predecessor    
			//{
			//	throw(invalid_iterator());
			//}
			if (node == m->m_nullNode)
			{
				node = m->LAST();
				return tmp;
			}
			RB_Node* result = node->left;     //get node's left child    
			while (result != m->m_nullNode)         //try to find node's left subtree's right most node    
			{
				if (result->right != m->m_nullNode)
				{
					result = result->right;
				}
				else
				{
					break;
				}
			}            //after while loop result==null or result's right child is null    
			if (result == m->m_nullNode)
			{
				RB_Node* index = node->parent;
				result = node;
				while (index != m->m_nullNode && result == index->left)
				{
					result = index;
					index = index->parent;
				}
				result = index;         // first right parent or null    
			}
			node = result;
			return tmp;
		}
		/**
		 * TODO --iter
		 */
		iterator & operator--() 
		{
			//if (node == m->m_root)       //null node has no predecessor    
			//{
			//	throw(invalid_iterator());
			//}
			if (node == m->m_nullNode)
			{
				node = m->LAST();
				return *this;
			}
			RB_Node* result = node->left;     //get node's left child    
			while (result != m->m_nullNode)         //try to find node's left subtree's right most node    
			{
				if (result->right != m->m_nullNode)
				{
					result = result->right;
				}
				else
				{
					break;
				}
			}            //after while loop result==null or result's right child is null    
			if (result == m->m_nullNode)
			{
				RB_Node* index = node->parent;
				result = node;
				while (index != m->m_nullNode && result == index->left)
				{
					result = index;
					index = index->parent;
				}
				result = index;         // first right parent or null    
			}
			node = result;
			return *this;
		}
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		value_type & operator*() const 
		{
			return *(node->data);
		}
		bool operator==(const iterator &rhs) const 
		{
			return (m == rhs.m && node == rhs.node);
		}
		bool operator==(const const_iterator &rhs) const 
		{
			return (m == rhs.m && node == rhs.node);
		}
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const 
		{
			return (m != rhs.m || node != rhs.node);
		}
		bool operator!=(const const_iterator &rhs) const 
		{
			return (m != rhs.m || node != rhs.node);
		}

		/**
		 * for the support of it->first. 
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		value_type* operator->() const noexcept 
		{
			return node->data;
		}
	};
	class const_iterator {
		// it should has similar member method as iterator.
		//  and it should be able to construct from an iterator.
		private:
			// data members.
		public:
			const map * m;
			const RB_Node* node;
			const_iterator(const map*ma = NULL, const RB_Node*no = NULL)
				:m(ma),node(no)
			{
				// TODO
			}
			const_iterator(const const_iterator &other) 
				:m(other.m),node(other.node)
			{
				// TODO
			}
			const_iterator(const iterator &other) 
				:m(other.m), node(other.node)
			{
				// TODO
			}
			// And other methods in iterator.
			// And other methods in iterator.
			// And other methods in iterator.
			const_iterator operator++(int)	//Ѱ�Һ�̣�����ֱ�ӵ���successor
			{
				const_iterator tmp = *this;
				if (node == m->m_nullNode)       //null node has no successor    
				{
					throw(invalid_iterator());
				}
				const RB_Node* result = node->right;   //get node's right node    
				while (result != m->m_nullNode)        //try to find node's right subtree's left most node    
				{
					if (result->left != m->m_nullNode)
					{
						result = result->left;
					}
					else
					{
						break;
					}
				}                              //after while loop result==null or result's left child is null    
				if (result == m->m_nullNode)
				{
					RB_Node* index = node->parent;
					result = node;
					while (index != m->m_nullNode && result == index->right)
					{
						result = index;
						index = index->parent;
					}
					result = index;         //first parent's left or null    
				}
				node = result;
				return tmp;
				//RB_Node<Key, T, Compare> tmp = *this;
				//RB_Node<Key,T,Compare>* ans = node->right;
				//if (ans == NULL)	//���û���Ҷ���
				//{
				//	if (node->parent == NULL)
				//	{
				//		node = NULL;
				//		return tmp;
				//	}
				//	if (node == node->parent->left)	//����Ǹ��׵�����ӣ��ȸ���С��
				//	{
				//		node = node->parent;
				//		return tmp;
				//	}
				//	else if(node==node->parent->right) //����Ǹ��׵��Ҷ��ӣ��ȸ��״�
				//	{
				//		RB_Node<Key, T, Compare>* n_tmp = node->parent;
				//		while (n_tmp->parent!=NULL && n_tmp == n_tmp->parent->right)
				//			n_tmp = n_tmp->parent;
				//		node = n_tmp->parent;
				//		return tmp;
				//	}
				//}
				//else
				//{
				//	while (ans->left != NULL)
				//		ans = ans->left;
				//	node = ans;
				//}
				//return tmp;
			}
			/**
			* TODO ++iter
			*/
			const_iterator & operator++()
			{
				if (node == m->m_nullNode)       //null node has no successor    
				{
					throw(invalid_iterator());
				}
				const RB_Node* result = node->right;   //get node's right node    
				while (result != m->m_nullNode)        //try to find node's right subtree's left most node    
				{
					if (result->left != m->m_nullNode)
					{
						result = result->left;
					}
					else
					{
						break;
					}
				}                              //after while loop result==null or result's left child is null    
				if (result == m->m_nullNode)
				{
					RB_Node* index = node->parent;
					result = node;
					while (index != m->m_nullNode && result == index->right)
					{
						result = index;
						index = index->parent;
					}
					result = index;         //first parent's left or null    
				}
				node = result;
				return *this;
			}
			/**
			* TODO iter--
			*/
			const_iterator operator--(int)
			{
				const_iterator tmp = *this;
				//if (node == m->m_root)       //null node has no predecessor    
				//{
				//	throw(invalid_iterator());
				//}
				if (node == m->m_nullNode)
				{
					node = m->LAST();
					return tmp;
				}
				const RB_Node* result = node->left;     //get node's left child    
				while (result != m->m_nullNode)         //try to find node's left subtree's right most node    
				{
					if (result->right != m->m_nullNode)
					{
						result = result->right;
					}
					else
					{
						break;
					}
				}            //after while loop result==null or result's right child is null    
				if (result == m->m_nullNode)
				{
					RB_Node* index = node->parent;
					result = node;
					while (index != m->m_nullNode && result == index->left)
					{
						result = index;
						index = index->parent;
					}
					result = index;         // first right parent or null    
				}
				node = result;
				return tmp;
			}
			/**
			* TODO --iter
			*/
			const_iterator & operator--()
			{
				//if (node == m->m_root)       //null node has no predecessor    
				//{
				//	throw(invalid_iterator());
				//}
				if (node == m->m_nullNode)
				{
					node = m->LAST();
					return *this;
				}
				const RB_Node* result = node->left;     //get node's left child    
				while (result != m->m_nullNode)         //try to find node's left subtree's right most node    
				{
					if (result->right != m->m_nullNode)
					{
						result = result->right;
					}
					else
					{
						break;
					}
				}            //after while loop result==null or result's right child is null    
				if (result == m->m_nullNode)
				{
					RB_Node* index = node->parent;
					result = node;
					while (index != m->m_nullNode && result == index->left)
					{
						result = index;
						index = index->parent;
					}
					result = index;         // first right parent or null    
				}
				node = result;
				return *this;
			}
			/**
			* a operator to check whether two iterators are same (pointing to the same memory).
			*/
			value_type & operator*() const
			{
				return *(node->data);
			}
			bool operator==(const iterator &rhs) const
			{
				return (m == rhs.m && node == rhs.node);
			}
			bool operator==(const const_iterator &rhs) const
			{
				return (m == rhs.m && node == rhs.node);
			}
			/**
			* some other operator for iterator.
			*/
			bool operator!=(const iterator &rhs) const
			{
				return (m != rhs.m || node != rhs.node);
			}
			bool operator!=(const const_iterator &rhs) const
			{
				return (m != rhs.m || node != rhs.node);
			}

			/**
			* for the support of it->first.
			* See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
			*/
			value_type* operator->() const noexcept
			{
				return node->data;
			}
	};
	/**
	 * TODO two constructors
	 */
	map() 
	{
		m_nullNode = new RB_Node();			//�½��սڵ�
		m_root = m_nullNode;				//��ʼ��rootΪ�սڵ�
		m_nullNode->right = this->m_root;	//
		m_nullNode->left = this->m_root;	//
		m_nullNode->parent = this->m_root;	//���нڵ㶼��ʼ��Ϊ�սڵ�
		m_nullNode->RB_COLOR = BLACK;
		SIZE = 0;
	}
	map(const map &other)		//��ǰ�������˳��ʵ�ֿ������캯��
	{
		m_nullNode = new RB_Node();
		m_root = m_nullNode;				//��ʼ��rootΪ�սڵ�
		m_nullNode->right = this->m_root;	//
		m_nullNode->left = this->m_root;	//
		m_nullNode->parent = this->m_root;	//���нڵ㶼��ʼ��Ϊ�սڵ�
		m_nullNode->RB_COLOR = BLACK;
		SIZE = other.SIZE;

		if (other.m_root != other.m_nullNode)//���other���ǿյ�
		{
			m_root = new RB_Node();
			//m_root->Count = other.m_root->Count;
			m_root->data = new value_type(*(other.m_root->data));
			m_root->parent = m_nullNode;
			m_root->left = m_root->right = m_nullNode;
			m_root->RB_COLOR = other.m_root->RB_COLOR;
			m_nullNode->right = m_root;	//
			m_nullNode->left = m_root;	//
			m_nullNode->parent = m_root;	//

			copy_left(m_root, other.m_root->left,other);
			copy_right(m_root, other.m_root->right,other);
			//������Ϊconst��ԭ����ָ������⣡��������������������������������
		}
	}

	void copy_left(RB_Node* self, RB_Node* NEW, const map &other)
	{
		if (NEW == other.m_nullNode)//����½���ǿսڵ�
			return;
		self->left = new RB_Node();
		//self->left->Count = NEW->Count;
		self->left->data = new value_type(*(NEW->data));
		self->left->parent = self;
		self->left->left = self->left->right = m_nullNode;
		self->left->RB_COLOR = NEW->RB_COLOR;
		if (NEW == other.m_nullNode->parent)
		{
			m_nullNode->parent = self->left;
		}
		copy_left(self->left, NEW->left,other);
		copy_right(self->left, NEW->right, other);
	}

	void copy_right(RB_Node* self, RB_Node* NEW, const map &other)
	{
		if (NEW == other.m_nullNode)//����½���ǿսڵ�
			return;
		self->right = new RB_Node();
		//self->right->Count = NEW->Count;
		self->right->data = new value_type(*(NEW->data));
		self->right->parent = self;
		self->right->left = self->right->right = m_nullNode;
		self->right->RB_COLOR = NEW->RB_COLOR;
		if (NEW == other.m_nullNode->parent)
		{
			m_nullNode->parent = self->right;
		}
		copy_left(self->right, NEW->left, other);
		copy_right(self->right, NEW->right, other);
	}

	/**
	 * TODO assignment operator
	 */
	map & operator=(const map &other) 
	{
		if (this == &other)
			return *this;
		//�ͷſռ�
		clear(m_root);
		delete m_nullNode;
		//�����µĿռ�
		m_nullNode = new RB_Node();
		m_root = m_nullNode;				//��ʼ��rootΪ�սڵ�
		m_nullNode->right = this->m_root;	//
		m_nullNode->left = this->m_root;	//
		m_nullNode->parent = this->m_root;	//���нڵ㶼��ʼ��Ϊ�սڵ�
		m_nullNode->RB_COLOR = BLACK;
		SIZE = other.SIZE;

		if (other.m_root != other.m_nullNode)//���other���ǿյ�
		{
			m_root = new RB_Node();
			//m_root->Count = other.m_root->Count;
			m_root->data = new value_type(*(other.m_root->data));
			m_root->parent = m_nullNode;
			m_root->left = m_root->right = m_nullNode;
			m_root->RB_COLOR = other.m_root->RB_COLOR;
			m_nullNode->right = m_root;	//
			m_nullNode->left = m_root;	//
			m_nullNode->parent = m_root;	//

			copy_left(m_root, other.m_root->left, other);
			copy_right(m_root, other.m_root->right, other);
			//������Ϊconst��ԭ����ָ������⣡��������������������������������
		}
		return *this;
	}
	/**
	 * TODO Destructors
	 */
	~map() 
	{
		clear(m_root);
		delete m_nullNode;
	}
	/**
	 * TODO
	 * access specified element with bounds checking
	 * Returns a reference to the mapped value of the element with key equivalent to key.
	 * If no such element exists, an exception of type `index_out_of_bound'
	 */
	T & at(const Key &key) 
	{
		RB_Node* index = m_root;
		Compare com;
		while (index != m_nullNode)
		{
			if (com(key, index->data->first))
			{
				index = index->left;  //�ȵ�ǰ��С������    
			}
			else if (!(com(key, index->data->first) || com(index->data->first, key)))	//������
			{
				break;
			}
			else
			{
				index = index->right;  //�ȵ�ǰ�Ĵ�����    
			}
			
		}
		if (index == m_nullNode)
			throw(index_out_of_bound());
		return index->data->second;
	}
	const T & at(const Key &key) const 
	{
		RB_Node* index = m_root;
		Compare com;
		while (index != m_nullNode)
		{
			if (com(key, index->data->first))
			{
				index = index->left;  //�ȵ�ǰ��С������    
			}
			else if (!(com(key, index->data->first) || com(index->data->first, key)))	//������
			{
				break;
			}
			else
			{
				index = index->right;  //�ȵ�ǰ�Ĵ�����    
			}
		}
		if (index == m_nullNode)
			throw(index_out_of_bound());
		return index->data->second;
	}
	/**
	 * TODO
	 * access specified element 
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */
	T & operator[](const Key &key) 
	{
		RB_Node* index = m_root;
		Compare com;
		while (index != m_nullNode)
		{
			if (com(key, index->data->first))
			{
				index = index->left;  //�ȵ�ǰ��С������    
			}
			else if (!(com(key, index->data->first) || com(index->data->first, key)))	//������
			{
				break;
			}
			else
			{
				index = index->right;  //�ȵ�ǰ�Ĵ�����    
			}
		}
		if (index == m_nullNode)
		{
			/*index = new RB_Node;*/
			pair<Key,T>* tmp = new pair<Key,T>(key,T());//!!!!!!!!!!!!!!!!!!!!û��Ĭ�Ϲ���
			/*tmp->first = key;*/
			iterator ans = insert(*tmp).first;
			delete tmp;
			return ans.node->data->second;
		}
		else
			return index->data->second;
	}
	/**
	 * behave like at() throw index_out_of_bound if such key does not exist.
	 */
	const T & operator[](const Key &key) const 
	{
		const_iterator tmp = find(key);
		if (tmp.node == m_nullNode)
		{
			throw(index_out_of_bound());
		}
		return tmp.node->data->second;
	}
	/**
	 * return a iterator to the beginning
	 */
	iterator begin() 
	{
		if (m_root == m_nullNode)
		{
			return iterator(this, m_root);
		}
		RB_Node* index = m_root;
		while (index->left != m_nullNode)
		{
			index = index->left;
		}
		return iterator(this, index);
	}
	const_iterator cbegin() const 
	{
		if (m_root == m_nullNode)
		{
			return const_iterator(this, m_root);
		}
		const RB_Node* index = m_root;
		while (index->left != m_nullNode)
		{
			index = index->left;
		}
		return const_iterator(this, index);
	}
	/**
	 * return a iterator to the end
	 * in fact, it returns past-the-end.
	 */
	iterator end() 
	{
		return iterator(this, m_nullNode);	//m_nullNode����һ�����������ڵ㣬����end()�Ķ���
	}
	const_iterator cend() const 
	{
		return const_iterator(this, m_nullNode);
	}
	/**
	 * checks whether the container is empty
	 * return true if empty, otherwise false.
	 */
	bool empty() const 
	{
		if (m_root == m_nullNode)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	/**
	 * returns the number of elements.
	 */
	size_t size() const 
	{
		return SIZE;
	}
	/**
	 * clears the contents
	 */
	void clear() 
	{
		clear(m_root);
		SIZE = 0;
		m_root = m_nullNode;
	}
	/**
	 * insert an element.
	 * return a pair, the first of the pair is
	 *   the iterator to the new element (or the element that prevented the insertion), 
	 *   the second one is true if insert successfully, or false.
	 */
	pair<iterator, bool> insert(const value_type &value) 
	{
		RB_Node* insert_point = m_nullNode;	//�����½ڵ����յĲ���λ��
		RB_Node* index = m_root;			//index�����˵�ǰ���ʵ��Ľ�㣬indexΪ�սڵ��ʱ��Ϳ���ִ�в�����
		Compare com;
		while (index != m_nullNode)
		{
			insert_point = index;
			
			if(com(value.first, index->data->first))
			{
				index = index->left;
			}
			else if(com(index->data->first,value.first))
			{
				index = index->right;
			}
			else
			{
				//index->Count++;
				return pair<iterator, bool>(iterator(this, index), false);
			}
		}
																	//��ʱinsert_point�Ѿ��ڲ���ڵ㴦�Ⱥ�
		RB_Node* insert_node = new RB_Node();
		insert_node->data = new value_type(value);				//�½�һ���ڵ�,����valueΪ�丳ֵ
		insert_node->RB_COLOR = RED;
		insert_node->right = m_nullNode;	//
		insert_node->left = m_nullNode;		//���½ڵ�����Ҷ��Ӷ���ֵΪ�սڵ㣬�����Ҫ
		if (insert_point == m_nullNode) //����������һ�ſ���    
		{
			m_root = insert_node;
			m_root->parent = m_nullNode;						//���ڵ�ĸ����ǿսڵ㣡��
			m_nullNode->left = m_root;							//
			m_nullNode->right = m_root;							//
			m_nullNode->parent = m_root;						//�սڵ�ĸ���ΪKeyֵ���Ľ��
		}
		else                            //�������Ĳ��ǿ�������ô�Ͱ�insert_node�嵽insert_point�ʵ�λ��
		{
			if (com(value.first , insert_point->data->first))
			{
				insert_point->left = insert_node;
			}
			else
			{
				insert_point->right = insert_node;
			}
			insert_node->parent = insert_point;
		}
		SIZE++;
		InsertFixUp(insert_node);    //����InsertFixUp�޸���������ʡ�    
		iterator ans(this,insert_node);
		return pair<iterator, bool>(ans, true);
	}
	void InsertFixUp(RB_Node* node)		//�����ά�������,nodeΪ����Ľڵ㣬�Ǹ�node*
	{
		while (node->parent->RB_COLOR == RED)
		{
			if (node->parent == node->parent->parent->left)   //    
			{
				RB_Node* uncle = node->parent->parent->right;
				if (uncle->RB_COLOR == RED)   //�������1��z������y�Ǻ�ɫ�ġ�    
				{
					node->parent->RB_COLOR = BLACK;
					uncle->RB_COLOR = BLACK;
					node->parent->parent->RB_COLOR = RED;
					node = node->parent->parent;
				}
				else if (uncle->RB_COLOR == BLACK)  //�������2��z������y�Ǻ�ɫ�ģ���    
				{
					if (node == node->parent->right) //��z���Һ���    
					{
						node = node->parent;
						RotateLeft(node);
					}
					//else                 //�������3��z������y�Ǻ�ɫ�ģ���z�����ӡ�    
					//{    
					node->parent->RB_COLOR = BLACK;
					node->parent->parent->RB_COLOR = RED;
					RotateRight(node->parent->parent);
					//}  
				}
			}
			else //�ⲿ�������Ϊ�������1�У�z�ĸ���������Ϊ�游���Һ����˵��������д�ġ�    
				 //15 else (same as then clause with "right" and "left" exchanged)    
			{
				RB_Node* uncle = node->parent->parent->left;
				if (uncle->RB_COLOR == RED)
				{
					node->parent->RB_COLOR = BLACK;
					uncle->RB_COLOR = BLACK;
					uncle->parent->RB_COLOR = RED;
					node = node->parent->parent;
				}
				else if (uncle->RB_COLOR == BLACK)
				{
					if (node == node->parent->left)
					{
						node = node->parent;
						RotateRight(node);     //������������ȣ�������Ϊ����    
					}
					//else    
					//{    
					node->parent->RB_COLOR = BLACK;
					node->parent->parent->RB_COLOR = RED;
					RotateLeft(node->parent->parent);   //������Ϊ���������ɡ�    
														//}    
				}
			}
		}
		m_root->RB_COLOR = BLACK;
	}

	/**
	 * erase the element at pos.
	 *
	 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
	 */
	void erase(iterator pos) 
	{
		if (pos.m != this)	//!!!!!!!!!!��������Լ��ĵ������Ļ�
		{
			throw(index_out_of_bound());
		}
		if (pos.node == m_nullNode)
			throw(index_out_of_bound());
		RB_Node* delete_point  = pos.node;
		if (delete_point->left != m_nullNode && delete_point->right != m_nullNode)	//���Ҷ��Ӿ���Ϊ��ʱ
		{
			RB_Node* successor = InOrderSuccessor(delete_point);			//��̽ڵ�
			delete delete_point->data;
			delete_point->data = new value_type(successor->data->first, successor->data->second);//delete_point�ú�̽ڵ����
			delete_point = successor;
		}
		RB_Node* delete_point_child;
		if (delete_point->right != m_nullNode)
		{
			delete_point_child = delete_point->right;
		}
		else if (delete_point->left != m_nullNode)
		{
			delete_point_child = delete_point->left;
		}
		else
		{
			delete_point_child = m_nullNode;
		}
		delete_point_child->parent = delete_point->parent;
		if (delete_point->parent == m_nullNode)//delete root node    
		{
			m_root = delete_point_child;
			m_nullNode->parent = m_root;
			m_nullNode->left = m_root;
			m_nullNode->right = m_root;
		}
		else if (delete_point == delete_point->parent->right)
		{
			delete_point->parent->right = delete_point_child;
		}
		else
		{
			delete_point->parent->left = delete_point_child;
		}
		if (delete_point->RB_COLOR == BLACK && !(delete_point_child == m_nullNode && delete_point_child->parent == m_nullNode))
		{
			DeleteFixUp(delete_point_child);
		}
		delete delete_point;
		SIZE--;
		return;
	}

	inline RB_Node* InOrderSuccessor(RB_Node* node)
	{
		if (node == m_nullNode)       //null node has no successor    
		{
			return m_nullNode;
		}
		RB_Node* result = node->right;   //get node's right node    
		while (result != m_nullNode)        //try to find node's right subtree's left most node    
		{
			if (result->left != m_nullNode)
			{
				result = result->left;
			}
			else
			{
				break;
			}
		}                              //after while loop result==null or result's left child is null    
		if (result == m_nullNode)
		{
			RB_Node* index = node->parent;
			result = node;
			while (index != m_nullNode && result == index->right)
			{
				result = index;
				index = index->parent;
			}
			result = index;         //first parent's left or null    
		}
		return result;
	}

	void DeleteFixUp(RB_Node* node)
	{
		while (node != m_root && node->RB_COLOR == BLACK)
		{
			if (node == node->parent->left)
			{
				RB_Node* brother = node->parent->right;
				if (brother->RB_COLOR == RED)   //���1��x���ֵ�w�Ǻ�ɫ�ġ�    
				{
					brother->RB_COLOR = BLACK;
					node->parent->RB_COLOR = RED;
					RotateLeft(node->parent);
				}
				else     //���2��x���ֵ�w�Ǻ�ɫ�ģ�    
				{
					if (brother->left->RB_COLOR == BLACK && brother->right->RB_COLOR == BLACK)
						//��w���������Ӷ��Ǻ�ɫ�ġ�    
					{
						brother->RB_COLOR = RED;
						node = node->parent;
					}
					else if (brother->right->RB_COLOR == BLACK)
						//���3��x���ֵ�w�Ǻ�ɫ�ģ�w���Һ����Ǻ�ɫ��w�������Ǻ�ɫ����    
					{
						brother->RB_COLOR = RED;
						brother->left->RB_COLOR = BLACK;
						RotateRight(brother);
					}
					//else if(brother->right->RB_COLOR == RED)    
					//���4��x���ֵ�w�Ǻ�ɫ�ģ���w���Һ���ʱ��ɫ�ġ�    
					//{    
					brother->RB_COLOR = node->parent->RB_COLOR;
					node->parent->RB_COLOR = BLACK;
					brother->right->RB_COLOR = BLACK;
					RotateLeft(node->parent);
					node = m_root;
					//}    
				}
			}
			else  //������������������1�У�node��Ϊ�Һ��Ӷ������ġ�    
				  //22        else (same as then clause with "right" and "left" exchanged)    
				  //ͬ����ԭ��һ�£�ֻ������������Ϊ����������������Ϊ���������ɡ��������벻�䡣    
			{
				RB_Node* brother = node->parent->left;
				if (brother->RB_COLOR == RED)
				{
					brother->RB_COLOR = BLACK;
					node->parent->RB_COLOR = RED;
					RotateRight(node->parent);
				}
				else
				{
					if (brother->left->RB_COLOR == BLACK && brother->right->RB_COLOR == BLACK)
					{
						brother->RB_COLOR = RED;
						node = node->parent;
					}
					else if (brother->left->RB_COLOR == BLACK)
					{
						brother->RB_COLOR = RED;
						brother->right->RB_COLOR = BLACK;
						RotateLeft(brother);
					}
					//else if(brother->left->RB_COLOR==RED)    
					//{    
					brother->RB_COLOR = node->parent->RB_COLOR;
					node->parent->RB_COLOR = BLACK;
					brother->left->RB_COLOR = BLACK;
					RotateRight(node->parent);
					node = m_root;
					//}    
				}
			}
		}
		m_nullNode->parent = m_root;   //���node��Ϊ����㣬    
		node->RB_COLOR = BLACK;    //����Ϊ��ɫ��    
	}
	/**
	 * Returns the number of elements with key 
	 *   that compares equivalent to the specified argument,
	 *   which is either 1 or 0 
	 *     since this container does not allow duplicates.
	 * The default method of check the equivalence is !(a < b || b > a)
	 */
	size_t count(const Key &key) const 
	{
		const_iterator tmp = find(key);
		return !(tmp.node==m_nullNode);
	}
	/**
	 * Finds an element with key equivalent to key.
	 * key value of the element to search for.
	 * Iterator to an element with key equivalent to key.
	 *   If no such element is found, past-the-end (see end()) iterator is returned.
	 */
	iterator find(const Key &key) 
	{
		RB_Node* index = m_root;
		Compare com;
		
		while (index != m_nullNode)
		{
			if (com(key,index->data->first)) //keyС
			{
				index = index->left;  //�ȵ�ǰ��С������    
			}
			else if (com(index->data->first,key))
			{
				index = index->right;  //�ȵ�ǰ�Ĵ�����    
			}
			else//������
			{
				break;
			}
		}
		return iterator(this, index);
	}
	const_iterator find(const Key &key) const 
	{
		RB_Node* index = m_root;
		Compare com;
		while (index != m_nullNode)
		{
			if (com(key, index->data->first))
			{
				index = index->left;  //�ȵ�ǰ��С������    
			}
			else if (com(index->data->first, key))
			{
				index = index->right;  //�ȵ�ǰ�Ĵ�����    
			}
			else	//������
			{
				break;
			}
		}
		return const_iterator(this, index);
	}

	//Ѱ��ǰ�����
	inline RB_Node* InOrderPredecessor(RB_Node* node)
	{
		if (node == m_nullNode)       //null node has no predecessor    
		{
			return m_nullNode;
		}
		RB_Node* result = node->left;     //get node's left child    
		while (result != m_nullNode)         //try to find node's left subtree's right most node    
		{
			if (result->right != m_nullNode)
			{
				result = result->right;
			}
			else
			{
				break;
			}
		}            //after while loop result==null or result's right child is null    
		if (result == m_nullNode)
		{
			RB_Node* index = node->parent;
			result = node;
			while (index != m_nullNode && result == index->left)
			{
				result = index;
				index = index->parent;
			}
			result = index;         // first right parent or null    
		}
		return result;
	}
};

}

#endif
