#include "gs_playing.h"

#include <string>
#include <sstream>
#include <Urho3D/Graphics/BillboardSet.h>
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

////#include "gs_playing.h"
#include "gs_main_menu.h"

#define NUM_MODELS 50

using namespace Urho3D;

gs_playing::gs_playing() : game_state() {

    URHO3D_LOGINFO("PLAY gs_playing");

    addPlayer();
    addEnemies();
    initNavigation();
    subscribeToEvents();
}

void gs_playing::subscribeToEvents() {
    URHO3D_LOGINFO("SUBSCRIBED gs_playing");

    SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(gs_playing, HandleKeyDown));
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(gs_playing, HandleUpdate));
    SubscribeToEvent(E_POSTRENDERUPDATE, URHO3D_HANDLER(gs_playing, HandlePostRenderUpdate));
    SubscribeToEvent(E_CROWD_AGENT_REPOSITION, URHO3D_HANDLER(gs_playing, HandleCrowdAgentReposition));
//    SubscribeToEvent(E_NODECOLLISION, URHO3D_HANDLER(Main, HandlePlayerCollision));
}

void gs_playing::HandleKeyDown(StringHash eventType, VariantMap &eventData) {
    using namespace KeyDown;
    int key = eventData[P_KEY].GetInt();
    if (key == KEY_ESCAPE) {
        GetSubsystem<Input>()->SetMouseVisible(!GetSubsystem<Input>()->IsMouseVisible());
        globals::instance()->toggleMenu = !globals::instance()->toggleMenu;
        globals::instance()->game_states.emplace_back(new gs_main_menu);
    }
}

void gs_playing::addEnemies() {
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
    ResourceCache *cache = globals::instance()->cache;

    playerNode = cameraNode_ -> CreateChild("Player");
    RigidBody *body = playerNode->CreateComponent<RigidBody>();
    body->SetCollisionLayer(1);
    body->SetMass(0.01f);
    body->SetRollingFriction(0.15f);
    body->SetAngularFactor(Vector3::ZERO);

    CollisionShape *shape = playerNode->CreateComponent<CollisionShape>();
    shape->SetCapsule(0.7f, 1.8f, Vector3(0.0f, 0.9f, 0.0f));
    AnimatedModel *modelObject = playerNode->CreateComponent<AnimatedModel>();
    modelObject->SetModel(cache->GetResource<Model>("Models/Kachujin/Kachujin.mdl"));
    modelObject->SetMaterial(cache->GetResource<Material>("Models/Kachujin/Materials/Kachujin.xml"));
    modelObject->SetCastShadows(true);

    playerNode->CreateComponent<AnimationController>();
}

void gs_playing::HandleUpdate(StringHash eventType,VariantMap& eventData) {
    float timeStep = eventData[Update::P_TIMESTEP].GetFloat();
    framecount_++;
    time_ += timeStep;

    float MOVE_SPEED = 10.0f;
    const float MOUSE_SENSITIVITY = 0.1f;

    Input *input = GetSubsystem<Input>();

    if (input->GetKeyDown('W')){
        cameraNode_->Translate(Vector3(0, 0, 1) * MOVE_SPEED * timeStep);
    }
    if (input->GetKeyDown('S')){
        cameraNode_->Translate(Vector3(0, 0, -1) * MOVE_SPEED * timeStep);
    }
    if (input->GetKeyDown('A')) {
        cameraNode_->Translate(Vector3(-1, 0, 0) * MOVE_SPEED * timeStep);
    }
    if (input->GetKeyDown('D')) {
        cameraNode_->Translate(Vector3(1, 0, 0) * MOVE_SPEED * timeStep);
    }


    if((int)time_ % 3 == 0 ) {
        UpdateEnemyDestination();
    }

    if (!GetSubsystem<Input>()->IsMouseVisible()) {
        IntVector2 mouseMove = input->GetMouseMove();
        static float yaw_ = 0;
        static float pitch_ = 0;
        yaw_ += MOUSE_SENSITIVITY * mouseMove.x_;
        pitch_ = Clamp(pitch_, -90.0f, 90.0f);
        cameraNode_->SetDirection(Vector3::FORWARD);
        playerNode->SetDirection(Vector3::FORWARD);
        cameraNode_->Yaw(yaw_);
        playerNode->Yaw(yaw_);
        cameraNode_->Pitch(pitch_);
        playerNode->Pitch(pitch_);
    }
}

