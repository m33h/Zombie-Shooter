//
// Created by Muman on 2018-09-01.
//

#ifndef ZOMBIE_SHOOTER_PLAYEREVENTS_H
#define ZOMBIE_SHOOTER_PLAYEREVENTS_H

namespace Urho3D
{
    URHO3D_EVENT(E_PLAYER_WOUNDED, PlayerWounded) {
        URHO3D_PARAM(P_PLAYER_HEALTH, PlayerHealth);
    }

    URHO3D_EVENT(E_PLAYER_DIED, PlayerDied) {}
}

#endif //ZOMBIE_SHOOTER_PLAYEREVENTS_H
