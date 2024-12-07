#include "GameLevel.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QIODevice>

GameLevel::GameLevel()
    : world(b2Vec2(0.0f, -10.0f)), worldScale(20.0f), joint(&world), hailLevel(0), earthquakeLevel(0) {
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
    joint.update();
}

std::vector<Placeable>& GameLevel::getPlaceables()
{
    return placeables;
}

std::vector<b2Body*>& GameLevel::getGroundBodies()
{
    return groundBodies;
}

void GameLevel::destroyBody(b2Body* body)
{
    if (body)
    {
        joint.deleteJointsForBody(body);
        world.DestroyBody(body);
    }
}

void GameLevel::destroyGround(b2Body* groundBody)
{
    auto it = std::find(groundBodies.begin(), groundBodies.end(), groundBody);
    if (it != groundBodies.end())
    {
        world.DestroyBody(*it); // Destroy the body in the Box2D world
        groundBodies.erase(it); // Remove it from the list
    }
}

float GameLevel::getWorldScale() const
{
    return worldScale;
}

Joint& GameLevel::getJoint()
{
    return joint;
}

bool GameLevel::saveLevel(const QString& filename) const
{
    QJsonObject levelObject;

    QJsonArray placeablesArray;
    for (const auto& placeable : placeables)
    {
        placeablesArray.append(placeable.toJson());
    }
    levelObject["placeables"] = placeablesArray;

    QJsonArray groundsArray;
    for (const b2Body* groundBody : groundBodies)
    {
        QJsonObject groundObj;
        b2Vec2 position = groundBody->GetPosition();
        groundObj["posX"] = position.x;
        groundObj["posY"] = position.y;

        const b2PolygonShape* shape = static_cast<const b2PolygonShape*>(groundBody->GetFixtureList()->GetShape());
        float width = std::abs(shape->m_vertices[2].x - shape->m_vertices[0].x);
        float height = std::abs(shape->m_vertices[2].y - shape->m_vertices[0].y);

        groundObj["width"] = width;
        groundObj["height"] = height;

        groundsArray.append(groundObj);
    }
    levelObject["grounds"] = groundsArray;
    levelObject["hailLevel"] = hailLevel;
    levelObject["earthquakeLevel"] = earthquakeLevel;

    QJsonDocument doc(levelObject);
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Failed to open file for writing:" << filename;
        return false;
    }
    file.write(doc.toJson());
    file.close();
    return true;
}

bool GameLevel::loadLevel(const QString& filename)
{
    clearLevel();

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Failed to open file for reading:" << filename;
        return false;
    }
    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject())
    {
        qDebug() << "Invalid JSON document in file:" << filename;
        return false;
    }
    QJsonObject levelObject = doc.object();

    QJsonArray groundsArray = levelObject["grounds"].toArray();
    for (const QJsonValue& value : groundsArray)
    {
        QJsonObject groundObj = value.toObject();
        float posX = groundObj["posX"].toDouble();
        float posY = groundObj["posY"].toDouble();
        float width = groundObj["width"].toDouble();
        float height = groundObj["height"].toDouble();
        createGround(posX, posY, width, height);
    }

    QJsonArray placeablesArray = levelObject["placeables"].toArray();
    for (const QJsonValue& value : placeablesArray)
    {
        QJsonObject placeableObj = value.toObject();
        Placeable placeable = Placeable::fromJson(placeableObj);

        createDynamicBody(placeable, placeable.getPosX(), placeable.getPosY());
    }

    if (levelObject.contains("hailLevel"))
        hailLevel = levelObject["hailLevel"].toInt();
    else
        hailLevel = 0;

    if (levelObject.contains("earthquakeLevel"))
        earthquakeLevel = levelObject["earthquakeLevel"].toInt();
    else
        earthquakeLevel = 0;


    return true;
}

void GameLevel::clearLevel()
{
    for (b2Body* body = world.GetBodyList(); body != nullptr;)
    {
        b2Body* nextBody = body->GetNext();
        world.DestroyBody(body);
        body = nextBody;
    }

    groundBodies.clear();
    placeables.clear();
}

void GameLevel::setHailLevel(int level)
{
    hailLevel = level;
}
int GameLevel::getHailLevel() const
{
    return hailLevel;
}

void GameLevel::setEarthquakeLevel(int level)
{
    earthquakeLevel = level;
}
int GameLevel::getEarthquakeLevel() const
{
    return earthquakeLevel;
}
