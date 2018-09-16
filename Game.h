//
// Created by Muman on 2018-08-30.
//

#ifndef ZOMBIE_SHOOTER_GAME_H
#define ZOMBIE_SHOOTER_GAME_H

#include <Urho3D/Urho3D.h>
#include <Urho3D/Core/Timer.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/CoreEvents.h>

using namespace Urho3D;

class Game : public Object {
    URHO3D_OBJECT(Game, Object)
    const int ROUND_DURATION = 20000;
    const int INITIAL_ZOMBIES_COUNT = 15;

    int currentRoundZombiesCount;
    int currentRound = 1;
    Timer nextRoundTimer;

    void RestartTimer();
    void NextRound();
    void HandleBeginFrame(StringHash eventType, VariantMap& eventData);
    float ZombieSpeed();
public:
    explicit Game(Context *context);
    virtual void Start();
    static void RegisterObject(Context *context);
};

#endif //ZOMBIE_SHOOTER_GAME_H
