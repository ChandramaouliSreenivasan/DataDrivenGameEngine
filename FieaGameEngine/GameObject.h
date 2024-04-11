#pragma once
#include "Attributed.h"
#include "IFactory.h"
#include <stddef.h>
using namespace Fiea::GameEngine;

struct TransformStruct {
	glm::vec4 Position;
	glm::vec4 Rotation;
	glm::vec4 Scale;
};

class GameObject : public Attributed
{
	RTTI_DECLARATIONS(GameObject, Attributed);
	std::string Name;
	TransformStruct Transform;
	GameObject** Children;
	size_t no_of_children = 0;
public:
	MAKE_FACTORY(GameObject);
	static std::vector<Signature> PrescribedAttributes() {
		return std::vector<Signature>{
			{"Name",DatumType::String,1,offsetof(GameObject,Name)},
			{"Position",DatumType::Vec4,offsetof(GameObject,Transform.Position)},
			{"Rotation",DatumType::Vec4,offsetof(GameObject,Transform.Rotation)},
			{"Scale",DatumType::Vec4,offsetof(GameObject,Transform.Scale)},
			{"ChildrenArray",DatumType::Pointer,offsetof(GameObject,Children)}
		};
	}
	static void registerSigs() {
		TypeRegister::registerType(TypeIdClass(), PrescribedAttributes());
	}
	GameObject() :Attributed(TypeIdClass()){}

#pragma region Getters and Setters
	const glm::vec4& GetRotation();
	const glm::vec4& GetPosition();
	const glm::vec4& GetScale();
	const std::string& GetName() { return Name; };
	void SetName(std::string& _Name) { Name = _Name; }
	void SetPosition(glm::vec4& NewPos);
	void SetRotation(glm::vec4& NewRot);
	void SetScale(glm::vec4& NewScale);
#pragma endregion Getters and Setters

	void Update(float GameTime);

};

