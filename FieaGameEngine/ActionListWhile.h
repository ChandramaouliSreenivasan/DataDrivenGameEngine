#pragma once
#include "ActionList.h"
#include "ActionIncrement.h"


using namespace Fiea::GameEngine;


class ActionListWhile: public ActionList
{
	RTTI_DECLARATIONS(ActionListWhile, ActionList);
	int condition;
public:
	ActionListWhile() :ActionList(TypeIdClass()) {};
	MAKE_FACTORY(ActionListWhile);
	static std::vector<Signature> Prescribed() {
		return std::vector<Signature>{
			{"Preamble", DatumType::Table,0,0},
			{ "Increment",DatumType::Table,0,0 },
			{ "condition",Int,1,offsetof(ActionListWhile,condition) }
		};
	}
	void Update(GameTime time) override;
};

