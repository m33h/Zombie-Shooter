#include "GameObject.h"

using namespace Urho3D;

class Zombie : public GameObject {
    URHO3D_OBJECT(Zombie, GameObject);

    int hitCount = 2;

    void showZombieDies();
    void showZombieGotHit();

public:
    Zombie(Context *context);
    ~Zombie();

    void GotHit();

    static void RegisterObject(Context *context);
};
