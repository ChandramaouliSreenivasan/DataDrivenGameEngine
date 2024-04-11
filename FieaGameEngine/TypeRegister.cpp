#include "pch.h"
#include "TypeRegister.h"

std::unordered_map<size_t, std::vector<Signature>>* TypeRegister::typeRegistry;

void TypeRegister::registerType(const size_t& type, const std::vector<Signature>& sigs)
{
	if (typeRegistry == nullptr)
		typeRegistry = new std::unordered_map<size_t, std::vector<Signature>>;
	typeRegistry->emplace(std::pair<size_t, std::vector<Signature>>{type, sigs});
}

void TypeRegister::registerChild(const size_t& type, const size_t& parent, const std::vector<Signature>& childsigs)
{
	auto found = typeRegistry->find(parent);
	if (found == typeRegistry->end()) {
		throw std::exception("parent not registered");
	}
	std::vector<Signature> Unified = typeRegistry->at(parent);
	Unified.reserve(Unified.size() + childsigs.size());
	Unified.insert(Unified.end(), childsigs.begin(), childsigs.end());
	typeRegistry->emplace(std::pair<size_t, std::vector<Signature>>{type, Unified});

}

std::vector<Signature> TypeRegister::getSigs(const size_t& type)
{
	auto found = typeRegistry->find(type);
	if (found == typeRegistry->end()) {
		throw std::exception("parent not registered");
	}
	return typeRegistry->at(type);
}

void TypeRegister::clear()
{
	if (typeRegistry) {
		typeRegistry->clear();
		delete typeRegistry;
	}
}
