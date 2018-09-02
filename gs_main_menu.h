#ifndef GS_MAIN_MENU_H
#define GS_MAIN_MENU_H

#define GAME_START 0
#define GAME_RESTART 1

#include "game_state.h"

#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Skybox.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/BorderImage.h>
#include <Urho3D/UI/CheckBox.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/UI/Window.h>
#include <Urho3D/UI/LineEdit.h>
#include <Urho3D/UI/ListView.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/IO/FileSystem.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Audio/Sound.h>
#include <Urho3D/Audio/SoundSource3D.h>
#include <Urho3D/Audio/SoundListener.h>
#include <Urho3D/Audio/Audio.h>
#include <Urho3D/Graphics/ParticleEmitter.h>
#include <Urho3D/Graphics/ParticleEffect.h>
#include <Urho3D/Scene/Node.h>

using namespace Urho3D;

class gs_main_menu : public game_state
{
    public:
        Urho3D::Window* window_menu;
        gs_main_menu(Scene *scene, Context* context, ResourceCache* cache, Node* cameraNode, Node* playerNode, int state);
        void update(Urho3D::StringHash eventType,Urho3D::VariantMap& eventData);
        void HandleNewGamePressed(Urho3D::StringHash eventType,Urho3D::VariantMap& eventData);
        void HandleKeyDown(Urho3D::StringHash eventType,Urho3D::VariantMap& eventData);
        void HandleClosePressed(Urho3D::StringHash eventType,Urho3D::VariantMap& eventData);
        void HandleResumePressed(Urho3D::StringHash eventType,Urho3D::VariantMap& eventData);
        virtual void Start();
    private:
        int state;
        Scene* scene_;
        Context* context_;
        ResourceCache* cache_;
        Node* playerNode_;
        Node* cameraNode_;
};

#endif // GS_MAIN_MENU_H
