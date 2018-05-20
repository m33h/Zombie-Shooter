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
#include "Mover.h"

using namespace Urho3D;

class MyApp : public Application {
    /// Last calculated path.
    PODVector<Vector3> currentPath_;
    /// Path end position.
    Vector3 endPos_;

public:
    int framecount_;
    float time_;
    SharedPtr<Text> text_;
    SharedPtr<Scene> scene_;
    SharedPtr<Node> cameraNode_;

    MyApp(Context *context) : Application(context), framecount_(0), time_(0) {
        context->RegisterFactory<Mover>();
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

        cameraNode_ = scene_->CreateChild("Camera");
        Camera *camera = cameraNode_->CreateComponent<Camera>();
        camera->SetFarClip(2000);

        Renderer *renderer = GetSubsystem<Renderer>();
        SharedPtr<Viewport> viewport(new Viewport(context_, scene_, cameraNode_->GetComponent<Camera>()));
        renderer->SetViewport(0, viewport);

        addEnemies();
        initNavigation();
        subscribeToEvents();
    }

    void subscribeToEvents() {
        SubscribeToEvent(E_BEGINFRAME, URHO3D_HANDLER(MyApp, HandleBeginFrame));
        SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(MyApp, HandleKeyDown));
        SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(MyApp, HandleUpdate));
        SubscribeToEvent(E_POSTRENDERUPDATE, URHO3D_HANDLER(MyApp, HandlePostRenderUpdate));
    }

    void addEnemies() {
        ResourceCache *cache = GetSubsystem<ResourceCache>();

        // Create animated models
        const unsigned NUM_MODELS = 1;
        const float MODEL_MOVE_SPEED = 2.0f;
        const float MODEL_ROTATE_SPEED = 100.0f;
        const BoundingBox bounds(Vector3(-20.0f, 0.0f, -20.0f), Vector3(20.0f, 0.0f, 20.0f));

        for (unsigned i = 0; i < NUM_MODELS; ++i) {
            Node *modelNode = scene_->CreateChild("Jill");
            modelNode->SetPosition(Vector3(Random(40.0f) - 20.0f, 0.0f, Random(40.0f) - 20.0f));
            modelNode->SetRotation(Quaternion(0.0f, Random(360.0f), 0.0f));

            RigidBody *body = modelNode->CreateComponent<RigidBody>();
            body->SetCollisionLayer(1);
            body->SetMass(0.01f);
            body->SetRollingFriction(0.15f);
            body->SetAngularFactor(Vector3::ZERO);

            CollisionShape *shape = modelNode->CreateComponent<CollisionShape>();
            //shape->SetSphere(1.0f);
            shape->SetCapsule(0.7f, 1.8f, Vector3(0.0f, 0.9f, 0.0f));

            AnimatedModel *modelObject = modelNode->CreateComponent<AnimatedModel>();
            modelObject->SetModel(cache->GetResource<Model>("Models/Kachujin/Kachujin.mdl"));
            modelObject->SetMaterial(cache->GetResource<Material>("Models/Kachujin/Materials/Kachujin.xml"));
            modelObject->SetCastShadows(true);

            // Create an AnimationState for a walk animation. Its time position will need to be manually updated to advance the
            // animation, The alternative would be to use an AnimationController component which updates the animation automatically,
            // but we need to update the model's position manually in any case
            Animation *walkAnimation = cache->GetResource<Animation>("Models/Kachujin/Kachujin_Walk.ani");

            AnimationState *state = modelObject->AddAnimationState(walkAnimation);
            // The state would fail to create (return null) if the animation was not found
            if (state) {
                // Enable full blending weight and looping
                state->SetWeight(1.0f);
                state->SetLooped(true);
                state->SetTime(Random(walkAnimation->GetLength()));
            }

            // Create our custom Mover component that will move & animate the model during each frame's update
            Mover *mover = modelNode->CreateComponent<Mover>();
            mover->SetParameters(MODEL_MOVE_SPEED, MODEL_ROTATE_SPEED, bounds);
        }
    }

    void UpdateEnemyDestination() {
        Camera *camera = cameraNode_->GetComponent<Camera>();

        Vector3 hitPos = cameraNode_->GetPosition();
        hitPos.y_ = 0;
        Drawable *hitDrawable;

        NavigationMesh *navMesh = scene_->GetComponent<NavigationMesh>();

        Vector3 pathPos = navMesh->FindNearestPoint(hitPos);
        Node *jackNode_ = scene_->GetChild("Jill");

        endPos_ = pathPos;
        navMesh->FindPath(currentPath_, jackNode_->GetPosition(), endPos_);
    }

    void FollowPath(float timeStep) {
        Node *jackNode_ = scene_->GetChild("Jill");

        if (currentPath_.Size()) {
            Vector3 nextWaypoint = currentPath_[0]; // NB: currentPath[0] is the next waypoint in order

            // Rotate Jack toward next waypoint to reach and move. Check for not overshooting the target
            float move = 5.0f * timeStep;
            float distance = (jackNode_->GetPosition() - nextWaypoint).Length();
            if (move > distance)
                move = distance;

            jackNode_->LookAt(nextWaypoint, Vector3::UP);
            jackNode_->Translate(Vector3::FORWARD * move);

            // Remove waypoint if reached it
            if (distance < 3.f)
                currentPath_.Erase(0);
        }
    }

    bool Raycast(float maxDistance, Vector3 &hitPos, Drawable *&hitDrawable) {
        hitDrawable = 0;

        UI *ui = GetSubsystem<UI>();
        IntVector2 pos = IntVector2(-36, 0);
        // Check the cursor is visible and there is no UI element in front of the cursor

        if (NULL != ui->GetCursor() && !ui->GetCursor()->IsVisible() || ui->GetElementAt(pos, true)) {
            return false;
        }

        Graphics *graphics = GetSubsystem<Graphics>();
        Camera *camera = cameraNode_->GetComponent<Camera>();
        Ray cameraRay = camera->GetScreenRay((float) pos.x_ / graphics->GetWidth(),
                                             (float) pos.y_ / graphics->GetHeight());
        // Pick only geometry objects, not eg. zones or lights, only get the first (closest) hit
        PODVector<RayQueryResult> results;
        RayOctreeQuery query(results, cameraRay, RAY_TRIANGLE, maxDistance, DRAWABLE_GEOMETRY);

        scene_->GetComponent<Octree>()->RaycastSingle(query);
        if (results.Size()) {
            RayQueryResult &result = results[0];
            hitPos = result.position_;
            hitDrawable = result.drawable_;
            return true;
        }

        hitPos = Vector3(30, 0, 20);

        return true;
    }

    void initNavigation() {
        NavigationMesh *navMesh = scene_->GetComponent<NavigationMesh>();
        navMesh->Build();
        UpdateEnemyDestination();
    }

    /**
    * Every frame's life must begin somewhere. Here it is.
    */
    void HandleBeginFrame(StringHash eventType, VariantMap &eventData) {
        // We really don't have anything useful to do here for this example.
        // Probably shouldn't be subscribing to events we don't care about.
    }

    void HandleKeyDown(StringHash eventType, VariantMap &eventData) {
        using namespace KeyDown;
        int key = eventData[P_KEY].GetInt();
        if (key == KEY_ESCAPE)
            engine_->Exit();

        if (key == KEY_TAB)    // toggle mouse cursor when pressing tab
        {
            GetSubsystem<Input>()->SetMouseVisible(!GetSubsystem<Input>()->IsMouseVisible());
            GetSubsystem<Input>()->SetMouseGrabbed(!GetSubsystem<Input>()->IsMouseGrabbed());
        }
    }

    void HandleUpdate(StringHash eventType, VariantMap &eventData) {
        float timeStep = eventData[Update::P_TIMESTEP].GetFloat();
        framecount_++;
        time_ += timeStep;

        float MOVE_SPEED = 10.0f;
        const float MOUSE_SENSITIVITY = 0.1f;

        Input *input = GetSubsystem<Input>();
        if (input->GetQualifierDown(1))  // 1 is shift, 2 is ctrl, 4 is alt
            MOVE_SPEED *= 10;
        if (input->GetKeyDown('W'))
            cameraNode_->Translate(Vector3(0, 0, 1) * MOVE_SPEED * timeStep);
        if (input->GetKeyDown('S'))
            cameraNode_->Translate(Vector3(0, 0, -1) * MOVE_SPEED * timeStep);
        if (input->GetKeyDown('A'))
            cameraNode_->Translate(Vector3(-1, 0, 0) * MOVE_SPEED * timeStep);
        if (input->GetKeyDown('D')) {
            cameraNode_->Translate(Vector3(1, 0, 0) * MOVE_SPEED * timeStep);
        }

        if((int)time_ % 5 == 0 ) {
            UpdateEnemyDestination();
        }

/*        if (input->GetMouseButtonPress(MOUSEB_LEFT))
            UpdateEnemyDestination();*/

        if (!GetSubsystem<Input>()->IsMouseVisible()) {
            IntVector2 mouseMove = input->GetMouseMove();
            static float yaw_ = 0;
            static float pitch_ = 0;
            yaw_ += MOUSE_SENSITIVITY * mouseMove.x_;
            pitch_ += MOUSE_SENSITIVITY * mouseMove.y_;
            pitch_ = Clamp(pitch_, -90.0f, 90.0f);
            cameraNode_->SetDirection(Vector3::FORWARD);
            cameraNode_->Yaw(yaw_);
            cameraNode_->Pitch(pitch_);
        }

        FollowPath(timeStep);
    }

    /**
    * After everything is rendered, there might still be things you wish
    * to add to the rendering. At this point you cannot modify the scene,
    * only post rendering is allowed. Good for adding things like debug
    * artifacts on screen or brush up lighting, etc.
    */
    void HandlePostRenderUpdate(StringHash eventType, VariantMap &eventData) {
      /*  scene_->GetComponent<NavigationMesh>()->DrawDebugGeometry(true);

        if (currentPath_.Size()) {

            Node *jackNode_ = scene_->GetChild("Jill");

            // Visualize the current calculated path
            DebugRenderer *debug = scene_->GetComponent<DebugRenderer>();
            debug->AddBoundingBox(BoundingBox(endPos_ - Vector3(0.1f, 0.1f, 0.1f), endPos_ + Vector3(0.1f, 0.1f, 0.1f)),
                                  Color(1.0f, 1.0f, 1.0f));

            // Draw the path with a small upward bias so that it does not clip into the surfaces
            Vector3 bias(0.0f, 0.05f, 0.0f);
            debug->AddLine(jackNode_->GetPosition() + bias, currentPath_[0] + bias, Color(1.0f, 1.0f, 1.0f));

            if (currentPath_.Size() > 1) {
                for (unsigned i = 0; i < currentPath_.Size() - 1; ++i)
                    debug->AddLine(currentPath_[i] + bias, currentPath_[i + 1] + bias, Color(1.0f, 1.0f, 1.0f));
            }
        }*/
    }
};

URHO3D_DEFINE_APPLICATION_MAIN(MyApp)