#include "pch.h"
#include "CppUnitTest.h"
#include "Temp.h"
#include "Datum.h"
#include "Scope.h"
#include "TypeRegister.h"
#include "AttributedArmoredCore.h"
#include "AttributedFoo.h"

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

namespace AttributedTest {
	TEST_CLASS(AttributedTestClass) {
		TEST_CLASS_INITIALIZE(InitClass) {
			AttributedArmoredCore::registerSigs();
			TypeRegister::registerChild(AttributedFoo::TypeIdClass(), AttributedArmoredCore::TypeIdClass(), AttributedFoo::Prescribed());
		}
		TEST_CLASS_CLEANUP(CleanupClass) {
			TypeRegister::clear();
		}
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
	TEST_METHOD(FirstTest) {
		AttributedArmoredCore AC1;
		AttributedArmoredCore AC2(AC1);
		Assert::IsTrue(AC2 == AC1);
		AC2.Append("Name").SetItemWithValue < std::string >("Ac2");
		Assert::IsFalse(AC2 == AC1);
		AttributedArmoredCore AC3;
		AC3.AppendAuxiliaryAttribute("check1");
		AC3 = AC2;
		Assert::IsTrue(AC3 == AC2);
		std::string check1 = "Name";
		std::string check2 = "Hp";
		std::string newAux = "Aux2";
		Assert::IsTrue(AC3.IsAttribute(check1));
		AC3.AppendAuxiliaryAttribute(newAux);
		AC3.AppendAuxiliaryAttribute("check1");
		AttributedArmoredCore AC4(AC3);
		Assert::IsTrue(AC3.IsAuxiliaryAttribute(newAux));
	}
	TEST_METHOD(InheritanceTest) {
		AttributedFoo AF1;
		std::string check = "HP";
		std::string check2 = "Attack";
		std::string check3 = "Position";
		std::string check4 = "Rotation";
		std::string check5 = "Foo";
		Assert::IsTrue(AF1.IsPrescribedAttribute(check));
		Assert::IsTrue(AF1.IsPrescribedAttribute(check2));
		Assert::IsTrue(AF1.IsPrescribedAttribute(check3));
		Assert::IsTrue(AF1.IsPrescribedAttribute(check4));
		Assert::IsTrue(AF1.IsPrescribedAttribute(check5));
	}
	};
}