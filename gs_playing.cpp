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
#include <Urho3D/Graphics/Texture2D.h>
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
#include "Urho3D/Core/Object.h"
#include <Urho3D/Scene/Node.h>
#include <Urho3D/UI/Sprite.h>

#include "gs_playing.h"
#include "gs_main_menu.h"

#define NUM_MODELS 20

using namespace Urho3D;
using namespace std;

gs_playing::gs_playing() : game_state() {
    SharedPtr<Scene> scene_;

    initUi();
    addPlayer();
    addEnemies();
    initNavigation();
    subscribeToEvents();
    updatePlayerHealthUiElement();
    updateKilledZombiesUiElement();
}

void gs_playing::subscribeToEvents() {
    URHO3D_LOGINFO("SUBSCRIBED gs_playing");

    SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(gs_playing, HandleKeyDown));
    SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(gs_playing, HandleUpdate));
    SubscribeToEvent(E_POSTRENDERUPDATE, URHO3D_HANDLER(gs_playing, HandlePostRenderUpdate));
    SubscribeToEvent(E_CROWD_AGENT_REPOSITION, URHO3D_HANDLER(gs_playing, HandleCrowdAgentReposition));
    SubscribeToEvent(E_MOUSEBUTTONDOWN, URHO3D_HANDLER(gs_playing, HandleMouseDown));
    SubscribeToEvent(E_NODECOLLISION, URHO3D_HANDLER(gs_playing, HandlePlayerCollision));
    SubscribeToEvent("ZOMBIE_KILLED", URHO3D_HANDLER(gs_playing, HandleZombieKilled));
    SubscribeToEvent("PLAYER_WOUNDED", URHO3D_HANDLER(gs_playing, HandlePlayerWounded));
}

void gs_playing::HandleKeyDown(StringHash eventType, VariantMap &eventData) {
    using namespace KeyDown;
    int key = eventData[P_KEY].GetInt();
    if (key == KEY_ESCAPE) {
        GetSubsystem<Input>()->SetMouseVisible(!GetSubsystem<Input>()->IsMouseVisible());
        globals::instance()->toggleMenu = true;
        globals::instance()->game_states.emplace_back(new gs_main_menu);
    }
}

void gs_playing::HandleZombieKilled(Urho3D::StringHash eventType, Urho3D::VariantMap &eventData) {
    globals::instance()->killedZombiesCount++;
    updateKilledZombiesUiElement();
}

void gs_playing::HandlePlayerWounded(Urho3D::StringHash eventType, Urho3D::VariantMap &eventData) {
    updatePlayerHealthUiElement();
    playerWoundedSound();
    redFlashScreenEffect();
}

void gs_playing::addEnemies() {
    const float MODEL_MOVE_SPEED = 2.0f;
    const float MODEL_ROTATE_SPEED = 100.0f;
    const BoundingBox bounds(Vector3(-20.0f, 0.0f, -20.0f), Vector3(20.0f, 0.0f, 20.0f));

    Node *jacks = globals::instance()->scene->CreateChild("Enemies");

    for (unsigned i = 0; i < NUM_MODELS; ++i) {
        Node *modelNode = jacks->CreateChild("Jill");
        modelNode->AddTag("Enemy");
        modelNode->SetPosition(Vector3(Random(40.0f) - 20.0f, 0.0f, Random(40.0f) - 20.0f));
        modelNode->SetRotation(Quaternion(0.0f, Random(180.0f), 0.0f));

        RigidBody *body = modelNode->CreateComponent<RigidBody>();
        body->SetCollisionLayer(1);
        body->SetMass(0.01f);
        body->SetRollingFriction(0.15f);
        body->SetAngularFactor(Vector3::ZERO);

        CollisionShape *shape = modelNode->CreateComponent<CollisionShape>();
        shape->SetCapsule(0.7f, 1.8f, Vector3(0.0f, 0.9f, 0.0f));

        AnimatedModel *modelObject = modelNode->CreateComponent<AnimatedModel>();
        modelObject->SetModel(globals::instance()->cache->GetResource<Model>("Models/Mutant/Mutant.mdl"));
        modelObject->SetMaterial(globals::instance()->cache->GetResource<Material>("Models/Mutant/Materials/mutant_M.xml"));
        modelObject->SetCastShadows(true);

        CrowdAgent* agent = modelNode->CreateComponent<CrowdAgent>();
        agent->SetHeight(2.0f);
        agent->SetMaxSpeed(3.0f);
        agent->SetMaxAccel(5.0f);

        modelNode->CreateComponent<AnimationController>();
        modelNode->CreateComponent<Zombie>();
    }
}

