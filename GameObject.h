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
public:
    GameObject(Context* context);
    virtual ~GameObject();
    virtual void FixedUpdate(float timeStep);
};

#endif // GAMEOBJECT_H
