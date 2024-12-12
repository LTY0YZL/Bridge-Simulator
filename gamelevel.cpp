#include "GameLevel.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QIODevice>

GameLevel::GameLevel()
    : world(b2Vec2(0.0f, -10.0f)),
    worldScale(20.0f),
    joint(&world),
    hailLevel(0),
    earthquakeLevel(0)
{
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
    createGround(0.0f, -5.0f, 1.0f, 50.0f);

    // Add some predefined placeables for testing directly here
    Placeable box1("Box1", 100, Qt::blue, 2.0f, 2.0f, 1.0f, 0.3f, 0.2f);
    createDynamicBody(box1, -10.0f, 20.0f); // Box1 at (-10, 20)

    Placeable box2("Box2", 100, Qt::red, 3.0f, 1.5f, 1.0f, 0.3f, 0.2f);
    createDynamicBody(box2, 5.0f, 15.0f); // Box2 at (5, 15)

    Placeable box3("Box3", 100, Qt::green, 1.0f, 1.0f, 1.0f, 0.5f, 0.4f);
    createDynamicBody(box3, 0.0f, 10.0f); // Box3 at (0, 10)
}

b2Body* GameLevel::createGround(float posX, float posY, float width, float height)
{
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(posX, posY);

    b2Body* newGroundBody = world.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(width / 2.0f, height / 2.0f);

    newGroundBody->CreateFixture(&groundBox, 0.0f);

    // Add the ground body to the vector
    groundBodies.push_back(newGroundBody);

    return newGroundBody;
}

