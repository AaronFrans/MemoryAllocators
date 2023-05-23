#include "CppUnitTest.h"
#include "..\Memory\MemoryOverrides.h"
#include "..\Memory\FixedSizeAllocator.h"

#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace dae
{
	const size_t block_size = 8;

	class Object
	{
	public:
		int m_integer{ 0 };
		float m_float{ 0 };
	};

	TEST_CLASS(FixedSizeAllocatorTest)
	{
	public:

		TEST_METHOD(SingleAllocation)
		{
			FixedSizeAllocator allocator(block_size);
			const size_t test_size = block_size;
			void* pointer;
			pointer = allocator.Acquire();
			Assert::IsNotNull(pointer);
			std::memset(pointer, 1, test_size);
			allocator.Release(pointer);
		}

		TEST_METHOD(InvalidRelease)
		{
			FixedSizeAllocator allocator(block_size);

			void* pointer = new char;
			Assert::ExpectException<std::runtime_error>([&]() {allocator.Release(pointer); });
			delete pointer;
		}

		TEST_METHOD(TwoAllocations)
		{
			FixedSizeAllocator allocator(block_size);

			const size_t test_size = block_size;
			void* pointer_a{};
			void* pointer_b{};
			pointer_a = allocator.Acquire();
			pointer_b = allocator.Acquire();
			Assert::IsNotNull(pointer_a);
			Assert::IsNotNull(pointer_b);
			std::memset(pointer_a, 1, test_size);
			std::memset(pointer_b, 1, test_size);
			allocator.Release(pointer_a);
			allocator.Release(pointer_b);
		}

		TEST_METHOD(ManySmallAllocations)
		{
			FixedSizeAllocator allocator(block_size);

			const size_t nbPieces = 16;
			void** pointers = new void* [nbPieces];
			const size_t test_size = block_size;
			for (int i = 0; i < nbPieces; i++)
			{
				pointers[i] = allocator.Acquire();
				Assert::IsNotNull(pointers[i]);
				std::memset(pointers[i], 1, test_size);
			}
			for (int i = 0; i < nbPieces; i++)
				allocator.Release(pointers[i]);
			delete[] pointers;
		}

		TEST_METHOD(OutOfMemory)
		{
			FixedSizeAllocator allocator(block_size);
			const size_t test_size = block_size;
			const size_t nbPieces = 100;
			void** pointers = new void* [nbPieces];
			for (int i = 0; i < nbPieces; i++)
			{
				pointers[i] = allocator.Acquire(test_size);
				Assert::IsNotNull(pointers[i]);
				std::memset(pointers[i], 1, test_size);
			};
			Assert::ExpectException<std::runtime_error>([&]() {allocator.Acquire(); });

			for (int i = 0; i < nbPieces; i++)
				allocator.Release(pointers[i]);

			delete[] pointers;
		}


		//TEST_METHOD(ObjectNewAllocation)
		//{
		//	FixedSizeAllocator allocator(block_size);
		//	Object* object = nullptr;
		//
		//	object = new(allocator)Object;
		//
		//	Assert::IsNotNull(object);
		//
		//	Assert::AreEqual(0, object->m_integer);
		//
		//	Assert::AreEqual(0.0f, object->m_float);
		//
		//	allocator.Release(object);
		//}

		TEST_METHOD(ObjectCastAllocation)
		{
			FixedSizeAllocator allocator(block_size);
			Object* object = nullptr;

			object = reinterpret_cast<Object*>(allocator.Acquire());

			Assert::IsNotNull(object);

			Assert::AreEqual(0, object->m_integer);

			Assert::AreEqual(0.0f, object->m_float);

			allocator.Release(object);
		}

		//NO Fragmentation possible
		//TEST_METHOD(Fragmentation)
		//{
			//const size_t object_size = 768;
			//
			//SingleLinkAllocator allocator(allocator_size);
			//const size_t nr_to_allocate = 64;
			//
			//Object* objects[nr_to_allocate];
			//
			//for (int i = 0; i < nr_to_allocate; i++)
			//{
			//	objects[i] = reinterpret_cast<Object*>(allocator.Acquire(sizeof(Object)));
			//}
			//
			//allocator.Release(objects[1]);
			//allocator.Release(objects[3]);
			//
			//Assert::ExpectException<std::runtime_error>([&allocator, object_size]() {
			//	allocator.Acquire(object_size);
			//	});
		//}



	};
}
