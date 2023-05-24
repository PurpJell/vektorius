#ifndef VEKTORIUS_H
#define VEKTORIUS_H

#include <iostream>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <iterator>
#include "studentasLib.h"

using namespace std;

template <class T> class vektorius
{
public:
	typedef T* iterator;
	typedef const T* const_iterator;
	typedef size_t size_type;
	typedef T value_type;
	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

	// member functions
	vektorius() { create(); }
	explicit vektorius(size_type n, const T& val = T{}) { create(n, val); }
	vektorius(const vektorius& v) { create(v.begin(), v.end()); } // copy konstruktorius
	vektorius<T>& operator=(const vektorius& rhs); // assignment konstruktorius
	~vektorius() { uncreate(); };

	void assign(size_type n, const T& val)
	{
		while (avail + n >= limit) grow();

		uninitialized_fill(avail, avail + n, val);

		avail += n;
	}

	template<class InputIt>
	void assign(InputIt first, InputIt last) {
		// Clear the vector
		clear();

		// Assign elements from the range [first, last)
		insert(begin(), first, last);
	}

	allocator<T> get_allocator() { return alloc; }

	// element access
	T& at(size_type pos)
	{
		if (data_ + pos < avail) return *(data_ + pos);
		else throw std::out_of_range("Index out of range!");
	}

	T& at(size_type pos) const {
		if (pos >= size()) {
			throw std::out_of_range("Index out of range!");
		}
		return *(data_ + pos);
	}

	T& operator[](size_type i) { return data_[i]; }
	const T& operator[](size_type i) const { return data_[i]; }
	T& front(){ return *data_; }
	T& back() { return *(avail - 1); }
	T* data() { return data_; }

	// iterators
	iterator begin() { return data_; }
	const iterator begin() const { return data_; }
	const_iterator cbegin() const noexcept { return data_; }
	iterator end() { return avail; }
	const iterator end() const { return avail; }
	const_iterator cend() const noexcept { return avail; }
	template <class T>
	reverse_iterator rbegin() { return reverse_iterator(end()); }
	const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
	reverse_iterator rend() { return reverse_iterator(begin()); }
	const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

	// capacity
	bool empty() { return begin() == end(); }
	size_type size() const { return avail - data_; }
	size_type max_size() const { return std::allocator_traits<std::allocator<T>>::max_size(alloc); }
	void reserve(size_type new_limit)
	{
		if (new_limit > max_size()) throw std::length_error("New limit too big!");
		if (data_ + new_limit > limit)
		{
			size_type new_size = max(new_limit, size_type(1));
			
			iterator new_data = alloc.allocate(new_size);
			iterator new_avail = uninitialized_copy(data_, avail, new_data);
		
			// atlaisvina sena vieta
			uncreate();

			// resetina rodykles
			data_ = new_data;
			avail = new_avail;
			limit = data_ + new_size;
		}
	}
	size_type capacity() const { return limit - data_; }

	void shrink_to_fit()
	{
		if (limit > avail)
		{
			iterator new_data = alloc.allocate(size());
			iterator new_avail = uninitialized_copy(data_, avail, new_data);

			uncreate();

			data_ = new_data;
			avail = new_avail;
			limit = data_ + size();
		}
	}

	// modifiers
	void clear()
	{
		uncreate();
		create();
	}

	iterator insert(iterator pos, const T& val)
	{
		size_type index = pos - data_;

		if (avail == limit)
		{
			size_type pos_index = pos - data_;
			grow();
			pos = data_ + pos_index;
		}

		iterator newPos = avail;
		while (newPos != pos)
		{
			*(newPos) = *(newPos - 1);
			--newPos;
		}

		*pos = val;
		++avail;

		return pos;
	}

	iterator insert(const_iterator pos, T&& val)
	{
		size_type index = pos - data_;

		if (avail == limit)
		{
			size_type pos_index = pos - data_;
			grow();
			pos = data_ + pos_index;
		}

		iterator newPos = avail;
		while (newPos != pos)
		{
			*(newPos) = std::move(*(newPos - 1));
			--newPos;
		}

		*pos = std::move(val);
		++avail;

		return pos;
	}

	iterator insert(const_iterator pos, size_type count, const T& value)
	{
		size_type index = pos - data_;

		if (count == 0)
			return data_ + index;

		while (avail + count >= limit)
			grow();

		iterator insert_pos = data_ + index;

		if (insert_pos < avail) {
			size_type num_shifted = avail - insert_pos;
			uninitialized_copy_n(make_reverse_iterator(avail), num_shifted, make_reverse_iterator(avail + count));
			std::copy_backward(insert_pos, avail - count, avail);
			std::fill_n(insert_pos, count, value);
			avail += count;
		}
		else {
			std::uninitialized_fill_n(avail, count, value);
			avail += count;
		}

		return data_ + index;
	}

	template <class InputIt>
	iterator insert(const_iterator pos, InputIt first, InputIt last) {

		// Check if the container is empty (data_ is nullptr)
		if (data_ == nullptr) {
			// Calculate the number of elements to insert
			size_type numElementsToInsert = std::distance(first, last);

			// Allocate memory for the elements
			data_ = alloc.allocate(numElementsToInsert);

			// Update the avail and limit pointers
			avail = data_;
			limit = data_ + numElementsToInsert;

			// Copy the elements into the allocated memory
			std::uninitialized_copy(first, last, avail);

			// Update the avail pointer
			avail += numElementsToInsert;

			// Return the iterator pointing to the inserted elements
			return iterator(data_);
		}
		size_type numElementsToInsert = std::distance(first, last);

		// Check if the capacity needs to be increased
		if (avail + numElementsToInsert > limit) {
			grow();
		}

		iterator insertPos = begin() + std::distance(cbegin(), pos);

		// Move the elements after the insertion point to create space
		std::move_backward(insertPos, end(), end() + numElementsToInsert);

		// Insert the new elements
		std::copy(first, last, insertPos);

		// Update the size
		avail += numElementsToInsert;

		return insertPos;
	}

	template <class... Args>
	iterator emplace(const_iterator pos, Args&&... args)
	{
		size_type index = pos - data_;

		if (avail == limit)
			grow();

		iterator position = data_ + index;

		std::move_backward(position, avail, avail + 1);

		*position = T(std::forward<Args>(args)...);

		++avail;

		return position;
	}

	iterator erase(iterator pos)
	{
		if (pos >= avail || pos < data_)
			throw std::out_of_range("Invalid iterator position!");

		iterator next = pos + 1;

		std::move(next, avail, pos);
		alloc.destroy(avail - 1);
		--avail;

		return pos;
	}

	iterator erase(iterator first, iterator last)
	{
		if (first >= avail || first < data_ || last > avail || last < data_)
			throw std::out_of_range("Invalid iterator range!");

		iterator new_end = std::move(last, avail, first);
		size_type num_erased = last - first;

		for (iterator it = new_end; it != avail; ++it)
			alloc.destroy(it);

		avail -= num_erased;

		return first;
	}

	void push_back(const T& val)
	{
		if (avail == limit) grow();
		unchecked_append(val);
	}

	template <class... Args>
	void emplace_back(Args&&... args)
	{
		if (avail == limit)
			grow();

		alloc.construct(avail++, std::forward<Args>(args)...);
	}

	void pop_back()
	{
		if (avail != data_)
		{
			--avail;
			alloc.destroy(avail);
		}
	}

	void resize(size_type new_size)
	{
		resize(new_size, T{});
	}

	void resize(size_type new_size, const T& value)
	{
		if (new_size < size())
		{
			iterator new_avail = data_ + new_size;
			while (avail != new_avail)
				alloc.destroy(--avail);
		}
		else if (new_size > size())
		{
			if (new_size > capacity())
				reserve(new_size);

			iterator new_avail = data_ + new_size;
			while (avail != new_avail)
				unchecked_append(value);
		}
	}

	void swap(vektorius& other) noexcept
	{
		std::swap(data_, other.data_);
		std::swap(limit, other.limit);
		std::swap(avail, other.avail);
		std::swap(alloc, other.alloc);
	}

	// non-member functions
	bool operator==(const vektorius& other) const
	{
		return std::equal(begin(), end(), other.begin(), other.end());
	}

	bool operator!=(const vektorius& other) const
	{
		return !(*this == other);
	}

	bool operator<(const vektorius& other) const
	{
		return std::lexicographical_compare(begin(), end(), other.begin(), other.end());
	}

	bool operator<=(const vektorius& other) const
	{
		return !(other < *this);
	}

	bool operator>(const vektorius& other) const
	{
		return (other < *this);
	}

	bool operator>=(const vektorius& other) const
	{
		return !(*this < other);
	}

	void swap(vektorius<T>& lhs, vektorius<T>& rhs)
	{
		lhs.swap(rhs);
	}

	void erase(const T& value)
	{
		erase(std::remove(data_, avail, value), avail);
	}

	template <class UnaryPredicate>
	void erase_if(UnaryPredicate pred)
	{
		erase(std::remove_if(data_, avail, pred), avail);
	}

	


private:

	iterator data_; // pirmasis vektoriaus elementas
	iterator limit; // pirmasis elementas po paskutiniojo
	iterator avail; // pirmas neuzimtas elementas

	allocator<T> alloc; // objektas atminties valdymui

	void create();
	void create(size_type, const T&);
	void create(const_iterator, const_iterator);

	void uncreate();

	void grow();
	void unchecked_append(const T&);

	template <class ForwardIt>
	iterator erase(ForwardIt first, ForwardIt last)
	{
		iterator new_end = std::copy(last, avail, first);
		destroy(new_end, avail);
		avail = new_end;
		return new_end;
	}
};

