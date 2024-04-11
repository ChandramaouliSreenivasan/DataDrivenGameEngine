#pragma once
#include "Scope.h"
using namespace Fiea::GameEngine;

class ScopeFactory
{
public:
    ScopeFactory() = default;

    virtual Scope* Create() const  = 0;
    virtual std::string ClassName() const =0;
};

class ScopeFactoryManager
{
private:
	inline static std::unordered_map<std::string, ScopeFactory*>* _factoryList = nullptr;
public:
    /// @brief Returns the found ScopeFactory in the manager map which matches the key
    /// @param className 
    /// @return 
    static ScopeFactory* Find(const std::string& className);


    /// @brief Constructs and returns a scope of the specific type matching the factory to the key
    /// @param className 
    /// @return 
    [[nodiscard]] static Scope* Create(const std::string& className);


    /// @brief adds a factory to the list
    /// @param concreteFactory 
    static void Add(ScopeFactory& concreteFactory);

    static void Remove(ScopeFactory& concreteFactory);

    /// @brief clears and deallocates the manager map
    static void cleanup();




};


#define MAKE_FACTORY(_Concrete)                              \
    class _Concrete##Factory: public ScopeFactory             \
    {                           \
        public:\
            Scope* Create() const override{ \
                return new _Concrete(); \
            } \
            std::string ClassName()const override{ \
                return #_Concrete; \
            } };

    
