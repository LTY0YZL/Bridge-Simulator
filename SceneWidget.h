#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include <QWidget>
#include <QWheelEvent>
#include <QTimer>
#include <QPainter>
#include <QMouseEvent>
#include <QColor>
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

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
signals:
    void mouseMovedInWorld(float x, float y); // Signal to update the UI with mouse location

private:
    int currentTool;
    float worldScale;
    void paintBackground(QPainter &painter);
    QTimer* simulationTimer;
    GameLevel* gameLevel;
    bool showPreview;
    bool placeablePreview;
    QPointF firstPoint;
    QPointF secondPoint;
    QPointF currentMousePos; // Tracks the current mouse position for preview
    bool isFirstPointSet;
    QPoint lastMousePos;    // Stores the last position of the mouse during a drag
    float panOffsetX;       // Pan offset along the x-axis
    float panOffsetY;       // Pan offset along the y-axis
    bool isPanning;         // Whether the user is currently panning
    Placeable currentBlock;
    Placeable* selectedPlaceable;


    void drawShape(QPainter &painter, const b2Body* body, const QColor &color);
    void drawJoints(QPainter& painter, const std::vector<b2Joint*>& joints);
    QPointF screenToWorld(const QPointF& screenPos) const; // Helper to convert screen to Box2D world coordinates
    QPointF box2DWorldToScreen(const b2Vec2& worldPos) const;
    Placeable* findPlaceableAt(const QPointF& worldPos, std::vector<Placeable>& placeables);
    std::vector<Placeable>::iterator findPlaceableIteratorAt( const QPointF& worldPos, std::vector<Placeable>& placeables);
    void recordTwoWorldPoint(const QPointF& worldPos);
    void createGroundWithTwoPoints(const QPointF& worldPos);
    void createJointWithTwoPoints(const QPointF& worldPos);
    void deleteGroundAt(const QPointF& worldPos);
    void createAnchorAt(const QPointF& worldPos);
    void deleteAnchorAt(const QPointF& worldPos);
    b2Body* findGroundAt(const QPointF& worldPos) const;
    void areaPreview(QPainter& painter, const QPointF& point1, const QPointF& point2);
    void linePreview(QPainter& painter, const QPointF& start, const QPointF& end);
    void drawPlaceablePreview(QPainter &painter);
    void syncSelectedPlaceable();
};

#endif // SCENEWIDGET_H
