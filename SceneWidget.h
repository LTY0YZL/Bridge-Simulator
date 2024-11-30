#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include <QWidget>
#include <QWheelEvent>
#include <QTimer>
#include <QPainter>
#include <QMouseEvent>
#include <QColor>
#include "GameLevel.h"

class SceneWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SceneWidget(GameLevel* level, QWidget* parent = nullptr);
    void startSimulation();
    void setCurrentTool(int ID);
    void setWorldScale(float scale);
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent* event) override;

signals:
    void mouseMovedInWorld(float x, float y); // Signal to update the UI with mouse location

private:
    int currentTool;
    float worldScale;
    void paintBackground(QPainter &painter);
    QTimer* simulationTimer;
    GameLevel* gameLevel;
    bool showPreview;
    QPointF firstPoint;
    QPointF secondPoint;
    QPointF currentMousePos; // Tracks the current mouse position for preview
    bool isFirstPointSet;


    void drawShape(QPainter &painter, const b2Body* body, const QColor &color);
    QPointF screenToWorld(const QPointF& screenPos) const; // Helper to convert screen to Box2D world coordinates
    Placeable* findPlaceableAt(const QPointF& worldPos, std::vector<Placeable>& placeables);
    std::vector<Placeable>::iterator findPlaceableIteratorAt( const QPointF& worldPos, std::vector<Placeable>& placeables);
    void recordTwoWorldPoint(const QPointF& worldPos);
    void createGroundWithTwoPoints(const QPointF& worldPos);
    void areaPreview(QPainter& painter, const QPointF& point1, const QPointF& point2);
    void linePreview(QPainter& painter, const QPointF& start, const QPointF& end);
};

#endif // SCENEWIDGET_H
