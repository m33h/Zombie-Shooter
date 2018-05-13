#include <Urho3D/Urho3D.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/Scene/Component.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Scene/Scene.h>

#include "Player.h"
#include <iostream>

using namespace Urho3D;

Player::Player(Context *context) : GameObject(context) {}
Player::~Player() {}

void Player::RegisterObject(Context *context) {
    context -> RegisterFactory<Player>();
}

void Player::SetControls(const Controls &newControls) {
    controls = newControls;
}

void Player::Start() {}

void Player::HandleUpdate(float timeStep) {}