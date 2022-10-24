#ifndef VECTOR_HPP
# define VECTOR_HPP

#include <memory>
#include <stdexcept>
#include "VectorIterator.hpp"
#include "utils.hpp"

namespace ft
{
	template < typename T, typename Allocator = std::allocator<T> >
	class vector
	{
		public:

			typedef T value_type;
			typedef Allocator allocator_type;
			typedef typename allocator_type::reference			  reference;
			typedef typename allocator_type::const_reference	const_reference;
			typedef typename allocator_type::pointer			    pointer;
			typedef typename allocator_type::const_pointer	  const_pointer;
			typedef ft::VectorIterator<T>						iterator;
			typedef ft::VectorIterator<const T> 				const_iterator;
			typedef ft::reverse_iterator<iterator>				reverse_iterator;
			typedef ft::reverse_iterator<const_iterator>		const_reverse_iterator;
			typedef typename allocator_type::size_type			size_type;
			typedef typename allocator_type::difference_type	difference_type;

		private:
			allocator_type	_alloc;
			pointer			_start;
			pointer			_end;
			pointer			_end_of_capacity;

		public:


		explicit vector(const allocator_type &alloc = allocator_type())
		: _alloc(alloc), _start(NULL), _end(NULL), _end_of_capacity(NULL){}

		explicit vector(size_type n, const value_type &val = value_type(), const allocator_type &alloc = allocator_type())
		: _alloc(alloc), _start(NULL), _end(NULL), _end_of_capacity(NULL)
		{
			this->_start = this->_alloc.allocate(n);
			this->_end = this->_start;
			this->_end_of_capacity = this->_start + n;
			while (n--)
				this->_alloc.construct(this->_end++, val);
		}

		template <typename InputIterator>
		vector(InputIterator first, InputIterator last,
				const allocator_type &alloc = allocator_type(),
				typename ft::enable_if< !ft::is_integral< InputIterator >::value, InputIterator >::type* = NULL)
		: _alloc(alloc), _start(NULL), _end(NULL), _end_of_capacity(NULL)
		{
			difference_type n = ft::distance(first, last);
			this->_start = this->_alloc.allocate(n);
			this->_end = this->_start;
			this->_end_of_capacity = this->_start + n;
			while (n--)
				this->_alloc.construct(this->_end++, *first++);
		}

		vector(const vector &x)
		: _alloc(x._alloc), _start(NULL), _end(NULL), _end_of_capacity(NULL)
		{
			difference_type n = x._end - x._start;
			this->_start = this->_alloc.allocate(n);
			this->_end = this->_start;
			pointer tmp = x._start;
			while (n--)
				_alloc.construct(_end++, *tmp++);
			this->_end_of_capacity = this->_end;
		}

		~vector()
		{
			this->clear();
			this->_alloc.deallocate(this->_start, this->_end_of_capacity - this->_start);
		}

		vector &operator=(const vector &x)
		{
			if (this != &x)
			{
				this->clear();
				this->assign(x.begin(), x.end());
			}
			return (*this);
		}

		iterator begin()
		{
			return (iterator(this->_start));
		}

		const_iterator begin() const
		{
			return (const_iterator(this->_start));
		}

		iterator end()
		{
			return (iterator(this->_end));
		}

		const_iterator end() const
		{
			return (const_iterator(this->_end));
		}

		reverse_iterator rbegin()
		{
			return (reverse_iterator(this->_end));
		}

		const_reverse_iterator rbegin() const
		{
			return (const_reverse_iterator(this->_end));
		}

		reverse_iterator rend()
		{
			return (reverse_iterator(this->_start));
		}

		const_reverse_iterator rend() const
		{
			return (const_reverse_iterator(this->_start));
		}


		size_type size() const
		{
			return (this->_end - this->_start);
		}

		size_type max_size() const
		{
			return (this->_alloc.max_size());
		}

		void resize(size_type n, value_type val = value_type())
		{
			if (n < size())
				erase(_start + n, _end);
			else if (n > size())
				insert(_end, n - size(), val);
		}

		size_type capacity() const
		{
			return (this->_end_of_capacity - this->_start);
		}

		bool empty() const
		{
			return (this->_start == this->_end);
		}

