#pragma once


#include <stdint.h>
#include <stddef.h>

template<typename T, size_t size>
class aligned_storage
{
private:
	uint8_t bytes_[size * sizeof(T) + alignof(T)];

	aligned_storage(const aligned_storage& other);
	aligned_storage& operator=(const aligned_storage& other);
public:
	aligned_storage() 
	{};

	void* operator[](size_t pos)
	{
		return const_cast<void*>(const_cast<const aligned_storage*>(this)->operator[](pos));
	}

	const void* operator[](size_t pos) const
	{
		const uint8_t* base = &bytes_[pos * sizeof(T)];
		return (uintptr_t)base % alignof(T) == 0 ? base : base + alignof(T) - (uintptr_t)base % alignof(T);
	}
};

