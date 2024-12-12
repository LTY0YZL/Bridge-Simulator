/**
 * Author: Yang Hong, Zimo Liu, Qixian Zhang
 * Date: 12/12/2024
 * Title: A9 Structure Simulator
 *
 * Summary: This file defines the MainWindow class, which serves as the central
 * control point for the application's UI and interactions, including handling
 * user input and managing levels.
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopServices>
#include <QUrl>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include "ui_MainWindow.h"
#include "SceneWidget.h"
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

signals:
         // No custom signals currently

private slots:
    // Slot to update the mouse location label
    void updateMouseLocation(float x, float y);

    // UI interaction slots
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
    void on_restartButton_clicked();
    void on_helpButton_clicked();
    void EditMode(bool enabled);
    void on_selectToolButton_clicked(); // This slot was in original code but not placed in sequence, now placed here for consistency.

private:
    Ui::MainWindow *ui;

    GameLevel* gameLevel;               // Pointer to the GameLevel instance
    SceneWidget* sceneWidget;           // Pointer to the SceneWidget
    QList<QPushButton*> controllableButtons;

    int currentLevelNumber;
    QString levelsDirectory;
};

#endif // MAINWINDOW_H
