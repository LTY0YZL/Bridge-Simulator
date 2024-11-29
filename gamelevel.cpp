#include "GameLevel.h"

GameLevel::GameLevel()
    : world(b2Vec2(0.0f, -10.0f)), groundBody(nullptr), worldScale(20.0f) {}

GameLevel::~GameLevel()
{
}

void GameLevel::initialize()
{
    // Create the ground
    createGround(100.0f, 10.0f);

    // Add some predefined placeables for testing directly here
    Placeable box1("Box1", Qt::blue, 2.0f, 2.0f, 1.0f, 0.3f, 0.2f);
    createDynamicBody(box1, -10.0f, 20.0f); // Box1 at (-10, 20)

    Placeable box2("Box2", Qt::red, 3.0f, 1.5f, 1.0f, 0.3f, 0.2f);
    createDynamicBody(box2, 5.0f, 15.0f); // Box2 at (5, 15)

    Placeable box3("Box3", Qt::green, 1.0f, 1.0f, 1.0f, 0.5f, 0.4f);
    createDynamicBody(box3, 0.0f, 10.0f); // Box3 at (0, 10)
}

void GameLevel::createGround(float width, float height)
{
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, 0.0f);
    groundBody = world.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(width / 2.0f, height / 2.0f);

    groundBody->CreateFixture(&groundBox, 0.0f);
}

b2Body* GameLevel::createDynamicBody(Placeable placeable, float posX, float posY)
{
    // Use Placeable's createBody to create a Box2D body
    b2Body* body = placeable.createBody(&world, posX, posY);

    // Add the Placeable to the list
    placeables.push_back(placeable);

    return body;
}

void GameLevel::stepWorld(float timeStep, int velocityIterations, int positionIterations)
{
    world.Step(timeStep, velocityIterations, positionIterations);
}

const std::vector<Placeable>& GameLevel::getPlaceables() const
{
    return placeables;
}

b2Body* GameLevel::getGroundBody() const
{
    return groundBody;
}

float GameLevel::getWorldScale() const
{
    return worldScale;
}
