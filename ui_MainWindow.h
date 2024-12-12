/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *mouseLocation;
    QPushButton *playButton;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents_2;
    QGridLayout *gridLayout_2;
    QVBoxLayout *toolArea_2;
    QPushButton *selectTool;
    QPushButton *deleteToolButton;
    QPushButton *boxToolButton;
<<<<<<< HEAD
    QPushButton *stringToolButton;
    QPushButton *rebarToolButton;
=======
    QPushButton *beamToolButton;
    QPushButton *frameToolButton;
<<<<<<< HEAD
>>>>>>> c4273a2 (Placeable Preview)
=======
    QPushButton *stringToolButton;
    QPushButton *rebarToolButton;
>>>>>>> 81e6226 (Finish merge)
    QSpacerItem *verticalSpacer_2;
    QLabel *label_2;
    QLabel *label_5;
    QLabel *label_6;
    QPushButton *helpButton;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout_2;
    QPushButton *nextLevelButton;
    QVBoxLayout *verticalLayout_10;
    QLabel *levelNumberLabel;
    QPushButton *goToLevelButton;
    QLineEdit *levelLineEdit;
    QLabel *label;
    QPushButton *saveButton;
    QPushButton *loadButton;
    QLabel *label_3;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QGridLayout *gridLayout;
    QVBoxLayout *toolArea;
    QPushButton *drawGroundButton;
    QPushButton *deleteGroundButton;
    QPushButton *createAnchorButton;
    QPushButton *deleteAnchorButton;
    QSpacerItem *verticalSpacer;
    QScrollArea *scrollArea_3;
    QWidget *scrollAreaWidgetContents_3;
    QGridLayout *gridLayout_3;
    QVBoxLayout *toolArea_3;
    QVBoxLayout *verticalLayout_6;
    QPushButton *hailButton;
    QSpinBox *hailSpinBox;
    QPushButton *earthquakeButton;
    QSpinBox *earthquakeSpinBox;
    QSpacerItem *verticalSpacer_3;
    QLabel *label_4;
    QPushButton *restartButton;
    QLabel *label_7;
    QLabel *label_8;
    QMenuBar *menubar;
    QMenu *menuSelectLevel_todo;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(912, 626);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayoutWidget = new QWidget(centralwidget);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(150, 40, 511, 471));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        mouseLocation = new QLabel(centralwidget);
        mouseLocation->setObjectName("mouseLocation");
        mouseLocation->setGeometry(QRect(470, 20, 141, 16));
        mouseLocation->setFrameShape(QFrame::Shape::Box);
        playButton = new QPushButton(centralwidget);
        playButton->setObjectName("playButton");
        playButton->setGeometry(QRect(150, 510, 511, 31));
        scrollArea_2 = new QScrollArea(centralwidget);
        scrollArea_2->setObjectName("scrollArea_2");
        scrollArea_2->setGeometry(QRect(10, 90, 111, 261));
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName("scrollAreaWidgetContents_2");
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 109, 259));
        gridLayout_2 = new QGridLayout(scrollAreaWidgetContents_2);
        gridLayout_2->setObjectName("gridLayout_2");
        toolArea_2 = new QVBoxLayout();
        toolArea_2->setObjectName("toolArea_2");
        selectTool = new QPushButton(scrollAreaWidgetContents_2);
        selectTool->setObjectName("selectTool");

        toolArea_2->addWidget(selectTool);

        deleteToolButton = new QPushButton(scrollAreaWidgetContents_2);
        deleteToolButton->setObjectName("deleteToolButton");

        toolArea_2->addWidget(deleteToolButton);

        boxToolButton = new QPushButton(scrollAreaWidgetContents_2);
        boxToolButton->setObjectName("boxToolButton");

        toolArea_2->addWidget(boxToolButton);

<<<<<<< HEAD
        stringToolButton = new QPushButton(scrollAreaWidgetContents_2);
        stringToolButton->setObjectName("stringToolButton");

        toolArea_2->addWidget(stringToolButton);

        rebarToolButton = new QPushButton(scrollAreaWidgetContents_2);
        rebarToolButton->setObjectName("rebarToolButton");

        toolArea_2->addWidget(rebarToolButton);
=======
        beamToolButton = new QPushButton(scrollAreaWidgetContents_2);
        beamToolButton->setObjectName("beamToolButton");

        toolArea_2->addWidget(beamToolButton);

        frameToolButton = new QPushButton(scrollAreaWidgetContents_2);
        frameToolButton->setObjectName("frameToolButton");

        toolArea_2->addWidget(frameToolButton);