		void reserve(size_type n)
		{
			if (n > max_size()) //최대 크기를 넘어가면 에러
				throw(std::length_error("Error: ft::vector::reserve"));
			else if (n > this->capacity())
			{
				pointer prev_start = this->_start;
				pointer prev_end = this->_end;
				pointer prev_end_of_capacity = this->_end_of_capacity;

				this->_start = this->_alloc.allocate(n);
				this->_end = this->_start;
				this->_end_of_capacity = this->_start + n;
				pointer tmp = prev_start;
				while (tmp != prev_end)
				{
					this->_alloc.construct(this->_end++, *tmp);
					this->_alloc.destroy(tmp++);
				}
				this->_alloc.deallocate(prev_start, prev_end_of_capacity - prev_start);
			}
		}

		reference operator[](size_type n)
		{
			return (*(this->_start + n));
		}

		const_reference operator[](size_type n) const
		{
			return (*(this->_start + n));
		}

		reference at(size_type n)
		{
			if (n >= this->size())
				throw(std::out_of_range("Error: ft::vector::at"));
			return ((*this)[n]);
		}

		const_reference at(size_type n) const {
			if (n >= this->size())
				throw(std::out_of_range("Error: ft::vector::at"));
			return ((*this)[n]);
		}

		reference front()
		{
			return (*(this->_start));
		}

		const_reference front() const
		{
			return (*(this->_start));
		}

		reference back()
		{
			return (*(this->_end - 1));
		}

		const_reference back() const
		{
			return (*(this->_end - 1));
		}

		template < typename InputIterator >
		void assign(InputIterator first, InputIterator last,
					typename ft::enable_if< !ft::is_integral< InputIterator >::value, InputIterator >::type * = NULL)
		{
			this->clear();
			size_type n = ft::distance(first, last);
			if (n <= this->capacity())
			{
				while (n--)
					this->_alloc.construct(this->_end++, *first++);
			}
			else
			{
				pointer prev_start = this->_start;
				pointer prev_end_of_capacity = this->_end_of_capacity;

				this->_start = this->_alloc.allocate(n);
				this->_end = this->_start;
				this->_end_of_capacity = this->_start + n;
				const_pointer tmp = &(*first);
				while(tmp != &(*last))
					this->_alloc.construct(this->_end++, *tmp++);
				this->_alloc.deallocate(prev_start, prev_end_of_capacity - prev_start);
			}
		}

		void assign(size_type n, const value_type &val)
		{
			this->clear();
			if (n <= this->capacity())
			{
				while (n--)
					this->_alloc.construct(this->_end++, val);
			}
			else
			{
				pointer prev_start = this->_start;
				pointer prev_end_of_capacity = this->_end_of_capacity;

				this->_start = this->_alloc.allocate(n);
				this->_end = this->_start;
				this->_end_of_capacity = this->_start + n;
				while (n--)
					this->_alloc.construct(this->_end++, val);
				this->_alloc.deallocate(prev_start, prev_end_of_capacity - prev_start);
			}
		}

		void push_back(const value_type &val)
		{
			if (this->_end == this->_end_of_capacity)
			{
				if (this->size() == 0)
					this->reserve(1);
				else
					this->reserve(this->capacity() * 2);
			}
			this->_alloc.construct(this->_end++, val);
		}

		void pop_back()
		{
			this->_alloc.destroy(--this->_end);
		}


		iterator insert(iterator position, const value_type &val)
		{
			size_type n = &(*position) - this->_start;
			this->insert(position, 1, val);
			return (this->begin() + n);
		}

		void insert(iterator position, size_type n, const value_type &val)
		{
			if (this->size() + n <= this->capacity())
			{
				pointer val_tmp = this->_end;
				size_type range = this->_end - &(*position);
				this->_end += n;
				pointer tmp = this->_end;
				while (range--)
					this->_alloc.construct(--tmp, *(--val_tmp));
				while (n--)
					this->_alloc.construct(--tmp, val);
			}
			else
			{
				pointer tmp = this->_start;
				size_type _size = n + this->size();
				size_type front_tmp = &(*position) - this->_start;
				size_type back_tmp = _end - &(*position);
				this->_start = _alloc.allocate(_size);
				this->_end = _start;
				this->_end_of_capacity = this->_start + _size;
				while (front_tmp--)
				{
					_alloc.construct(this->_end++, *tmp);
					_alloc.destroy(tmp++);
				}
				while (n--)
					_alloc.construct(this->_end++, val);
				while (back_tmp--)
				{
					_alloc.construct(this->_end++, *tmp);
					_alloc.destroy(tmp++);
				}
			}
		}

