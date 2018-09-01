#include <Urho3D/Urho3D.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/Scene/Component.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Scene/Scene.h>
#include "Urho3D/IO/Log.h"
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Scene/SceneEvents.h>
#include "Player.h"
#include "game_state.h"
#include "gs_main_menu.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <Urho3D/Graphics/AnimatedModel.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Physics/PhysicsEvents.h>
#include <Urho3D/Audio/Sound.h>
#include <Urho3D/Audio/SoundSource.h>

using namespace Urho3D;

Player::Player(Context *context) : GameObject(context), healthPoints(100) {
}

Player::~Player() {}

void Player::RegisterObject(Context *context) {
    context -> RegisterFactory<Player>();
}

void Player::HandleUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData) {}

void Player::HandleCollision(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData) {

    Node* node = dynamic_cast<Node *>(eventData[Urho3D::NodeCollision::P_OTHERNODE].GetPtr());

    if (nullptr != node && node->HasTag("Enemy") && isPlayerAlive()) {
        healthPoints -= 2;
        SendEvent("PLAYER_WOUNDED");

        if(healthPoints <= 0) {
            PlayUserDieSound();
            //todo: move this to function and block 'Resume' option
            globals::instance()->game_states.emplace_back(new gs_main_menu);
            globals::instance()->toggleMenu = !globals::instance()->toggleMenu;
            SendEvent("PLAYER_DIED");
        }
    }
}

void Player::SubscribeToEvents() {
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(Player, HandleUpdate));
    SubscribeToEvent(GetNode(), E_NODECOLLISION, URHO3D_HANDLER(Player, HandleCollision));
}

void Player::PlayUserDieSound() {
    const String& soundResourceName = "assets/Music/player_die.wav";
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    Sound* sound = cache->GetResource<Sound>(soundResourceName);

    if (sound)
    {
        SoundSource* soundSource = GetScene()->CreateComponent<SoundSource>();
        soundSource->SetAutoRemoveMode(REMOVE_COMPONENT);
        soundSource->Play(sound);
        soundSource->SetGain(0.75f);
    }
}

bool Player::isPlayerAlive() {
    return healthPoints > 0;
}

void Player::Start() {
    LogicComponent::Start();
    SubscribeToEvents();
}
