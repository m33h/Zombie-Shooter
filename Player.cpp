#include <Urho3D/Urho3D.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/Scene/Component.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Scene/Scene.h>
#include "Urho3D/IO/Log.h"

#include "Player.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <Urho3D/Graphics/AnimatedModel.h>

using namespace Urho3D;

Player::Player(Context *context) : GameObject(context), healthPoints(100) {}
Player::~Player() {}

void Player::RegisterObject(Context *context) {
    context -> RegisterFactory<Player>();
    SharedPtr<Player> player = context->CreateObject<Player>();
}

void Player::SetControls(const Controls &newControls) {
    controls = newControls;
}

void Player::HandleUpdate(float timeStep) {}

void Player::HandleCollision(StringHash eventType, VariantMap& eventData) {}