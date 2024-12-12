#ifndef JOINT_H
#define JOINT_H

#include <Box2D/Box2D.h>
#include <QDebug>
#include <QPainter>
#include <vector>

#include "Placeable.h"

class Joint
{
public:
    struct JointData {
        b2Joint* joint;
        float maxForce; // The maximum force that this joint can handle before breaking
    };

    explicit Joint(b2World* world);

    // Connection methods
    b2Joint* connectRevoluteJoint(b2Body* bodyA, b2Body* bodyB, const b2Vec2 &anchorA, const b2Vec2 &anchorB);
    b2Joint* connectDistanceJoint(Placeable &bodyA, Placeable &bodyB, const b2Vec2 &anchorA, const b2Vec2 &anchorB,
                                  float frequencyHz, float dampingRatio, float maxForce);

    // Accessors
    std::vector<b2Joint*>& getJoints() const;
    const std::vector<JointData>& getJointsData() const;

    // Joint management
    void deleteJointsForBody(b2Body* body);
    void update();         // Update joints and handle breaking logic
    void deleteAllJoints();

private:
    b2World* world;
    std::vector<JointData> joints;
};

#endif // JOINT_H
