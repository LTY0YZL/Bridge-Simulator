#ifndef GAMELEVEL_H
#define GAMELEVEL_H

#include <Box2D/Box2D.h>
#include <vector>
#include "Placeable.h"
#include "joint.h"

class GameLevel
{
public:
    GameLevel();
    ~GameLevel();

    void initialize();
    b2Body* createGround(float posX, float posY, float width, float height);
    b2Body* createDynamicBody(Placeable placeable, float posX, float posY);
    void stepWorld(float timeStep, int velocityIterations, int positionIterations);
    void destroyBody(b2Body* body);
    void destroyGround(b2Body* groundBody);
    std::vector<Placeable>& getPlaceables();
    std::vector<b2Body*>& getGroundBodies();
    b2Body* getGroundBody() const;

    float getWorldScale() const;

    Joint& getJoint();

    bool saveLevel(const QString& filename) const;
    bool loadLevel(const QString& filename);

    void setHailLevel(int level);
    int getHailLevel() const;

    void setEarthquakeLevel(int level);
    int getEarthquakeLevel() const;

    int getPlaceableIDForBody(b2Body* body) const;
    b2Body* getBodyForPlaceableID(int id) const;

private:
    b2World world;
    std::vector<b2Body*> groundBodies;
    std::vector<Placeable> placeables;

    float worldScale; // Scaling factor for Box2D world
    void clearLevel();
    Joint joint;

    int hailLevel;
    int earthquakeLevel;

    int nextPlaceableID;
};

#endif // GAMELEVEL_H
