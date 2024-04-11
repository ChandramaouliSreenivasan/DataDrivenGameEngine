/// <summary>
/// The declaration of the test parse handler and its wrapper.
/// </summary>

#pragma once
#include "ParseCoordinator.h"
#include "IParseHelper.h"
#include "IWrapper.h"
namespace Fiea::GameEngine
{
	class TestParseHandler final : public IParseHelper {
	public:
		class Wrapper : public IWrapper {
			RTTI_DECLARATIONS(TestParseHandler::Wrapper, IWrapper);

		public:
			// this is the "output" for this wrapper
			std::size_t maxDepth{ 0 };

			//bool Equals(const RTTI* rhs) const;
			bool Equals(const RTTI* rhs) const;
			std::string ToString() const;
		};

		void Initialize() override;
		void Cleanup() override;
		bool Start(const std::string& key, Json::Value& object, IWrapper* wrapper, bool isArray = false) override;
		bool End(Json::Value& object, IWrapper* wrapper, const std::string& key) override;

		bool initializeCalled{ false };
		bool cleanupCalled{ false };
		size_t startCount=0;
		size_t endCount=0;
	};
}

