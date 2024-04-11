#pragma once
#include "RTTI.h"
#include "Scope.h"

using namespace Fiea::GameEngine;
class IWrapper :public RTTI {
	RTTI_DECLARATIONS(IWrapper, RTTI);
private:
	size_t NestingDepth = 0;
public:
	IWrapper() = default;
	void IncrementDepth() { ++NestingDepth; };
	void DecrementDepth() { --NestingDepth; };
	size_t Depth() { return NestingDepth; }
	~IWrapper() = default;
};