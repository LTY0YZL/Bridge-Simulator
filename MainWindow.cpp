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
    controllableButtons = {
        ui->selectTool,
        ui->rebarToolButton,
        ui->frameToolButton,
        ui->deleteToolButton,
        ui->drawGroundButton,
        ui->deleteGroundButton,
        ui->beamToolButton,
        ui->stringToolButton,
        ui->deleteAnchorButton,
        ui->createAnchorButton
    };

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
    ui->helpLabel->setText("Weight Load tool click to add load to structure.");
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
    QString defaultFilePath = QDir::currentPath() + "/temp.json";
    if (gameLevel->saveLevel(defaultFilePath))
    {
        qDebug() << "Level successfully saved to" << defaultFilePath;
    }
    else
    {
        qDebug() << "Failed to save level to" << defaultFilePath;
    }
    sceneWidget->startSimulation();
    ui->playButton->setEnabled(false);
    EditMode(false);
    sceneWidget->setCurrentTool(0);
}

void MainWindow::on_pushButton_clicked()
{
    float randomX = (rand() % 20) - 10.0f; // Random X position between -10 and 10
    float skyY = 30.0f; // Fixed Y position above the ground
    Placeable skyBox("SkyBox", 100, Qt::red, 0.5f, 0.5f); // Customize name, color, and size as needed

    // Add the Placeable to the GameLevel
    gameLevel->createDynamicBody(skyBox, randomX, skyY);

    // Update the SceneWidget to reflect the changes
    sceneWidget->update();
}

void MainWindow::on_selectTool_clicked()
{
    sceneWidget->setCurrentTool(1);
    ui->helpLabel->setText("Select tool click to select, DRAG to move, SCROLL to rotate selected object. ");
}

void MainWindow::on_boxToolButton_clicked()
{
    sceneWidget->setCurrentTool(0);
    ui->helpLabel->setText("Weight Load tool click to add load to structure.");
}

void MainWindow::on_deleteToolButton_clicked()
{
    sceneWidget->setCurrentTool(2);
    ui->helpLabel->setText("Delete tool click to delete Placeable (Will remove joints).");
}

void MainWindow::on_beamToolButton_clicked()
{
    sceneWidget->setCurrentTool(3);
    ui->helpLabel->setText("Beam tool click to add beam.");
}

void MainWindow::on_frameToolButton_clicked()
{
    sceneWidget->setCurrentTool(4);
    ui->helpLabel->setText("Frame tool click to add frame.");
}

void MainWindow::on_stringToolButton_clicked()
{
    sceneWidget->setCurrentTool(5);
    ui->helpLabel->setText("String tool click two points to add (connect Anchor and Placeable), loose connection.");
}

void MainWindow::on_rebarToolButton_clicked()
{
    sceneWidget->setCurrentTool(6);
    ui->helpLabel->setText("Rebar tool click two points to add (connect Anchor and Placeable), tight connection.");
}

void MainWindow::on_drawGroundButton_clicked()
{
    sceneWidget->setCurrentTool(-1);
    ui->helpLabel->setText(" Draw Ground tool RIGHT click two points to add ground.");
}

void MainWindow::on_deleteGroundButton_clicked()
{
    sceneWidget->setCurrentTool(-2);
    ui->helpLabel->setText(" Delete Ground tool RIGHT click to delete target.");
}

void MainWindow::on_createAnchorButton_clicked()
{
    sceneWidget->setCurrentTool(-3);
    ui->helpLabel->setText(" Draw Anchor tool RIGHT click to add Anchor(Placed on ground only).");
}

void MainWindow::on_deleteAnchorButton_clicked()
{
    sceneWidget->setCurrentTool(-4);
    ui->helpLabel->setText(" Delete Anchor tool RIGHT click to delete target.");
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
            ui->hailSpinBox->setValue(gameLevel->getHailLevel());
            ui->earthquakeSpinBox->setValue(gameLevel->getEarthquakeLevel());
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
    EditMode(true);
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
        ui->hailSpinBox->setValue(gameLevel->getHailLevel());
        ui->earthquakeSpinBox->setValue(gameLevel->getEarthquakeLevel());
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
    EditMode(true);
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
        ui->hailSpinBox->setValue(gameLevel->getHailLevel());
        ui->earthquakeSpinBox->setValue(gameLevel->getEarthquakeLevel());
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
    EditMode(true);
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
    // Path to the default temporary save file
    QString tempFilePath = QDir::currentPath() + "/temp.json";

    // Fallback to level1.json if temp.json doesn't exist
    QString fallbackFilePath = QString("%1/level1.json").arg(levelsDirectory);

    QString fileToLoad;
    if (QFile::exists(tempFilePath))
    {
        fileToLoad = tempFilePath;
        qDebug() << "Loading from temporary save file:" << tempFilePath;
    }
    else
    {
        fileToLoad = fallbackFilePath;
        qDebug() << "Temporary save file not found. Falling back to:" << fallbackFilePath;
    }

    // Stop simulation before loading the level
    sceneWidget->stopSimulation();

    // Load the level from the determined file path
    if (gameLevel->loadLevel(fileToLoad))
    {
        sceneWidget->update();
        ui->playButton->setEnabled(true);
        ui->hailSpinBox->setValue(gameLevel->getHailLevel());
        ui->earthquakeSpinBox->setValue(gameLevel->getEarthquakeLevel());
        QMessageBox::information(this, "Restart Level", QString("Level loaded successfully from %1.").arg(fileToLoad));
        qDebug() << "Level loaded successfully from" << fileToLoad;
    }
    else
    {
        QMessageBox::warning(this, "Restart Level", "Failed to restart the level.");
        qDebug() << "Failed to load level from" << fileToLoad;
    }
    EditMode(true);
}

void MainWindow::on_helpButton_clicked()
{
    QUrl helpUrl("https://docs.google.com/document/d/1uLB2VtSkPZH3JmeTMuocncqlBDtEi_mnIQOjkWmUlU8/edit?usp=sharing");

    if (!QDesktopServices::openUrl(helpUrl)) {
        qDebug() << "Failed to open URL:" << helpUrl.toString();
    }
}

void MainWindow::EditMode(bool enabled)
{
    QList<QWidget*> buttons = findChildren<QWidget*>();

    for (auto &button : controllableButtons)
    {
        if (button != ui->boxToolButton) {
            button->setVisible(enabled);
        }
    }
    ui->playButton->setEnabled(enabled);
    ui->boxToolButton->setEnabled(true);
}

void MainWindow::on_selectToolButton_clicked()
{

}

