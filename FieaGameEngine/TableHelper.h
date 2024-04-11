#pragma once
#include "IParseHelper.h"
#include "IWrapper.h"
using namespace Fiea::GameEngine;
class TableHelper final :public IParseHelper
{
public:
	class TableWrapper : public IWrapper {
		RTTI_DECLARATIONS(TableHelper::TableWrapper, IWrapper);
		Scope* _internalScope;
		Scope* _currentscope;
	public:
		TableWrapper(Scope* passIn) : _internalScope(passIn),_currentscope(passIn) {};
		Scope* Get() { return _currentscope; }
		void Set(Scope* newScope) { _currentscope = newScope; };
		bool Equals(const RTTI* rhs) const;
		std::string ToString() const;

	};
	void Initialize() override;
	void Cleanup() override;
	bool Start(const std::string& key, Json::Value& object, IWrapper* wrapper, bool isArray = false) override;
	bool End(Json::Value& object,IWrapper* wrapper, const std::string& key) override;

	bool isVector(Json::Value& jsonValue);
	bool isMat4(Json::Value& jsonValue);
	glm::mat4 jsonValueToMat4(const Json::Value& jsonValue);
	bool initializeCalled{ false };
	bool cleanupCalled{ false };
};

