#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Input/Controls.h>

#include "GameObject.h"

using namespace Urho3D;

class Player : public GameObject {
        int healthPoints;
        URHO3D_OBJECT(Player, GameObject);

        void SubscribeToEvents();
    public:
        Player(Context *context);
        ~Player();
        static void RegisterObject(Context* context);

        int GetHealthPoints() { return healthPoints; }
        void SetControls(const Controls& newControls);
        virtual void HandleUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
        void HandleCollision(StringHash eventType, VariantMap& eventData);
        Controls controls;
};