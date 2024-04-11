#pragma once
#include "IParseHelper.h"
#include "IWrapper.h"
using namespace Fiea::GameEngine;
class IntParseHandler final :public IParseHelper
{
public:
	class IntWrapper : public IWrapper {
		RTTI_DECLARATIONS(IntParseHandler::IntWrapper, IWrapper);
		int* _value;
	public:
		IntWrapper(int* x) :_value(x) {};
		int& Get() { return *_value; }

		//bool Equals(const RTTI* rhs) const;
		bool Equals(const RTTI* rhs) const;
		std::string ToString() const;
	};
	void Initialize() override;
	void Cleanup() override;
	bool Start(const std::string& key, Json::Value& object, IWrapper* wrapper, bool isArray = false) override;
	bool End(Json::Value& object,IWrapper* wrapper, const std::string& key) override;

	bool initializeCalled{ false };
	bool cleanupCalled{ false };
};

