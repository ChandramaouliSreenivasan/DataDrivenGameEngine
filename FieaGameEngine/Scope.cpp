#include "pch.h"
#include "Scope.h"

using namespace Fiea::GameEngine;

RTTI_DEFINITIONS(Scope);
//takes a constant string and returns a reference to a Datum.
Datum& Scope::Append(const std::string item) {
	auto found = mData.find(item);
	if (found == mData.end()) {
		orderingVector.push_back({ item,&mData[item] });
		return mData[item];
	}
	return found->second;
}
//takes a constant string and returns a reference to Scope. This should return a reference to a Scope with the associated name
Fiea::GameEngine::Scope& Scope::AppendScope(const std::string item) {
	auto found = mData.find(item);
	if (found == mData.end()) {
		Scope* newScope = new Scope;
		newScope->mParent = this;
		mData[item].PushItem(newScope);
		orderingVector.push_back({ item,&mData[item] });
		return *newScope;
	}
	if (mData[item].GetType() != Table && mData[item].GetType() != Unknown)
		throw std::logic_error("Type Mismatch");
	Scope* newScope = new Scope;
	newScope->mParent = this;
	mData[item].PushItem(newScope);
	return *newScope;
}
//Default constructor constructing with empty, allocated storage.
Scope::Scope(size_t i) {
	orderingVector.reserve(i);
	mData.reserve(i);
}
void Fiea::GameEngine::Scope::Erase(std::string key)
{
	if (mData.find(key) != mData.end()) {
		auto it = std::find(orderingVector.begin(), orderingVector.end(), std::pair<std::string, Datum*>{key,&Append(key)});
		orderingVector.erase(it);
		mData.erase(key);
	}
}
//destructor
Scope::~Scope() {
	if (mParent != nullptr)
	{
		mParent->Orphan(this);
	}
	//destruct my child datums
	for (auto& [key, datum] : mData)
	{
		if (datum._datatype == DatumType::Table)
		{
			while(datum.Size>0)
			{
				Scope* scope = datum.GetItemAsValue<Scope*>(datum.Size-1);
				scope->mParent = nullptr;
				if (scope) {
					if (!scope->AdoptedNotCreated)
					{
						//scope->~Scope();
						delete(scope);

					}
				}
				datum.RemoveAt(datum.Size-1);
			}
		}
	}
}

//copy constructor
Fiea::GameEngine::Scope::Scope(const Scope& other)
{
	mParent = other.mParent;
	for (auto& [key, value] : other.orderingVector) {
		if (value->_datatype == Table) {
			for (size_t i = 0; i < value->Size; ++i) {
				Scope scopeCpy(*value->GetItemAsValue<Scope*>(i));
				scopeCpy.mParent = this;
				mData[key].PushItem(new Scope(scopeCpy));
				orderingVector.push_back({ key,&mData[key] });
			}
			continue;
		}
		Datum datCopy(*value);
		mData.insert({ key,datCopy });
		orderingVector.push_back({ key,&mData[key] });
		
	}
}

//move contsructor
 Fiea::GameEngine::Scope::Scope( Scope&& other) noexcept
{
	mParent = other.mParent;
	for (auto& [key, value] : other.orderingVector) {
		if (value->_datatype == Table) {
			for (size_t i = 0; i < value->Size; ++i) {
				Scope scopeCpy(*value->GetItemAsValue<Scope*>(i));
				scopeCpy.mParent = this;
				mData[key].PushItem(new Scope(scopeCpy));
				orderingVector.push_back({ key,&mData[key] });
			}
			continue;
		}
		Datum datCopy(std::move(*value));
		mData.insert({ key,datCopy });
		orderingVector.push_back({ key,&mData[key] });

	}
}


