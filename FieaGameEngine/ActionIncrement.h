#pragma once
#include "Action.h"
#include "GameObject.h"


typedef size_t GameTime;
class ActionIncrement : public Action
{
	RTTI_DECLARATIONS(ActionIncrement, Action);
	std::string ActionTarget;
	int AmountToIncrement;
public:
	MAKE_FACTORY(ActionIncrement);
	ActionIncrement() : Action(TypeIdClass()){};
	static std::vector<Signature> PrescribedAttributes() {
		return std::vector<Signature>{
			{"Target", String, 1, offsetof(ActionIncrement, ActionTarget)},
			{"IncrementAmount",Int,1,offsetof(ActionIncrement,AmountToIncrement)},
		};
	}
	void Update(GameTime t) override;
};

