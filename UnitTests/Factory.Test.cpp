#include "pch.h"
#include "AttributedArmoredCore.h"
#include "ParseCoordinator.h"
#include"TableHelper.h"
#include "CppUnitTest.h"


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

namespace FactoryTest {
	TEST_CLASS(FactoryTestClass) {
	TEST_CLASS_INITIALIZE(InitClass) {
		AttributedArmoredCore::registerSigs();

	}
	TEST_CLASS_CLEANUP(CleanupClass) {
		TypeRegister::clear();
		//ScopeFactoryManager::cleanup();
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
	
	TEST_METHOD(JsonTest) {
		std::string JsonData = R"(
		{
		"{AttributedArmoredCore}ArmoredCore1":{ 
		 "Name": "ArmoredCore",
		 "HP": 250,
		"Attack": 9.5,
		"Position": [1.0,2.0,3.0,4.0],
		"Rotation": [
					  [1.0, 0.0, 0.0, 0.0],
					  [0.0, 1.0, 0.0, 0.0],
					  [0.0, 0.0, 1.0, 0.0],
					  [0.0, 0.0, 0.0, 1.0]
					]
		}}
		)";
		std::string check1 = "Attack";
		std::string check2 = "Position";
		std::string check3 = "Rotation";
		std::string check4 = "Name";
		AttributedArmoredCore::AttributedArmoredCoreFactory Factory;
		ScopeFactoryManager::Add(Factory);
		Scope S;
		TableHelper::TableWrapper wrapper(&S);
		TableHelper helper;
		ParseCoordinator parser(&wrapper);
		helper.Initialize();
		parser.AddHelper(&helper);
		auto deserialized = parser.DeserializeObject(JsonData);
		helper.Cleanup();
		Assert::IsTrue(deserialized);
		Assert::IsTrue(*wrapper.Get() == S);
		Datum* D = &S.Append("ArmoredCore1");
		Scope* newptr = &(D->operator[](0));
		AttributedArmoredCore * Ac = newptr->As<AttributedArmoredCore>();
		Assert::IsTrue(Ac->IsPrescribedAttribute(check1));
		Assert::IsTrue(Ac->IsPrescribedAttribute(check2));
		Assert::IsTrue(Ac->IsPrescribedAttribute(check3));
		Assert::IsTrue(Ac->IsPrescribedAttribute(check4));
		ScopeFactoryManager::cleanup();
	}
	};
}