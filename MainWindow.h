#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "GameLevel.h"
#include "SceneWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateMouseLocation(float x, float y); // Slot to update the mouse location label

    void on_playButton_clicked();

    void on_pushButton_clicked();

    void on_selectTool_clicked();

    void on_boxToolButton_clicked();

private:
    Ui::MainWindow *ui;
    GameLevel* gameLevel; // Pointer to the GameLevel instance
    SceneWidget* sceneWidget;
};

#endif // MAINWINDOW_H