>>>>>>> c4273a2 (Placeable Preview)

        stringToolButton = new QPushButton(scrollAreaWidgetContents_2);
        stringToolButton->setObjectName("stringToolButton");

        toolArea_2->addWidget(stringToolButton);

        rebarToolButton = new QPushButton(scrollAreaWidgetContents_2);
        rebarToolButton->setObjectName("rebarToolButton");

        toolArea_2->addWidget(rebarToolButton);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        toolArea_2->addItem(verticalSpacer_2);


        gridLayout_2->addLayout(toolArea_2, 0, 0, 1, 1);

        scrollArea_2->setWidget(scrollAreaWidgetContents_2);
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(10, 40, 111, 16));
        label_5 = new QLabel(centralwidget);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(150, 20, 321, 16));
        QFont font;
        font.setPointSize(11);
        font.setBold(true);
        label_5->setFont(font);
        label_6 = new QLabel(centralwidget);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(10, 60, 111, 19));
        label_6->setFont(font);
        helpButton = new QPushButton(centralwidget);
        helpButton->setObjectName("helpButton");
        helpButton->setGeometry(QRect(10, 360, 111, 51));
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(670, 230, 201, 343));
        verticalLayout_5 = new QVBoxLayout(layoutWidget);
        verticalLayout_5->setObjectName("verticalLayout_5");
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        nextLevelButton = new QPushButton(layoutWidget);
        nextLevelButton->setObjectName("nextLevelButton");

        verticalLayout_2->addWidget(nextLevelButton);

        verticalLayout_10 = new QVBoxLayout();
        verticalLayout_10->setObjectName("verticalLayout_10");
        levelNumberLabel = new QLabel(layoutWidget);
        levelNumberLabel->setObjectName("levelNumberLabel");

        verticalLayout_10->addWidget(levelNumberLabel);

        goToLevelButton = new QPushButton(layoutWidget);
        goToLevelButton->setObjectName("goToLevelButton");

        verticalLayout_10->addWidget(goToLevelButton);

        levelLineEdit = new QLineEdit(layoutWidget);
        levelLineEdit->setObjectName("levelLineEdit");

        verticalLayout_10->addWidget(levelLineEdit);


        verticalLayout_2->addLayout(verticalLayout_10);

        label = new QLabel(layoutWidget);
        label->setObjectName("label");

        verticalLayout_2->addWidget(label);

        saveButton = new QPushButton(layoutWidget);
        saveButton->setObjectName("saveButton");

        verticalLayout_2->addWidget(saveButton);

        loadButton = new QPushButton(layoutWidget);
        loadButton->setObjectName("loadButton");

        verticalLayout_2->addWidget(loadButton);

        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName("label_3");
        label_3->setFont(font);

        verticalLayout_2->addWidget(label_3);


        verticalLayout_5->addLayout(verticalLayout_2);

        scrollArea = new QScrollArea(layoutWidget);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
        scrollAreaWidgetContents->setGeometry(QRect(0, -13, 183, 165));
=======
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 197, 123));
>>>>>>> 8ee3171 (Udptae A bit UI)
=======
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 180, 132));
>>>>>>> 3e6fb54 (Added a restart button to restart the level and play again. reduce earthquake shaking effect)
=======
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 183, 165));
>>>>>>> 81e6226 (Finish merge)
=======
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 183, 136));
>>>>>>> 286b58d (Add helpInfo page)
        gridLayout = new QGridLayout(scrollAreaWidgetContents);
        gridLayout->setObjectName("gridLayout");
        toolArea = new QVBoxLayout();
        toolArea->setObjectName("toolArea");
        drawGroundButton = new QPushButton(scrollAreaWidgetContents);
        drawGroundButton->setObjectName("drawGroundButton");

        toolArea->addWidget(drawGroundButton);

        deleteGroundButton = new QPushButton(scrollAreaWidgetContents);
        deleteGroundButton->setObjectName("deleteGroundButton");

        toolArea->addWidget(deleteGroundButton);

        createAnchorButton = new QPushButton(scrollAreaWidgetContents);
        createAnchorButton->setObjectName("createAnchorButton");

        toolArea->addWidget(createAnchorButton);

        deleteAnchorButton = new QPushButton(scrollAreaWidgetContents);
        deleteAnchorButton->setObjectName("deleteAnchorButton");

        toolArea->addWidget(deleteAnchorButton);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        toolArea->addItem(verticalSpacer);


        gridLayout->addLayout(toolArea, 0, 0, 1, 1);

        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout_5->addWidget(scrollArea);

        scrollArea_3 = new QScrollArea(centralwidget);
        scrollArea_3->setObjectName("scrollArea_3");
        scrollArea_3->setGeometry(QRect(670, 20, 201, 201));
        scrollArea_3->setWidgetResizable(true);
        scrollAreaWidgetContents_3 = new QWidget();
        scrollAreaWidgetContents_3->setObjectName("scrollAreaWidgetContents_3");
        scrollAreaWidgetContents_3->setGeometry(QRect(0, 0, 199, 199));
        gridLayout_3 = new QGridLayout(scrollAreaWidgetContents_3);
        gridLayout_3->setObjectName("gridLayout_3");
        toolArea_3 = new QVBoxLayout();
        toolArea_3->setObjectName("toolArea_3");
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName("verticalLayout_6");
        hailButton = new QPushButton(scrollAreaWidgetContents_3);
        hailButton->setObjectName("hailButton");

        verticalLayout_6->addWidget(hailButton);

        hailSpinBox = new QSpinBox(scrollAreaWidgetContents_3);
        hailSpinBox->setObjectName("hailSpinBox");
        hailSpinBox->setMaximum(5);

        verticalLayout_6->addWidget(hailSpinBox);

        earthquakeButton = new QPushButton(scrollAreaWidgetContents_3);
        earthquakeButton->setObjectName("earthquakeButton");

        verticalLayout_6->addWidget(earthquakeButton);

        earthquakeSpinBox = new QSpinBox(scrollAreaWidgetContents_3);
        earthquakeSpinBox->setObjectName("earthquakeSpinBox");
        earthquakeSpinBox->setMaximum(6);

        verticalLayout_6->addWidget(earthquakeSpinBox);


        toolArea_3->addLayout(verticalLayout_6);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        toolArea_3->addItem(verticalSpacer_3);


        gridLayout_3->addLayout(toolArea_3, 0, 0, 1, 1);

        scrollArea_3->setWidget(scrollAreaWidgetContents_3);
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(670, 0, 111, 16));
        restartButton = new QPushButton(centralwidget);
        restartButton->setObjectName("restartButton");
        restartButton->setGeometry(QRect(150, 550, 511, 31));
        label_7 = new QLabel(centralwidget);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(150, 0, 81, 16));
        QFont font1;
        font1.setPointSize(10);
        font1.setBold(true);
        label_7->setFont(font1);
        label_8 = new QLabel(centralwidget);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(230, 0, 53, 15));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 912, 20));
        menuSelectLevel_todo = new QMenu(menubar);
        menuSelectLevel_todo->setObjectName("menuSelectLevel_todo");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuSelectLevel_todo->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        mouseLocation->setText(QCoreApplication::translate("MainWindow", "Mouse World Location", nullptr));
        playButton->setText(QCoreApplication::translate("MainWindow", "Simulate", nullptr));
        selectTool->setText(QCoreApplication::translate("MainWindow", "Select", nullptr));
        deleteToolButton->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
