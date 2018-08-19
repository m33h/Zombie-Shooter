#include <stdio.h>
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
#include <Urho3D/Physics/PhysicsWorld.h>
#include <Urho3D/Navigation/Navigable.h>
#include <Urho3D/Navigation/NavigationMesh.h>
#include <Urho3D/Navigation/CrowdAgent.h>
#include <Urho3D/Graphics/AnimationController.h>
#include <Urho3D/Navigation/NavigationEvents.h>
#include <Urho3D/Navigation/Obstacle.h>
#include <Urho3D/Navigation/OffMeshConnection.h>
#include "Urho3D/IO/Log.h"
#include <Urho3D/Audio/Audio.h>
#include <Urho3D/Audio/AudioEvents.h>
#include <Urho3D/Audio/Sound.h>
#include <Urho3D/Audio/SoundSource.h>
#include <Bullet/LinearMath/btIDebugDraw.h>

#include "Player.h"
#include "Zombie.h"
#include "gs_main_menu.h"
#include "gs_playing.h"

using namespace Urho3D;

#define NUM_MODELS 50


class Main : public Application
{
    StringHash EVENT_ENEMY_HIT = StringHash("asdasdasd");

    public:
        int framecount_;
        float time_;
        SharedPtr<Text> text_;
        SharedPtr<Scene> scene_;
        SharedPtr<Node> cameraNode_;
        SharedPtr<Node> playerNode;
        Context* context;
        PhysicsWorld* pw;

    Main(Context *context) : Application(context), framecount_(0), time_(0), context(context) {
        Player::RegisterObject(context);
        Zombie::RegisterObject(context);
    }

    virtual void Setup() {
        engineParameters_["FullScreen"] = false;
        engineParameters_["WindowWidth"] = 1280;
        engineParameters_["WindowHeight"] = 720;
        engineParameters_["WindowResizable"] = true;
    }

    virtual void Start() {
        ResourceCache *cache = GetSubsystem<ResourceCache>();
        GetSubsystem<UI>()->GetRoot()->SetDefaultStyle(cache->GetResource<XMLFile>("UI/DefaultStyle.xml"));

        GetSubsystem<UI>()->GetRoot()->AddChild(text_);

        initSight();

        scene_ = new Scene(context_);
        XMLFile *sceneFile = cache->GetResource<XMLFile>("assets/scenes/mainScene.xml");
        scene_->LoadXML(sceneFile->GetRoot());

        scene_->CreateComponent<Octree>();
        scene_->CreateComponent<DebugRenderer>();

        cameraNode_ = scene_->CreateChild("Camera");
        cameraNode_->SetPosition(Vector3(0, 2, 0));
        Camera *camera = cameraNode_->CreateComponent<Camera>();
        camera->SetFarClip(2000);

        Renderer *renderer = GetSubsystem<Renderer>();
        SharedPtr<Viewport> viewport(new Viewport(context_, scene_, cameraNode_->GetComponent<Camera>()));
        renderer->SetViewport(0, viewport);

        pw = scene_->GetComponent<PhysicsWorld>();

        subscribeToEvents();

        globals::instance()->playerNode=playerNode;
        globals::instance()->cameraNode=cameraNode_;
        globals::instance()->cache=cache;
        globals::instance()->camera=camera;
        globals::instance()->scene=scene_;
        globals::instance()->context=context_;
        globals::instance()->ui_root=GetSubsystem<UI>()->GetRoot();
        globals::instance()->engine=engine_;
        globals::instance()->game_states.emplace_back(new gs_playing);
    }

    void initSight() {
        ResourceCache *cache = GetSubsystem<ResourceCache>();

        BorderImage *aimView = new BorderImage(context);

        aimView->SetTexture(cache->GetResource<Texture2D>("Textures/aim.png"));
        aimView->SetAlignment(HA_CENTER, VA_CENTER);
        aimView->SetSize(128, 128);

        GetSubsystem<UI>()->GetRoot()->AddChild(aimView);
    }

    void HandleMouseDown(StringHash eventType, VariantMap &eventData) {

        using namespace MouseButtonDown;
        int key = eventData[P_BUTTON].GetInt();

        if(key == MOUSEB_LEFT) {
            Shoot();
        }
    }

    void subscribeToEvents() {
        SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(Main, HandleKeyDown));
    }

    void HandlePlayerCollision(StringHash eventType, VariantMap& eventData) {}

    virtual void Stop() {}

    void HandleKeyDown(StringHash eventType, VariantMap &eventData) {
        using namespace KeyDown;
        int key = eventData[P_KEY].GetInt();
        if (key == KEY_ESCAPE) {
            globals::instance()->game_states.emplace_back(new gs_main_menu);
            globals::instance()->toggleMenu = !globals::instance()->toggleMenu;
        }
    }

    void Shoot() {

        PlayShootingSound();

        Graphics* graphics = GetSubsystem<Graphics>();
        Camera* camera = cameraNode_->GetComponent<Camera>();
        Ray cameraRay = camera->GetScreenRay((float) graphics->GetWidth()/2/graphics->GetWidth(), (float)graphics->GetHeight()/2/graphics->GetHeight());

        PODVector<RayQueryResult> results;
        RayOctreeQuery query(results, cameraRay, RAY_TRIANGLE, M_INFINITY, DRAWABLE_GEOMETRY);
        scene_->GetComponent<Octree>()->Raycast(query);

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

    void PlayShootingSound() {
        const String& soundResourceName = "assets/Music/shoot.wav";

        // Get the sound resource
        ResourceCache* cache = GetSubsystem<ResourceCache>();
        Sound* sound = cache->GetResource<Sound>(soundResourceName);

        if (sound)
        {
            SoundSource* soundSource = scene_->CreateComponent<SoundSource>();
            soundSource->SetAutoRemoveMode(REMOVE_COMPONENT);
            soundSource->Play(sound);
            soundSource->SetGain(0.75f);
        }
    }
    void HandleUpdate(StringHash eventType,VariantMap& eventData) {}
    void HandleBeginFrame(StringHash eventType, VariantMap& eventData) {}
    void HandleRenderUpdate(StringHash eventType, VariantMap & eventData) {}
    void HandlePostUpdate(StringHash eventType,VariantMap& eventData) {}
    void HandleEndFrame(StringHash eventType,VariantMap& eventData) {}
};

URHO3D_DEFINE_APPLICATION_MAIN(Main);