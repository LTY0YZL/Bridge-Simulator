#include "Placeable.h"
#include <QDateTime>

Placeable::Placeable(const QString& name, const int,
                     const QColor& color,
                     float width,
                     float height,
                     float density,
                     float friction,
                     float restitution)
    : cost(cost), name(name), color(color), displayColor(color),
    width(width),height(height), density(density), friction(friction), restitution(restitution),body(nullptr), posX(0.0f), posY(0.0f), hail(false), id(-1) {}

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
    // Store the position in the member variables
    this->posX = posX;
    this->posY = posY;

    // Define the body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(posX, posY);

    b2Body* newBody = world->CreateBody(&bodyDef);

    // If this is hail, use a circular shape
    if (hail && name == "Hail") {
        b2CircleShape circle;
        // radius is half the width if we assume width=height for hail
        circle.m_radius = width / 2.0f;

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &circle;
        fixtureDef.density = density;
        fixtureDef.friction = friction;
        fixtureDef.restitution = restitution;

        newBody->CreateFixture(&fixtureDef);
    } else {
        // Default to a box shape for non-hail
        b2PolygonShape boxShape;
        boxShape.SetAsBox(width / 2.0f, height / 2.0f);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &boxShape;
        fixtureDef.density = density;
        fixtureDef.friction = friction;
        fixtureDef.restitution = restitution;

        newBody->CreateFixture(&fixtureDef);
    }

    // Assign the created body to this Placeable
    assignBody(newBody);

    return newBody;
}

QJsonObject Placeable::toJson() const
{
    QJsonObject obj;
    obj["name"] = name;
    obj["cost"] = cost;
    obj["color"] = color.name();
    obj["width"] = width;
    obj["height"] = height;
    obj["density"] = density;
    obj["friction"] = friction;
    obj["restitution"] = restitution;
    obj["posX"] = posX;
    obj["posY"] = posY;
    obj["isHail"] = hail;
    obj["id"] = id;
    return obj;
}

Placeable Placeable::fromJson(const QJsonObject& obj)
{
    QString name = obj["name"].toString();
    int cost = obj["cost"].toInt();
    QColor color(obj["color"].toString());
    float width = obj["width"].toDouble();
    float height = obj["height"].toDouble();
    float density = obj["density"].toDouble();
    float friction = obj["friction"].toDouble();
    float restitution = obj["restitution"].toDouble();
    float posX = obj["posX"].toDouble();
    float posY = obj["posY"].toDouble();

    Placeable placeable(name, cost, color, width, height, density, friction, restitution);
    placeable.setPosition(posX, posY);

    if (obj.contains("isHail")) {
        bool isHail = obj["isHail"].toBool();
        placeable.setAsHail(isHail);
    }

    if (obj.contains("id")) {
        placeable.setID(obj["id"].toInt());
    }

    return placeable;
}

void Placeable::setPosition(float x, float y)
{
    posX = x;
    posY = y;
}

float Placeable::getPosX() const
{
    return posX;
}

float Placeable::getPosY() const
{
    return posY;
}

void Placeable::setAsHail(bool hailValue)
{
    hail = hailValue;
}

bool Placeable::isHailStone() const
{
    return hail;
}

void Placeable::setCreationTime(const QDateTime& time)
{
    creationTime = time;
}

QDateTime Placeable::getCreationTime() const
{
    return creationTime;
}
float Placeable::getWidth() const
{
    return width;
}
float Placeable::getHeight() const
{
    return height;
}

void Placeable::setID(int idValue)
{
    id = idValue;
}

int Placeable::getID() const
{
    return id;
}
