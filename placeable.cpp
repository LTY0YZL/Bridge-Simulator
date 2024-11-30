#include "Placeable.h"

Placeable::Placeable(const QString& name, const int,
                     const QColor& color,
                     float width,
                     float height,
                     float density,
                     float friction,
                     float restitution)
    : cost(cost), name(name), color(color), displayColor(color),
    width(width),height(height), density(density), friction(friction), restitution(restitution),body(nullptr) {}

void Placeable::assignBody(b2Body* body)
{
    this->body = body;
}
int Placeable::getCost() const
{
    return cost;
}
b2Body* Placeable::getBody() const
{
    return body;
}

QString Placeable::getName() const
{
    return name;
}

QColor Placeable::getColor() const
{
    return displayColor;
}

void Placeable::setName(const QString& name)
{
    this->name = name;
}

void Placeable::setDisplayColor( QColor color)
{
    this->displayColor = color;
}

// Method to create a Box2D body
b2Body* Placeable::createBody(b2World* world, float posX, float posY)
{
    // Define the body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(posX, posY);

    b2Body* newBody = world->CreateBody(&bodyDef);

    // Define the box shape
    b2PolygonShape boxShape;
    boxShape.SetAsBox(width / 2.0f, height / 2.0f);

    // Define the fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.density = density; // Weight
    fixtureDef.friction = friction;
    fixtureDef.restitution = restitution; // Bounceyness

    // Attach the fixture to the body
    newBody->CreateFixture(&fixtureDef);

    // Assign the created body to this Placeable
    assignBody(newBody);

    return newBody;
}

