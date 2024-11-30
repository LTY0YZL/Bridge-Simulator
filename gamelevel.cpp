#include "GameLevel.h"
#include <QDebug>

GameLevel::GameLevel()
    : world(b2Vec2(0.0f, -10.0f)), worldScale(20.0f) {
    qDebug() << "GameLevel initialized with worldScale:" << worldScale;
}

GameLevel::~GameLevel()
{
    for (b2Body* body : groundBodies)
    {
        world.DestroyBody(body);
    }
}

void GameLevel::initialize()
{
    // Create the ground
    createGround(0.0f, 5.0f, 20.0f, 2.0f);
    createGround(0.0f, -5.0f, 1.0f,50.0f);

    // Add some predefined placeables for testing directly here
    Placeable box1("Box1", 100,Qt::blue, 2.0f, 2.0f, 1.0f, 0.3f, 0.2f);
    createDynamicBody(box1, -10.0f, 20.0f); // Box1 at (-10, 20)

    Placeable box2("Box2",100, Qt::red, 3.0f, 1.5f, 1.0f, 0.3f, 0.2f);
    createDynamicBody(box2, 5.0f, 15.0f); // Box2 at (5, 15)

    Placeable box3("Box3",100, Qt::green, 1.0f, 1.0f, 1.0f, 0.5f, 0.4f);
    createDynamicBody(box3, 0.0f, 10.0f); // Box3 at (0, 10)
}

void GameLevel::createGround(float posX, float posY, float width, float height)
{
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(posX, posY);

    b2Body* newGroundBody = world.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(width / 2.0f, height / 2.0f);

    newGroundBody->CreateFixture(&groundBox, 0.0f);

    // Add the ground body to the vector
    groundBodies.push_back(newGroundBody);
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

std::vector<Placeable>& GameLevel::getPlaceables()
{
    return placeables;
}

std::vector<b2Body*>& GameLevel::getGroundBodies()
{

    return groundBodies;

}

float GameLevel::getWorldScale() const
{
    return worldScale;
}