void gs_playing::addPlayer() {
    playerNode = globals::instance()->playerNode;
    playerNode->SetPosition(Vector3(0,0,0));
    cameraNode_ = globals::instance()->cameraNode;
    globals::instance()->cameraNode=cameraNode_;
    cameraNode_->SetPosition(Vector3(0,3,0));
    Camera *camera = globals::instance()->camera;
    camera->SetFarClip(2000);

    Renderer *renderer = GetSubsystem<Renderer>();
    SharedPtr<Viewport> viewport(
            new Viewport(
                    globals::instance()->context,
                    globals::instance()->scene,
                    globals::instance()->camera
            )
    );
    renderer->SetViewport(0, viewport);
    ResourceCache *cache = globals::instance()->cache;

    CollisionShape *shape = playerNode->CreateComponent<CollisionShape>();
    shape->SetCapsule(0.7f, 1.8f, Vector3(0.0f, 0.9f, 0.0f));

    RigidBody *body = playerNode->CreateComponent<RigidBody>();
    body->SetCollisionLayer(1);
    body->SetMass(0.01f);
    body->SetRollingFriction(0.15f);
    body->SetAngularFactor(Vector3::ZERO);

    AnimatedModel *modelObject = playerNode->CreateComponent<AnimatedModel>();
    modelObject->SetModel(cache->GetResource<Model>("Models/Kachujin/Kachujin.mdl"));
    modelObject->SetMaterial(cache->GetResource<Material>("Models/Kachujin/Materials/Kachujin.xml"));
    modelObject->SetCastShadows(true);
}

void gs_playing::HandleUpdate(StringHash eventType,VariantMap& eventData) {
    float timeStep = eventData[Update::P_TIMESTEP].GetFloat();
    framecount_++;
    time_ += timeStep;

    float MOVE_SPEED = 10.0f;
    const float MOUSE_SENSITIVITY = 0.1f;

    Input *input = GetSubsystem<Input>();

    if((int)time_ % 3 == 0 ) {
        gs_playing::UpdateEnemyDestination();
    }

    if (!GetSubsystem<Input>()->IsMouseVisible()) {
        IntVector2 mouseMove = input->GetMouseMove();
        static float yaw_ = 0;
        static float pitch_ = 0;
        yaw_ += MOUSE_SENSITIVITY * mouseMove.x_;
        pitch_ += MOUSE_SENSITIVITY * mouseMove.y_;
        cameraNode_->SetDirection(Vector3::FORWARD);
        playerNode->SetDirection(cameraNode_->GetDirection());
        cameraNode_->Yaw(yaw_);
        cameraNode_->Pitch(pitch_);
        Vector3 cameraDirVector = cameraNode_->GetDirection();

        if (input->GetKeyDown('W'))
            playerNode->Translate(Vector3(cameraDirVector.x_, 0, cameraDirVector.z_) * MOVE_SPEED * timeStep);
        if (input->GetKeyDown('S'))
            playerNode->Translate(-1 * Vector3(cameraDirVector.x_, 0, cameraDirVector.z_) * MOVE_SPEED * timeStep);
        if (input->GetKeyDown('A'))
            playerNode->Translate(cameraDirVector.CrossProduct(Vector3(0,1,0)) * MOVE_SPEED * timeStep);
        if (input->GetKeyDown('D'))
            playerNode->Translate(-1 * cameraDirVector.CrossProduct(Vector3(0,1,0)) * MOVE_SPEED * timeStep);
    }
}

void gs_playing::HandlePostRenderUpdate(StringHash eventType, VariantMap & eventData){
    // Visualize navigation mesh, obstacles and off-mesh connections
   // globals::instance()->scene->GetComponent<NavigationMesh>()->DrawDebugGeometry(true);
    // Visualize agents' path and position to reach
    //globals::instance()->scene->GetComponent<CrowdManager>()->DrawDebugGeometry(true);
}