b2Body* GameLevel::createDynamicBody(Placeable placeable, float posX, float posY, float rotation)
{
    placeable.setID(nextPlaceableID++);
    // Use Placeable's createBody to create a Box2D body
    b2Body* body = placeable.createBody(&world, posX, posY, rotation);

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
    QMap<b2Body*, int> bodyToIDMap;
    int nextID = 0;

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
        bodyToIDMap[const_cast<b2Body*>(groundBody)] = nextID++;
    }

    QJsonArray placeablesArray;
    for (const auto& placeable : placeables)
    {
        placeablesArray.append(placeable.toJson());
        bodyToIDMap[placeable.getBody()] = nextID++;
    }
    levelObject["placeables"] = placeablesArray;

    levelObject["grounds"] = groundsArray;
    levelObject["hailLevel"] = hailLevel;
    levelObject["earthquakeLevel"] = earthquakeLevel;

    // Save joints
    QJsonArray jointsArray;
    for (const Joint::JointData& jd : joint.getJointsData())
    {
        QJsonObject jointObj;
        b2Joint* jointPtr = jd.joint;

        b2Body* bodyA = jointPtr->GetBodyA();
        b2Body* bodyB = jointPtr->GetBodyB();

        jointObj["bodyA_id"] = bodyToIDMap.value(bodyA, -1);
        jointObj["bodyB_id"] = bodyToIDMap.value(bodyB, -1);

        jointObj["type"] = static_cast<int>(jointPtr->GetType());
        jointObj["maxForce"] = jd.maxForce;

        b2Vec2 anchorA = jointPtr->GetAnchorA();
        b2Vec2 anchorB = jointPtr->GetAnchorB();
        jointObj["anchorAX"] = anchorA.x;
        jointObj["anchorAY"] = anchorA.y;
        jointObj["anchorBX"] = anchorB.x;
        jointObj["anchorBY"] = anchorB.y;

        if (jointPtr->GetType() == e_distanceJoint)
        {
            const b2DistanceJoint* distanceJoint = static_cast<const b2DistanceJoint*>(jointPtr);
            jointObj["length"] = distanceJoint->GetLength();
            jointObj["frequencyHz"] = distanceJoint->GetFrequency();
            jointObj["dampingRatio"] = distanceJoint->GetDampingRatio();
        }

        jointsArray.append(jointObj);
    }
    levelObject["joints"] = jointsArray;

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
    QMap<int, b2Body*> bodyMap;
    int groundID = bodyMap.size();

    QJsonArray groundsArray = levelObject["grounds"].toArray();
    for (const QJsonValue& value : groundsArray)
    {
        QJsonObject groundObj = value.toObject();
        float posX = groundObj["posX"].toDouble();
        float posY = groundObj["posY"].toDouble();
        float width = groundObj["width"].toDouble();
        float height = groundObj["height"].toDouble();
        b2Body* groundBody = createGround(posX, posY, width, height);

        bodyMap[groundID] = groundBody;
        groundID++;
    }

    QJsonArray placeablesArray = levelObject["placeables"].toArray();
    placeables.clear();
    nextPlaceableID = bodyMap.size();
    for (const QJsonValue& value : placeablesArray)
    {
        QJsonObject placeableObj = value.toObject();
        Placeable placeable = Placeable::fromJson(placeableObj);

        float rotation = placeableObj.contains("rotation") ? placeableObj["rotation"].toDouble() : 0.0f;

        b2Body* dynamicBody = createDynamicBody(placeable, placeable.getPosX(), placeable.getPosY(), rotation);
        bodyMap[placeable.getID()] = dynamicBody;
    }

    if (levelObject.contains("hailLevel"))
        hailLevel = levelObject["hailLevel"].toInt();
    else
        hailLevel = 0;

    if (levelObject.contains("earthquakeLevel"))
        earthquakeLevel = levelObject["earthquakeLevel"].toInt();
    else
        earthquakeLevel = 0;

    for (auto& p : placeables)
    {
        bodyMap.insert(p.getID(), p.getBody());
    }

    if (levelObject.contains("joints"))
    {
        qDebug() << "try connect";
        QJsonArray jointsArray = levelObject["joints"].toArray();
        for (const QJsonValue& val : jointsArray)
        {
            QJsonObject jointObj = val.toObject();

            int idA = jointObj["bodyA_id"].toInt();
            int idB = jointObj["bodyB_id"].toInt();
            b2Body* bodyA = bodyMap.value(idA, nullptr);
            b2Body* bodyB = bodyMap.value(idB, nullptr);
            if (!bodyA || !bodyB || bodyA == bodyB) continue;

            b2Vec2 anchorA(
                (float)jointObj["anchorAX"].toDouble(),
                (float)jointObj["anchorAY"].toDouble()
                );
            b2Vec2 anchorB(
                (float)jointObj["anchorBX"].toDouble(),
                (float)jointObj["anchorBY"].toDouble()
                );

            int typeInt = jointObj["type"].toInt();
            b2JointType type = static_cast<b2JointType>(typeInt);
            if (type == e_revoluteJoint)
            {
                qDebug() << "try connect revolute joint";
                joint.connectRevoluteJoint(bodyA, bodyB, anchorA, anchorB);
            }
            else if (type == e_distanceJoint)
            {
                float frequencyHz = (float)jointObj["frequencyHz"].toDouble();
                float dampingRatio = (float)jointObj["dampingRatio"].toDouble();
                float maxForce = (float)jointObj["maxForce"].toDouble();

                Placeable* pA = nullptr;
                Placeable* pB = nullptr;
                for (auto &p : placeables)
                {
                    if (p.getBody() == bodyA) pA = &p;
                    if (p.getBody() == bodyB) pB = &p;
                }
                if (pA && pB)
                {
                    qDebug() << "try connect distance joint";
                    joint.connectDistanceJoint(*pA, *pB, anchorA, anchorB, frequencyHz, dampingRatio, maxForce);
                }
            }
            else
            {
                qDebug() << "Unknown joint type:" << typeInt;
            }
        }
    }
    return true;
}

void GameLevel::clearLevel()
{
    for (b2Body* body = world.GetBodyList(); body != nullptr;)
    {
        b2Body* nextBody = body->GetNext();
        joint.deleteJointsForBody(body);
        world.DestroyBody(body);
        body = nextBody;
    }

    groundBodies.clear();
    placeables.clear();
    joint.deleteAllJoints();
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
