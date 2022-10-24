#ifndef RBTREE_HPP
# define RBTREE_HPP

#include <stdexcept>
#include "RBTreeIterator.hpp"
#include "printMap.hpp"

namespace ft
{

	template < typename T, typename Compare = ft::less<T>, typename Alloc = std::allocator<T> >
	class RBTree {
		public :

			typedef T	value_type;
			typedef T*	pointer;
			typedef const T*	const_pointer;
			typedef T&	reference;
			typedef const T&	const_reference;
			typedef Compare	value_comp;
			typedef Alloc	allocator_type;
			typedef size_t	size_type;
			typedef ft::RBTreeNode<T>	node_type;
			typedef typename ft::RBTreeIterator<T, T*, T&>	iterator;
			typedef typename ft::RBTreeIterator<T, const T*, const T&>	const_iterator;
			typedef typename Alloc::template rebind<node_type>::other	node_allocator_type;

		private:

			node_type*	_root;
			node_type*	_nil;
			size_type	_size;
			value_comp	_comp;
			node_allocator_type	_node_alloc;

		public:

			RBTree() : _root(NULL), _nil(NULL), _size(0), _comp(value_comp()), _node_alloc(node_allocator_type())
			{
				this->_nil = make_nil();
				this->_root = this->_nil;
			}

			RBTree(const RBTree& x) : _root(NULL), _nil(NULL), _size(0), _comp(value_comp()), _node_alloc(node_allocator_type())
			{
				this->_nil = make_nil();
				copy(x);
				this->_nil->parent = get_max_value_node();
			}

			~RBTree()
			{
				clear();
				_node_alloc.destroy(this->_nil);
				_node_alloc.deallocate(this->_nil, 1);
			}

			RBTree& operator=(const RBTree& x)
			{
				if (this != &x)
					copy(x);
				return (*this);
			}

			void copy(const RBTree& x)
			{
				clear();
				copy(x._root);
			}

			void copy(node_type* node)
			{
				if (node->value == NULL)
					return ;
				insert(*node->value);
				if (node->leftChild->value != NULL)
					copy(node->leftChild);
				if (node->rightChild->value != NULL)
					copy(node->rightChild);
			}

			node_type* get_begin() const
			{
				node_type* tmp = this->_root;
				while (tmp->leftChild->value != NULL)
					tmp = tmp->leftChild;
				return (tmp);
			}

			node_type* get_end() const
			{
				return (this->_nil);
			}

			bool empty() const
			{
				return (this->_size == 0);
			}

			size_type size() const
			{
				return (this->_size);
			}

			size_type max_size() const
			{
				return _node_alloc.max_size();
			}


			ft::pair<node_type*, bool> insert(const value_type& val, node_type* hint = NULL)
			{
				node_type* new_node = make_node(val);
				node_type* position = this->_root;
				if (this->_size == 0)
				{
					this->_root = new_node;
					this->_root->leftChild = this->_nil;
					this->_root->rightChild = this->_nil;
					this->_root->parent = this->_nil;
					this->_root->color = BLACK;
					this->_nil->parent = this->_root;
					this->_size++;
					return ft::make_pair(this->_root, true);
				}
				if (hint != NULL && hint->value != NULL)
					position = check_hint(val, hint);
				ft::pair<node_type*, bool> is_valid = get_position(position, new_node);
				if (is_valid.second == false)
				{
					_node_alloc.destroy(new_node);
					_node_alloc.deallocate(new_node, 1);
					return (is_valid);
				}
				insert_case1(new_node);
				this->_size++;
				this->_nil->parent = get_max_value_node();
				return (ft::make_pair(new_node, true));
			}


			size_type erase(node_type* node)
			{
				if (node->value == NULL)
					return (0);

				node_type* target = replace_erase_node(node);
				node_type* child;
				if (target->rightChild->value == NULL)
					child = target->leftChild;
				else
					child = target->rightChild;

				replace_node(target, child);
				if (target->color == BLACK)
				{
					if (child->color == RED)
						child->color = BLACK;
					else
						delete_case1(child);
				}
				this->_size--;
				if (target->parent->value == NULL)
					this->_root = this->_nil;
				delete target;
				this->_nil->parent = get_max_value_node();
				return (1);
			}

			void swap(RBTree& x)
			{
				swap(_root, x._root);
				swap(_nil, x._nil);
				swap(_comp, x._comp);
				swap(_node_alloc, x._node_alloc);
				swap(_size, x._size);
			}

			void clear(node_type* node = NULL)
			{
				if (node == NULL)
					node = this->_root;
				if (node->leftChild->value != NULL)
				{
					clear(node->leftChild);
					node->leftChild = this->_nil;
				}
				if (node->rightChild->value != NULL)
				{
					clear(node->rightChild);
					node->rightChild = this->_nil;
				}
				// delete
				if (node->value != NULL)
				{
					if (node == this->_root)
						this->_root = this->_nil;
					_node_alloc.destroy(node);
					_node_alloc.deallocate(node, 1);
					this->_size--;
				}
			}

