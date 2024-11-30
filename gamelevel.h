#ifndef GAMELEVEL_H
#define GAMELEVEL_H

#include <Box2D/Box2D.h>
#include <vector>
#include "Placeable.h"

class GameLevel
{
public:
    GameLevel();
    ~GameLevel();

    void initialize();
    void createGround(float posX, float posY, float width, float height);
    b2Body* createDynamicBody(Placeable placeable, float posX, float posY);
    void stepWorld(float timeStep, int velocityIterations, int positionIterations);

    std::vector<Placeable>& getPlaceables();
    std::vector<b2Body*>& getGroundBodies();
    b2Body* getGroundBody() const;

    float getWorldScale() const;

private:
    b2World world;
    std::vector<b2Body*> groundBodies;
    std::vector<Placeable> placeables;

    float worldScale; // Scaling factor for Box2D world
};

#endif // GAMELEVEL_H
