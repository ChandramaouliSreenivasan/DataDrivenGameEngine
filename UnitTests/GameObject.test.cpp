#include "pch.h"
#include "GameObject.h"
#include "CppUnitTest.h"
#include "TableHelper.h"
#include "ParseCoordinator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace GameObjTest {

	TEST_CLASS(GameObjectTest) {
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
private:
	inline static _CrtMemState _startMemState;
	TEST_CLASS_INITIALIZE(ClassInit) {
		GameObject::registerSigs();
	}
	TEST_CLASS_CLEANUP(ClassCLeanup) {
		TypeRegister::clear();
	}
public:
	TEST_METHOD(ObjectCreationTesting) {
		GameObject G1;
		std::string Name = "Object 1";
		glm::vec4 Pos = glm::vec4(1);
		glm::vec4 Rot = glm::vec4(2);
		glm::vec4 Scale = glm::vec4(3);
		std::string check1 = "Name";
		std::string check2 = "Position";
		std::string check3 = "Rotation";
		std::string check4 = "Scale";
		G1.SetName(Name);
		G1.SetPosition(Pos);
		G1.SetRotation(Rot);
		G1.SetScale(Scale);
		Assert::IsTrue(G1.IsPrescribedAttribute(check1));
		Assert::IsTrue(G1.IsPrescribedAttribute(check2));
		Assert::IsTrue(G1.IsPrescribedAttribute(check3));
		Assert::IsTrue(G1.IsPrescribedAttribute(check4));
	}
	TEST_METHOD(ChildrenAndUpdateTesting) {
		std::string JsonInput = R"({
    "{GameObject}ContainingObject": {
        "Name": "MainObj",
        "Position": [
            1,
            2,
            3,
            1.2
        ],
        "Rotation": [
            1,
            2,
            3,
            1.2
        ],
        "Scale": [
            1,
            2,
            3,
            1.2
        ],
        "{GameObject}children":{
		"Name": "Child",
        "Position": [
            1,
            2,
            3,
            1.2
        ],
        "Rotation": [
            1,
            2,
            3,
            1.2
        ],
        "Scale": [
            1,
            2,
            3,
            1.2
        ]
		}
    }
})";
		std::string check1 = "Name";
		std::string check2 = "Position";
		std::string check3 = "Rotation";
		std::string check4 = "Scale";
		GameObject::GameObjectFactory Factory;
		ScopeFactoryManager::Add(Factory);
		Scope S;
		TableHelper::TableWrapper wrapper(&S);
		TableHelper helper;
		ParseCoordinator parser(&wrapper);
		helper.Initialize();
		parser.AddHelper(&helper);
		bool deserialized = parser.DeserializeObject(JsonInput);
		helper.Cleanup();
		Assert::IsTrue(deserialized);
		Datum* D = S.Find("ContainingObject");
		Assert::IsNotNull(D);
		Scope* newptr = &((*D)[0]);
		GameObject* GO = newptr->As<GameObject>();
		Assert::IsTrue(GO->IsPrescribedAttribute(check1));
		ScopeFactoryManager::cleanup();
	}
	TEST_METHOD(ArrayOfChildren) {
        std::string Input = R"({
    "{GameObject}ContainingObject": {
        "Name": "MainObj",
        "Position": [
            1,
            2,
            3,
            1.2
        ],
        "Rotation": [
            1,
            2,
            3,
            1.2
        ],
        "Scale": [
            1,
            2,
            3,
            1.2
        ],
        "{GameObject}children": [
            {
                "Name": "Child",
                "Position": [
                    1,
                    2,
                    3,
                    1.2
                ],
                "Rotation": [
                    1,
                    2,
                    3,
                    1.2
                ],
                "Scale": [
                    1,
                    2,
                    3,
                    1.2
                ]
            },
            {
                "Name": "Child2",
                "Position": [
                    1,
                    2,
                    3,
                    1.2
                ],
                "Rotation": [
                    1,
                    2,
                    3,
                    1.2
                ],
                "Scale": [
                    1,
                    2,
                    3,
                    1.2
                ]
            }
        ]
    }
})";
        std::string check1 = "Name";
        std::string check2 = "Position";
        std::string check3 = "Rotation";
        std::string check4 = "Scale";
        GameObject::GameObjectFactory Factory;
        ScopeFactoryManager::Add(Factory);
        Scope S;
        TableHelper::TableWrapper wrapper(&S);
        TableHelper helper;
        ParseCoordinator parser(&wrapper);
        helper.Initialize();
        parser.AddHelper(&helper);
        bool deserialized = parser.DeserializeObject(Input);
        helper.Cleanup();
        Assert::IsTrue(deserialized);
        Datum* D = S.Find("ContainingObject");
        Assert::IsNotNull(D);
        Scope* newptr = &((*D)[0]);
        GameObject* GO = newptr->As<GameObject>();
        Assert::IsTrue(GO->IsPrescribedAttribute(check1));
        GO->Update(2);
        GameObject* Child1 = GO->Append("children").GetItemAsValue<Scope*>(0)->As<GameObject>();
        GameObject* Child2 = GO->Append("children").GetItemAsValue<Scope*>(1)->As<GameObject>();
        Assert::IsTrue(Child1->updateCalled);
        Assert::IsTrue(Child2->updateCalled);
        Assert::IsTrue(GO->updateCalled);
        ScopeFactoryManager::cleanup();
	}
	};
}