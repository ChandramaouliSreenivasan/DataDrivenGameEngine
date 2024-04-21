#pragma once
#include "Action.h"
using namespace Fiea::GameEngine;
class ActionList:public Action
{
	RTTI_DECLARATIONS(ActionList, Action);
public:
	ActionList(size_t id = TypeIdClass()): Action(id){}
	MAKE_FACTORY(ActionList);
	static std::vector<Signature> PrescribedSignatures() {
		return std::vector<Signature>{
			{"Actions", DatumType::Table, 0, 0}
		};
	}
	virtual void Update(GameTime time) override;
};

