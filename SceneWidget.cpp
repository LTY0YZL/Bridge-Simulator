#include "SceneWidget.h"
#include <algorithm>

SceneWidget::SceneWidget(GameLevel* level, QWidget* parent)
    : QWidget(parent),
    currentTool(0),
    worldScale(20.0f),
    simulationTimer(new QTimer(this)),
    gameLevel(level),
    showPreview(false),
    placeablePreview(true),
    isFirstPointSet(false),
    panOffsetX(0.0f),
    panOffsetY(0.0f),
    isPanning(false),
    currentBlock(Placeable("Box", 50, Qt::blue, 2.0f, 2.0f, 2,0.5,0.1))
{
    setMouseTracking(true);
    connect(simulationTimer, &QTimer::timeout, [this]() {
        gameLevel->stepWorld(1.0f / 60.0f, 6, 2);

        handleHail();        // Handle hail functionality
        handleEarthquake();  // Handle earthquake functionality

        update();
    });
}

void SceneWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    // Paint the background
    paintBackground(painter);

    // Translate the painter to apply pan offsets
    painter.translate(width() / 2 + panOffsetX, height() + panOffsetY);

    // Apply scaling to make y-axis point upwards
    painter.scale(1, -1);

    // Draw all ground bodies
    const auto& groundBodies = gameLevel->getGroundBodies();
    for (const b2Body* groundBody : groundBodies)
    {
        drawShape(painter, groundBody, Qt::green);
    }

    // Draw all joints
    const auto& joints = gameLevel->getJoint().getJoints();
    drawJoints(painter, joints);

    // Draw all dynamic bodies
    const auto& placeables = gameLevel->getPlaceables();
    for (const auto& placeable : placeables)
    {
        if (placeable.getBody())
        {
            drawShape(painter, placeable.getBody(), placeable.getColor());
        }
    }

    // Draw all joints
    const auto& joints = gameLevel->getJoint().getJoints();
    drawJoints(painter, joints);

    // Draw the preview if applicable
    if (showPreview && isFirstPointSet)
    {
        if (currentTool == -1)
        {
            areaPreview(painter, firstPoint, currentMousePos);
            linePreview(painter, firstPoint, currentMousePos);
        }
<<<<<<< HEAD
        else if (currentTool == 3 || currentTool == 4)
=======
        else if (currentTool == 5 || currentTool == 6)
>>>>>>> 81e6226 (Finish merge)
        {
            linePreview(painter, firstPoint, currentMousePos);
        }
    }

    drawPlaceablePreview(painter);
}

void SceneWidget::setWorldScale(float scale) // World scale for zooming
{
    if (scale > 0) {
        worldScale = scale;
    } else {
        qDebug() << "Invalid world scale. Scale must be positive.";
    }
}

void SceneWidget::startSimulation()
{
    if (!simulationTimer->isActive()) {
        simulationTimer->start(16); // Start at ~60 FPS
    }
}

void SceneWidget::stopSimulation()
{
    if (simulationTimer->isActive())
    {
        simulationTimer->stop();
    }
}

void SceneWidget::paintBackground(QPainter &painter)
{
    painter.setBrush(QBrush(Qt::lightGray));
    painter.drawRect(0, 0, width(), height());
}

void SceneWidget::drawShape(QPainter &painter, const b2Body* body, const QColor &color)
{
    painter.setBrush(color);

    for (const b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext())
    {
        const b2Shape* shape = fixture->GetShape();
        if (shape->GetType() == b2Shape::e_polygon)
        {
            const b2PolygonShape* polygon = static_cast<const b2PolygonShape*>(shape);

            QPolygonF qPolygon;
            for (int i = 0; i < polygon->m_count; ++i)
            {
                b2Vec2 vertex = body->GetWorldPoint(polygon->m_vertices[i]);
                QPointF qPoint = box2DWorldToScreen(vertex);
                qPolygon << qPoint;
            }
            painter.drawPolygon(qPolygon);
        }
        else if (shape->GetType() == b2Shape::e_circle)
        {
            const b2CircleShape* circle = static_cast<const b2CircleShape*>(shape);
            b2Vec2 center = body->GetWorldPoint(circle->m_p);
            float radius = circle->m_radius * worldScale;

            QPointF screenCenter = box2DWorldToScreen(center);
            painter.drawEllipse(screenCenter, radius, radius);
        }
    }
}

