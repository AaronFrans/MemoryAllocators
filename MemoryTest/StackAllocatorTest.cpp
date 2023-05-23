#include "CppUnitTest.h"
#include "..\Memory\MemoryOverrides.h"
#include "..\Memory\StackAllocator.h"

#include <vector>
#include "../Memory/SingleLinkAllocator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace dae
{
	const size_t allocator_size = 1024;
	const size_t pointer_size = sizeof(void*); // this is a different value for x64/Win32 targets

	class Object
	{
	public:
		int m_integer{ 0 };
		float m_float{ 0 };
	};

	TEST_CLASS(StackAllocatorTests)
	{
	public:

		TEST_METHOD(SingleAllocation)
		{
			StackAllocator allocator(allocator_size);

			const size_t test_size = allocator_size - pointer_size;
			void* pointer;
			pointer = allocator.Acquire(test_size);
			Assert::IsNotNull(pointer);
			std::memset(pointer, 1, test_size);
			allocator.Release();
		}

		TEST_METHOD(InvalidMarker)
		{
			StackAllocator allocator(allocator_size);

			void* pointer = new char;

			Assert::ExpectException<std::runtime_error>([&]() {allocator.SetMarker(pointer); });
			delete pointer;
		}

		TEST_METHOD(TwoAllocations)
		{
			StackAllocator allocator(allocator_size);

			const size_t test_size = allocator_size / 2 - pointer_size;
			void* pointer_a{};
			void* pointer_b{};
			pointer_a = allocator.Acquire(test_size);
			pointer_b = allocator.Acquire(test_size);
			Assert::IsNotNull(pointer_a);
			Assert::IsNotNull(pointer_b);
			std::memset(pointer_a, 1, test_size);
			std::memset(pointer_b, 1, test_size);
			allocator.Release();
		}

		TEST_METHOD(ManySmallAllocations)
		{
			StackAllocator allocator(allocator_size);

			const size_t nbPieces = allocator_size / 16;
			void** pointers = new void* [nbPieces];
			const size_t test_size = allocator_size / nbPieces - pointer_size;
			for (int i = 0; i < nbPieces; i++)
			{
				pointers[i] = allocator.Acquire(test_size);
				Assert::IsNotNull(pointers[i]);
				std::memset(pointers[i], 1, test_size);
			}


			allocator.Release();
			delete[] pointers;
		}

		TEST_METHOD(OutOfMemory)
		{
			StackAllocator allocator(allocator_size);
			const size_t test_size = allocator_size - pointer_size;
			void* pointer;
			pointer = allocator.Acquire(test_size);
			Assert::IsNotNull(pointer);
			std::memset(pointer, 1, test_size);
			Assert::ExpectException<std::runtime_error>([&]() {allocator.Acquire(16); });
			allocator.Release(pointer);
		}

		TEST_METHOD(ObjectAllocation)
		{
			SingleLinkAllocator allocator(allocator_size);
			Object* object = nullptr;

			object = reinterpret_cast<Object*>(allocator.Acquire(sizeof(Object)));

			Assert::IsNotNull(object);

			Assert::AreEqual(0, object->m_integer);

			Assert::AreEqual(0.0f, object->m_float);

			allocator.Release(object);
		}

		//No Fragmentation
		//TEST_METHOD(Fragmentation)
		//{
		//	
		//
		//	
		//	
		//
		//	
		//
		//	
		//	
		//	
		//	
		//
		//	
		//	
		//
		//	
		//	
		//	
		//}



	};
}
