#pragma once
#include "Datum.h"
#include "Scope.h"
#include "Attributed.h"
#include "json/value.h"
#include"json/writer.h"
#include "IWrapper.h"
namespace Fiea::GameEngine {
	class IParseHelper
	{
	public:
		virtual void Initialize() = 0;
		virtual void Cleanup() = 0;
		virtual bool Start(const std::string& key, Json::Value& object, IWrapper* wrapper, bool isArray = false) = 0;
		virtual bool End(Json::Value& object, IWrapper* wrapper, const std::string& key) = 0;
	};

}