<<<<<<< HEAD
<<<<<<< HEAD
        boxToolButton->setText(QCoreApplication::translate("MainWindow", "box", nullptr));
        stringToolButton->setText(QCoreApplication::translate("MainWindow", "String", nullptr));
        rebarToolButton->setText(QCoreApplication::translate("MainWindow", "Rebar", nullptr));
=======
        boxToolButton->setText(QCoreApplication::translate("MainWindow", "Testbox", nullptr));
=======
        boxToolButton->setText(QCoreApplication::translate("MainWindow", "Weight", nullptr));
>>>>>>> f917bc5 (Select Drag Rotate Done)
        beamToolButton->setText(QCoreApplication::translate("MainWindow", "Beam", nullptr));
        frameToolButton->setText(QCoreApplication::translate("MainWindow", "Frame", nullptr));
<<<<<<< HEAD
>>>>>>> c4273a2 (Placeable Preview)
=======
        stringToolButton->setText(QCoreApplication::translate("MainWindow", "String", nullptr));
        rebarToolButton->setText(QCoreApplication::translate("MainWindow", "Rebar", nullptr));
>>>>>>> 81e6226 (Finish merge)
        label_2->setText(QCoreApplication::translate("MainWindow", "In Game Tool:", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Scroll to Zoom, Middle Mouse To Shift", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Click To Use", nullptr));
        helpButton->setText(QCoreApplication::translate("MainWindow", "Help Page", nullptr));
        nextLevelButton->setText(QCoreApplication::translate("MainWindow", "Play Next Level", nullptr));
        levelNumberLabel->setText(QCoreApplication::translate("MainWindow", "level", nullptr));
        goToLevelButton->setText(QCoreApplication::translate("MainWindow", "Go to Level", nullptr));
        levelLineEdit->setText(QCoreApplication::translate("MainWindow", "Enter level number", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Level Edit Tool:", nullptr));
        saveButton->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
        loadButton->setText(QCoreApplication::translate("MainWindow", "Load", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Right Click To Use", nullptr));
        drawGroundButton->setText(QCoreApplication::translate("MainWindow", "DrawGround", nullptr));
        deleteGroundButton->setText(QCoreApplication::translate("MainWindow", "DeleteGround", nullptr));
        createAnchorButton->setText(QCoreApplication::translate("MainWindow", "Create Anchor", nullptr));
        deleteAnchorButton->setText(QCoreApplication::translate("MainWindow", "Delete Anchor", nullptr));
        hailButton->setText(QCoreApplication::translate("MainWindow", "Hail", nullptr));
        earthquakeButton->setText(QCoreApplication::translate("MainWindow", "Earthquake", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Weather System:", nullptr));
        restartButton->setText(QCoreApplication::translate("MainWindow", "Restart", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "HELP INFO:", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "TextLabel", nullptr));
        menuSelectLevel_todo->setTitle(QCoreApplication::translate("MainWindow", "Structural Simulation V0.1", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
