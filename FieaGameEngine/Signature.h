#pragma once
#include "Datum.h"
namespace Fiea::GameEngine {
	class Signature
	{
	public:
		std::string Name;
		DatumType Type = DatumType::Unknown;
		size_t Size = 0;
		size_t Offest = 0;
	};
}

