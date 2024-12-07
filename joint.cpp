#include "Joint.h"

Joint::Joint(b2World* world) : world(world) {}

// Method to connect two Placeable objects using a Box2D RevoluteJoint
b2Joint* Joint::connectRevoluteJoint(b2Body* bodyA, b2Body* bodyB, const b2Vec2& anchorA, const b2Vec2& anchorB)
{
    if (!bodyA || !bodyB)
    {
        qDebug() << "One or both bodies are not initialized.";
        return nullptr;
    }

    b2RevoluteJointDef jointDef;
    jointDef.bodyA = bodyA;
    jointDef.bodyB = bodyB;
    jointDef.localAnchorA = bodyA->GetLocalPoint(anchorA);
    jointDef.localAnchorB = bodyB->GetLocalPoint(anchorB);

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

    jointDef.localAnchorA = bodyA.getBody()->GetLocalPoint(anchorA);
    jointDef.localAnchorB = bodyB.getBody()->GetLocalPoint(anchorB);

    jointDef.length = (anchorA - anchorB).Length();

    jointDef.frequencyHz = frequencyHz; // Stiffness
    jointDef.dampingRatio = dampingRatio; // Damping (resistance)

    b2Joint* joint = world->CreateJoint(&jointDef);
    joints.push_back({ joint, maxForce });
    return joint;
}

// Method to get all joints for drawing or other purposes
std::vector<b2Joint*>& Joint::getJoints() const
{
    static std::vector<b2Joint*> jointList;
    jointList.clear();

    for (const auto& jointData : joints)
    {
        jointList.push_back(jointData.joint);
    }

    return jointList;
}

void Joint::deleteJointsForBody(b2Body* body)
{
    for (auto it = joints.begin(); it != joints.end();)
    {
        b2Joint* joint = it->joint;

        if (joint->GetBodyA() == body || joint->GetBodyB() == body)
        {
            qDebug() << "Deleting joint connected to body at" << body;
            world->DestroyJoint(joint);
            it = joints.erase(it);
        }
        else
        {
            ++it;
        }
    }
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

void Joint::deleteAllJoints() {
    for (auto& jd : joints) {
        world->DestroyJoint(jd.joint);
    }
    joints.clear();
}
