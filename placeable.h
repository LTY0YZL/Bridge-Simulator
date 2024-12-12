#ifndef PLACEABLE_H
#define PLACEABLE_H

#include <QString>
#include <QColor>
#include <QJsonObject>
#include <QDateTime>
#include <Box2D/Box2D.h>

class Placeable
{
public:
    // Constructor
    Placeable(const QString &name,
              int cost = 50,
              const QColor &color = Qt::blue,
              float width = 1.0f,
              float height = 1.0f,
              float density = 0.5f,
              float friction = 0.5f,
              float restitution = 0.5f);

    // Box2D body creation
    b2Body* createBody(b2World *world, float posX, float posY, float rotation);

    // Accessors
    int getCost() const;
    QString getName() const;
    QColor getColor() const;
    QColor getOriginalColor() const;
    b2Body* getBody() const;
    float getWidth() const;
    float getHeight() const;
    float getPosX() const;
    float getPosY() const;

    // Mutators
    void setName(const QString &name);
    void setDisplayColor(QColor color);
    void setPosition(float x, float y);
    void setRotation(float angle);
    void assignBody(b2Body *body);

    // Serialization
    QJsonObject toJson() const;
    static Placeable fromJson(const QJsonObject &obj);

    // Hail-related methods
    void setAsHail(bool hail);
    bool isHailStone() const;

    // Creation time tracking
    void setCreationTime(const QDateTime &time);
    QDateTime getCreationTime() const;

    // ID handling
    void setID(int id);
    int getID() const;

private:
    int cost;
    QString name;
    QColor color;
    QColor displayColor;

    float width;
    float height;
    float density;
    float friction;
    float restitution;

    b2Body *body;

    float posX;
    float posY;

    bool hail;
    QDateTime creationTime;
    int id;
};

#endif // PLACEABLE_H
