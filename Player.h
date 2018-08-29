#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Input/Controls.h>

#include "GameObject.h"

using namespace Urho3D;

class Player : public GameObject {
        URHO3D_OBJECT(Player, GameObject);
        int healthPoints;
        bool isPlayerAlive();
        void SubscribeToEvents();
        void PlayUserDieSound();
public:
        Player(Context *context);
        ~Player();
        static void RegisterObject(Context* context);
        virtual void Start();
        int GetHealthPoints() { return healthPoints; }
        void HandleUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
        void HandleCollision(StringHash eventType, VariantMap& eventData);

};