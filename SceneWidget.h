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
    void drawShape(QPainter &painter, const b2Body* body, const QColor &color);
    QPointF screenToWorld(const QPointF& screenPos) const; // Helper to convert screen to Box2D world coordinates
    QTimer* simulationTimer;
    GameLevel* gameLevel; // Pointer to GameLevel
};

#endif // SCENEWIDGET_H
