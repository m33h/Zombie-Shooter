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
#include "Mover.h"

using namespace Urho3D;
/**
* Using the convenient Application API we don't have
* to worry about initializing the engine or writing a main.
* You can probably mess around with initializing the engine
* and running a main manually, but this is convenient and portable.
*/
class MyApp : public Application
{
    public:
        int framecount_;
        float time_;
        SharedPtr<Text> text_;
        SharedPtr<Scene> scene_;
        SharedPtr<Node> boxNode_;
        SharedPtr<Node> cameraNode_;

        /**
        * This happens before the engine has been initialized
        * so it's usually minimal code setting defaults for
        * whatever instance variables you have.
        * You can also do this in the Setup method.
        */
        MyApp(Context * context) : Application(context),framecount_(0),time_(0)
        {
            context->RegisterFactory<Mover>();
        }

        /**
        * This method is called before the engine has been initialized.
        * Thusly, we can setup the engine parameters before anything else
        * of engine importance happens (such as windows, search paths,
        * resolution and other things that might be user configurable).
        */
        virtual void Setup()
        {
            // These parameters should be self-explanatory.
            // See http://urho3d.github.io/documentation/1.5/_main_loop.html
            // for a more complete list.
            engineParameters_["FullScreen"]=false;
            engineParameters_["WindowWidth"]=1280;
            engineParameters_["WindowHeight"]=720;
            engineParameters_["WindowResizable"]=true;
        }

