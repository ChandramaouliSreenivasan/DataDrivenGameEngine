#pragma once
#include "Signature.h"
#include <unordered_map>

using namespace Fiea::GameEngine;
class TypeRegister
{
public:
	TypeRegister() = default;
	
	TypeRegister(const TypeRegister& other) = delete;
	TypeRegister& operator=(const TypeRegister& other) = delete;

	static void registerType(const size_t& type, const std::vector<Signature>& sigs);
	static void registerChild(const size_t& type, const size_t& parent, const std::vector<Signature>& childsigs);
	static std::vector<Signature> getSigs(const size_t& type);
	static void clear();
private:
	static std::unordered_map<size_t, std::vector<Signature>>* typeRegistry;
};

