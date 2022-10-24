#ifndef RBTREEITERATOR_HPP
# define RBTREEITERATOR_HPP

#include "utils.hpp"
#include "RBTreeNode.hpp"


namespace ft
{
	template <typename T, typename Pointer = T*, typename Reference = T&>
	class RBTreeIterator : public ft::iterator<ft::bidirectional_iterator_tag, T>
	{
		public :
			typedef const T		value_type;
			typedef Pointer		pointer;
			typedef Reference	reference;
			typedef typename ft::iterator<ft::bidirectional_iterator_tag, T>::difference_type	difference_type;
			typedef typename ft::iterator<ft::bidirectional_iterator_tag, T>::iterator_category	iterator_category;
			typedef ft::RBTreeNode<T> node_type;
		protected :
			node_type* _node;

		public:

			RBTreeIterator(node_type* ptr = NULL) : _node(ptr) {}
			RBTreeIterator(const RBTreeIterator<T>& copy) : _node(copy.base()) {}
			RBTreeIterator& operator=(const RBTreeIterator& copy)
			{
				if (this != &copy)
					this->_node = copy.base();
				return (*this);
			}
			virtual ~RBTreeIterator() {}
			node_type* const &base() const
			{
				return (this->_node);
			}

			reference operator*() const
			{
				return (*this->_node->value);
			}
			pointer operator->() const
			{
				return (this->_node->value);
			}

			RBTreeIterator& operator++()
			{
				node_type* tmp = NULL;
				if (_node->rightChild->value != NULL)
				{
					tmp = _node->rightChild;

					while (tmp->leftChild->value != NULL)
						tmp = tmp->leftChild;
				}
				else
				{
					tmp = _node->parent;
					if (tmp->rightChild == _node)
					{
						while (tmp->parent->rightChild == tmp)
							tmp = tmp->parent;
						tmp = tmp->parent;
					}
				}
				_node = tmp;
				return (*this);
			}

			RBTreeIterator operator++(int)
			{
				RBTreeIterator tmp = *this;
				++(*this);
				return (tmp);
			}

			RBTreeIterator& operator--()
			{
				node_type* tmp = NULL;
				if (_node->value == NULL)
					tmp = _node->parent;
				else if (_node->leftChild->value != NULL)
				{
					tmp = _node->leftChild;
					while (tmp->rightChild->value != NULL)
						tmp = tmp->rightChild;
				}
				else
				{
					tmp = _node->parent;
					if (tmp->leftChild == _node)
					{
						while (tmp->parent->leftChild == tmp)
							tmp = tmp->parent;
						tmp = tmp->parent;
					}
				}
				_node = tmp;
				return (*this);
			}

			RBTreeIterator operator--(int)
			{
				RBTreeIterator tmp = *this;
				--(*this);
				return (tmp);
			}

			bool operator==(const RBTreeIterator& iter)
			{
				return (_node == iter.base());
			}

			bool operator!=(const RBTreeIterator& iter)
			{
				return (_node != iter.base());
			}


	};
}

#endif
