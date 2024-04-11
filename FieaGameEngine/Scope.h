#pragma once
#include "RTTI.h"
#include "Datum.h"
#include <unordered_map>
#include <vector>
#include <functional>
#include "framework.h"

namespace Fiea::GameEngine {
	class Scope :public RTTI {
		RTTI_DECLARATIONS(Scope,RTTI);
	private:
		std::unordered_map<std::string, Datum> mData;
		std::vector<std::pair<std::string, Datum*>> orderingVector;
		Scope* mParent = nullptr;
		bool AdoptedNotCreated = false;
	public:
		Scope(size_t beginningcapacity = 0);
		void Erase(std::string key);
		~Scope();
		Scope(const Scope& other);
		Scope(Scope&& other) noexcept;
		Datum& Append(const std::string item);
		Scope& AppendScope(const std::string item);
		Scope* GetParent() { return mParent; };
		Datum* Find(const std::string key);
		const Datum* Find(const std::string key) const;
		Datum* Search(const std::string key, Scope** ContainingScope = nullptr);
		const Datum* Search(const std::string key,  const Scope** ContainingScope = nullptr) const;
		bool IsAncestorOf(Scope* TestChild);
		bool IsDescendantOf(Scope* TestAncestor);
		void Orphan(Scope* ChildToOrphan);
		Datum* FindContainedScope(const Scope* ScopeToFind, size_t& idx);
		bool operator==(Scope& other);
		bool operator!=(Scope& other);
		Scope& operator=(Scope& other);
		Scope& operator=(Scope&& other) noexcept;
		Datum& operator[](const std::string key);
		Datum& operator[](size_t idx);
		void Adopt(Scope& ChildToAdopt, std::string key);
		size_t Size() { return orderingVector.size(); }
		size_t Capacity() { return orderingVector.capacity(); }
		void HeapAdoptAlloc() { AdoptedNotCreated = false; }
	};
}