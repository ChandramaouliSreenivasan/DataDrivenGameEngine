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


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
typedef Fiea::GameEngine::Temp Temp;
typedef Fiea::GameEngine::Datum Datum;
typedef Fiea::GameEngine::Scope Scope;
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
namespace ScopeTest {
	TEST_CLASS(ScopeTest) {
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
	//constructors and copy constructors
	TEST_METHOD(ConstructionOfScopes) {
		Scope S1(5);
		Assert::AreEqual(S1.Capacity(), size_t(5));
		S1.Append("Int").PushItem<int>(1);
		S1.Append("Int").PushItem<int>(2);
		S1.Append("Int").PushItem<int>(3);
		S1.Append("Int").PushItem<int>(4);
		S1.Append("Int").PushItem<int>(5);

		Scope S3(5);
		S3.Append("Int").PushItem<int>(1);
		S3.Append("Int").PushItem<int>(2);
		S3.Append("Int").PushItem<int>(3);
		S3.Append("Int").PushItem<int>(4);
		S3.Append("Int").PushItem<int>(5);


		Scope S2(S1);
		//Equality Operator Check
		Assert::IsTrue(S2 == S1);

		S2.Append("Int").SetItemWithValue<int>(10,0);
		//Is Copy Deep
		Assert::IsTrue(S2 != S1);

		Scope S4(std::move(S3));

		Assert::IsTrue(S4 == S3);
	}
	TEST_METHOD(AssignmentOperators) {
		Scope S1(5);
		S1.Append("Int").PushItem<int>(1);
		S1.Append("float").PushItem<float>(float(2));
		S1.Append("Int").PushItem<int>(3);
		S1.Append("Int").PushItem<int>(4);
		S1.Append("Int").PushItem<int>(5);

		Scope S2;
		S2 = S1;

		Assert::IsTrue(S2 == S1);

		S2.Append("Int").SetItemWithValue<int>(10, 0);
		//Is Copy Deep
		Assert::IsTrue(S2 != S1);

		Scope S3;
		S3 = std::move(S1);
		Assert::IsTrue(S3 == S1);

		S3.Append("Int").SetItemWithValue<int>(10, 0);
		Assert::IsTrue(S3 != S1);
	}
	//Search and Parentage tested here
	TEST_METHOD(ParentingAndTrees) {
		Scope S1;
		S1.Append("Int").PushItem<int>(5);
		S1.Append("Float").PushItem<float>(float(1.5));
		S1.Append("String").PushItem<std::string>("Hello World");
		Scope& S11a = S1.AppendScope("First");
		Scope& S11b = S1.AppendScope("First");
		Scope& S2 = S1.AppendScope("Second");
		Datum* foundDatum;
		Scope** foundScope = new Scope*;
		foundDatum = S2.Search("Int",foundScope);
		Assert::IsNotNull(foundDatum);
		Assert::IsTrue(foundDatum == &(S1.Append("Int")));
		Assert::IsNotNull(foundScope);
		Assert::IsTrue(*foundScope == &S1);
		size_t i;
		Assert::IsNotNull(S1.FindContainedScope(&S11a, i));

		Assert::IsTrue(S2.IsDescendantOf(&S1));
		Assert::IsTrue(S1.IsAncestorOf(&S2));
		Assert::IsTrue(S11a.IsDescendantOf(&S1));

		delete foundScope;
	}
	//Find Tested Here
	TEST_METHOD(FindTesting) {
		Scope S1;
		S1.Append("Int").PushItem<int>(4);
		S1.Append("Float").PushItem<float>((float)3.5);
		S1.Append("String").PushItem<std::string>("FooBar");

		//Checking negative result
		Assert::IsNull(S1.Find("Vector"));

		//Checking positive result
		Assert::IsNotNull(S1.Find("Int"));
	}
	//Adopt and Orphan Tested
	TEST_METHOD(AdoptOrphan) {
		Scope S1;
		S1.Append("Int").PushItem<int>(4);
		S1.Append("Float").PushItem<float>((float)3.5);
		S1.Append("String").PushItem<std::string>("FooBar");

		Scope S2;
		S2.Adopt(S1, "First");

		Assert::IsTrue(S1.GetParent() == &S2);

	}
	};
}