template <class T> vektorius<T>& vektorius<T>::operator=(const vektorius& rhs)
{
	if (&rhs != this)
	{
		uncreate();

		create(rhs.begin(), rhs.end());
	}
	return *this;
}

template<class T> void vektorius<T>::create()
{
	data_ = avail = limit = nullptr;
}

template<class T> void vektorius<T>::create(size_type n, const T& val)
{
	data_ = alloc.allocate(n); // grazina ptr i array pirma elementa
	limit = avail = data_ + n; // sustato rodykles i vietas
	uninitialized_fill(data_, limit, val); // inicializuoja elementus su val reiksme
}

template<class T> void vektorius<T>::create(const_iterator i, const_iterator j)
{
	data_ = alloc.allocate(j - i); // isskirti vietos j-i elementams
	limit = avail = uninitialized_copy(i, j, data_); // nukopijuoja elementus is intervalo
}

template<class T> void vektorius<T>::uncreate()
{
	if (data_)
	{
		iterator it = avail;
		while (it != data_) alloc.destroy(--it);

		alloc.deallocate(data_, limit - data_);
	}
	data_ = limit = avail = nullptr;
}

template<class T> void vektorius<T>::grow()
{
	size_type new_size = max(2 * (limit - data_), ptrdiff_t(1)); // padvigubina vieta

	// isskiria vietos ir kopijuoja egzistuojancius elementus
	iterator new_data = alloc.allocate(new_size);
	iterator new_avail = uninitialized_copy(data_, avail, new_data);

	// atlaisvina sena vieta
	uncreate();

	// resetina rodykles
	data_ = new_data;
	avail = new_avail;
	limit = data_ + new_size;
}

template <class T> void vektorius<T>::unchecked_append(const T& val)
{
	alloc.construct(avail++, val);
}

#endif