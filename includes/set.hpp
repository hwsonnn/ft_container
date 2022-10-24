#ifndef MAP_HPP
# define MAP_HPP

#include "RBTree.hpp"

namespace ft
{

	template < class Key, class Compare = ft::less<Key>, class Alloc = std::allocator<Key> >
	class set {
		public :

			typedef Key	key_type;
			typedef Key	value_type;
			typedef Compare	key_compare;

			typedef Compare	value_compare;
			typedef Alloc	allocator_type;
			typedef typename allocator_type::reference			reference;
			typedef typename allocator_type::const_reference	const_reference;
			typedef typename allocator_type::pointer			pointer;
			typedef typename allocator_type::const_pointer		const_pointer;
			typedef typename ft::RBTreeIterator<value_type, value_type*, value_type&>	iterator;
			typedef typename ft::RBTreeIterator<value_type, const value_type*, const value_type&>	const_iterator;
			typedef ft::reverse_iterator<iterator>				reverse_iterator;
			typedef ft::reverse_iterator<const_iterator>		const_reverse_iterator;
			typedef typename allocator_type::difference_type	difference_type;
			typedef typename allocator_type::size_type			size_type;
			typedef ft::RBTree<value_type, value_compare>		rb_tree;
			typedef ft::RBTreeNode<value_type>					node_type;


		private:
			allocator_type	_alloc;
			rb_tree			_tree;
			key_compare		_comp;

		public:

			explicit set (const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type()) : _alloc(alloc), _tree(), _comp(comp) {}

			template <class InputIterator>
			set (InputIterator first, InputIterator last,
					const key_compare& comp = key_compare(),
					const allocator_type& alloc = allocator_type(),
					typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = NULL) : _alloc(alloc), _tree(), _comp(comp)
			{
				insert(first, last);
			}

			set (const set& x) : _alloc(x._alloc), _tree(), _comp(x._comp)
			{
				*this = x;
			}

			~set() {}

			set& operator=(const set& x)
			{
				if (this != &x)
					this->_tree.copy(x._tree);
				return *this;
			}

			iterator begin()
			{
				return iterator(this->_tree.get_begin());
			}
			const_iterator begin() const
			{
				return const_iterator(this->_tree.get_begin());
			}

			iterator end()
			{
				return iterator(this->_tree.get_end());
			}
			const_iterator end() const
			{
				return const_iterator(this->_tree.get_end());
			}

			reverse_iterator rbegin()
			{
				return reverse_iterator(this->_tree.get_end());
			}
			const_reverse_iterator rbegin() const
			{
				return const_reverse_iterator(this->_tree.get_end());
			}

			reverse_iterator rend()
			{
				return reverse_iterator(this->_tree.get_begin());
			}
			const_reverse_iterator rend() const
			{
				return const_reverse_iterator(this->_tree.get_begin());
			}

			bool empty() const
			{
				return (this->_tree.empty());
			}
			size_type size() const
			{
				return (this->_tree.size());
			}
			size_type max_size() const
			{
				return (this->_tree.max_size());
			}

			pair<iterator, bool> insert(const value_type& val)
			{
				ft::pair<node_type*, bool> res = _tree.insert(val);
				return (ft::make_pair(iterator(res.first), res.second));
			}

			iterator insert(iterator position, const value_type& val)
			{
				return (iterator(this->_tree.insert(val, position.base()).first));
			}

			template <class InputIterator>
			void insert(InputIterator first, InputIterator last,
			typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = NULL)
			{
				while (first != last)
					this->_tree.insert(*first++);
			}

			void erase(iterator position)
			{
				this->_tree.erase(position.base());
			}

			size_type erase(const key_type& k)
			{
				return (this->_tree.erase(_tree.find(value_type(k))));
			}

			void erase(iterator first, iterator last)
			{
				while (first != last)
					erase(first++);
			}

			void swap(set& x)
			{
				this->_tree.swap(x._tree);
			}

			void clear()
			{
				this->_tree.clear();
			}

			key_compare key_comp() const
			{
				return (key_compare());
			}

			value_compare value_comp() const
			{
				return (value_compare());
			}

			iterator find(const key_type& k)
			{
				return (iterator(this->_tree.find(value_type(k))));
			}

			const_iterator find(const key_type& k) const
			{
				return (const_iterator(this->_tree.find(value_type(k))));
			}

			size_type count(const key_type& k) const
			{
				if (this->_tree.find(value_type(k))->value != NULL)
					return (1);
				else
					return (0);
			}


			iterator lower_bound(const key_type& k)
			{
				return (iterator(this->_tree.lower_bound(value_type(k))));
			}

			const_iterator lower_bound(const key_type& k) const
			{
				return (const_iterator(this->_tree.lower_bound(value_type(k))));
			}


			iterator upper_bound(const key_type& k)
			{
				return (iterator(this->_tree.upper_bound(value_type(k))));
			}
			const_iterator upper_bound(const key_type& k) const
			{
				return (const_iterator(this->_tree.upper_bound(value_type(k))));
			}


			pair<iterator, iterator> equal_range(const key_type& k)
			{
				return (ft::make_pair(lower_bound(k), upper_bound(k)));
			}

			allocator_type get_allocator() const
			{
				return (this->_alloc);
			}

			void showTree()
			{
				this->_tree.showMap();
			}


	};


	template <class Key, class Compare, class Alloc>
	bool operator==(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs)
	{
		return (lhs.size() == rhs.size() && ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
	}

	template <class Key, class Compare, class Alloc>
	bool operator!=(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs)
	{
		return (!(lhs == rhs));
	}

	template <class Key, class Compare, class Alloc>
	bool operator<(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs)
	{
		return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}

	template <class Key, class Compare, class Alloc>
	bool operator<=(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs)
	{
		return (!(rhs < lhs));
	}

	template <class Key, class Compare, class Alloc>
	bool operator>(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs)
	{
		return (rhs < lhs);
	}

	template <class Key, class Compare, class Alloc>
	bool operator>=(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs)
	{
		return (!(lhs < rhs));
	}

	template <class Key, class Compare, class Alloc>
	void swap(set<Key, Compare, Alloc>& x, set<Key, Compare, Alloc>& y)
	{
		x.swap(y);
	}
}

#endif
