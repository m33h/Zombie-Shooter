#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Input/Controls.h>

#include "GameObject.h"

using namespace Urho3D;

class Player : public GameObject {
        int healthPoints;
        URHO3D_OBJECT(Player, GameObject);

    public:
        Player(Context *context);
        ~Player();
        static void RegisterObject(Context* context);

        void SetControls(const Controls& newControls);
        virtual void HandleUpdate(float timeStep);
        void HandleCollision(StringHash eventType, VariantMap& eventData);
        Controls controls;
};