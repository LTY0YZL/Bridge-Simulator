#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "SceneWidget.h"
#include "GameLevel.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    gameLevel(new GameLevel),
    sceneWidget(nullptr),
    currentLevelNumber(1),
    levelsDirectory("levels")
{
    ui->setupUi(this);

    // Initialize the game level
    QString initialLevelFile = QString("%1/level%2.json").arg(levelsDirectory).arg(currentLevelNumber);
    if (gameLevel->loadLevel(initialLevelFile))
    {
        qDebug() << "Initial level loaded successfully.";
    }
    else
    {
        qDebug() << "Failed to load the initial level.";
        gameLevel->initialize();
    }

    ui->levelNumberLabel->setText(QString("Level %1").arg(currentLevelNumber));

    // Create SceneWidget and pass GameLevel
    sceneWidget = new SceneWidget(gameLevel, this);
    ui->verticalLayout->addWidget(sceneWidget);

    ui->playButton->setEnabled(true);

    // Connect the mouseMovedInWorld signal to update the label
    connect(sceneWidget, &SceneWidget::mouseMovedInWorld, this, &MainWindow::updateMouseLocation);

}

MainWindow::~MainWindow()
{
    delete gameLevel;
    delete ui;
}

void MainWindow::updateMouseLocation(float x, float y)
{
    // Update the QLabel with the mouse location
    ui->mouseLocation->setText(QString("X: %1, Y: %2").arg(x, 0, 'f', 2).arg(y, 0, 'f', 2));
}

void MainWindow::on_playButton_clicked()
{
    sceneWidget->startSimulation();
    ui->playButton->setEnabled(false);
}


void MainWindow::on_pushButton_clicked()
{
    float randomX = (rand() % 20) - 10.0f; // Random X position between -10 and 10
    float skyY = 30.0f; // Fixed Y position above the ground
    Placeable skyBox("SkyBox", 100, Qt::red,  0.5f, 0.5f); // Customize name, color, and size as needed

    // Add the Placeable to the GameLevel
    gameLevel->createDynamicBody(skyBox, randomX, skyY);

    // Update the SceneWidget to reflect the changes
    sceneWidget->update();
}


void MainWindow::on_selectTool_clicked()
{
    sceneWidget->setCurrentTool(1);
}


void MainWindow::on_boxToolButton_clicked()
{
    sceneWidget->setCurrentTool(0);
    qDebug() << "Tool 0";
}


void MainWindow::on_deleteToolButton_clicked()
{
    sceneWidget->setCurrentTool(2);
}

void MainWindow::on_beamToolButton_clicked()
{
    sceneWidget->setCurrentTool(3);
}


void MainWindow::on_frameToolButton_clicked(){
    sceneWidget->setCurrentTool(4);
}

void MainWindow::on_stringToolButton_clicked()
{
    sceneWidget->setCurrentTool(5);
}

void MainWindow::on_rebarToolButton_clicked()
{
    sceneWidget->setCurrentTool(6);
}

void MainWindow::on_drawGroundButton_clicked()
{
    sceneWidget->setCurrentTool(-1);
}


void MainWindow::on_deleteGroundButton_clicked()
{
    sceneWidget->setCurrentTool(-2);
}

void MainWindow::on_createAnchorButton_clicked()
{
    sceneWidget->setCurrentTool(-3);
}

void MainWindow::on_deleteAnchorButton_clicked()
{
    sceneWidget->setCurrentTool(-4);
}

void MainWindow::on_saveButton_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save Level", "", "JSON Files (*.json)");
    if (!filename.isEmpty())
    {
        if (gameLevel->saveLevel(filename))
        {
            QMessageBox::information(this, "Save Level", "Level saved successfully.");
        }
        else
        {
            QMessageBox::warning(this, "Save Level", "Failed to save the level.");
        }
    }
}

void MainWindow::on_loadButton_clicked()
{
    qDebug() << "Load button clicked.";

    QString filename = QFileDialog::getOpenFileName(this, "Load Level", "", "JSON Files (*.json)");
    if (!filename.isEmpty())
    {
        if (gameLevel->loadLevel(filename))
        {
            sceneWidget->stopSimulation();
            ui->playButton->setEnabled(true);
            sceneWidget->update();

            sceneWidget->hailSpawnPaused = true;
            QTimer::singleShot(3000, sceneWidget, [this]() {
                sceneWidget->hailSpawnPaused = false;
            });

            QMessageBox::information(this, "Load Level", QString("Level %1 loaded successfully.").arg(currentLevelNumber));
            qDebug() << "Level" << currentLevelNumber << "loaded successfully.";
        }
        else
        {
            QMessageBox::warning(this, "Load Level", "Failed to load the level.");
        }
    }
}

