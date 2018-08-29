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
#include <Urho3D/UI/Sprite.h>

#include "gs_main_menu.h"
#include "gs_playing.h"

using namespace Urho3D;


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

    Main(Context *context) : Application(context), framecount_(0), time_(0), context(context) {
        Player::RegisterObject(context);
        Zombie::RegisterObject(context);
        URHO3D_LOGINFO("init main");
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

        scene_ = new Scene(context_);
        XMLFile *sceneFile = cache->GetResource<XMLFile>("assets/scenes/mainScene.xml");
        scene_->LoadXML(sceneFile->GetRoot());
        scene_->CreateComponent<Octree>();
        scene_->CreateComponent<DebugRenderer>();

        playerNode = scene_->CreateChild("Player");
        playerNode->CreateComponent<Player>();

        subscribeToEvents();

        globals::instance()->playerNode=playerNode;
        globals::instance()->cache=cache;
        globals::instance()->scene=scene_;
        globals::instance()->context=context_;
        globals::instance()->ui_root=GetSubsystem<UI>()->GetRoot();
        globals::instance()->engine=engine_;
        globals::instance()->game_states.emplace_back(new gs_playing);
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

    void HandleUpdate(StringHash eventType,VariantMap& eventData) {}
    void HandleBeginFrame(StringHash eventType, VariantMap& eventData) {}
    void HandleRenderUpdate(StringHash eventType, VariantMap & eventData) {}
    void HandlePostUpdate(StringHash eventType,VariantMap& eventData) {}
    void HandleEndFrame(StringHash eventType,VariantMap& eventData) {}
};

URHO3D_DEFINE_APPLICATION_MAIN(Main);