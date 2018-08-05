#include "gs_playing.h"

#include <Urho3D/Graphics/BillboardSet.h>
#include <string>
#include <sstream>

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Geometry.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Skybox.h>
#include <Urho3D/Graphics/AnimatedModel.h>
#include <Urho3D/Graphics/Animation.h>
#include <Urho3D/Graphics/AnimationState.h>
#include <Urho3D/Physics/PhysicsEvents.h>
#include <Urho3D/Physics/RigidBody.h>
#include <Urho3D/Physics/CollisionShape.h>
#include <Urho3D/Physics/Constraint.h>
#include <Urho3D/Navigation/Navigable.h>
#include <Urho3D/Navigation/NavigationMesh.h>
#include <Urho3D/Navigation/CrowdAgent.h>
#include <Urho3D/Graphics/AnimationController.h>
#include <Urho3D/Navigation/NavigationEvents.h>
#include <Urho3D/Navigation/Obstacle.h>
#include <Urho3D/Navigation/OffMeshConnection.h>
#include "Urho3D/IO/Log.h"
#include <Urho3D/Scene/Node.h>


////#include "gs_playing.h"
#include "gs_main_menu.h"

#define NUM_MODELS 50

using namespace Urho3D;

gs_playing::gs_playing() : game_state() {

    URHO3D_LOGINFO("PLAY");


//
//    CollisionShape *shape = playerNode->CreateComponent<CollisionShape>();
//    shape->SetCapsule(0.7f, 1.8f, Vector3(0.0f, 0.9f, 0.0f));
//    AnimatedModel *modelObject = playerNode->CreateComponent<AnimatedModel>();
//    modelObject->SetModel(globals::instance()->cache->GetResource<Model>("Models/Kachujin/Kachujin.mdl"));
//    modelObject->SetMaterial(globals::instance()->cache->GetResource<Material>("Models/Kachujin/Materials/Kachujin.xml"));
//    modelObject->SetCastShadows(true);
//
//    playerNode->CreateComponent<AnimationController>();

    cameraNode_ = globals::instance()->scene->CreateChild("Camera");
    cameraNode_->SetPosition(Vector3(0,2,0));
    Camera *camera = cameraNode_->CreateComponent<Camera>();
    camera->SetFarClip(2000);

    Renderer *renderer = GetSubsystem<Renderer>();
    SharedPtr<Viewport> viewport(
        new Viewport(
            globals::instance()->context,
            globals::instance()->scene,
            cameraNode_->GetComponent<Camera>()
        )
    );
    renderer->SetViewport(0, viewport);


    playerNode = cameraNode_ -> CreateChild("Player");
    RigidBody *body = playerNode->CreateComponent<RigidBody>();
    body->SetCollisionLayer(1);
    body->SetMass(0.01f);
    body->SetRollingFriction(0.15f);
    body->SetAngularFactor(Vector3::ZERO);


    ////
    ResourceCache *cache = globals::instance()->cache;

    const float MODEL_MOVE_SPEED = 2.0f;
    const float MODEL_ROTATE_SPEED = 100.0f;
    const BoundingBox bounds(Vector3(-20.0f, 0.0f, -20.0f), Vector3(20.0f, 0.0f, 20.0f));

    Node *jacks = globals::instance()->scene->CreateChild("Jacks");

    for (unsigned i = 0; i < NUM_MODELS; ++i) {
        Node *modelNode = jacks->CreateChild("Jill");
        modelNode->SetPosition(Vector3(Random(40.0f) - 20.0f, 0.0f, Random(40.0f) - 20.0f));
        modelNode->SetRotation(Quaternion(0.0f, Random(360.0f), 0.0f));

        RigidBody *body = modelNode->CreateComponent<RigidBody>();
        body->SetCollisionLayer(1);
        body->SetMass(0.01f);
        body->SetRollingFriction(0.15f);
        body->SetAngularFactor(Vector3::ZERO);

        CollisionShape *shape = modelNode->CreateComponent<CollisionShape>();
        shape->SetCapsule(0.7f, 1.8f, Vector3(0.0f, 0.9f, 0.0f));

        AnimatedModel *modelObject = modelNode->CreateComponent<AnimatedModel>();
        modelObject->SetModel(globals::instance()->cache->GetResource<Model>("Models/Kachujin/Kachujin.mdl"));
        modelObject->SetMaterial(globals::instance()->cache->GetResource<Material>("Models/Kachujin/Materials/Kachujin.xml"));
        modelObject->SetCastShadows(true);

        CrowdAgent* agent = modelNode->CreateComponent<CrowdAgent>();
        agent->SetHeight(2.0f);
        agent->SetMaxSpeed(3.0f);
        agent->SetMaxAccel(5.0f);

        modelNode->CreateComponent<AnimationController>();
    }
}

void gs_playing::addPlayer() {
}

void gs_playing::HandleUpdate(StringHash eventType,VariantMap& eventData) {
    URHO3D_LOGINFO("UPDATE");

    float timeStep = eventData[Update::P_TIMESTEP].GetFloat();
    framecount_++;
    time_ += timeStep;

    float MOVE_SPEED = 10.0f;
    const float MOUSE_SENSITIVITY = 0.1f;

    Input *input = GetSubsystem<Input>();

    if (input->GetKeyDown('W')){
        URHO3D_LOGINFO("W pressed gs_playing");
        cameraNode_->Translate(Vector3(0, 0, 1) * MOVE_SPEED * timeStep);
    }
    if (input->GetKeyDown('S')){
        globals::instance()->cameraNode->Translate(Vector3(0, 0, -1) * MOVE_SPEED * timeStep);
    }
    if (input->GetKeyDown('A')) {
        cameraNode_->Translate(Vector3(-1, 0, 0) * MOVE_SPEED * timeStep);
    }
    if (input->GetKeyDown('D')) {
        cameraNode_->Translate(Vector3(1, 0, 0) * MOVE_SPEED * timeStep);
    }

    if((int)time_ % 3 == 0 ) {
        if(!globals::instance()->toggleMenu) {
            Camera *camera = cameraNode_->GetComponent<Camera>();

            Vector3 hitPos = cameraNode_->GetPosition();
            hitPos.y_ = 0;

            NavigationMesh *navMesh = globals::instance()->scene->GetComponent<NavigationMesh>();

            Vector3 pathPos = navMesh->FindNearestPoint(hitPos, Vector3(1.0f, 1.0f, 1.0f));

            Node* jackGroup = globals::instance()->scene->GetChild("Jacks");

            globals::instance()->scene->GetComponent<CrowdManager>()->SetCrowdTarget(pathPos, jackGroup);
        }
    }

    if (!GetSubsystem<Input>()->IsMouseVisible()) {
        IntVector2 mouseMove = input->GetMouseMove();
        static float yaw_ = 0;
        static float pitch_ = 0;
        yaw_ += MOUSE_SENSITIVITY * mouseMove.x_;
        pitch_ = Clamp(pitch_, -90.0f, 90.0f);
        cameraNode_->SetDirection(Vector3::FORWARD);
        globals::instance()->playerNode->SetDirection(Vector3::FORWARD);
        cameraNode_->Yaw(yaw_);
        globals::instance()->playerNode->Yaw(yaw_);
        cameraNode_->Pitch(pitch_);
        globals::instance()->playerNode->Pitch(pitch_);
    }
}

void gs_playing::HandleKeyDown(StringHash eventType,VariantMap& eventData) {}