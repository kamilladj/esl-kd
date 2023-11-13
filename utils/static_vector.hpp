#pragma once

#ifndef STATIC_VECTOR_HPP
#define STATIC_VECTOR_HPP

#include "aligned_storage.hpp"
#include <utility>

template<typename T, size_t size>
class static_vector
{
private:
	// properly aligned uninitialized storage for N T's
	aligned_storage<T, size> m_storage;
	size_t m_size;

	//utility functions used by =operator & swap method
	void resize(size_t min, size_t max)
	{
		for (size_t i = min; i < max; i++)
			pop_back();
	}

	void move_to(static_vector& vec1, static_vector& vec2, size_t min, size_t max)
	{
		for (size_t i = min; i < max; i++)
		{
			vec2.push_back((vec1)[i]);
			(vec1).pop_back();
		}
	}
public:
	static_vector()
		: m_storage()
		, m_size(0)
	{}

	static_vector(const static_vector& other)
		: m_storage()
		, m_size(other.m_size)
	{
		for (size_t i = 0; i < m_size; i++)
		{
			new(m_storage[i]) T(other[i]);
		}
	}

	static_vector& operator=(const static_vector& other)
	{
		if (this != &other)
		{
			size_t min = std::min(m_size, other.m_size);
			size_t max = std::max(m_size, other.m_size);
			for (size_t i = 0; i < min; i++)
			{
				(*this)[i] = other[i];
			}

			if (min == m_size)
			{
				for (size_t i = min; i < max; i++)
					push_back(other[i]);
			}
			else
				resize(min, max);
		}
		return *this;
	}

	// Create an object in aligned storage
	template<typename V>
	void push_back(const V& v)
	{
		if (m_size == size)
			throw std::overflow_error("Stack overflow!");

		new(m_storage[m_size]) T(v);
		++m_size;
	}

	void pop_back()
	{
		if (m_size == 0)
			throw std::underflow_error("Stack underflow!");

		((T*)m_storage[m_size - 1])->~T();
		--m_size;
	}

	// Access an object in aligned storage
	T& operator[](size_t pos)
	{
		return *(T*)m_storage[pos];
	}

	const T& operator[](size_t pos) const
	{
		return *(T*)m_storage[pos];
	}

	size_t get_size() const
	{
		return m_size;
	}

	void swap(static_vector& other)
	{
		size_t min = std::min(m_size, other.m_size);
		size_t max = std::max(m_size, other.m_size);
		for (size_t i = 0; i < min; i++)
		{
			std::swap((*this)[i], other[i]);
		}

		if (min == m_size)
			move_to(other, *this, min, max);
		else
			move_to(*this, other, min, max);
	}

	// Destroy objects from aligned storage
	~static_vector()
	{
		for (size_t i = m_size; i > 0; i--)
		{
			((T*)m_storage[i-1])->~T();
		}
	}
};

#endif STATIC_VECTOR_HPP