void SceneWidget::drawJoints(QPainter& painter, const std::vector<b2Joint*>& joints)
{
    QPen jointPen(Qt::SolidLine);
    jointPen.setColor(Qt::black); // Set the color to blue for joints
    jointPen.setWidth(5);
    painter.setPen(jointPen);

    for (const auto& joint : joints)
    {
        if (!joint || !joint->GetBodyA() || !joint->GetBodyB())
            return;

        b2Vec2 anchorA = joint->GetAnchorA();
        b2Vec2 anchorB = joint->GetAnchorB();

        QPointF screenPointA = box2DWorldToScreen(anchorA);
        QPointF screenPointB = box2DWorldToScreen(anchorB);

        painter.drawLine(screenPointA, screenPointB);
    }
}

QPointF SceneWidget::screenToWorld(const QPointF& screenPos) const
{
    // Adjust screen position by pan offset before applying transformation
    float box2dX = (screenPos.x() - (width() / 2 + panOffsetX)) / worldScale;
    float box2dY = (height() + panOffsetY - screenPos.y()) / worldScale;
    return QPointF(box2dX, box2dY);
}

QPointF SceneWidget::box2DWorldToScreen(const b2Vec2& worldPos) const
{
    float screenX = (worldPos.x * worldScale) ;
    float screenY = (worldPos.y * worldScale);
    return QPointF(screenX, screenY);
}


void SceneWidget::setCurrentTool(int ID)
{
    if (selectedPlaceable)
    {
        // Reset the color of the previously selected object
        selectedPlaceable->setDisplayColor(selectedPlaceable->getOriginalColor());
        selectedPlaceable = nullptr; // Clear the selection
    }
    currentTool=ID;
    placeablePreview = (ID == 0 || ID == 3 || ID == 4);
    if (ID == 0)
    {
        currentBlock= Placeable("Box", 50, Qt::blue, 2.0f, 2.0f, 2,0.5,0.1);
    }
    else if (ID == 3)
    {
        currentBlock= Placeable("Box", 200, Qt::gray, 2.0f, 10.0f, 20,0.5,0.1); // Beam
    }
    else if (ID == 4)
    {
        currentBlock= Placeable("Box", 100, Qt::darkGray, 10.0f, 1.0f,10,0.5,0.1); // frame
    }
    else
    {
        placeablePreview = false; // Disable preview
    }
}

void SceneWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MiddleButton)
    {
        isPanning = true;               // Start the panning operation
        lastMousePos = event->pos();    // Store the initial position of the mouse
    }
    else
    {
        // Existing code for other tools
        QPointF worldPos = screenToWorld(event->pos());

        if (event->button() == Qt::RightButton)
        {
<<<<<<< HEAD
            if (currentTool == -1) //Tool to create ground objects
=======
            if (currentTool == -1) // Tool to create ground objects
>>>>>>> 81e6226 (Finish merge)
            {
                createGroundWithTwoPoints(worldPos);
                return;
            }
            else if (currentTool == -2) // Tool to delete ground objects
            {
                deleteGroundAt(worldPos);
                return;
            }
            else if (currentTool == -3) // Tool to create anchor objects
            {
                createAnchorAt(worldPos);
                return;
            }
            else if (currentTool == -4) // Tool to delete anchor objects
            {
                deleteAnchorAt(worldPos);
                return;
            }
        }

        auto& placeables = gameLevel->getPlaceables();
        if (currentTool == 0 || currentTool == 3 || currentTool == 4) // Left click Case
        {
<<<<<<< HEAD
            Placeable newPlaceable("Box", 50, Qt::blue, 2.0f, 2.0f);
            gameLevel->createDynamicBody(newPlaceable, worldPos.x(), worldPos.y());
=======
            gameLevel->createDynamicBody(currentBlock, worldPos.x(), worldPos.y());
            update();
>>>>>>> c4273a2 (Placeable Preview)
        }
        else if (currentTool == 1) // Select tool
        {
            Placeable* placeable = findPlaceableAt(worldPos, placeables);
            if (placeable)
            {
<<<<<<< HEAD
                placeable->setDisplayColor(QColor("forestgreen"));
=======
                if (selectedPlaceable){
                    // Reset the color
                    selectedPlaceable->setDisplayColor(selectedPlaceable->getOriginalColor());
                }
                // Update the selected placeable
                selectedPlaceable = placeable;
                selectedPlaceable->setDisplayColor(QColor("forestgreen"));
                update();
>>>>>>> f917bc5 (Select Drag Rotate Done)
            }
        }
        else if (currentTool == 2)
        {
            auto it = findPlaceableIteratorAt(worldPos, placeables);
            if (it != placeables.end())
            {
                b2Body* body = it->getBody();
                if (body)
                {
                    gameLevel->destroyBody(body);
                }
                placeables.erase(it);
            }
        }
<<<<<<< HEAD
        else if (currentTool == 3 || currentTool == 4) // Select two placeables to create a joint
        {
            createJointWithTwoPoints(worldPos);
            return;
=======
        else if (currentTool == 5 || currentTool == 6) // Select two placeables to create a joint
        {
            createJointWithTwoPoints(worldPos);
            update();
>>>>>>> 81e6226 (Finish merge)
        }
    }
    update();
}

void SceneWidget::mouseMoveEvent(QMouseEvent* event)
{
    QPointF screenPos = event->pos();
    QPointF worldPos = screenToWorld(screenPos);
    if (isPanning)
    {
        // Calculate the difference between the current and last mouse position
        QPoint delta = event->pos() - lastMousePos;

        // Update the pan offsets
        panOffsetX += delta.x();
        panOffsetY += delta.y();

        // Update the last mouse position
        lastMousePos = event->pos();

        // Trigger repaint to reflect the panning
        update();
    }
    else if (selectedPlaceable && event->buttons() & Qt::LeftButton)
    {
        // Move the selected placeable
        b2Body* body = selectedPlaceable->getBody();
        if (body)
        {
            body->SetTransform(b2Vec2(worldPos.x(), worldPos.y()), body->GetAngle());
            syncSelectedPlaceable();
            update();
        }
    }
    else
    {
        if (placeablePreview) {
            currentMousePos = worldPos; // Update preview position
            update();
        }
        if (isFirstPointSet) // Update preview if the first point is set
        {
            currentMousePos = worldPos;
            showPreview = true;
            update(); // Repaint to show the preview
        }
        emit mouseMovedInWorld(worldPos.x(), worldPos.y()); // Emit the signal with world coordinates
    }
}

void SceneWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MiddleButton)
    {
        isPanning = false; // End the panning operation
    }
}

void SceneWidget::wheelEvent(QWheelEvent* event)
{
    if (selectedPlaceable)
    {
        b2Body* body = selectedPlaceable->getBody();
        if (body)
        {
            float currentAngle = body->GetAngle();
            float rotationStep = 15.0f * (b2_pi / 180.0f); // 15 degrees in radians
            if (event->angleDelta().y() > 0)
            {
                body->SetTransform(body->GetPosition(), currentAngle - rotationStep);
            }
            else
            {
                body->SetTransform(body->GetPosition(), currentAngle + rotationStep);
            }
            syncSelectedPlaceable();
            update();
        }
    }
    else
        if (event->angleDelta().y() > 0) {
            // Zoom in by increasing worldScale by 5%
            worldScale *= 1.05;
        } else {
            worldScale /= 1.05;
        }

    // Clamp worldScale to avoid invalid values
    if (worldScale < 0.5f) {
        worldScale = 1.0f; // Minimum zoom level
    }
    else if (worldScale > 200.0f) {
        worldScale = 200.0f; // Minimum zoom level
    }
    update();
}

