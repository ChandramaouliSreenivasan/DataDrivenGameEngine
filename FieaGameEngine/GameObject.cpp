#include "pch.h"
#include "GameObject.h"
using namespace Fiea::GameEngine;
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
    // TODO: insert return statement here
    return Transform.Scale;
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

void GameObject::Update(float GameTime)
{
    for (size_t i = 0; i < no_of_children; ++i) {
        Children[i]->Update(GameTime);
    }
}
