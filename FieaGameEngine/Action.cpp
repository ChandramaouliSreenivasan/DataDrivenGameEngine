#include "pch.h"
#include "Action.h"

using namespace Fiea::GameEngine;

RTTI_DEFINITIONS(Action);
void Action::SetName(const std::string& Input)
{
	Name = Input;
}
