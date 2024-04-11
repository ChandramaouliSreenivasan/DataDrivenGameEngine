#pragma once
#include "AttributedArmoredCore.h"
#include "TypeRegister.h"

using namespace Fiea::GameEngine;

class AttributedFoo : public AttributedArmoredCore
{
	RTTI_DECLARATIONS(AttributedFoo, AttributedArmoredCore);
public:

	static std::vector<Signature> Prescribed() {
		return std::vector<Signature>{
			{"Foo",DatumType::Int,1,offsetof(AttributedFoo,FooPrescribed)}
		};
	}

private:
	int FooPrescribed = 5;
public:
	AttributedFoo() :AttributedArmoredCore(TypeIdClass()) {};
	AttributedFoo(AttributedFoo& other) = default;
	AttributedFoo(AttributedFoo&& other) = default;
	AttributedFoo& operator=(AttributedFoo& rhs) = default;
	AttributedFoo& operator=(AttributedFoo&& rhs) = default;
	~AttributedFoo() = default;
};

