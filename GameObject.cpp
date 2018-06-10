#include <Urho3D/Urho3D.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Scene/Component.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/Scene/Serializable.h>
#include <Urho3D/IO/Deserializer.h>
#include <Urho3D/Container/Str.h>
#include <Urho3D/Graphics/ParticleEffect.h>

#include "GameObject.h"

void GameObject::RegisterObject(Context* context)
{
    context->RegisterFactory<GameObject>();
}

GameObject::GameObject(Context* context) : LogicComponent(context), context_(context) {}
GameObject::~GameObject() {}
void GameObject::FixedUpdate(float step) {}