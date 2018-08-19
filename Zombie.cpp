#include <Urho3D/Urho3D.h>
#include <Urho3D/Core/Context.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/Scene/Component.h>
#include <Urho3D/Scene/Node.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Audio/Audio.h>
#include <Urho3D/Audio/AudioEvents.h>
#include <Urho3D/Audio/Sound.h>
#include <Urho3D/Audio/SoundSource.h>
#include "Urho3D/IO/Log.h"
#include "Zombie.h"

Zombie::Zombie(Context *context) : GameObject(context) {}
Zombie::~Zombie() {}

void Zombie::RegisterObject(Context *context) {
    context->RegisterFactory<Zombie>();
}

void Zombie::GotHit() {
    hitCount--;

    if (hitCount <=0) {
        showZombieDies();
        node_->Remove();
    } else {
        showZombieGotHit();
    }
}

void Zombie::showZombieDies() {
    // todo play "die" sound

    // show "die" animation
}

void Zombie::showZombieGotHit() {
    // todo add blood particle

    const String& soundResourceName = "assets/Music/zombie_hit.wav";

    // Get the sound resource
    ResourceCache* cache = GetSubsystem<ResourceCache>();
    Sound* sound = cache->GetResource<Sound>(soundResourceName);

    if (sound)
    {
        SoundSource* soundSource = GetScene()->CreateComponent<SoundSource>();
        soundSource->SetAutoRemoveMode(REMOVE_COMPONENT);
        soundSource->Play(sound);
        soundSource->SetGain(0.75f);
    }
}