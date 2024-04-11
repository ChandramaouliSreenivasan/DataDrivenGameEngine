#pragma once
#include "Scope.h"
#include "TypeRegister.h"
namespace Fiea::GameEngine 
{
	class Attributed : public Scope
	{
		RTTI_DECLARATIONS(Attributed, Scope);
	public:
		Attributed(size_t type = TypeIdClass());
		void InitializePrescribedAttributes();
		virtual ~Attributed() = default;
		bool IsAttribute(std::string& key) const ;
		bool IsPrescribedAttribute(std::string& key) const;
		bool IsAuxiliaryAttribute(std::string& key) const;
		Datum& AppendAuxiliaryAttribute(std::string Key);
		Attributed(Attributed& other);
		Attributed(Attributed&& other) noexcept;
		Attributed& operator=(Attributed& other);
		Attributed& operator=(Attributed&& other) noexcept;
	protected:
		std::vector<std::string> AuxiliaryAttributes;
		std::vector<Signature> PrescribedAttributesVector;
	};
}

