#include "pch.h"
#include "GameObject.h"
#include "CppUnitTest.h"

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
		std::string check5 = "ChildrenArray";
		G1.SetName(Name);
		G1.SetPosition(Pos);
		G1.SetRotation(Rot);
		G1.SetScale(Scale);
		Assert::IsTrue(G1.IsPrescribedAttribute(check1));
		Assert::IsTrue(G1.IsPrescribedAttribute(check2));
		Assert::IsTrue(G1.IsPrescribedAttribute(check3));
		Assert::IsTrue(G1.IsPrescribedAttribute(check4));
		Assert::IsTrue(G1.IsPrescribedAttribute(check5));
	}

	};
}