		template < typename InputIterator >
		void insert(iterator position, InputIterator first, InputIterator last,
			typename ft::enable_if< !ft::is_integral< InputIterator >::value >::type* = NULL)
		{
			size_type n = ft::distance(first, last);
			if (this->size() + n <= this->capacity())
			{
				pointer val_tmp = this->_end;
				size_type range = this->_end - &(*position);
				this->_end += n;
				pointer tmp = this->_end;
				while (range--)
					this->_alloc.construct(--tmp, *(--val_tmp));
				while (n--)
					this->_alloc.construct(--tmp, *(--last));
			}
			else
			{
				pointer tmp = this->_start;
				size_type _size = n + this->size();
				size_type front_tmp = &(*position) - this->_start;
				size_type back_tmp = this->_end - &(*position);
				this->_start = this->_alloc.allocate(_size);
				this->_end = this->_start;
				this->_end_of_capacity = this->_start + _size;
				while (front_tmp--)
				{
					_alloc.construct(this->_end++, *tmp);
					_alloc.destroy(tmp++);
				}
				while (n--)
					_alloc.construct(this->_end++, *first++);
				while (back_tmp--)
				{
					_alloc.construct(this->_end++, *tmp);
					_alloc.destroy(tmp++);
				}
			}
		}

		iterator erase(iterator position)
		{
			this->_alloc.destroy(&(*position));
			size_type n = this->_end - &(*position) - 1;
			pointer tmp = &(*position);

			while (n--)
			{
				this->_alloc.construct(tmp, *(tmp + 1));
				this->_alloc.destroy(tmp++);
			}
			--this->_end;
			return (position);
		}

		iterator erase(iterator first, iterator last)
		{
			pointer tmp = &(*first);
			while (tmp != &(*last))
				_alloc.destroy(tmp++);
			size_type n = this->_end - &(*last);
			size_type range = ft::distance(first, last);
			tmp = &(*first);
			while (n--)
			{
				_alloc.construct(tmp, *last++);
				_alloc.destroy(tmp++);
			}
			this->_end -= range;
			return (first);
		}

		void swap(vector &x) {
			if (*this == x)
				return ;

			allocator_type tmp_alloc = x._alloc;
			pointer tmp_start = x._start;
			pointer tmp_end = x._end;
			pointer tmp_end_of_capacity = x._end_of_capacity;

			x._alloc = this->_alloc;
			x._start = this->_start;
			x._end = this->_end;
			x._end_of_capacity = this->_end_of_capacity;

			this->_alloc = tmp_alloc;
			this->_start = tmp_start;
			this->_end = tmp_end;
			this->_end_of_capacity = tmp_end_of_capacity;
		}

		void clear()
		{
			while (this->_start != this->_end)
				this->_alloc.destroy(--this->_end);
		}

		allocator_type get_allocator() const
		{
			return (this->_alloc);
		}
	};

	template <typename T, typename Alloc>
	bool operator==(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
	{
		if (lhs.size() != rhs.size())
			return (false);
		typename ft::vector< T, Alloc >::const_iterator it_lhs = lhs.begin();
		typename ft::vector< T, Alloc >::const_iterator it_rhs = rhs.begin();

		while (it_lhs != lhs.end())
		{
			if (it_rhs == rhs.end() || *it_lhs++ != *it_rhs++)
				return (false);
		}
		return (true);
	}

	template < typename T, typename Alloc>
	bool operator!=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
	{
		return (!(lhs == rhs));
	}

	template <typename T, typename Alloc>
	bool operator<(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
	{
		return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}

	template <typename T, typename Alloc>
	bool operator<=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
	{
		return (!(rhs < lhs));
	}

	template <typename T, typename Alloc>
	bool operator>(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
	{
		return (rhs < lhs);
	}

	template <typename T, typename Alloc>
	bool operator>=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
	{
		return (!(lhs < rhs));
	}

	template <typename T, typename Alloc>
	void swap(vector< T, Alloc > &x, vector< T, Alloc > &y)
	{
		x.swap(y);
	}
}

#endif