void gs_playing::HandlePostRenderUpdate(StringHash eventType, VariantMap & eventData){
    // Visualize navigation mesh, obstacles and off-mesh connections
    globals::instance()->scene->GetComponent<NavigationMesh>()->DrawDebugGeometry(true);
    // Visualize agents' path and position to reach
    globals::instance()->scene->GetComponent<CrowdManager>()->DrawDebugGeometry(true);
}


void gs_playing::HandleCrowdAgentReposition(StringHash eventType, VariantMap& eventData) {
    static const char *WALKING_ANI = "Models/Kachujin/Kachujin_Walk.ani";

    using namespace CrowdAgentReposition;

    Node *node = static_cast<Node *>(eventData[P_NODE].GetPtr());
    CrowdAgent *agent = static_cast<CrowdAgent *>(eventData[P_CROWD_AGENT].GetPtr());
    Vector3 velocity = eventData[P_VELOCITY].GetVector3();
    float timeStep = eventData[P_TIMESTEP].GetFloat();

    // Only Jack agent has animation controller
    AnimationController *animCtrl = node->GetComponent<AnimationController>();
    if (animCtrl) {
        float speed = velocity.Length();
        if (animCtrl->IsPlaying(WALKING_ANI)) {
            float speedRatio = speed / agent->GetMaxSpeed();
            // Face the direction of its velocity but moderate the turning speed based on the speed ratio and timeStep
            node->SetRotation(node->GetRotation().Slerp(Quaternion(Vector3::FORWARD, velocity),
                                                        10.0f * timeStep * speedRatio));
            // Throttle the animation speed based on agent speed ratio (ratio = 1 is full throttle)
            animCtrl->SetSpeed(WALKING_ANI, speedRatio * 1.5f);
        } else
            animCtrl->Play(WALKING_ANI, 0, true, 0.1f);

        // If speed is too low then stop the animation
        if (speed < agent->GetRadius())
            animCtrl->Stop(WALKING_ANI, 0.5f);
    }
}

void gs_playing::initNavigation() {
    CrowdManager* crowdManager = globals::instance()->scene->CreateComponent<CrowdManager>();
    CrowdObstacleAvoidanceParams params = crowdManager->GetObstacleAvoidanceParams(0);
    params.velBias = 0.5f;
    params.adaptiveDivs = 7;
    params.adaptiveRings = 3;
    params.adaptiveDepth = 3;
    crowdManager->SetObstacleAvoidanceParams(0, params);

    NavigationMesh *navMesh = globals::instance()->scene->GetComponent<NavigationMesh>();
    navMesh->SetDrawNavAreas(true);
    navMesh->SetDrawOffMeshConnections(true);
    navMesh->SetAgentHeight(10.0f);
    // Set nav mesh cell height to minimum (allows agents to be grounded)
    navMesh->SetCellHeight(0.05f);
    navMesh->SetPadding(Vector3(0.0f, 10.0f, 0.0f));
    navMesh->Build();
    //CreateBoxOffMeshConnections(navMesh);

    UpdateEnemyDestination();
}

void gs_playing::UpdateEnemyDestination() {
    URHO3D_LOGINFO("update enemy destination");

    Camera *camera = globals::instance()->cameraNode->GetComponent<Camera>();

    Vector3 hitPos = globals::instance()->cameraNode->GetPosition();
    hitPos.y_ = 0;

    NavigationMesh *navMesh = globals::instance()->scene->GetComponent<NavigationMesh>();
    Vector3 pathPos = navMesh->FindNearestPoint(hitPos, Vector3(10.0f, 1.0f, 1.0f));
    Node* jackGroup = globals::instance()->scene->GetChild("Jacks");
    globals::instance()->scene->GetComponent<CrowdManager>()->SetCrowdTarget(pathPos, jackGroup);
}