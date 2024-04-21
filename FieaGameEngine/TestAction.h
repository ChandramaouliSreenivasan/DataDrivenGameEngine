#pragma once
#include "Action.h"
#include "GameObject.h"
using namespace Fiea::GameEngine;

class TestAction:public Action
{
	RTTI_DECLARATIONS(TestAction, Action);
public:
	MAKE_FACTORY(TestAction);
	bool UpdateActivated;
	void Update(GameTime time) override;
	TestAction(): Action(){}
};