// takes a constant string and returns the address of a Datum. This should return the address of the Datum associated with the given name in this Scope, if it exists, and nullptr otherwise.
Datum* Fiea::GameEngine::Scope::Find(const std::string key)
{
	auto found = mData.find(key);
	if (found == mData.end())
		return nullptr;
	return &mData[key];
}
// takes a constant string and returns the address of a Datum. This should return the address of the Datum associated with the given name in this Scope, if it exists, and nullptr otherwise.
const Datum* Fiea::GameEngine::Scope::Find(const std::string key) const
{
	auto found = mData.find(key);
	if (found == mData.end())
		return nullptr;
	return &(found->second);
}
//takes a constant string and the address of a Scope double pointer variable (whose default value could be nullptr), and which returns the address of a Datum which coreresponds to the key
Datum* Fiea::GameEngine::Scope::Search(const std::string key, Scope** ContainingScope)
{
	Datum* retVal = Find(key);
	if (retVal == nullptr) {
		if (mParent) {
			return mParent->Search(key, ContainingScope);
		}
		return retVal;
	}
	if (ContainingScope)
		*ContainingScope = this;
	return retVal;
}
//const-correct search
const Datum* Fiea::GameEngine::Scope::Search(const std::string key, const Scope** ContainingScope) const
{
	const Datum* retVal = Find(key);
	if (retVal == nullptr) {
		if (mParent) {
			return mParent->Search(key, ContainingScope);
		}
		return retVal;
	}
	if (ContainingScope)
		*ContainingScope = this;
	return retVal;
}
//tests if the provided object is an ancestor of the calling object
bool Fiea::GameEngine::Scope::IsDescendantOf(Scope* TestParent) {
	if (mParent == nullptr) {
		return false;
	}
	if (mParent != TestParent) {
		return mParent->IsDescendantOf(TestParent);
	}
	if (mParent == TestParent) {
		return true;
	}
	return false;
}
//remove a child from records
void Fiea::GameEngine::Scope::Orphan(Scope* ChildToOrphan)
{
	size_t i = 0;
	Datum* D = FindContainedScope(ChildToOrphan, i);
	if(D)
		D->RemoveAt(i);
}
//Find and return the datum which contains the given child scope.
//@param ScopeToFind pointer to the scope we need to find
//@param idx output param, location of the found scope in the datum which contains it
Datum* Fiea::GameEngine::Scope::FindContainedScope(const Scope* ScopeToFind, size_t& idx)
{
	for (auto& [key, datum] : mData) {
		if (datum.GetType() == Table) {
			for (size_t i = 0; i < datum.GetSize(); ++i) {
				if (datum.GetItemAsValue<Scope*>(i) == ScopeToFind) {
					idx = i;
					return &datum;
				}
			}
		}
	}
	return nullptr;
}
//comparison operator
bool Fiea::GameEngine::Scope::operator==(Scope& other)
{
	if (mData.size() != other.mData.size())
		return false;
	for (size_t i = 0; i < orderingVector.size(); ++i) {
		if (orderingVector[i].first != other.orderingVector[i].first)
			return false;
		if (*orderingVector[i].second != *other.orderingVector[i].second)
			return false;
	}
	return true;
}
//inequality operator
bool Fiea::GameEngine::Scope::operator!=(Scope& other)
{
	return !(*this == other);
}
//assignment operator
Scope& Fiea::GameEngine::Scope::operator=(Scope& other)
{
	if (mParent != nullptr)
	{
		mParent->Orphan(this);
	}
	for (auto& [key, datum] : mData)
	{
		if (datum._datatype == DatumType::Table)
		{
			for (size_t i = 0; i < datum.Size; ++i)
			{
				Scope* scope = datum.GetItemAsValue<Scope*>(i);
				scope->mParent = nullptr;
				delete(scope);
				datum.RemoveAt(i);
			}
		}
	}
	orderingVector.clear();
	mData.clear();
	mParent = other.mParent;
	for (auto& [key, value] : other.orderingVector) {
		if (value->_datatype == Table) {
			for (size_t i = 0; i < value->Size; ++i) {
				Scope scopeCpy(*value->GetItemAsValue<Scope*>(i));
				scopeCpy.mParent = this;
				mData[key].PushItem(new Scope(scopeCpy));
				orderingVector.push_back({ key,&mData[key] });
			}
			continue;
		}
		Datum datCopy(*value);
		mData.insert({ key,datCopy });
		orderingVector.push_back({ key,&mData[key] });

	}
	return *this;
}
//dereference operator taking string
Datum& Fiea::GameEngine::Scope::operator[](const std::string key)
{
	// TODO: insert return statement here
	return Append(key);
}
//dereference operator taking index
Datum& Fiea::GameEngine::Scope::operator[](size_t idx)
{
	// TODO: insert return statement here
	if (idx >= orderingVector.size() || idx < 0)
		throw std::out_of_range("Out of Range");
	return *orderingVector.at(idx).second;
}
//Adopt a scope 
void Fiea::GameEngine::Scope::Adopt(Scope& ChildToAdopt, std::string key)
{
	if (ChildToAdopt.mParent) {
		ChildToAdopt.mParent->Orphan(&ChildToAdopt);
		ChildToAdopt.mParent = this;
		ChildToAdopt.AdoptedNotCreated = true;
		mData[key].PushItem(&ChildToAdopt);
		return;
	}
	ChildToAdopt.mParent = this;
	ChildToAdopt.AdoptedNotCreated = true;
	Datum& d = Append(key);
	d.PushItem(&ChildToAdopt);
}


//checks if the passed object is a descendant of the calling object, logical reverse of the IsDescendantOf function
bool Fiea::GameEngine::Scope::IsAncestorOf(Scope* TestDescendant) {
	return TestDescendant->IsDescendantOf(this);
}

Scope& Fiea::GameEngine::Scope::operator=(Scope&& other) noexcept{
	if (this == &other) {
		return *this;
	}
	if (mParent != nullptr)
	{
		mParent->Orphan(this);
	}
	for (auto& [key, datum] : mData)
	{
		if (datum._datatype == DatumType::Table)
		{
			for (size_t i = 0; i < datum.Size; ++i)
			{
				Scope* scope = datum.GetItemAsValue<Scope*>(i);
				scope->mParent = nullptr;
				delete(scope);
				datum.RemoveAt(i);
			}
		}
	}
	orderingVector.clear();
	mData.clear();
	mParent = other.mParent;
	for (auto& [key, value] : other.orderingVector) {
		if (value->_datatype == Table) {
			for (size_t i = 0; i < value->Size; ++i) {
				Scope scopeCpy(*value->GetItemAsValue<Scope*>(i));
				scopeCpy.mParent = this;
				mData[key].PushItem(new Scope(scopeCpy));
				orderingVector.push_back({ key,&mData[key] });
			}
			continue;
		}
		Datum datMove(std::move( * value));
		mData.insert({ key,datMove });
		orderingVector.push_back({ key,&mData[key] });

	}
	return *this;
}
