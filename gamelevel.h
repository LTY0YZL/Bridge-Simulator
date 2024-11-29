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
    void createGround(float width, float height);
    b2Body* createDynamicBody(Placeable placeable, float posX, float posY);
    void stepWorld(float timeStep, int velocityIterations, int positionIterations);

    const std::vector<Placeable>& getPlaceables() const;
    b2Body* getGroundBody() const;

    // Add getter for world scale
    float getWorldScale() const;

private:
    b2World world;
    b2Body* groundBody;
    std::vector<Placeable> placeables;

    float worldScale; // Scaling factor for Box2D world
};

#endif // GAMELEVEL_H
