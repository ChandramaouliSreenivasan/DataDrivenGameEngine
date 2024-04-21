#pragma once
#include "Attributed.h"
#include "IFactory.h"

using namespace Fiea::GameEngine;
typedef size_t GameTime;

class Action: public Attributed
{
	RTTI_DECLARATIONS(Action, Attributed);
	std::string Name;
public:

	Action(size_t typeinfo = TypeIdClass()):Attributed(typeinfo){}

	/// @brief Getter for the Name
	/// @return the name associated with this object
	const std::string& GetName() { return Name; }
	
	/// @brief Set Name
	/// @param Input the name to set
	void SetName(const std::string& Input);

	static std::vector<Signature> PrescribedAttributes() {
		return std::vector<Signature>{
			{"Name",DatumType::String,1,offsetof(Action,Name)}
		};
	}

	static void registerSigs() {
		TypeRegister::registerType(TypeIdClass(), PrescribedAttributes());
	}
	virtual void Update(GameTime t) = 0;
};