void MainWindow::on_nextLevelButton_clicked()
{
    currentLevelNumber++;

    QString filename = QString("%1/level%2.json").arg(levelsDirectory).arg(currentLevelNumber);

    qDebug() << "Attempting to load level from:" << filename;

    QFile file(filename);
    if (!file.exists())
    {
        QMessageBox::warning(this, "Next Level", "No more levels available.");
        currentLevelNumber--;
        return;
    }

    if (gameLevel->loadLevel(filename))
    {
        sceneWidget->stopSimulation();
        ui->playButton->setEnabled(true);
        ui->levelNumberLabel->setText(QString("Level %1").arg(currentLevelNumber));
        sceneWidget->update();

        QMessageBox::information(this, "Next Level", QString("Level %1 loaded successfully.").arg(currentLevelNumber));
        qDebug() << "Level" << currentLevelNumber << "loaded successfully.";
    }
    else
    {
        QMessageBox::warning(this, "Next Level", "Failed to load the next level.");
        currentLevelNumber--;
    }
}

void MainWindow::on_goToLevelButton_clicked()
{
    QString levelText = ui->levelLineEdit->text().trimmed();

    bool ok;
    int levelNumber = levelText.toInt(&ok);

    if (!ok || levelNumber <= 0)
    {
        QMessageBox::warning(this, "Invalid Input", "Please enter a valid positive integer for the level number.");
        return;
    }

    QString filename = QString("%1/level%2.json").arg(levelsDirectory).arg(levelNumber);

    qDebug() << "Attempting to load level from:" << filename;

    QFile file(filename);
    if (!file.exists())
    {
        QMessageBox::warning(this, "Level Not Found", QString("Level %1 does not exist.").arg(levelNumber));
        return;
    }

    if (gameLevel->loadLevel(filename))
    {
        sceneWidget->stopSimulation();
        ui->playButton->setEnabled(true);
        currentLevelNumber = levelNumber;
        ui->levelNumberLabel->setText(QString("Level %1").arg(currentLevelNumber));
        sceneWidget->update();

        QMessageBox::information(this, "Level Loaded", QString("Level %1 loaded successfully.").arg(currentLevelNumber));
        qDebug() << "Level" << currentLevelNumber << "loaded successfully.";
    }
    else
    {
        QMessageBox::warning(this, "Load Failed", QString("Failed to load Level %1.").arg(levelNumber));
    }
}

void MainWindow::on_hailButton_clicked()
{
    int hLevel = ui->hailSpinBox->value();
    gameLevel->setHailLevel(hLevel);
    qDebug() << "Hail level set to:" << hLevel;
    QMessageBox::information(this, "Hail Level Set", QString("Hail level set to %1").arg(hLevel));
}

void MainWindow::on_earthquakeButton_clicked()
{
    int eqLevel = ui->earthquakeSpinBox->value();
    gameLevel->setEarthquakeLevel(eqLevel);
    qDebug() << "Earthquake level set to:" << eqLevel;
    QMessageBox::information(this, "Earthquake Level Set", QString("Earthquake level set to %1").arg(eqLevel));
}

void MainWindow::on_restartButton_clicked()
{
    QString filename = QString("%1/level%2.json").arg(levelsDirectory).arg(currentLevelNumber);
    sceneWidget->stopSimulation();

    if (gameLevel->loadLevel(filename))
    {
        sceneWidget->update();
        ui->playButton->setEnabled(true);
        QMessageBox::information(this, "Restart Level", QString("Level %1 restarted successfully.").arg(currentLevelNumber));
        qDebug() << "Level" << currentLevelNumber << "restarted successfully.";
    }
    else
    {
        QMessageBox::warning(this, "Restart Level", "Failed to restart the level.");
    }
}

void MainWindow::on_helpButton_clicked()
{
    QUrl helpUrl("https://docs.google.com/document/d/1uLB2VtSkPZH3JmeTMuocncqlBDtEi_mnIQOjkWmUlU8/edit?usp=sharing");

    if (!QDesktopServices::openUrl(helpUrl)) {
        qDebug() << "Failed to open URL:" << helpUrl.toString();
    }
}

