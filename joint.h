#ifndef JOINT_H
#define JOINT_H

#include <Box2D/Box2D.h>
#include <QDebug>
#include "Placeable.h"

class Joint
{
public:
    Joint(b2World* world);

    // Method to connect two Placeable objects using a Box2D RevoluteJoint
    b2Joint* connectRevoluteJoint(Placeable& bodyA, Placeable& bodyB, const b2Vec2& anchorA, const b2Vec2& anchorB);

    // Method to connect two Placeable objects using a Box2D DistanceJoint
    b2Joint* connectDistanceJoint(Placeable& bodyA, Placeable& bodyB, const b2Vec2& anchorA, const b2Vec2& anchorB, float frequencyHz, float dampingRatio, float maxForce);

    // Method to update joints and handle breaking logic
    void update();

private:
    b2World* world;
    struct JointData {
        b2Joint* joint;
        float maxForce; // The maximum force that this joint can handle before breaking
    };
    std::vector<JointData> joints;
};

#endif // JOINT_H
