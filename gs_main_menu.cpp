#include <string>
#include <sstream>

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Geometry.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Skybox.h>
#include <Urho3D/Graphics/AnimatedModel.h>
#include <Urho3D/Graphics/Animation.h>
#include <Urho3D/Graphics/AnimationState.h>
#include <Urho3D/Physics/PhysicsEvents.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Physics/Constraint.h>
#include <Urho3D/Navigation/Navigable.h>
#include <Urho3D/Navigation/NavigationMesh.h>
#include <Urho3D/Navigation/CrowdAgent.h>
#include <Urho3D/Graphics/AnimationController.h>
#include <Urho3D/Navigation/NavigationEvents.h>
#include <Urho3D/Navigation/Obstacle.h>
#include <Urho3D/Navigation/OffMeshConnection.h>
#include "Urho3D/IO/Log.h"
#include <Urho3D/Scene/Node.h>

#include <stdio.h>
#include "gs_main_menu.h"
#include "gs_playing.h"

using namespace Urho3D;
using namespace std;

gs_main_menu::gs_main_menu(Scene *scene, Context* context, ResourceCache* cache, Node* cameraNode, Node* playerNode, int state) :
        scene_(scene),
        context_(context),
        cache_(cache),
        cameraNode_(cameraNode),
        playerNode_(playerNode),
        game_state(context),
        state(state)
{
    window_menu=new Window(context_);
    gui_elements.push_back(window_menu);
    globals::instance()->ui_root->AddChild(window_menu);

    window_menu->SetSize(300,200);
    window_menu->SetLayout(LM_FREE,0,IntRect(10,10,10,10));
    window_menu->SetAlignment(HA_CENTER,VA_CENTER);
    window_menu->SetName("Window");
    window_menu->SetColor(Color(.0,.15,.3,.5));
    window_menu->SetStyleAuto();
    GetSubsystem<Input>()->SetMouseGrabbed(false);
    GetSubsystem<Input>()->SetMouseVisible(true);

    SubscribeToEvent(E_UPDATE,URHO3D_HANDLER(gs_main_menu,update));
    SubscribeToEvent(E_KEYDOWN,URHO3D_HANDLER(gs_main_menu,HandleKeyDown));

    if(state == GAME_START) {
        {
            Button* button=new Button(context_);
            button->SetPosition(10,10);
            button->SetFixedSize(270,80);
            button->SetName("Button");
            button->SetStyleAuto();
            button->SetOpacity(0.75);
            {
                Text* t=new Text(context_);
                t->SetFont(cache_->GetResource<Font>("assets/fonts/Anonymous Pro.ttf"),20);
                t->SetHorizontalAlignment(HA_CENTER);
                t->SetVerticalAlignment(VA_CENTER);
                t->SetName("Text");
                t->SetText("Start new game");
                button->AddChild(t);
            }
            window_menu->AddChild(button);
            SubscribeToEvent(button,E_RELEASED,URHO3D_HANDLER(gs_main_menu,HandleNewGamePressed));
        }
    }

    else if(state == GAME_RESTART) {
        {
            Button* button=new Button(context_);
            button->SetPosition(10,10);
            button->SetFixedSize(270,80);
            button->SetName("Button");
            button->SetStyleAuto();
            button->SetOpacity(0.75);
            {
                Text* t=new Text(context_);
                t->SetFont(cache_->GetResource<Font>("assets/fonts/Anonymous Pro.ttf"),20);
                t->SetHorizontalAlignment(HA_CENTER);
                t->SetVerticalAlignment(VA_CENTER);
                t->SetName("Text");
                t->SetText("Resume");
                button->AddChild(t);
            }
            window_menu->AddChild(button);
            SubscribeToEvent(button,E_RELEASED,URHO3D_HANDLER(gs_main_menu,HandleResumePressed));
        }
    }

    else if(state == GAME_FINISHED) {
        {
            Button* button=new Button(context_);
            button->SetPosition(10,10);
            button->SetFixedSize(270,80);
            button->SetName("Button");
            button->SetStyleAuto();
            button->SetOpacity(0.75);
            {
                Text* t=new Text(context_);
                t->SetFont(cache_->GetResource<Font>("assets/fonts/Anonymous Pro.ttf"),20);
                t->SetHorizontalAlignment(HA_CENTER);
                t->SetVerticalAlignment(VA_CENTER);
                t->SetName("Text");
                t->SetText("Try again");
                button->AddChild(t);
            }
            window_menu->AddChild(button);
            SubscribeToEvent(button,E_RELEASED,URHO3D_HANDLER(gs_main_menu,HandleTryAgainPressed));
        }
    }

    {
        Button* button=new Button(context_);
        button->SetPosition(10,110);
        button->SetFixedSize(270,80);
        button->SetName("Button");
        button->SetStyleAuto();
        button->SetOpacity(0.75);
        {
            Text* t=new Text(context_);
            t->SetFont(cache_->GetResource<Font>("assets/fonts/Anonymous Pro.ttf"),20);
            t->SetHorizontalAlignment(HA_CENTER);
            t->SetVerticalAlignment(VA_CENTER);
            t->SetName("Text");
            t->SetText("Quit");
            button->AddChild(t);
        }
        window_menu->AddChild(button);
        SubscribeToEvent(button,E_RELEASED,URHO3D_HANDLER(gs_main_menu,HandleClosePressed));
    }
}

void gs_main_menu::HandleClosePressed(Urho3D::StringHash eventType,Urho3D::VariantMap& eventData)
{
    globals::instance()->engine->Exit();
}

void gs_main_menu::HandleNewGamePressed(Urho3D::StringHash eventType,Urho3D::VariantMap& eventData) {
    GetSubsystem<Input>()->SetMouseVisible(false);
    GetSubsystem<Input>()->SetMouseGrabbed(true);
    globals::instance()->game_states.clear();
    globals::instance()->game_states.emplace_back(new gs_playing(scene_, context_, cache_, cameraNode_, playerNode_, state));
}
void gs_main_menu::HandleResumePressed(Urho3D::StringHash eventType,Urho3D::VariantMap& eventData)
{
    globals::instance()->toggleMenu = false;
    GetSubsystem<Input>()->SetMouseVisible(false);
    GetSubsystem<Input>()->SetMouseGrabbed(true);
    globals::instance()->game_states.resize(1);
}
void gs_main_menu::HandleTryAgainPressed(Urho3D::StringHash eventType,Urho3D::VariantMap& eventData)
{
    Player* player = playerNode_->GetComponent<Player>();
    player->ResetHealthPoints();
    player->ResetZombieKilled();
    player->ResetUi();
    globals::instance()->toggleMenu = false;
    GetSubsystem<Input>()->SetMouseVisible(false);
    GetSubsystem<Input>()->SetMouseGrabbed(true);
    globals::instance()->game_states.front()->reset();
    globals::instance()->game_states.resize(1);

}

void gs_main_menu::update(StringHash eventType,VariantMap& eventData) {}
void gs_main_menu::HandleKeyDown(StringHash eventType,VariantMap& eventData) {}
void gs_main_menu::Start() {}


