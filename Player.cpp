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
#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <Urho3D/Graphics/AnimatedModel.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Physics/PhysicsEvents.h>

using namespace Urho3D;

Player::Player(Context *context) : GameObject(context), healthPoints(100) {
    SubscribeToEvents();
}

Player::~Player() {}

void Player::RegisterObject(Context *context) {
    context -> RegisterFactory<Player>();
}

void Player::SetControls(const Controls &newControls) {
    controls = newControls;
}

void Player::HandleUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData) {}

void Player::HandleCollision(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData) {

    Urho3D::RefCounted* node = eventData[Urho3D::NodeCollision::P_OTHERNODE].GetPtr();
    healthPoints -= 10;

    if(healthPoints > 0) {
        SendEvent("PLAYER_WOUNDED");
    } else {
        SendEvent("PLAYER_DIED");
    }
}

void Player::SubscribeToEvents() {
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(Player, HandleUpdate));
    SubscribeToEvent(GetNode(), E_NODECOLLISION, URHO3D_HANDLER(Player, HandleCollision));
}