        /**
        * This method is called after the engine has been initialized.
        * This is where you set up your actual content, such as scenes,
        * models, controls and what not. Basically, anything that needs
        * the engine initialized and ready goes in here.
        */
        virtual void Start()
        {
            ResourceCache* cache=GetSubsystem<ResourceCache>();
            GetSubsystem<UI>()->GetRoot()->SetDefaultStyle(cache->GetResource<XMLFile>("UI/DefaultStyle.xml"));

            GetSubsystem<UI>()->GetRoot()->AddChild(text_);

            scene_= new Scene(context_);
            XMLFile *sceneFile = cache -> GetResource<XMLFile>("assets/scenes/mainScene.xml");
            scene_ -> LoadXML(sceneFile -> GetRoot());

            scene_->CreateComponent<Octree>();
            scene_->CreateComponent<DebugRenderer>();

            cameraNode_=scene_->CreateChild("Camera");
            Camera* camera=cameraNode_->CreateComponent<Camera>();
            camera->SetFarClip(2000);

            Renderer* renderer=GetSubsystem<Renderer>();
            SharedPtr<Viewport> viewport(new Viewport(context_,scene_,cameraNode_->GetComponent<Camera>()));
            renderer->SetViewport(0,viewport);

            // Create animated models
            const unsigned NUM_MODELS = 50;
            const float MODEL_MOVE_SPEED = 2.0f;
            const float MODEL_ROTATE_SPEED = 100.0f;
            const BoundingBox bounds(Vector3(-20.0f, 0.0f, -20.0f), Vector3(20.0f, 0.0f, 20.0f));

            for (unsigned i = 0; i < NUM_MODELS; ++i) {
                Node *modelNode = scene_->CreateChild("Jill");
                modelNode->SetPosition(Vector3(Random(40.0f) - 20.0f, 0.0f, Random(40.0f) - 20.0f));
                modelNode->SetRotation(Quaternion(0.0f, Random(360.0f), 0.0f));

                // add physics to our model

                RigidBody* body = modelNode->CreateComponent<RigidBody>();
                body->SetMass(0.0f);
                body->SetRollingFriction(0.15f);

                CollisionShape* shape = modelNode->CreateComponent<CollisionShape>();
                shape->SetSphere(1.0f);

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

            // We subscribe to the events we'd like to handle.
            // In this example we will be showing what most of them do,
            // but in reality you would only subscribe to the events
            // you really need to handle.
            // These are sort of subscribed in the order in which the engine
            // would send the events. Read each handler method's comment for
            // details.
            SubscribeToEvent(E_BEGINFRAME,URHO3D_HANDLER(MyApp,HandleBeginFrame));
            SubscribeToEvent(E_KEYDOWN,URHO3D_HANDLER(MyApp,HandleKeyDown));
            SubscribeToEvent(E_UPDATE,URHO3D_HANDLER(MyApp,HandleUpdate));
            SubscribeToEvent(E_POSTUPDATE,URHO3D_HANDLER(MyApp,HandlePostUpdate));
            SubscribeToEvent(E_RENDERUPDATE,URHO3D_HANDLER(MyApp,HandleRenderUpdate));
            SubscribeToEvent(E_POSTRENDERUPDATE,URHO3D_HANDLER(MyApp,HandlePostRenderUpdate));
            SubscribeToEvent(E_ENDFRAME,URHO3D_HANDLER(MyApp,HandleEndFrame));
        }

        /**
        * Good place to get rid of any system resources that requires the
        * engine still initialized. You could do the rest in the destructor,
        * but there's no need, this method will get called when the engine stops,
        * for whatever reason (short of a segfault).
        */
        virtual void Stop()
        {
        }

        /**
        * Every frame's life must begin somewhere. Here it is.
        */
        void HandleBeginFrame(StringHash eventType,VariantMap& eventData)
        {
            // We really don't have anything useful to do here for this example.
            // Probably shouldn't be subscribing to events we don't care about.
        }

        /**
        * Input from keyboard is handled here. I'm assuming that Input, if
        * available, will be handled before E_UPDATE.
        */
        void HandleKeyDown(StringHash eventType,VariantMap& eventData)
        {
            using namespace KeyDown;
            int key=eventData[P_KEY].GetInt();
            if(key==KEY_ESCAPE)
                engine_->Exit();

            if(key==KEY_TAB)    // toggle mouse cursor when pressing tab
            {
                GetSubsystem<Input>()->SetMouseVisible(!GetSubsystem<Input>()->IsMouseVisible());
                GetSubsystem<Input>()->SetMouseGrabbed(!GetSubsystem<Input>()->IsMouseGrabbed());
            }
        }

        /**
        * You can get these events from when ever the user interacts with the UI.
        */
        void HandleClosePressed(StringHash eventType,VariantMap& eventData)
        {
            engine_->Exit();
        }
        /**
        * Your non-rendering logic should be handled here.
        * This could be moving objects, checking collisions and reaction, etc.
        */
        void HandleUpdate(StringHash eventType,VariantMap& eventData)
        {
            float timeStep=eventData[Update::P_TIMESTEP].GetFloat();
            framecount_++;
            time_+=timeStep;

            float MOVE_SPEED=10.0f;
            const float MOUSE_SENSITIVITY=0.1f;

            Input* input=GetSubsystem<Input>();
            if(input->GetQualifierDown(1))  // 1 is shift, 2 is ctrl, 4 is alt
                MOVE_SPEED*=10;
            if(input->GetKeyDown('W'))
                cameraNode_->Translate(Vector3(0,0, 1)*MOVE_SPEED*timeStep);
            if(input->GetKeyDown('S'))
                cameraNode_->Translate(Vector3(0,0,-1)*MOVE_SPEED*timeStep);
            if(input->GetKeyDown('A'))
                cameraNode_->Translate(Vector3(-1,0,0)*MOVE_SPEED*timeStep);
            if(input->GetKeyDown('D'))
                cameraNode_->Translate(Vector3( 1,0,0)*MOVE_SPEED*timeStep);

            if(!GetSubsystem<Input>()->IsMouseVisible())
            {
                IntVector2 mouseMove=input->GetMouseMove();
                static float yaw_=0;
                static float pitch_=0;
                yaw_+=MOUSE_SENSITIVITY*mouseMove.x_;
                pitch_+=MOUSE_SENSITIVITY*mouseMove.y_;
                pitch_=Clamp(pitch_,-90.0f,90.0f);
                cameraNode_->SetDirection(Vector3::FORWARD);
                cameraNode_->Yaw(yaw_);
                cameraNode_->Pitch(pitch_);
            }
        }
        /**
        * Anything in the non-rendering logic that requires a second pass,
        * it might be well suited to be handled here.
        */
        void HandlePostUpdate(StringHash eventType,VariantMap& eventData)
        {
            // We really don't have anything useful to do here for this example.
            // Probably shouldn't be subscribing to events we don't care about.
        }
        /**
        * If you have any details you want to change before the viewport is
        * rendered, try putting it here.
        * See http://urho3d.github.io/documentation/1.32/_rendering.html
        * for details on how the rendering pipeline is setup.
        */
        void HandleRenderUpdate(StringHash eventType, VariantMap & eventData)
        {
            // We really don't have anything useful to do here for this example.
            // Probably shouldn't be subscribing to events we don't care about.
        }
        /**
        * After everything is rendered, there might still be things you wish
        * to add to the rendering. At this point you cannot modify the scene,
        * only post rendering is allowed. Good for adding things like debug
        * artifacts on screen or brush up lighting, etc.
        */
        void HandlePostRenderUpdate(StringHash eventType, VariantMap & eventData)
        {
            // We could draw some debuggy looking thing for the octree.
            // scene_->GetComponent<Octree>()->DrawDebugGeometry(true);
        }
        /**
        * All good things must come to an end.
        */
        void HandleEndFrame(StringHash eventType,VariantMap& eventData)
        {
            // We really don't have anything useful to do here for this example.
            // Probably shouldn't be subscribing to events we don't care about.
        }
};

/**
* This macro is expanded to (roughly, depending on OS) this:
*
* > int RunApplication()
* > {
* > Urho3D::SharedPtr<Urho3D::Context> context(new Urho3D::Context());
* > Urho3D::SharedPtr<className> application(new className(context));
* > return application->Run();
* > }
* >
* > int main(int argc, char** argv)
* > {
* > Urho3D::ParseArguments(argc, argv);
* > return function;
* > }
*/
URHO3D_DEFINE_APPLICATION_MAIN(MyApp)