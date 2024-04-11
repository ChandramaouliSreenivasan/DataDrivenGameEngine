#include "RTTI.h"
#include "Datum.h"
#include "Scope.h"
#include "Attributed.h"
#include "json/json.h"
#include "IWrapper.h"
#include "fstream"

namespace Fiea::GameEngine {

	class IParseHelper;


	class ParseCoordinator {
	public:
		
	private:
		IWrapper* AssociatedWrapper;
		std::vector<IParseHelper*> HelperList;
	public:
		ParseCoordinator(IWrapper* wrapper);
		~ParseCoordinator();

		//removing copy and move constructor
		ParseCoordinator(const ParseCoordinator& rhs) = delete;
		ParseCoordinator(const ParseCoordinator&& rhs) = delete;

		bool DeserializeObject(std::string json);

		bool DeserializeObjectFromFile(std::string filename);

		//getter method for associated wrapper
		IWrapper* GetWrapper() { return AssociatedWrapper; }
		void AddHelper(IParseHelper* HelperToAdd);
		void RemoveHelper(IParseHelper* HelperToRemove);

		//removing copy and move semantic assignment operator
		ParseCoordinator& operator=(const ParseCoordinator& rhs) = delete;
		ParseCoordinator& operator=(const ParseCoordinator&& rhs) = delete;

	private:
		bool ParseMembers(Json::Value& Object);
		bool Parse(const std::string& key, Json::Value& value, bool isArray);
	};
}