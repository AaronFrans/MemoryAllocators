#pragma once
#include "MemoryAllocator.h"

namespace dae
{

	class FixedSizeAllocator final : public MemoryAllocator
	{
	public:
		// Constructor
		explicit FixedSizeAllocator(size_t blockSize);

		// Destructor
		~FixedSizeAllocator() override;

		void* Acquire(size_t size = 0) override;
		// MemoryAllocator interface
		void Release(void* ptr) override;

	private:
		static constexpr size_t MaxBlocks = 100;

		struct BlockInfo
		{
			bool allocated;
		};

		size_t GetBlockIndex(void* ptr);

		size_t m_BlockSize;
		char* m_MemoryPool;
		BlockInfo* m_Blocks;
	};
}

