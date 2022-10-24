#ifndef ITERATOR_HPP
# define ITERATOR_HPP

#include <cstddef>

namespace ft
{
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag {};
	struct random_access_iterator_tag : public bidirectional_iterator_tag {};

	template <class Iterator>
	struct iterator_traits
	{
		typedef typename Iterator::value_type			value_type;
		typedef typename Iterator::difference_type		difference_type;
		typedef typename Iterator::iterator_category	iterator_category;
		typedef typename Iterator::pointer				pointer;
		typedef typename Iterator::reference 			reference;
	};

	template <class T>
	struct iterator_traits<T*>
	{
		typedef T			value_type;
		typedef ptrdiff_t	difference_type;
		typedef random_access_iterator_tag iterator_category;
		typedef T*			pointer;
		typedef T&			reference;
	};

	template <class T>
	struct iterator_traits<const T*>
	{
		typedef T value_type;
		typedef ptrdiff_t difference_type;
		typedef random_access_iterator_tag iterator_category;
		typedef const T* pointer;
		typedef const T& reference;
	};


	template <class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
	struct iterator
	{
		typedef T value_type;
		typedef Distance difference_type;
		typedef Pointer pointer;
		typedef Reference reference;
		typedef Category iterator_category;
	};


	template <class Iterator>
	class reverse_iterator
	{
		public:
			typedef Iterator iterator_type;
			typedef typename iterator_traits<Iterator>::iterator_category	iterator_category;
			typedef typename iterator_traits<Iterator>::difference_type		difference_type;
			typedef typename iterator_traits<Iterator>::value_type			value_type;
			typedef typename iterator_traits<Iterator>::pointer				pointer;
			typedef typename iterator_traits<Iterator>::reference			reference;
		protected:
			Iterator current;


		public:
			reverse_iterator() : current() {}
			explicit reverse_iterator (iterator_type it) : current(it) {}
			template <class Iter>
			reverse_iterator (const reverse_iterator<Iter>& rev_it) : current(rev_it.base()) {}

			template <typename Iter>
			reverse_iterator& operator=(const reverse_iterator<Iter>& rev_it)
			{
				this->current = rev_it.current;
				return (*this);
			}

			iterator_type base() const
			{
				return (this->current);
			}

			reference operator*() const
			{
				iterator_type tmp = this->current;
				--tmp;
				return (*tmp);
			}

			pointer operator->() const
			{
				return ( &(operator*()) );
			}

			reverse_iterator& operator++()
			{
				--this->current;
				return (*this);
			}

			reverse_iterator operator++(int)
			{
				reverse_iterator tmp = *this;
				++(*this);
				return (tmp);
			}

			reverse_iterator& operator--()
			{
				++this->current;
				return (*this);
			}

			reverse_iterator operator--(int)
			{
				reverse_iterator tmp = *this;
				--(*this);
				return (tmp);
			}

			reverse_iterator operator+(difference_type n) const
			{
				return (reverse_iterator(current - n));
			}

			reverse_iterator& operator+=(difference_type n)
			{
				this->current -= n;
				return (*this);
			}

			reverse_iterator operator-(difference_type n) const
			{
				return (reverse_iterator(current + n));
			}

			reverse_iterator& operator-=(difference_type n)
			{
				this->current += n;
				return (*this);
			}

			reference operator[](difference_type n) const
			{
				return (this->current[-n - 1]);
			}
	};

	template <class Iter1, class Iter2>
	bool operator==(const reverse_iterator<Iter1>& lhs, const reverse_iterator<Iter2>& rhs)
	{
		return ( lhs.base() == rhs.base() );
	}

	template <class Iter1, class Iter2>
	bool operator!=(const reverse_iterator<Iter1>& lhs, const reverse_iterator<Iter2>& rhs)
	{
		return ( lhs.base() != rhs.base() );
	}

	template <class Iter1, class Iter2>
	bool operator<(const reverse_iterator<Iter1>& lhs, const reverse_iterator<Iter2>& rhs)
	{
		return ( lhs.base() > rhs.base() );
	}

	template <class Iter1, class Iter2>
	bool operator>(const reverse_iterator<Iter1>& lhs, const reverse_iterator<Iter2>& rhs)
	{
		return ( lhs.base() < rhs.base() );
	}

	template <class Iter1, class Iter2>
	bool operator>=(const reverse_iterator<Iter1>& lhs, const reverse_iterator<Iter2>& rhs)
	{
		return ( lhs.base() <= rhs.base() );
	}

	template <class Iter1, class Iter2>
	bool operator<=(const reverse_iterator<Iter1>& lhs, const reverse_iterator<Iter2>& rhs)
	{
		return ( lhs.base() >= rhs.base() );
	}

	template <class Iterator>
	reverse_iterator<Iterator> operator+(typename reverse_iterator<Iterator>::difference_type n, const reverse_iterator<Iterator>& it)
	{
		return ( it.base() + n );
	}

	template <class Iterator>
	typename reverse_iterator<Iterator>::difference_type operator-(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs)
	{
		return ( rhs.base() - lhs.base() );
	}

	template <class Iter>
	bool operator==(const reverse_iterator<Iter>& lhs, const reverse_iterator<Iter>& rhs)
	{
		return ( lhs.base() == rhs.base() );
	}

	template <class Iter>
	bool operator!=(const reverse_iterator<Iter>& lhs, const reverse_iterator<Iter>& rhs)
	{
		return ( lhs.base() != rhs.base() );
	}

	template <class Iter>
	bool operator<(const reverse_iterator<Iter>& lhs, const reverse_iterator<Iter>& rhs)
	{
		return ( lhs.base() > rhs.base() );
	}

	template <class Iter>
	bool operator>(const reverse_iterator<Iter>& lhs, const reverse_iterator<Iter>& rhs)
	{
		return ( lhs.base() < rhs.base() );
	}

	template <class Iter>
	bool operator>=(const reverse_iterator<Iter>& lhs, const reverse_iterator<Iter>& rhs)
	{
		return ( lhs.base() <= rhs.base() );
	}

	template <class Iter>
	bool operator<=(const reverse_iterator<Iter>& lhs, const reverse_iterator<Iter>& rhs)
	{
		return ( lhs.base() >= rhs.base() );
	}

	template <class Iterator, class Iterator_c>
	typename ft::reverse_iterator<Iterator>::difference_type operator-(const ft::reverse_iterator<Iterator> &lhs, const ft::reverse_iterator<Iterator_c> &rhs)
	{
		return (rhs.base() - lhs.base());
	}
}

#endif
