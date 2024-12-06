#ifndef PLACEABLE_H
#define PLACEABLE_H

#include <QString>
#include <QColor>
#include <Box2D/Box2D.h>
#include <QJsonObject>
#include <QDateTime>

class Placeable
{
public:
    Placeable(const QString& name,
              const int cost = 50,
              const QColor& color= Qt::blue,
              float width = 1.0f,
              float height = 1.0f,
              float density = 0.5f,
              float friction = 0.5f,
              float restitution = 0.5f);

    // Method to create a Box2D body in the given world
    b2Body* createBody( b2World* world, float posX, float posY);

    // Accessors
    int getCost() const;
    QString getName() const;
    QColor getColor() const;
    b2Body* getBody() const;

    // Mutators
    void setName(const QString& name);
    void setDisplayColor(QColor color);
    void setPosition(float x, float y);

    // Assign Box2D body to Placeable
    void assignBody(b2Body* body);

    // Serialization methods
    QJsonObject toJson() const;
    static Placeable fromJson(const QJsonObject& obj);

    // Accessors for position
    float getPosX() const;
    float getPosY() const;

    //hail tracking
    void setAsHail(bool hail);
    bool isHailStone() const;

    void setCreationTime(const QDateTime& time);
    QDateTime getCreationTime() const;

private:
    int cost;
    QString name;       // Name of the object
    QColor color;       // Color of the object
    QColor displayColor;// Color for display
    float width;        // Width of the object
    float height;       // Height of the object
    float density;      // Density for Box2D fixture
    float friction;     // Friction for Box2D fixture
    float restitution;  // Restitution (bounciness) for Box2D fixture
    b2Body* body;       // Pointer to the associated Box2D body
    float posX;         // Position X for serialization
    float posY;         // Position Y for serialization
    bool hail;          // Indicates hail
    QDateTime creationTime;
};

#endif // PLACEABLE_H
