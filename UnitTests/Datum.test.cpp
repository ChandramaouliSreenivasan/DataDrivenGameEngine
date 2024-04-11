#pragma warning (disable:4715)

#include "pch.h"
#include "CppUnitTest.h"
#include "Temp.h"
#include "Datum.h"
#include "Scope.h"
#include "Foo.h"
#include "Empty.h"
#include "EmptyChild.h"
#include "AttributedArmoredCore.h"

typedef Fiea::GameEngine::Temp Temp;
typedef Fiea::GameEngine::Datum Datum;
typedef Fiea::GameEngine::Scope Scope;


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<Datum>(const Datum& D) {
		RETURN_WIDE_STRING(&D);
	}
	template<>
	inline std::wstring ToString<Datum>(Datum* D) {
		RETURN_WIDE_STRING(D);
	}
	template<>
	inline std::wstring ToString<Datum>(const Datum* D) {
		RETURN_WIDE_STRING(D);
	}
}
namespace DatumTest
{
	TEST_CLASS(DatumTest)
	{
	private:
		inline static _CrtMemState _startMemState;
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
		}
		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState))
			{
				_CrtDumpMemoryLeaks();
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}
		TEST_METHOD(ConstructorsandGetters) {
			Datum D1;
			Datum D2(D1);
			D1.PushItem<int>(1);
			Assert::AreEqual(D1.GetItemAsValue<int>(0), 1);
			D1.PushItem<int>(2);
			D1.SetItemWithValue<int>(1, size_t(1));
			Assert::AreEqual(D1.GetItemAsValue<int>(1), 1);
		}
		TEST_METHOD(AssignmentOperators) {
			int iarray[] = {1,2,3,4,5};
			Datum D1;
			for (int i = 0; i < 5; i++) {
				D1.PushItem(iarray[i]);
			}
			Datum D2;
			D2 = D1;
			Assert::IsTrue(D2 == D1);
			Datum D3;
			D3 = std::move(D2);
			//D3 = D2;
			float x;
			Assert::ExpectException<std::logic_error>([&D1, &x] {x = D1.GetItemAsValue<float>(size_t(0)); });
			D3.PushItem<int>(6);
			D3.SetItemWithValue<int>(2, size_t(5));
			Assert::AreEqual(D3.GetItemAsValue<int>(1), D3.GetItemAsValue<int>(5));
			Assert::IsFalse(D3.GetSize() == D3.GetCapacity());
			D3.shrink_to_fit();
			Assert::IsTrue(D3.GetSize() == D3.GetCapacity());
			D3.clear();
			Assert::IsTrue(D3.GetType() == Fiea::GameEngine::DatumType::Unknown);
			Datum D5(D1);
			Assert::AreEqual(D5.GetCapacity(), D1.GetCapacity());
			D5.Resize(4);
			Assert::AreEqual(D5.GetItemAsValue<int>(D5.GetSize() - 1), 4);
			D5.Reserve(10);
			Datum D6;
			D6 = 4;
			Assert::AreEqual(D6.GetItemAsValue<int>(0), 4);
			D6.SetStorage<int>(iarray, 5);
			Datum D7;
			D7.PushItem<float>(2.5);
			Assert::ExpectException<std::logic_error>([&D7] {D7.PushItem<int>(1); });
		}
		
		TEST_METHOD(RTTITest) {
			Foo F1;
			Assert::IsTrue(F1.Is(F1.TypeIdClass()));
			Empty E1;
			EmptyChild EC1;
			Assert::IsTrue(EC1.Is(E1.TypeIdClass()));
			Assert::IsTrue(EC1.As<Empty>());
			const EmptyChild CEC;
			Assert::IsTrue(CEC.As<Empty>());
		}
	};
	TEST_CLASS(AttributedTest) {
	private:
		inline static _CrtMemState _startMemState;
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
		}
		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState))
			{
				_CrtDumpMemoryLeaks();
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}
		
	};
}