#pragma once
#include "Attributed.h"
#include "IFactory.h"
namespace Fiea::GameEngine {
	class AttributedArmoredCore : public Attributed
	{
		RTTI_DECLARATIONS(AttributedArmoredCore,Attributed);
	public:
		MAKE_FACTORY(AttributedArmoredCore);
		static std::vector<Signature> PrescribedAttributes() {
			return std::vector<Signature>{
				{"Name",DatumType::String,1,offsetof(AttributedArmoredCore,Name)},
				{"HP",DatumType::Int,1,offsetof(AttributedArmoredCore,Hp)},
				{"Attack",DatumType::Float,1,offsetof(AttributedArmoredCore,Attack)},
				{"Position",DatumType::Vec4,1,offsetof(AttributedArmoredCore,Position)},
				{"Rotation",DatumType::Mat4,1,offsetof(AttributedArmoredCore,Rotation)}
			};
		}
		static void registerSigs() { TypeRegister::registerType(TypeIdClass(), PrescribedAttributes()); }
		AttributedArmoredCore(size_t type = TypeIdClass()): Attributed(type) {}
		AttributedArmoredCore(AttributedArmoredCore& other) = default;
		AttributedArmoredCore(AttributedArmoredCore&& other) noexcept = default;
		~AttributedArmoredCore();

		AttributedArmoredCore& operator=(AttributedArmoredCore& other) = default;

		AttributedArmoredCore& operator=(AttributedArmoredCore&& other) noexcept = default;

		bool operator==(AttributedArmoredCore& other);
		bool operator!=(AttributedArmoredCore& other);
	private:
		std::string Name = "Main";
		int Hp = 100;
		float Attack = 3.5;
		glm::vec4 Position = glm::vec4(2);
		glm::mat4x4 Rotation = glm::mat4(3);
	};
}

