#include "pch.h"
#include "ActionList.h"

RTTI_DEFINITIONS(ActionList);

void ActionList::Update(GameTime time)
{
    Datum* D2 = Find("Actions");
    assert(D2 != nullptr);
    for (size_t i = 0; i < D2->GetSize(); ++i) {
        Scope* S = D2->GetItemAsValue<Scope*>(i);
        assert(S != nullptr);
        Action* ActionObj = static_cast<Action*>(S);
        assert(ActionObj != nullptr);
        ActionObj->Update(time);
    }
}
