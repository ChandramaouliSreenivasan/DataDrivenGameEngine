#include "pch.h"
#include "TestAction.h"
RTTI_DEFINITIONS(TestAction);
void TestAction::Update(GameTime time)
{
	UpdateActivated = true;
}
