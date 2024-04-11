#include "pch.h"
#include "CppUnitTest.h"
#include "Temp.h"
#include "Datum.h"
#include "Scope.h"
#include "Foo.h"
#include "Empty.h"
#include "EmptyChild.h"
#include "AttributedArmoredCore.h"
#include "TestParseHandler.h"
#include "IntParseHandler.h"
#include "TableHelper.h"

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

namespace ParserTest {
	TEST_CLASS(TestAndIntParsing) {
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
	TEST_METHOD(NestedObject) { 
		std::string nested = R"(
        {
            "int":1,
            "obj":{
                "int":1,
                "obj":{
                    "int":1,
                    "str":"abc"
                },
                "str":"abc"
            },
            "obj2":{
                "int":1,
                "obj":{
                    "int":1,
                    "str":"abc"
                },
                "str":"abc"
            },
            "str":"abc"
        }
    )";

		TestParseHandler::Wrapper wrapper;            
		ParseCoordinator parser(&wrapper);            
		TestParseHandler handler;
		handler.Initialize();
		parser.AddHelper(&handler);
		bool deserialized = parser.DeserializeObject(nested);
		handler.Cleanup();
		Assert::IsTrue(deserialized);                      
		Assert::IsTrue(handler.initializeCalled);            
		Assert::IsTrue(handler.cleanupCalled);          
		Assert::AreEqual(14, int(handler.startCount));            
		Assert::AreEqual(14, int(handler.endCount));            
		Assert::AreEqual(3, int(wrapper.maxDepth)); 
	}
	TEST_METHOD(IntJsonTest) {
		std::string intobj = R"({"int":1})";
		int a;
		IntParseHandler::IntWrapper wrapper(&a);
		ParseCoordinator parser(&wrapper);
		IntParseHandler handler;
		handler.Initialize();
		parser.AddHelper(&handler);
		bool deserialized = parser.DeserializeObject(intobj);
		handler.Cleanup();
		Assert::IsTrue(deserialized);
		Assert::IsTrue(wrapper.Get() == a);
	}
	TEST_METHOD(IntJsonFileTest) {
		std::string file = "jsonvalidator.json";
		int a;
		IntParseHandler::IntWrapper wrapper(&a);
		ParseCoordinator parser(&wrapper);
		IntParseHandler handler;
		handler.Initialize();
		parser.AddHelper(&handler);
		bool deserialized = parser.DeserializeObjectFromFile(file);
		handler.Cleanup();
		Assert::IsTrue(deserialized);
		Assert::IsTrue(wrapper.Get() == a);
	}
	TEST_METHOD(TableParseTest) {
		std::string JsonData = R"(
        {
            "int":1,
            "obj":{
                "int":1,
                "obj":{
                    "int":1,
                    "str":"abc"
                },
                "str":"abc"
            },
            "obj2":{
                "int":1,
                "obj":{
                    "int":1,
                    "str":"abc"
                },
                "str":"abc"
            },
            "str":"abc"
        }
    )";
		Scope S;
		TableHelper::TableWrapper wrapper(&S);
		ParseCoordinator parser(&wrapper);
		TableHelper helper;
		helper.Initialize();
		parser.AddHelper(&helper);
		bool deserialized = parser.DeserializeObject(JsonData);
		helper.Cleanup();
		Assert::IsTrue(deserialized);
		Assert::IsTrue(*wrapper.Get() == S);
	}
	};
}