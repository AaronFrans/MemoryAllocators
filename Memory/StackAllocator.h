#pragma once
#include "MemoryAllocator.h"

namespace dae
{

	class StackAllocator final : public MemoryAllocator
	{
	public:
		explicit StackAllocator(size_t size);
		~StackAllocator() override;


		void* Acquire(size_t size = 0) override;

		void Release(void* ptr = nullptr) override;

		void* GetMarker() const;

		void SetMarker(void* marker);
	private:


		size_t m_Size;
		char* m_MemoryPool;
		char* m_CurrentPtr;
		char* m_Marker;




	};

}
