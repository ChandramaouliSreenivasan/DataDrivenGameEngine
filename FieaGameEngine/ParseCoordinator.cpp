#include "pch.h"
#include "IParseHelper.h"
#include "ParseCoordinator.h"
#include "iostream"
#include <Windows.h>

using namespace Fiea::GameEngine;



Fiea::GameEngine::ParseCoordinator::ParseCoordinator(IWrapper* wrapper)
{
	AssociatedWrapper = wrapper;
}

Fiea::GameEngine::ParseCoordinator::~ParseCoordinator()
{
	AssociatedWrapper = nullptr;
	HelperList.clear();
}

bool Fiea::GameEngine::ParseCoordinator::DeserializeObject(std::string json)
{
	bool flag = false;
	std::istringstream jsonStream(json);
	Json::CharReaderBuilder Builder;
	Json::String Errs;
	Json::Value root;
	bool parsetojson = Json::parseFromStream(Builder, jsonStream, &root,&Errs);
	if (!parsetojson) {
		throw std::exception("Json is not properly parsed");
	}
	flag = ParseMembers(root);
	return flag;
}

bool Fiea::GameEngine::ParseCoordinator::DeserializeObjectFromFile(std::string filename)
{
	bool flag = false;
	std::ifstream file(filename);
	if (!file.is_open()) {
		return false;
	}
	std::string jsonData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	std::istringstream jsonStream(jsonData);
	Json::CharReaderBuilder Builder;
	Json::String Errs;
	Json::Value root;
	bool parsetojson = Json::parseFromStream(Builder, jsonStream, &root, &Errs);
	if (!parsetojson) {
		throw std::exception("Json is not properly parsed");
	}
	flag = ParseMembers(root);
	return flag;

}

void Fiea::GameEngine::ParseCoordinator::AddHelper(IParseHelper* HelperToAdd)
{
	HelperList.push_back(HelperToAdd);
}

void Fiea::GameEngine::ParseCoordinator::RemoveHelper(IParseHelper* HelperToRemove)
{
	if (HelperList.empty()) {
		throw std::out_of_range("Cannot delete from empty");
	}
	HelperList.erase(std::remove(HelperList.begin(), HelperList.end(), HelperToRemove), HelperList.end());
}

bool Fiea::GameEngine::ParseCoordinator::ParseMembers(Json::Value& Object)
{
	bool flag = false;
	AssociatedWrapper->IncrementDepth();
	for (auto it = Object.begin(); it != Object.end(); ++it) {
		flag = Parse(it.key().asString(), *it, (*it).isArray());
		if (flag == false) {
			return false;
		}
	}
	AssociatedWrapper->DecrementDepth();
	return true;
}

bool Fiea::GameEngine::ParseCoordinator::Parse(const std::string& key, Json::Value& value, bool isArray)
{
	bool flag = false;
	IParseHelper* h1 = nullptr;
	if (value.isObject()) {
		for (auto h : HelperList) {
			if (h->Start(key, value, AssociatedWrapper, true)) {
				flag = true;
				h1 = h;
				break;
				
			}
		}
		if (flag == false) {
			return false;
		}
		flag = ParseMembers(value);
		h1->End(value,AssociatedWrapper, key);
	}
	else if (value.isArray()) {
		for (unsigned int i = 0; i < value.size(); i++) {
			if (value[i].isObject()) {
				for (auto h : HelperList) {
					if (h->Start(key, value[i], AssociatedWrapper, true)) {
						flag = true;
						h->End(value,AssociatedWrapper, key);
					}
				}
				return ParseMembers(value);
			}
		}
		for (auto h : HelperList) {
			if (h->Start( key, value, AssociatedWrapper, true)) {
				flag = true;
				h->End(value,AssociatedWrapper, key);
			}
		}
	}
	else {
		for (auto h : HelperList) {
			if (h->Start( key, value, AssociatedWrapper, false)) {
				flag = true;
				h->End(value,AssociatedWrapper, key);
			}
		}
	}
	return flag;
}