			node_type* find(value_type val) const
			{
				node_type* res = this->_root;
				if (this->_size == 0)
					return (this->_nil);
				while (res->value != NULL && (_comp(val, *res->value) || _comp(*res->value, val)))
				{
					if (_comp(val, *res->value))
						res = res->leftChild;
					else
						res = res->rightChild;
				}
				return (res);
			}

			node_type* lower_bound(const value_type& val) const
			{
				iterator it(get_begin());
				iterator ite(get_end());
				while (it != ite && _comp(*it, val))
					it++;
				return (it.base());
			}

			node_type* upper_bound(const value_type& val) const
			{
				iterator it(get_begin());
				iterator ite(get_end());
				while (it != ite && !_comp(val, *it))
					it++;
				return (it.base());
			}

			void showMap() { ft::printMap(_root, 0); }

		private :
			node_type* get_grandparent(node_type* node) const
			{
				if (node != NULL && node->parent != NULL)
					return (node->parent->parent);
				else
					return (NULL);
			}

			node_type* get_uncle(node_type* node) const
			{
				node_type* grand = get_grandparent(node);
				if (grand == NULL)
					return (NULL);
				if (grand->leftChild == node->parent)
					return (grand->rightChild);
				else
					return (grand->leftChild);
			}

			node_type* get_sibling(node_type* node) const
			{
				if (node == node->parent->leftChild)
					return (node->parent->rightChild);
				else
					return (node->parent->leftChild);
			}

			node_type* get_max_value_node() const
			{
				node_type* tmp = _root;
				while (tmp->rightChild->value != NULL)
					tmp = tmp->rightChild;
				return (tmp);
			}

			node_type* make_nil()
			{
				node_type* res = _node_alloc.allocate(1);
				_node_alloc.construct(res, node_type());
				res->color = BLACK;
				res->leftChild = res;
				res->rightChild = res;
				res->parent = res;
				res->value = NULL;
				return (res);
			}

			node_type* make_node(const value_type& val)
			{
				node_type* res = _node_alloc.allocate(1);
				_node_alloc.construct(res, node_type(val));
				return (res);
			}


			node_type* check_hint(value_type val, node_type* hint)
			{
				if (_comp(*hint->value, *_root->value) && _comp(val, *hint->value))
					return (hint);
				else if (_comp(*hint->value, *_root->value) && _comp(*hint->value, val))
					return (_root);
				else if (_comp(*_root->value, *hint->value) && _comp(val, *hint->value))
					return (_root);
				else if (_comp(*_root->value, *hint->value) && _comp(*hint->value, val))
					return (hint);
				else
					return (_root);
			}

			ft::pair<node_type*, bool> get_position(node_type* position, node_type* node)
			{
				while (position->value != NULL)
				{
					if (_comp(*node->value, *position->value))
					{
						if (position->leftChild->value == NULL)
						{
							position->leftChild = node;
							node->parent = position;
							node->leftChild = _nil;
							node->rightChild = _nil;
							node->color = RED;
							break;
						}
						else
							position = position->leftChild;
					}
					else if (_comp(*position->value, *node->value))
					{
						if (position->rightChild->value == NULL)
						{
							position->rightChild = node;
							node->parent = position;
							node->leftChild = _nil;
							node->rightChild = _nil;
							node->color = RED;
							break;
						}
						else
							position = position->rightChild;
					}
					else
						return (ft::make_pair(position, false));
				}
				return (ft::make_pair(position, true));
			}

			node_type* replace_erase_node(node_type* node)
			{

				node_type* res;
				if (node->leftChild->value != NULL)
				{
					res = node->leftChild;
					while (res->rightChild->value != NULL)
						res = res->rightChild;
				}
				else if (node->rightChild->value != NULL)
				{
					res = node->rightChild;
					while (res->leftChild->value != NULL)
						res = res->leftChild;
				}
				else
					return (node);

				node_type* tmp_parent = node->parent;
				node_type* tmp_left = node->leftChild;
				node_type* tmp_right = node->rightChild;
				RBColor tmp_color = node->color;

				node->leftChild = res->leftChild;
				if (res->leftChild->value != NULL)
					res->leftChild->parent = node;
				node->rightChild = res->rightChild;
				if (res->rightChild->value != NULL)
					res->rightChild->parent = node;

				if (tmp_parent->leftChild == node)
					tmp_parent->leftChild = res;
				else if (tmp_parent->rightChild == node)
					tmp_parent->rightChild = res;

				if (res == tmp_left)
				{
					tmp_right->parent = res;
					res->rightChild = tmp_right;
					node->parent = res;
					res->leftChild = node;
				}
				else if (res == tmp_right)
				{
					tmp_left->parent = res;
					res->leftChild = tmp_left;
					node->parent = res;
					res->rightChild = node;
				}
				else
				{
					tmp_left->parent = res;
					res->leftChild = tmp_left;
					tmp_right->parent = res;
					res->rightChild = tmp_right;
					node->parent = res->parent;
					res->parent->rightChild = node;
				}

				res->parent = tmp_parent;

				if (res->parent->value == NULL)
					this->_root = res;
				node->color = res->color;
				res->color = tmp_color;

				return (node);
			}

