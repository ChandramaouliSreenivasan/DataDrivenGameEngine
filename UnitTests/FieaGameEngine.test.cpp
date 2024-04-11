#include "pch.h"
#include "CppUnitTest.h"
#include "Temp.h"
#include "Datum.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
typedef Fiea::GameEngine::Temp Temp;
namespace FieaGameEnginetest
{
	TEST_CLASS(FieaGameEnginetest)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			Temp T1;
			Assert::IsTrue(T1.TestFunction());
		}
	};
}
