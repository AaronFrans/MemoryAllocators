#include "FixedSizeAllocator.h"
#include <cstddef>
#include <stdexcept>

dae::FixedSizeAllocator::FixedSizeAllocator(size_t blockSize)
	: MemoryAllocator()
	, m_BlockSize(blockSize)
	, m_MemoryPool(nullptr)
	, m_Blocks(nullptr)
{
	// Allocate memory for the memory pool and block information
	m_MemoryPool = new char[m_BlockSize * MaxBlocks];
	m_Blocks = new BlockInfo[MaxBlocks]{};

	// Initialize the block information
	for (size_t i = 0; i < MaxBlocks; ++i)
		m_Blocks[i].allocated = false;
}

dae::FixedSizeAllocator::~FixedSizeAllocator()
{
	delete[] m_MemoryPool;
	delete[] m_Blocks;
}


void* dae::FixedSizeAllocator::Acquire(size_t)
{
	for (size_t i = 0; i < MaxBlocks; ++i)
	{
		if (!m_Blocks[i].allocated)
		{
			m_Blocks[i].allocated = true;

			char* blockAddress = m_MemoryPool + (i * m_BlockSize);

			std::memset(blockAddress, 0, m_BlockSize);

			return static_cast<void*>(blockAddress);
		}
	}

	throw std::runtime_error("No free block available in FixedSizeAllocator::Acquire");
}

void dae::FixedSizeAllocator::Release(void* ptr)
{
	size_t blockIndex = GetBlockIndex(ptr);
	if (blockIndex < MaxBlocks && m_Blocks[blockIndex].allocated)
	{
		m_Blocks[blockIndex].allocated = false;
	}
	else
	{
		throw std::runtime_error("Invalid pointer provided to FixedSizeAllocator::Release");
	}
}

size_t dae::FixedSizeAllocator::GetBlockIndex(void* ptr)
{
	char* blockAddress = static_cast<char*>(ptr);
	size_t offset = blockAddress - m_MemoryPool;
	return offset / m_BlockSize;
}
