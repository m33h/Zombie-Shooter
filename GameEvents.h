//
// Created by Muman on 2018-08-31.
//

#ifndef ZOMBIE_SHOOTER_GAMEEVENTS_H
#define ZOMBIE_SHOOTER_GAMEEVENTS_H

#include <Urho3D/Urho3D.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Core/Object.h>

namespace Urho3D
{
    URHO3D_EVENT(E_GAME_START, GameStart) {
        URHO3D_PARAM(P_ZOMBIES_IN_ROUND, ZombiesInRound);
    }

    URHO3D_EVENT(E_GAME_NEXT_ROUND, GameNextRound) {
        URHO3D_PARAM(P_ZOMBIES_IN_ROUND, ZombiesInRound);
    }

    URHO3D_EVENT(E_GAME_NEXT_ROUND_TIME, GameNextRoundTime) {
        URHO3D_PARAM(P_NEXT_ROUND_TIME, NextRoundTime);
    }
}

#endif //ZOMBIE_SHOOTER_GAMEEVENTS_H
