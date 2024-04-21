#include "pch.h"
#include "ActionIncrement.h"

RTTI_DEFINITIONS(ActionIncrement);

void ActionIncrement::Update(GameTime t)
{
	Scope* Parent = GetParent();
	Action* Parent_type = Parent->As<Action>();
	Datum* FoundDatum = Parent_type->Search(ActionTarget);
	assert(FoundDatum);
	FoundDatum->GetItemAsValue<int>(0) += AmountToIncrement;
}
