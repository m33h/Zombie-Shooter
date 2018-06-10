#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#pragma once

#include  <Urho3D/Scene/LogicComponent.h>

namespace Urho3D
{
class Scene;
}

using namespace Urho3D;

class GameObject : public LogicComponent
{
    URHO3D_OBJECT(GameObject, LogicComponent);
    SharedPtr<Scene> scene_;
public:
    GameObject(Context* context);
    GameObject(Context* context, SharedPtr<Scene> scene);
    Context* context_;
    virtual ~GameObject();
    static void RegisterObject(Context* context);
    virtual void FixedUpdate(float timeStep);
};

#endif // GAMEOBJECT_H
