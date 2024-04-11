#include "pch.h"
#include "AttributedArmoredCore.h"

namespace Fiea::GameEngine {
	RTTI_DEFINITIONS(AttributedArmoredCore);
	
	AttributedArmoredCore::~AttributedArmoredCore()
	{
		PrescribedAttributesVector.clear();
		AuxiliaryAttributes.clear();
	}
	bool AttributedArmoredCore::operator==(AttributedArmoredCore& other) 
	{
		return Scope::operator==((Scope&)other);
	}

	bool AttributedArmoredCore::operator!=(AttributedArmoredCore& other)
	{
		return !operator==(other);
	}

	
}