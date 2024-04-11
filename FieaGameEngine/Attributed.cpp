#include "pch.h"
#include "Attributed.h"

namespace Fiea::GameEngine {
	RTTI_DEFINITIONS(Attributed);
	/// @return the Datum associated with the key
	/// @brief is there an attribute with the given name(prescribed or auxiliary) in this class?
	/// @param key 
	/// @return a boolean
	Datum& Attributed::AppendAuxiliaryAttribute(std::string Key)
	{
		bool flag = false;
		for (auto Sig : PrescribedAttributesVector) {
			if (Key == Sig.Name)
				throw std::logic_error("Attribute already Present");
		}
		for (auto a : AuxiliaryAttributes) {
			if (a == Key)
				flag = true;
		}
		if(!flag)
			AuxiliaryAttributes.push_back(Key);
		return Append(Key);
	}
	//copy constructor
	Attributed::Attributed(Attributed& other)
	{
		PrescribedAttributesVector = other.PrescribedAttributesVector;
		InitializePrescribedAttributes();
		RTTI* thisPtr = this;
		Append("Self").SetStorage<RTTI*>(&thisPtr, 1);
		AuxiliaryAttributes = other.AuxiliaryAttributes;
		for (auto a : AuxiliaryAttributes) {
			AppendAuxiliaryAttribute(a) = other.Append(a);
		}
	}
	Attributed::Attributed(Attributed&& other) noexcept
	{
		PrescribedAttributesVector = other.PrescribedAttributesVector;
		InitializePrescribedAttributes();
		RTTI* thisPtr = this;
		Append("Self").SetStorage<RTTI*>(&thisPtr, 1);
		AuxiliaryAttributes = other.AuxiliaryAttributes;
		for (auto a : AuxiliaryAttributes) {
			AppendAuxiliaryAttribute(a) = other.Append(a);
		}
	}
	Attributed& Attributed::operator=(Attributed& other)
	{
		if (this != &other) {
			if (!AuxiliaryAttributes.empty()) {
				for (auto x : AuxiliaryAttributes) {
					this->Erase(x);
				}
				AuxiliaryAttributes.clear();
			}
			for (auto x : other.AuxiliaryAttributes) {
				AppendAuxiliaryAttribute(x) = other.Append(x);
			}
		}
		return *this;
	}
	Attributed& Attributed::operator=(Attributed&& other) noexcept
	{
		if (this != &other) {
			if (!AuxiliaryAttributes.empty()) {
				for (auto x : AuxiliaryAttributes) {
					this->Erase(x);
				}
				AuxiliaryAttributes.clear();
			}
			for (auto x : other.AuxiliaryAttributes) {
				AppendAuxiliaryAttribute(x) = other.Append(x);
			}
		}
		return *this;
		return *this;
	}
	/// @brief Checks if the key already exists as an attribute, and if so, allows the pushing
	/// @param Key 

	Attributed::Attributed(size_t type)
	{
		PrescribedAttributesVector = TypeRegister::getSigs(type);
		InitializePrescribedAttributes();
		RTTI* thisPtr = this;
		Append("Self").SetStorage<RTTI*>(&thisPtr, 1);
	}

	bool Attributed::IsAttribute(std::string& key) const {
		return (IsPrescribedAttribute(key) || IsAuxiliaryAttribute(key));
	}
	/// @brief is there a prescribed attribute with the given name in this class?
	/// @param key 
	/// @return a boolean
	bool Attributed::IsPrescribedAttribute(std::string& key) const
	{
		for (auto sig : PrescribedAttributesVector) {
			if (key == sig.Name)
				return true;
		}
		return false;
	}
	/// @brief is there an auxiliary attribute with the given name in this class? 
	/// @param key 
	/// @return a boolean
	bool Attributed::IsAuxiliaryAttribute(std::string& key) const
	{
		for (auto name : AuxiliaryAttributes) {
			if (key == name)
				return true;
		}
		return false;
	}
	void Attributed::InitializePrescribedAttributes()
	{
		//self marker
		char* ch = (char*)this;
		for (Signature s : PrescribedAttributesVector) {
			switch (s.Type) {
			case Int:
				Append(s.Name).SetStorage<int>((int*)(ch + s.Offest), s.Size);
				break;
			case Float:
				Append(s.Name).SetStorage<float>((float*)(ch + s.Offest), s.Size);
				break;
			case String:
				Append(s.Name).SetStorage<std::string >((std::string*)(ch + s.Offest), s.Size);
				break;
			case Vec4:
				Append(s.Name).SetStorage<glm::vec4>((glm::vec4*)(ch + s.Offest), s.Size);
				break;
			case Mat4:
				Append(s.Name).SetStorage<glm::mat4x4>((glm::mat4x4*)(ch + s.Offest), s.Size);
				break;
			}
		}
	}
}
