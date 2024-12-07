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

    void on_deleteToolButton_clicked();

    void on_stringToolButton_clicked();

    void on_rebarToolButton_clicked();

    void on_drawGroundButton_clicked();

    void on_deleteGroundButton_clicked();

    void on_createAnchorButton_clicked();

    void on_deleteAnchorButton_clicked();

    void on_saveButton_clicked();

    void on_loadButton_clicked();

    void on_nextLevelButton_clicked();

    void on_goToLevelButton_clicked();

    void on_hailButton_clicked();

    void on_earthquakeButton_clicked();

    void on_beamToolButton_clicked();

    void on_frameToolButton_clicked();

private:
    Ui::MainWindow *ui;
    GameLevel* gameLevel; // Pointer to the GameLevel instance
    SceneWidget* sceneWidget;

    int currentLevelNumber;
    QString levelsDirectory;
};

#endif // MAINWINDOW_H
