#include "pch.h"
#include "IFactory.h"

ScopeFactory* ScopeFactoryManager::Find(const std::string& className)
{
    if (_factoryList == nullptr)
        return nullptr;
    auto f = _factoryList->find(className);
    if (f == _factoryList->end()) {
        return nullptr;
    }
    return f->second;
}

Scope* ScopeFactoryManager::Create(const std::string& className)
{
    if (_factoryList == nullptr)
        return nullptr;
    auto f = _factoryList->find(className);
    if (f == _factoryList->end()) {
        return nullptr;
    }
    return f->second->Create();
}

void ScopeFactoryManager::Add(ScopeFactory& concreteFactory)
{
    if (_factoryList == nullptr) {
        _factoryList = new std::unordered_map<std::string, ScopeFactory*>();
    }
    _factoryList->emplace(std::pair<std::string, ScopeFactory*>{concreteFactory.ClassName(), & concreteFactory});
}

void ScopeFactoryManager::cleanup()
{
    if (_factoryList) {
        _factoryList->clear();
        delete(_factoryList);
    }
}
