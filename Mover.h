//
// Copyright (c) 2008-2017 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#pragma once

#include <Urho3D/Scene/LogicComponent.h>

using namespace Urho3D;

/// Custom logic component for moving the animated model and rotating at area edges.
class Mover : public LogicComponent
{
    URHO3D_OBJECT(Mover, LogicComponent);
    
public:
    /// Construct.
    Mover(Context* context) :
            LogicComponent(context),
            moveSpeed_(0.0f),
            rotationSpeed_(0.0f)
    {
        // Only the scene update event is needed: unsubscribe from the rest for optimization
        SetUpdateEventMask(USE_UPDATE);
    }
    
    /// Set motion parameters: forward movement speed, rotation speed, and movement boundaries.
    //void SetParameters(float moveSpeed, float rotateSpeed, const BoundingBox& bounds);

    void SetParameters(float moveSpeed, float rotationSpeed, const BoundingBox& bounds)
    {
        moveSpeed_ = moveSpeed;
        rotationSpeed_ = rotationSpeed;
        bounds_ = bounds;
    }

    /// Handle scene update. Called by LogicComponent base class.
    void Update(float timeStep)
    {
        node_->Translate(Vector3::FORWARD * moveSpeed_ * timeStep);

        // If in risk of going outside the plane, rotate the model right
        Vector3 pos = node_->GetPosition();
        if (pos.x_ < bounds_.min_.x_ || pos.x_ > bounds_.max_.x_ || pos.z_ < bounds_.min_.z_ || pos.z_ > bounds_.max_.z_)
            node_->Yaw(rotationSpeed_ * timeStep);

        // Get the model's first (only) animation state and advance its time. Note the convenience accessor to other components
        // in the same scene node
        AnimatedModel* model = node_->GetComponent<AnimatedModel>(true);
        if (model->GetNumAnimationStates())
        {
            AnimationState* state = model->GetAnimationStates()[0];
            state->AddTime(timeStep);
        }
    }
    
    /// Return forward movement speed.
    float GetMoveSpeed() const { return moveSpeed_; }
    /// Return rotation speed.
    float GetRotationSpeed() const { return rotationSpeed_; }
    /// Return movement boundaries.
    const BoundingBox& GetBounds() const { return bounds_; }
    
private:
    /// Forward movement speed.
    float moveSpeed_;
    /// Rotation speed.
    float rotationSpeed_;
    /// Movement boundaries.
    BoundingBox bounds_;
};
