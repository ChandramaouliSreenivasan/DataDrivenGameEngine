#include "pch.h"
#include "ActionListWhile.h"

RTTI_DEFINITIONS(ActionListWhile);

void ActionListWhile::Update(GameTime time)
{
	Action* Preamble =static_cast<Action*>( Find("Preamble")->GetItemAsValue<Scope*>(0));
	Action* Increment = static_cast<Action*>(Find("Increment")->GetItemAsValue<Scope*>(0));
	Preamble->Update(time);
	Datum* D2 = Find("Actions");
	assert(D2 != nullptr);
	while (condition) {
		for (size_t i = 0; i < D2->GetSize();++i) 
		{
			Scope* childAct = D2->GetItemAsValue<Scope*>(i);
			assert(childAct);
			Action* Act = static_cast<Action*>(childAct);
			Act->Update(time);
		}
		Increment->Update(condition);
	}
}