			void replace_node(node_type* node, node_type* child)
			{
				child->parent = node->parent;
				if (node->parent->leftChild == node)
					node->parent->leftChild = child;
				else
					node->parent->rightChild = child;
			}

			void insert_case1(node_type* node)
			{
				if (node->parent->value != NULL)
					insert_case2(node);
				else
					node->color = BLACK;
			}

			void insert_case2(node_type* node)
			{
				if (node->parent->color == RED)
					insert_case3(node);
			}

			void insert_case3(node_type* node)
			{
				node_type* uncle = get_uncle(node);
				node_type* grand;
				if (uncle->value != NULL && uncle->color == RED)
				{
					node->parent->color = BLACK;
					uncle->color = BLACK;
					grand = get_grandparent(node);
					grand->color = RED;
					insert_case1(grand);
				}
				else
					insert_case4(node);
			}

			void insert_case4(node_type* node)
			{

				node_type* grand = get_grandparent(node);
				if (node == node->parent->rightChild && node->parent == grand->leftChild)
				{
					rotate_left(node->parent);
					node = node->leftChild;
				}
				else if (node == node->parent->leftChild && node->parent == grand->rightChild)
				{
					rotate_right(node->parent);
					node = node->rightChild;
				}
				insert_case5(node);
			}

			void insert_case5(node_type* node)
			{

				node_type* grand = get_grandparent(node);
				node->parent->color = BLACK;
				grand->color = RED;
				if (node == node->parent->leftChild)
					rotate_right(grand);
				else
					rotate_left(grand);
			}


			void rotate_left(node_type* node)
			{
				node_type* child = node->rightChild;
				node_type* parent = node->parent;
				if (child->leftChild->value != NULL)
					child->leftChild->parent = node;
				node->rightChild = child->leftChild;
				node->parent = child;
				child->leftChild = node;
				child->parent = parent;
				if (parent->value != NULL)
				{
					if (parent->leftChild == node)
						parent->leftChild = child;
					else
						parent->rightChild = child;
				}
				else
					this->_root = child;
			}

			void rotate_right(node_type* node)
			{
				node_type* child = node->leftChild;
				node_type* parent = node->parent;
				if (child->rightChild->value != NULL)
					child->rightChild->parent = node;
				node->leftChild = child->rightChild;
				node->parent = child;
				child->rightChild = node;
				child->parent = parent;
				if (parent->value != NULL)
				{
					if (parent->rightChild == node)
						parent->rightChild = child;
					else
						parent->leftChild = child;
				}
				else
					this->_root = child;
			}

			void delete_case1(node_type* node)
			{

				if (node->parent->value != NULL)
					delete_case2(node);
			}

			void delete_case2(node_type* node)
			{

				node_type* sibling = get_sibling(node);
				if (sibling->color == RED)
				{
					node->parent->color = RED;
					sibling->color = BLACK;
					if (node == node->parent->leftChild)
						rotate_left(node->parent);
					else
						rotate_right(node->parent);
				}
				delete_case3(node);
			}


			void delete_case3(node_type* node)
			{

				node_type* sibling = get_sibling(node);
				if (node->parent->color == BLACK && sibling->color == BLACK && sibling->leftChild->color == BLACK && sibling->rightChild->color == BLACK)
				{
					sibling->color = RED;
					delete_case1(node->parent);
				}
				else
					delete_case4(node);
			}


			void delete_case4(node_type* node)
			{

				node_type* sibling = get_sibling(node);
				if (node->parent->color == RED && sibling->color == BLACK && sibling->leftChild->color == BLACK && sibling->rightChild->color == BLACK)
				{
					sibling->color = RED;
					node->parent->color = BLACK;
				}
				else
					delete_case5(node);
			}

			void delete_case5(node_type* node)
			{

				node_type* sibling = get_sibling(node);

				if (sibling->color == BLACK)
				{
					if (node == node->parent->leftChild && sibling->rightChild->color == BLACK && sibling->leftChild->color == RED)
					{
						sibling->color = RED;
						sibling->leftChild->color = BLACK;
						rotate_right(sibling);
					}
					else if (node == node->parent->rightChild && sibling->leftChild->color == BLACK && sibling->rightChild->color == RED)
					{
						sibling->color = RED;
						sibling->rightChild->color = BLACK;
						rotate_left(sibling);
					}
				}
				delete_case6(node);
			}

			void delete_case6(node_type* node)
			{

				node_type* sibling = get_sibling(node);
				sibling->color = node->parent->color;
				node->parent->color = BLACK;
				if (node == node->parent->leftChild)
				{
					sibling->rightChild->color = BLACK;
					rotate_left(node->parent);
				}
				else
				{
					sibling->leftChild->color = BLACK;
					rotate_right(node->parent);
				}
			}

			template <typename _T>
			void swap(_T& a, _T& b)
			{
				_T tmp(a);
				a = b;
				b = tmp;
			}

	};
}


#endif
