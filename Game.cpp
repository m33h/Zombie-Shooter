//
// Created by Muman on 2018-08-30.
//
#include "Game.h"
#include "GameEvents.h"

using namespace Urho3D;

Game::Game(Context *context) : Object(context) {
    nextRoundTimer = Timer();
    currentRoundZombiesCount = INITIAL_ZOMBIES_COUNT;
}

void Game::RegisterObject(Context *context) {
    context->RegisterFactory<Game>();
}

void Game::Start() {
    RestartTimer();
    SubscribeToEvent(E_UPDATE ,URHO3D_HANDLER(Game, HandleBeginFrame));

    VariantMap params;
    params[Urho3D::GameStart::P_ZOMBIES_IN_ROUND] = currentRoundZombiesCount;
    SendEvent(E_GAME_START, params);
}

void Game::NextRound() {
    ++currentRound;
    currentRoundZombiesCount *= 1.3;
    RestartTimer();

    VariantMap params;
    params[Urho3D::GameNextRound::P_ZOMBIES_IN_ROUND] = currentRoundZombiesCount;
    SendEvent(E_GAME_NEXT_ROUND, params);
}

void Game::RestartTimer() {
    nextRoundTimer.Reset();
}

void Game::HandleBeginFrame(StringHash eventType, VariantMap &eventData) {
    VariantMap params;
    params[Urho3D::GameNextRoundTime::P_NEXT_ROUND_TIME] = (ROUND_DURATION - nextRoundTimer.GetMSec(false))/1000;
    SendEvent(E_GAME_NEXT_ROUND_TIME, params);

    if(nextRoundTimer.GetMSec(false) >= ROUND_DURATION) {
        NextRound();
    }
}

float Game::ZombieSpeed() {
    return 7.0 + (float)currentRound;
}
