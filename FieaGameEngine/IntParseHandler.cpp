#include "pch.h"
#include "IntParseHandler.h"

RTTI_DEFINITIONS(IntParseHandler::IntWrapper);

bool IntParseHandler::IntWrapper::Equals(const RTTI* rhs) const
{
	if (rhs == nullptr) return false;

	const IntParseHandler::IntWrapper* other = rhs->As<IntParseHandler::IntWrapper>();
	if (other)
		return true;
	else
		return false;
}

std::string IntParseHandler::IntWrapper::ToString() const
{
    return "IntParseHandler";
}

void IntParseHandler::Initialize()
{
	initializeCalled = true;
}

void IntParseHandler::Cleanup()
{
	cleanupCalled = true;
}

bool IntParseHandler::Start(const std::string& key, Json::Value& object, IWrapper* wrapper, bool isArray)
{
	IntParseHandler::IntWrapper* check = wrapper->As<IntParseHandler::IntWrapper>();
	if (check == nullptr || isArray) {
		return false;
	}
	check->Get() = object.asInt();
	return true;
}

bool IntParseHandler::End(Json::Value& object,IWrapper* wrapper, const std::string& key)
{
	IntParseHandler::IntWrapper* check = wrapper->As<IntParseHandler::IntWrapper>();
	if (check == nullptr) {
		return false;
	}
	return true;
}