void gs_playing::HandleCrowdAgentReposition(StringHash eventType, VariantMap& eventData) {
    static const char *WALKING_ANI = "Models/Mutant/Mutant_Walk.ani";

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
            node->SetRotation(node->GetRotation().Slerp(Quaternion(Vector3::BACK, velocity),
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
    gs_playing::UpdateEnemyDestination();
}

void gs_playing::UpdateEnemyDestination() {
//    URHO3D_LOGINFO("update enemy destination");
    if(!globals::instance()->toggleMenu){
        Vector3 hitPos = playerNode->GetPosition();
        hitPos.y_ = 0;

        NavigationMesh *navMesh = globals::instance()->scene->GetComponent<NavigationMesh>();
        Vector3 pathPos = navMesh->FindNearestPoint(hitPos, Vector3(1.0f, 1.0f, 1.0f));
        Node* jackGroup = globals::instance()->scene->GetChild("Jacks");
        globals::instance()->scene->GetComponent<CrowdManager>()->SetCrowdTarget(pathPos, jackGroup);
    }
}

void gs_playing::HandleMouseDown(StringHash eventType, VariantMap &eventData) {
    using namespace MouseButtonDown;
    int key = eventData[P_BUTTON].GetInt();

    if(key == MOUSEB_LEFT) {
        Shoot();
    }
}

void gs_playing::Shoot() {
    PlayShootingSound();

    Graphics* graphics = GetSubsystem<Graphics>();
    Camera* camera = globals::instance()->cameraNode->GetComponent<Camera>();
    Ray cameraRay = camera->GetScreenRay((float) graphics->GetWidth()/2/graphics->GetWidth(), (float)graphics->GetHeight()/2/graphics->GetHeight());

    PODVector<RayQueryResult> results;
    RayOctreeQuery query(results, cameraRay, RAY_TRIANGLE, M_INFINITY, DRAWABLE_GEOMETRY);
    globals::instance()->scene->GetComponent<Octree>()->Raycast(query);

    if (results.Size())
    {
        RayQueryResult& result = results[0];

        if(result.node_->HasTag("Enemy")) {

            if(result.node_->HasComponent<Zombie>()) {
                result.node_->GetComponent<Zombie>()->GotHit();
            }
        }
    }
}

void gs_playing::PlayShootingSound() {
    const String& soundResourceName = "assets/Music/shoot.wav";

    Sound* sound = globals::instance()->cache->GetResource<Sound>(soundResourceName);

    if (sound)
    {
        SoundSource* soundSource = globals::instance()->scene->CreateComponent<SoundSource>();
        soundSource->SetAutoRemoveMode(REMOVE_COMPONENT);
        soundSource->Play(sound);
        soundSource->SetGain(0.75f);
    }
}

void gs_playing::initShootingAim() {
    BorderImage *aimView = new BorderImage(globals::instance()->context);
    aimView->SetAlignment(HA_CENTER, VA_CENTER);
    aimView->SetSize(128, 128);
    aimView->SetTexture(globals::instance()->cache->GetResource<Texture2D>("assets/Textures/aim.png"));
    GetSubsystem<UI>()->GetRoot()->AddChild(aimView);
}

void gs_playing::initUi() {
    initShootingAim();
    initKilledZombieUiElement();
    initPlayerHealthUiElement();
}

void gs_playing::initKilledZombieUiElement() {
    UIElement* root = GetSubsystem<UI>()->GetRoot();
    ResourceCache* cache = GetSubsystem<ResourceCache>();

    UIElement* zombieCountUiGroup = new UIElement(context_);
    zombieCountUiGroup->SetLayout(LM_HORIZONTAL);
    zombieCountUiGroup->SetVerticalAlignment(VA_TOP);
    zombieCountUiGroup->SetHorizontalAlignment(HA_RIGHT);
    zombieCountUiGroup->SetMinSize(200, 100);

    BorderImage* skullImage = new BorderImage(context_);
    skullImage->SetName("ivSkullImage");
    skullImage->SetTexture(cache->GetResource<Texture2D>("assets/Textures/skull_white.png")); // Set texture
    skullImage->SetBlendMode(BLEND_ADD);
    skullImage->SetHorizontalAlignment(HA_RIGHT);
    skullImage->SetVerticalAlignment(VA_CENTER);
    skullImage->SetSize(92, 92);

    Text* killedZombiesText = new Text(context_);
    killedZombiesText->SetName("tvKilledZombieCount");
    killedZombiesText->SetText("0");
    killedZombiesText->SetStyleAuto();
    killedZombiesText->SetColor(Color(1,0,0));
    killedZombiesText->SetFont(cache->GetResource<Font>("assets/fonts/Anonymous Pro.ttf"), 48);
    killedZombiesText->SetHorizontalAlignment(HA_CENTER);
    killedZombiesText->SetVerticalAlignment(VA_CENTER);

    zombieCountUiGroup->AddChild(killedZombiesText);
    zombieCountUiGroup->AddChild(skullImage);
    root->AddChild(zombieCountUiGroup);
}

void gs_playing::initPlayerHealthUiElement() {
    UIElement* root = GetSubsystem<UI>()->GetRoot();
    ResourceCache* cache = GetSubsystem<ResourceCache>();

    UIElement* playerHealthUiGroup = new UIElement(context_);
    playerHealthUiGroup->SetLayout(LM_HORIZONTAL);
    playerHealthUiGroup->SetVerticalAlignment(VA_TOP);
    playerHealthUiGroup->SetHorizontalAlignment(HA_LEFT);
    playerHealthUiGroup->SetMinSize(200, 100);

    BorderImage* heartImage = new BorderImage(context_);
    heartImage->SetName("ivHearthImage");
    heartImage->SetTexture(cache->GetResource<Texture2D>("assets/Textures/heart.png")); // Set texture
    heartImage->SetBlendMode(BLEND_REPLACE);
    heartImage->SetHorizontalAlignment(HA_LEFT);
    heartImage->SetVerticalAlignment(VA_CENTER);
    heartImage->SetSize(92, 92);

    Text* playerHealthText = new Text(context_);
    playerHealthText->SetName("tvPlayerHealth");
    playerHealthText->SetColor(Color(0,1,0));
    playerHealthText->SetStyleAuto();
    playerHealthText->SetFont(cache->GetResource<Font>("assets/fonts/Anonymous Pro.ttf"), 48);
    playerHealthText->SetHorizontalAlignment(HA_RIGHT);
    playerHealthText->SetVerticalAlignment(VA_CENTER);

    playerHealthUiGroup->AddChild(heartImage);
    playerHealthUiGroup->AddChild(playerHealthText);
    root->AddChild(playerHealthUiGroup);
}

void gs_playing::updatePlayerHealthUiElement() {
    UIElement* root = GetSubsystem<UI>()->GetRoot();

    Text* tvPlayerHealth = dynamic_cast<Text *>(root->GetChild("tvPlayerHealth", true));

    Node* playerNode = FindPlayerNode();

    if(playerNode) {
        int playerHpPercentage = playerNode->GetComponent<Player>()->GetHealthPoints();

        if (playerHpPercentage < 30) {
            tvPlayerHealth->SetColor(Color(1,0,0));
        } else if (playerHpPercentage < 60) {
            tvPlayerHealth->SetColor(Color(1,1,0));
        } else {
            tvPlayerHealth->SetColor(Color(0,1,0));
        }

        tvPlayerHealth->SetText(String(playerHpPercentage));
    }
}

void gs_playing::updateKilledZombiesUiElement() {
    UIElement* root = GetSubsystem<UI>()->GetRoot();
    Text* tvKilledZombieCount = dynamic_cast<Text *>(root->GetChild("tvKilledZombieCount", true));
    int killedZombiesCount = globals::instance()->killedZombiesCount;
    tvKilledZombieCount->SetText(String(killedZombiesCount));
}

Node *gs_playing::FindPlayerNode() {
    return globals::instance()->scene->GetChild("Player", true);
}

void gs_playing::HandlePlayerCollision(StringHash eventType, VariantMap &eventData) {}

void gs_playing::playerWoundedSound(){
    URHO3D_LOGINFO("wound sound");

}
void gs_playing::redFlashScreenEffect(){
    URHO3D_LOGINFO("flash screen");

}
