#include "StackAllocator.h"
#include <stdexcept>

dae::StackAllocator::StackAllocator(size_t size)
	: MemoryAllocator()
	, m_Size(size)
	, m_MemoryPool(nullptr)
	, m_CurrentPtr(nullptr)
	, m_Marker(nullptr)
{
	m_MemoryPool = new char[m_Size];
	m_CurrentPtr = m_MemoryPool;
	m_Marker = m_CurrentPtr;
}

dae::StackAllocator::~StackAllocator()
{
	delete[] m_MemoryPool;
}

void* dae::StackAllocator::Acquire(size_t size)
{
	if (m_CurrentPtr + size <= m_MemoryPool + m_Size)
	{
		void* ptr = static_cast<void*>(m_CurrentPtr);
		m_CurrentPtr += size;
		return ptr;
	}

	throw std::runtime_error("No free block available in StackAllocator::Acquire");
}

void dae::StackAllocator::Release(void*)
{
	m_CurrentPtr = m_Marker;
}

void* dae::StackAllocator::GetMarker() const
{
	return static_cast<void*>(m_Marker);
}

void dae::StackAllocator::SetMarker(void* marker)
{
	// Set the marker to the provided pointer
	m_Marker = static_cast<char*>(marker);

	// Check if the marker is within the valid range
	if (m_Marker >= m_MemoryPool && m_Marker <= m_CurrentPtr)
	{
		// Update the stack pointer to the marker
		m_CurrentPtr = m_Marker;
	}
	else
	{
		throw std::runtime_error("Invalid marker provided to StackAllocator::FreeToMarker");
	}
}
