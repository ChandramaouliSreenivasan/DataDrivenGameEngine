#include "pch.h"
#include "Action.h"
#include "ActionList.h"
#include "ActionListWhile.h"
#include "TestAction.h"
#include "TableHelper.h"
#include "ParseCoordinator.h"
#include "CppUnitTest.h"


typedef Fiea::GameEngine::Datum Datum;
typedef Fiea::GameEngine::Scope Scope;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ActionTest {
	TEST_CLASS(ActionTestClass) {
		TEST_CLASS_INITIALIZE(InitClass) {
			GameObject::registerSigs();
			Action::registerSigs();
			TypeRegister::registerChild(ActionIncrement::TypeIdClass(), Action::TypeIdClass(), ActionIncrement::PrescribedAttributes());
			TypeRegister::registerChild(ActionList::TypeIdClass(), Action::TypeIdClass(), ActionList::PrescribedSignatures());
			TypeRegister::registerChild(ActionListWhile::TypeIdClass(), ActionList::TypeIdClass(), ActionListWhile::Prescribed());
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
	TEST_METHOD(Test1)
	{
		
		TestAction::TestActionFactory F1;
		GameObject obj;
		ScopeFactoryManager::Add(F1);
		obj.CreateAction("TestAction", "Action1");
		Scope* Action1 = obj.Append("Actions").GetItemAsValue<Scope*>(0);
		TestAction* TA1 = static_cast<TestAction*>(Action1);
		Assert::IsNotNull(TA1);
		Assert::AreEqual(TA1->GetName(), std::string("Action1"));
		obj.Update(0);
		Assert::IsTrue(TA1->UpdateActivated);
		obj.CreateAction("TestAction", "Action2");
		obj.CreateAction("TestAction", "Action3");
		obj.CreateAction("TestAction", "Action4");
		obj.CreateAction("TestAction", "Action5");
		ScopeFactoryManager::cleanup();
	}
	TEST_METHOD(Test2)
	{
		std::string JsonInput = R"({
"{GameObject}Monster": {
"Name": "Object1",
 "Position": [1,2,3,4],
 "Rotation": [1,2,3,4],
 "Scale": [1,2,3,4],
 "health": 10,
 "armor": 20,
"{ActionListWhile}Actions":{
	"condition":0,
	"{ActionIncrement}Preamble":{"Target":"condition",
	"IncrementAmount":3
},
	"{ActionIncrement}Actions":{"Target":"health",
	"IncrementAmount":-2
},
	"{ActionIncrement}Increment":{"Target":"condition",
	"IncrementAmount": -1
}
}
}
})";
		ActionIncrement AI1;
		ActionListWhile ALW2;
		ActionIncrement::ActionIncrementFactory F1;
		ActionListWhile::ActionListWhileFactory F2;
		GameObject::GameObjectFactory Factory;
		ScopeFactoryManager::Add(Factory);
		ScopeFactoryManager::Add(F1);
		ScopeFactoryManager::Add(F2);
		Scope S;
		TableHelper::TableWrapper wrapper(&S);
		TableHelper helper;
		ParseCoordinator parser(&wrapper);
		helper.Initialize();
		parser.AddHelper(&helper);
		bool deserialized = parser.DeserializeObject(JsonInput);
		helper.Cleanup();
		Assert::IsTrue(deserialized);
		Datum* D = S.Find("Monster");
		Assert::IsNotNull(D);
		Scope* newptr = &((*D)[0]);
		GameObject* GO = newptr->As<GameObject>();
		ActionListWhile* ALW = static_cast<ActionListWhile*>(GO->Find("Actions")->GetItemAsValue<Scope*>(0));
		ActionIncrement* Increment = static_cast<ActionIncrement*>(ALW->Find("Increment")->GetItemAsValue<Scope*>(0));
		ActionIncrement* Action = static_cast<ActionIncrement*>(ALW->Find("Actions")->GetItemAsValue<Scope*>(0));
		Assert::IsNotNull(ALW);
		Assert::IsNotNull(GO);
		GO->Update(4);
		Assert::IsTrue(GO->updateCalled);
		Assert::AreEqual(GO->Find("health")->GetItemAsValue<int>(0), 4);
		ScopeFactoryManager::cleanup();
	}
	};
}

