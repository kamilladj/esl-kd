#pragma once

#include <stdint.h>


namespace utils
{

	template<typename T, size_t sz = 0>
	class aligned_storage
	{
	private:

		uint8_t m_bytes[sz * sizeof(T) + alignof(T)];

	public:

		aligned_storage() {}

		aligned_storage(const aligned_storage& other) = delete;

	public:

		void* operator[](size_t pos)
		{
			return const_cast<void*>(const_cast<const aligned_storage*>(this)->operator[](pos));
		}

		const void* operator[](size_t pos) const
		{
			const uint8_t* base = &m_bytes[pos * sizeof(T)];
			return (uintptr_t)base % alignof(T) == 0 ? base : base + alignof(T) - (uintptr_t)base % alignof(T);
		}
	};


	template<typename T>
	class aligned_storage<T, 0>
	{
	private:

		uint8_t* m_bytes;

	public:

		aligned_storage(size_t size)
			: m_bytes(new uint8_t[size * sizeof(T) + alignof(T)])
		{}

		aligned_storage(const aligned_storage& other) = delete;

		~aligned_storage()
		{
			delete[] m_bytes;
		}

	public:

		void* operator[](size_t pos)
		{
			return const_cast<void*>(const_cast<const aligned_storage*>(this)->operator[](pos));
		}

		const void* operator[](size_t pos) const
		{
			const uint8_t* base = &m_bytes[pos * sizeof(T)];
			return (uintptr_t)base % alignof(T) == 0 ? base : base + alignof(T) - (uintptr_t)base % alignof(T);
		}
	};

}  // namespace utils