Placeable* SceneWidget::findPlaceableAt(const QPointF& worldPos, std::vector<Placeable>& placeables)
{
    for (auto& placeable : placeables)
    {
        b2Body* body = placeable.getBody();
        if (body)
        {
            for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext())
            {
                if (fixture->TestPoint(b2Vec2(worldPos.x(), worldPos.y())))
                {
                    return &placeable;
                }
            }
        }
    }
    return nullptr; // No matching placeable found
}

std::vector<Placeable>::iterator SceneWidget::findPlaceableIteratorAt(
    const QPointF& worldPos, std::vector<Placeable>& placeables)
{
    for (auto it = placeables.begin(); it != placeables.end(); ++it)
    {
        b2Body* body = it->getBody();
        if (body)
        {
            for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext())
            {
                if (fixture->TestPoint(b2Vec2(worldPos.x(), worldPos.y())))
                {
                    return it; // Get the vector contains the item want to deal with
                }
            }
        }
    }
    return placeables.end();
}

void SceneWidget::recordTwoWorldPoint(const QPointF& worldPos)
{
    if (!isFirstPointSet)
    {
        // Record the first point
        firstPoint = worldPos;
        isFirstPointSet = true;
    }
    else
    {
        // Record the second point
        secondPoint = worldPos;
        isFirstPointSet = false; // Reset for future use
        qDebug() << "Box2D World Points: First =" << firstPoint << ", Second =" << secondPoint;
    }
}

void SceneWidget::createJointWithTwoPoints(const QPointF& worldPos)
{
    recordTwoWorldPoint(worldPos);
    auto& placeables = gameLevel->getPlaceables();

    // If both points are recorded, create the ground body
    if (!isFirstPointSet) // Means second point was just set
    {
        Placeable* firstPlaceable = findPlaceableAt(firstPoint, placeables);
        Placeable* secondPlaceable = findPlaceableAt(secondPoint, placeables);
        if (firstPlaceable && secondPlaceable && firstPlaceable != secondPlaceable)
        {
            b2Vec2 anchorA(firstPoint.x(), firstPoint.y());
            b2Vec2 anchorB(secondPoint.x(), secondPoint.y());

<<<<<<< HEAD
<<<<<<< HEAD
            if (currentTool == 3) // Create a more elastic joint
            {
                gameLevel->getJoint().connectDistanceJoint(*firstPlaceable, *secondPlaceable, anchorA, anchorB, 10.0f, 0.2f, 50.0f);
            }
            else if (currentTool == 4) // Create a less elastic joint
=======
            if (currentTool == 5) // Create a more elastic joint
            {
                gameLevel->getJoint().connectDistanceJoint(*firstPlaceable, *secondPlaceable, anchorA, anchorB, 10.0f, 0.2f, 50.0f);
            }
            else if (currentTool == 6) // Create a less elastic joint
>>>>>>> 81e6226 (Finish merge)
=======
            if (currentTool == 6) // Create a more elastic joint
            {
                gameLevel->getJoint().connectDistanceJoint(*firstPlaceable, *secondPlaceable, anchorA, anchorB, 10.0f, 0.2f, 50.0f);
            }
            else if (currentTool == 5) // Create a less elastic joint
>>>>>>> f55cda1 (adjust joint force)
            {
                gameLevel->getJoint().connectDistanceJoint(*firstPlaceable, *secondPlaceable, anchorA, anchorB, 1.0f, 0.8f, 30.0f);
            }
        }
        showPreview = false;
        update();
    }
}

void SceneWidget::createGroundWithTwoPoints(const QPointF& worldPos)
{
    recordTwoWorldPoint(worldPos);

    // If both points are recorded, create the ground body
    if (!isFirstPointSet) // Means second point was just set
    {
        // Calculate dimensions and center for the ground body
        float width = std::abs(secondPoint.x() - firstPoint.x());
        float height = std::abs(secondPoint.y() - firstPoint.y());
        float centerX = (firstPoint.x() + secondPoint.x()) / 2.0f;
        float centerY = (firstPoint.y() + secondPoint.y()) / 2.0f;

        qDebug() << "Creating ground body at center (" << centerX << "," << centerY
                 << ") with width =" << width << " and height =" << height;
        gameLevel->createGround(centerX, centerY, width, height);

        update();
    }
}

