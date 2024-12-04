#include "Joint.h"

Joint::Joint(b2World* world) : world(world) {}

// Method to connect two Placeable objects using a Box2D RevoluteJoint
b2Joint* Joint::connectRevoluteJoint(Placeable& bodyA, Placeable& bodyB, const b2Vec2& anchorA, const b2Vec2& anchorB)
{
    if (bodyA.getBody() == nullptr || bodyB.getBody() == nullptr)
    {
        qDebug() << "One or both bodies are not initialized.";
        return nullptr;
    }

    b2RevoluteJointDef jointDef;
    jointDef.bodyA = bodyA.getBody();
    jointDef.bodyB = bodyB.getBody();
    jointDef.localAnchorA = anchorA;
    jointDef.localAnchorB = anchorB;

    b2Joint* joint = world->CreateJoint(&jointDef);
    joints.push_back({ joint, std::numeric_limits<float>::max() });
    return joint;
}

// Method to connect two Placeable objects using a Box2D DistanceJoint
b2Joint* Joint::connectDistanceJoint(Placeable& bodyA, Placeable& bodyB, const b2Vec2& anchorA, const b2Vec2& anchorB, float frequencyHz, float dampingRatio, float maxForce)
{
    if (bodyA.getBody() == nullptr || bodyB.getBody() == nullptr)
    {
        qDebug() << "One or both bodies are not initialized.";
        return nullptr;
    }

    b2DistanceJointDef jointDef;
    jointDef.bodyA = bodyA.getBody();
    jointDef.bodyB = bodyB.getBody();
    jointDef.localAnchorA = anchorA;
    jointDef.localAnchorB = anchorB;

    // b2Vec2 worldAnchorA = bodyA.getBody()->GetWorldPoint(anchorA);
    // b2Vec2 worldAnchorB = bodyB.getBody()->GetWorldPoint(anchorB);
    // jointDef.length = (worldAnchorB - worldAnchorA).Length();

    jointDef.length = (anchorA - anchorB).Length();

    jointDef.frequencyHz = frequencyHz; // Stiffness
    jointDef.dampingRatio = dampingRatio; // Damping (resistance)

    b2Joint* joint = world->CreateJoint(&jointDef);
    joints.push_back({ joint, maxForce });
    return joint;
}

// Method to update joints and handle breaking logic
void Joint::update()
{
    for (auto it = joints.begin(); it != joints.end();)
    {
        b2Joint* joint = it->joint;
        float maxForce = it->maxForce;

        if (b2DistanceJoint* distanceJoint = dynamic_cast<b2DistanceJoint*>(joint))
        {
            // Check the reaction force of the joint
            float forceMagnitude = distanceJoint->GetReactionForce(1.0f).Length();
            if (forceMagnitude > maxForce)
            {
                // Destroy the joint if the force exceeds the threshold
                qDebug() << "Joint has broken due to exceeding force threshold.";
                world->DestroyJoint(joint);
                it = joints.erase(it); // Remove the joint from the list
                continue;
            }
        }
        ++it;
    }
}
