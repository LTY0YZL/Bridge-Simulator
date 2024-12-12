/**
 * Author: Yang Hong, Zimo Liu, Qixian Zhang
 * Date: 12/12/2024
 * Title:  A9 Structure Simulator
 *
 * Summary: This file defines the SceneWidget class, which handles rendering
 * and user interaction for the application's canvas, supporting drawing,
 * panning, zooming, and editing structures in the simulation.
 */
#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include <QWidget>
#include <QColor>
#include <QMouseEvent>
#include <QPainter>
#include <QTimer>
#include <QWheelEvent>

#include "GameLevel.h"
#include "Joint.h"

class SceneWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SceneWidget(GameLevel* level, QWidget* parent = nullptr);

    void startSimulation();
    void stopSimulation();
    void setCurrentTool(int ID);
    void setWorldScale(float scale);

    bool hailSpawnPaused = false;

signals:
    void mouseMovedInWorld(float x, float y);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    // Rendering and utility methods
    void paintBackground(QPainter &painter);
    void drawShape(QPainter &painter, const b2Body* body, const QColor &color);
    void drawJoints(QPainter &painter, const std::vector<b2Joint*>& joints);
    void drawPlaceablePreview(QPainter &painter);
    void areaPreview(QPainter &painter, const QPointF &point1, const QPointF &point2);
    void linePreview(QPainter &painter, const QPointF &start, const QPointF &end);

    // Coordinate transformation
    QPointF screenToWorld(const QPointF &screenPos) const;
    QPointF box2DWorldToScreen(const b2Vec2 &worldPos) const;

    // Interaction and editing
    void recordTwoWorldPoint(const QPointF &worldPos);
    void createGroundWithTwoPoints(const QPointF &worldPos);
    void createJointWithTwoPoints(const QPointF &worldPos);
    void deleteGroundAt(const QPointF &worldPos);
    void createAnchorAt(const QPointF &worldPos);
    void deleteAnchorAt(const QPointF &worldPos);
    b2Body* findGroundAt(const QPointF &worldPos) const;
    Placeable* findPlaceableAt(const QPointF &worldPos, std::vector<Placeable> &placeables);
    std::vector<Placeable>::iterator findPlaceableIteratorAt(const QPointF &worldPos, std::vector<Placeable> &placeables);
    void syncSelectedPlaceable();

    // Environmental effects
    void handleHail();
    void handleEarthquake();

    // Member variables
    GameLevel* gameLevel;
    QTimer* simulationTimer;

    int currentTool;
    float worldScale;
    bool showPreview;
    bool placeablePreview;

    bool isFirstPointSet;
    bool isPanning;

    QPointF firstPoint;
    QPointF secondPoint;
    QPointF currentMousePos;
    QPoint lastMousePos;

    float panOffsetX;
    float panOffsetY;

    Placeable currentBlock;
    Placeable* selectedPlaceable = nullptr;
};

#endif // SCENEWIDGET_H
