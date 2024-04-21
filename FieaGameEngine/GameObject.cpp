#include "pch.h"
#include "GameObject.h"
namespace Fiea::GameEngine {
    RTTI_DEFINITIONS(GameObject);
    const glm::vec4& GameObject::GetRotation()
    {
        return Transform.Rotation;
    }

    const glm::vec4& GameObject::GetPosition()
    {
        return Transform.Position;
    }

    const glm::vec4& GameObject::GetScale()
    {
        return Transform.Scale;
    }

    void GameObject::CreateAction(std::string AcitonType, std::string ActionName)
    {
        Scope* CreatedAction = ScopeFactoryManager::Create(AcitonType);
        CreatedAction->Append("Name").SetItemWithValue<std::string>(ActionName);
        Adopt(*CreatedAction, "Actions");
        CreatedAction->HeapAdoptAlloc();
    }

    void GameObject::SetPosition(glm::vec4& NewPos)
    {
        Transform.Position = NewPos;
    }

    void GameObject::SetRotation(glm::vec4& NewRot)
    {
        Transform.Rotation = NewRot;
    }

    void GameObject::SetScale(glm::vec4& NewScale) {
        Transform.Scale = NewScale;
    }

    void GameObject::Update(const GameTime& time)
    {
        Datum* D = Find("children");
        assert(D != nullptr);
        for (size_t i = 0; i < D->GetSize(); ++i) {
            Scope* S = D->GetItemAsValue<Scope*>(i);
            assert(S != nullptr);
            GameObject* childObj = static_cast<GameObject*>(S);
            assert(childObj != nullptr);
            childObj->Update(time);
        }
        Datum* D2 = Find("Actions");
        assert(D2 != nullptr);
        for (size_t i = 0; i < D2->GetSize(); ++i) {
            Scope* S = D2->GetItemAsValue<Scope*>(i);
            assert(S != nullptr);
            Action* ActionObj = static_cast<Action*>(S);
            assert(ActionObj != nullptr);
            ActionObj->Update(time);
        }
        updateCalled = true;
    }

}