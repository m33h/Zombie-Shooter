#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <string>
#include <memory>
#include <vector>
#include <map>
#include <chrono>
#include <iostream>
#include <fstream>

#include <Urho3D/Urho3D.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/UI/UIElement.h>
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Urho3D.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Material.h>

class game_state;

class globals
{
public:
    Urho3D::Node* playerNode;
    Urho3D::Node* cameraNode;
    Urho3D::Camera* camera=0;
    Urho3D::Scene* scene=0;
    Urho3D::ResourceCache* cache=0;
    Urho3D::Context* context=0;
    Urho3D::UIElement* ui_root=0;
    Urho3D::Engine* engine=0;
    Urho3D::PhysicsWorld* physical_world=0;
    bool toggleMenu=false;
    int killedZombiesCount=0;
    std::vector<std::unique_ptr<game_state>> game_states;

    static globals* instance()
    {
        static globals g;
        return &g;
    }
};

class game_state : public Urho3D::Object
{
public:
    std::vector<Urho3D::Node*> nodes;
    std::vector<Urho3D::UIElement*> gui_elements;
    game_state(Urho3D::Context* context) : context_(context), Urho3D::Object(context) {}

    virtual ~game_state()
    {
        for(auto e:gui_elements)
            e->Remove();
        gui_elements.clear();
        for(auto n:nodes)
            n->Remove();
        nodes.clear();
    }

    URHO3D_OBJECT(game_state,Urho3D::Object)
private:
    Urho3D::Context* context_;

};

#endif // GAME_STATE_H
