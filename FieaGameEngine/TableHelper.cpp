#include "pch.h"
#include "TableHelper.h"
#include <regex>
#include "IFactory.h"
using namespace Fiea::GameEngine;

RTTI_DEFINITIONS(TableHelper::TableWrapper);

bool TableHelper::TableWrapper::Equals(const RTTI* rhs) const
{
	if (rhs == nullptr) return false;

	const TableHelper::TableWrapper* other = rhs->As<TableHelper::TableWrapper>();
	if (other)
		return true;
	else
		return false;
}

std::string TableHelper::TableWrapper::ToString() const
{
    return std::string("TableHelper");
}

void TableHelper::Initialize()
{
	initializeCalled = true;
}

void TableHelper::Cleanup()
{
	cleanupCalled = true;
}

bool TableHelper::Start(const std::string& key, Json::Value& object, IWrapper* wrapper, bool isArray)
{
	TableHelper::TableWrapper* check = wrapper->As<TableHelper::TableWrapper>();
	if (!check) {
		return false;
	}
	
	if (object.isInt()) {
		if (check->Get()->Find(key)) {
			check->Get()->Append(key).SetItemWithValue<int>(object.asInt());
			return true;
		}
		check->Get()->Append(key).PushItem<int>(object.asInt());
	}
	else if (object.isString()) {
		if (check->Get()->Find(key)) {
			check->Get()->Append(key).SetItemWithValue<std::string>(object.asString());
			return true;
		}
		check->Get()->Append(key).PushItem<std::string>(object.asString());
	}
	else if (object.isDouble()) {
		if (check->Get()->Find(key)) {
			check->Get()->Append(key).SetItemWithValue<float>(object.asFloat());
			return true;
		}
		check->Get()->Append(key).PushItem<float>(float(object.asDouble()));
	}
	else if (isVector(object)) {
		if (check->Get()->Find(key)) {
			glm::vec4 pushVector = glm::vec4(object[0].asFloat(), object[1].asFloat(), object[2].asFloat(), object[3].asFloat());
			check->Get()->Append(key).SetItemWithValue<glm::vec4>(pushVector);
			return true;
		}
		glm::vec4 pushVector = glm::vec4(object[0].asFloat(), object[1].asFloat(), object[2].asFloat(), object[3].asFloat());
		check->Get()->Append(key).PushItem<glm::vec4>(pushVector);
	}
	else if (isMat4(object)) {
		if (check->Get()->Find(key)) {
			check->Get()->Append(key).SetItemWithValue<glm::mat4>(jsonValueToMat4(object));
			return true;
		}
		check->Get()->Append(key).PushItem<glm::mat4>(jsonValueToMat4(object));
	}
	else if (object.isObject()) {
		std::regex pattern("\\{(.*?)\\}");
		std::smatch matches;
		if (std::regex_search(key, matches, pattern)) {
			if (matches.size() == 2) {
				Scope* newptr = ScopeFactoryManager::Create(matches[1]);
				if (newptr) {
					check->Get()->Adopt(*newptr, matches.suffix());
					newptr->HeapAdoptAlloc();
					check->Set(newptr);
				}
			}
		}
		else
			check->Set(& check->Get()->AppendScope(key));
	}
	
	return true;
}

bool TableHelper::End(Json::Value& object,IWrapper* wrapper, const std::string& key)
{
	TableHelper::TableWrapper* check = wrapper->As<TableHelper::TableWrapper>();
	if (!check) {
		return false;
	}
	if (object.isObject()) {
		check->Set(check->Get()->GetParent());
			
	}
	return true;
}

bool TableHelper::isVector(Json::Value& jsonValue)
{
	if (!jsonValue.isArray() || jsonValue.size() != 4) {
		return false; // JSON value is not an array or doesn't have 4 elements
	}

	// Check if all elements are numerical values
	for (unsigned int i = 0; i < 4; ++i) {
		if (!jsonValue[i].isNumeric()) {
			return false;
		}
	}

	// JSON value has 4 numerical elements, assume it's a glm::vec4
	return true;
}

bool TableHelper::isMat4(Json::Value& jsonValue)
{
	if (!jsonValue.isArray() || jsonValue.size() != 4) {
		return false; // JSON value is not an array or doesn't have 4 elements (rows)
	}

	// Check if each row is an array of 4 numerical values
	for (unsigned int i = 0; i < 4; ++i) {
		if (!jsonValue[i].isArray() || jsonValue[i].size() != 4) {
			return false; // Row is not an array or doesn't have 4 elements (columns)
		}
		for (unsigned int j = 0; j < 4; ++j) {
			if (!jsonValue[i][j].isNumeric()) {
				return false; // Element is not a numerical value
			}
		}
	}

	// JSON value represents a glm::mat4
	return true;
}

glm::mat4 TableHelper::jsonValueToMat4(const Json::Value& jsonValue) {
	glm::mat4 mat4Value;
	for (unsigned int i = 0; i < 4; ++i) {
		for (unsigned int j = 0; j < 4; ++j) {
			mat4Value[i][j] = jsonValue[i][j].asFloat();
		}
	}
	return mat4Value;
}