void SceneWidget::deleteGroundAt(const QPointF& worldPos)
{
    b2Body* groundBody = findGroundAt(worldPos);
    if (groundBody)
    {
        gameLevel->getJoint().deleteJointsForBody(groundBody);
        gameLevel->destroyGround(groundBody); // Call GameLevel to remove it
        update();
    }
}

b2Body* SceneWidget::findGroundAt(const QPointF& worldPos) const
{
    const auto& groundBodies = gameLevel->getGroundBodies();
    for (b2Body* body : groundBodies)
    {
        for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext())
        {
            if (fixture->TestPoint(b2Vec2(worldPos.x(), worldPos.y())))
            {
                return body;
            }
        }
    }
    return nullptr; // No ground object found
}

void SceneWidget::createAnchorAt(const QPointF& worldPos)
{
    b2Body* groundBody = findGroundAt(worldPos);
    if (groundBody)
    {
        Placeable newPlaceable("Dynamic", 50, Qt::red, 1.0f, 1.0f);
        b2Body* dynamicBody = gameLevel->createDynamicBody(newPlaceable, worldPos.x(), worldPos.y());

        if (dynamicBody)
        {
            b2Vec2 anchor(worldPos.x(), worldPos.y());
            gameLevel->getJoint().connectRevoluteJoint(dynamicBody, groundBody, anchor, anchor);
            qDebug() << "Revolute joint created between dynamic body and ground at:" << anchor.x << anchor.y;
        }
    }
    update();
}

void SceneWidget::deleteAnchorAt(const QPointF& worldPos)
{
    auto& placeables = gameLevel->getPlaceables();
    auto it = findPlaceableIteratorAt(worldPos, placeables);
    if (it != placeables.end())
    {
        b2Body* body = it->getBody();
        if (body)
        {
            gameLevel->destroyBody(body);
        }
        placeables.erase(it);
    }
    update();
}

void SceneWidget::areaPreview(QPainter& painter, const QPointF& point1, const QPointF& point2)
{
    float width = std::abs(point2.x() - point1.x());
    float height = std::abs(point2.y() - point1.y());
    float centerX = (point1.x() + point2.x()) / 2.0f;
    float centerY = (point1.y() + point2.y()) / 2.0f;

    QRectF previewRect(
        (centerX - width / 2.0f) * worldScale,
        (centerY - height / 2.0f) * worldScale,
        width * worldScale,
        height * worldScale
        );

    painter.setBrush(QBrush(Qt::yellow, Qt::DiagCrossPattern));
    painter.setPen(Qt::DashLine);
    painter.drawRect(previewRect);
}

void SceneWidget::linePreview(QPainter& painter, const QPointF& start, const QPointF& end)
{
    QPen dashedPen(Qt::DashLine);
    dashedPen.setColor(Qt::red); // Set the color to red
    dashedPen.setWidth(2);
    painter.setPen(dashedPen);

    // Apply world-to-screen transformation
    QPointF screenStart(start.x() * worldScale, start.y() * worldScale);
    QPointF screenEnd(end.x() * worldScale, end.y() * worldScale);

    // Draw the line
    painter.drawLine(screenStart, screenEnd);
}
<<<<<<< HEAD
=======

