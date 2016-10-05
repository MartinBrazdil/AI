#pragma once

#include "Tensor.h"

template<typename T>
class Tensor<T>::Iterator : public std::iterator<std::random_access_iterator_tag, T>
{
public:
	using difference_type = typename std::iterator<std::random_access_iterator_tag, T>::difference_type;

	Iterator() : _ptr(nullptr) {}
	Iterator(T* rhs) : _ptr(rhs) {}
	Iterator(const Iterator &rhs) : _ptr(rhs._ptr) {}
	/* inline Iterator& operator=(T* rhs) {_ptr = rhs; return *this;} */
	/* inline Iterator& operator=(const Iterator &rhs) {_ptr = rhs._ptr; return *this;} */
	inline Iterator& operator+=(difference_type rhs) { _ptr += rhs; return *this; }
	inline Iterator& operator-=(difference_type rhs) { _ptr -= rhs; return *this; }
	inline T& operator*() const { return *_ptr; }
	inline T* operator->() const { return _ptr; }
	inline T& operator[](difference_type rhs) const { return _ptr[rhs]; }

	inline Iterator& operator++() { ++_ptr; return *this; }
	inline Iterator& operator--() { --_ptr; return *this; }
	inline Iterator operator++(int) const { Iterator tmp(*this); ++_ptr; return tmp; }
	inline Iterator operator--(int) const { Iterator tmp(*this); --_ptr; return tmp; }
	/* inline Iterator operator+(const Iterator& rhs) {return Iterator(_ptr+rhs.ptr);} */
	inline difference_type operator-(const Iterator& rhs) const { return Iterator(_ptr - rhs.ptr); }
	inline Iterator operator+(difference_type rhs) const { return Iterator(_ptr + rhs); }
	inline Iterator operator-(difference_type rhs) const { return Iterator(_ptr - rhs); }
	friend inline Iterator operator+(difference_type lhs, const Iterator& rhs) { return Iterator(lhs + rhs._ptr); }
	friend inline Iterator operator-(difference_type lhs, const Iterator& rhs) { return Iterator(lhs - rhs._ptr); }

	inline bool operator==(const Iterator& rhs) const { return _ptr == rhs._ptr; }
	inline bool operator!=(const Iterator& rhs) const { return _ptr != rhs._ptr; }
	inline bool operator>(const Iterator& rhs) const { return _ptr > rhs._ptr; }
	inline bool operator<(const Iterator& rhs) const { return _ptr < rhs._ptr; }
	inline bool operator>=(const Iterator& rhs) const { return _ptr >= rhs._ptr; }
	inline bool operator<=(const Iterator& rhs) const { return _ptr <= rhs._ptr; }
private:
	T* _ptr;
};