void SceneWidget::drawPlaceablePreview(QPainter &painter)
{
    if (!placeablePreview) return;

    // Get the current block's size and color
    float width = currentBlock.getWidth();
    float height = currentBlock.getHeight();
    QColor previewColor = currentBlock.getColor();
    previewColor.setAlpha(128);
    // Convert world coordinates to screen coordinates
    QPointF screenPos = box2DWorldToScreen(b2Vec2(currentMousePos.x(), currentMousePos.y()));

    // Calculate the rectangle for the preview
    QRectF previewRect(
        screenPos.x() - (width * worldScale / 2.0f),
        screenPos.y() - (height * worldScale / 2.0f),
        width * worldScale,
        height * worldScale
        );

    // Set the brush and pen for the preview
    painter.setBrush(QBrush(previewColor, Qt::SolidPattern));
    QPen dashedPen(Qt::DashLine);
    dashedPen.setColor(previewColor.darker());
    dashedPen.setWidth(2);
    painter.setPen(dashedPen);

    // Draw the preview
    painter.drawRect(previewRect);
}
void SceneWidget::syncSelectedPlaceable()
{
    if (selectedPlaceable)
    {
        b2Body* body = selectedPlaceable->getBody();
        if (body)
        {
            b2Vec2 pos = body->GetPosition();
            float angle = body->GetAngle();

            selectedPlaceable->setPosition(pos.x, pos.y);
            selectedPlaceable->setRotation(angle);
        }
    }
}
void SceneWidget::handleHail()
{
    int hLevel = gameLevel->getHailLevel();
    if (hLevel > 0 && !hailSpawnPaused)
    {
        static int hailCounter = 0;
        hailCounter++;
        int spawnInterval = std::max(1, 5); // Spawn frequency
        if (hailCounter % spawnInterval == 0)
        {
            int hailAmount = 5; // Number of hailstones to spawn per interval
            float hailSize = 0.2f + 0.2f * hLevel;

            for (int i = 0; i < hailAmount; ++i)
            {
                float randomX = ((rand() % 40) - 20.0f) + static_cast<float>(rand() % 10) / 10.0f;// Wider range: -20.0 to 20.0
                float spawnY = 60.0f;                   // Spawn height

                Placeable hailStone("Hail", 0, Qt::white, hailSize, hailSize, 3.0f, 0.3f, 0.1f);
                hailStone.setAsHail(true);
                hailStone.setCreationTime(QDateTime::currentDateTime());

                gameLevel->createDynamicBody(hailStone, randomX, spawnY);
            }
        }
    }

    // Remove hailstones older than 15 seconds
    auto& placeables = gameLevel->getPlaceables();
    QDateTime now = QDateTime::currentDateTime();

    placeables.erase(std::remove_if(placeables.begin(), placeables.end(),
                                    [this, &now](const Placeable& p) {
                                        if (p.isHailStone()) {
                                            qint64 elapsedMs = p.getCreationTime().msecsTo(now);
                                            if (elapsedMs > 15000) {
                                                if (b2Body* body = p.getBody()) {
                                                    gameLevel->destroyBody(body);
                                                }
                                                return true;
                                            }
                                        }
                                        return false;
                                    }), placeables.end());
}


<<<<<<< HEAD
>>>>>>> c4273a2 (Placeable Preview)
=======
void SceneWidget::handleEarthquake()
{
    int eqLevel = gameLevel->getEarthquakeLevel();
    if (eqLevel > 0)
    {
        float forceFactor = 0.1f; // Adjust this value to control the impulse intensity
        float shakeIntensity = (float)eqLevel * 2.0f; // Adjust Earthquake intensity based on level
        float shakeX = ((float)(rand() % 100) / 100.0f - 0.5f) * shakeIntensity;
        float shakeY = ((float)(rand() % 100) / 100.0f - 0.5f) * shakeIntensity;

        float displayShakeFactor = 0.1f; // Manage display shaking
        panOffsetX = shakeX * worldScale * displayShakeFactor;
        panOffsetY = shakeY * worldScale * displayShakeFactor;

        auto& placeables = gameLevel->getPlaceables();
        for (auto &p : placeables)
        {
            b2Body* body = p.getBody();
            if (!body) continue;

            // Calculate object size factor based on width and height
            float sizeFactor = p.getWidth() * p.getHeight();

            float impulseX = ((float)(rand() % 100) / 100.0f - 0.5f) * (eqLevel * forceFactor * sizeFactor * sizeFactor);

            body->ApplyLinearImpulse(b2Vec2(impulseX, 0), body->GetWorldCenter(), true);
        }
    }
    else
    {
        panOffsetX = 0;
        panOffsetY = 0;
    }
}

>>>>>>> 2f6f446 (Update Weather System)
