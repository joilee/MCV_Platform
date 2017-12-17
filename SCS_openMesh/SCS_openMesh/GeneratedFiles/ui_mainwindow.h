/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <glWidget.h>
#include "scsLocalModelGLWidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QAction *action;
    QAction *action_3;
    QAction *action_saveLocal;
    QAction *action_6;
    QAction *action_startMesh;
    QAction *action_9;
    QAction *computerOption;
    QAction *action_8;
    QAction *action_11;
    QAction *action_ShowAll;
    QAction *action_obj;
    QAction *action_matFile;
    QAction *action_localscene;
    QAction *action_loadPlugin;
    QAction *action_deletePlugin;
    QAction *action_run;
    QAction *action_json;
    QAction *action_SaveSimuPlane;
    QAction *action_loadSimuPlane;
    QAction *action_showPoint;
    QAction *action_showLine;
    QAction *action_showFace;
    QAction *action_GenerateModelPara;
    QAction *action_scatter;
    QAction *action_saveResult;
    QAction *action_loadResult;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QTabWidget *tabWidget_Dispaly;
    GLWidget *ModelView;
    scsLocalModelGLWidget *simuArea;
    GLWidget *simuPlane;
    QMenuBar *menuBar;
    QMenu *menuFILE;
    QMenu *menuMesh;
    QMenu *menuComputer;
    QMenu *menuVisualize;
    QMenu *menuAbout;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *dockWidget_outputLog;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QTextBrowser *textBrowser;
    QProgressBar *progressBar;
    QDockWidget *dockWidget_outline;
    QWidget *dockWidgetContents_2;
    QVBoxLayout *verticalLayout_5;
    QDockWidget *dockWidget_ShowPanel;
    QWidget *dockWidgetContents_6;
    QVBoxLayout *verticalLayout_4;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_3;
    QGridLayout *gridLayout;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QSlider *horizontalSlider_Scene_Alpha;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_2;
    QSlider *horizontalSlider_Plane_Alpha;
    QGroupBox *groupBox_2;
    QGroupBox *groupBox_3;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_5;
    QLineEdit *lineEdit_SimuPlane_siteID_2;
    QLabel *label_6;
    QLineEdit *lineEdit_SimuPlane_PCI_2;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QLineEdit *lineEdit_SimuPlane_siteID;
    QLabel *label_4;
    QLineEdit *lineEdit_SimuPlane_PCI;
    QGroupBox *groupBox_4;
    QWidget *layoutWidget_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_7;
    QLineEdit *lineEdit_ModelID;
    QDockWidget *dockWidget_Property;
    QWidget *dockWidgetContents_4;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QStringLiteral("MainWindowClass"));
        MainWindowClass->resize(1280, 892);
        MainWindowClass->setMinimumSize(QSize(1280, 800));
        action = new QAction(MainWindowClass);
        action->setObjectName(QStringLiteral("action"));
        action_3 = new QAction(MainWindowClass);
        action_3->setObjectName(QStringLiteral("action_3"));
        action_saveLocal = new QAction(MainWindowClass);
        action_saveLocal->setObjectName(QStringLiteral("action_saveLocal"));
        action_6 = new QAction(MainWindowClass);
        action_6->setObjectName(QStringLiteral("action_6"));
        action_startMesh = new QAction(MainWindowClass);
        action_startMesh->setObjectName(QStringLiteral("action_startMesh"));
        action_9 = new QAction(MainWindowClass);
        action_9->setObjectName(QStringLiteral("action_9"));
        computerOption = new QAction(MainWindowClass);
        computerOption->setObjectName(QStringLiteral("computerOption"));
        action_8 = new QAction(MainWindowClass);
        action_8->setObjectName(QStringLiteral("action_8"));
        action_11 = new QAction(MainWindowClass);
        action_11->setObjectName(QStringLiteral("action_11"));
        action_ShowAll = new QAction(MainWindowClass);
        action_ShowAll->setObjectName(QStringLiteral("action_ShowAll"));
        action_obj = new QAction(MainWindowClass);
        action_obj->setObjectName(QStringLiteral("action_obj"));
        action_matFile = new QAction(MainWindowClass);
        action_matFile->setObjectName(QStringLiteral("action_matFile"));
        action_localscene = new QAction(MainWindowClass);
        action_localscene->setObjectName(QStringLiteral("action_localscene"));
        action_loadPlugin = new QAction(MainWindowClass);
        action_loadPlugin->setObjectName(QStringLiteral("action_loadPlugin"));
        QIcon icon;
        icon.addFile(QStringLiteral("Resources/pluginInsert.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_loadPlugin->setIcon(icon);
        action_deletePlugin = new QAction(MainWindowClass);
        action_deletePlugin->setObjectName(QStringLiteral("action_deletePlugin"));
        action_run = new QAction(MainWindowClass);
        action_run->setObjectName(QStringLiteral("action_run"));
        QIcon icon1;
        icon1.addFile(QStringLiteral("Resources/run.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_run->setIcon(icon1);
        action_json = new QAction(MainWindowClass);
        action_json->setObjectName(QStringLiteral("action_json"));
        action_SaveSimuPlane = new QAction(MainWindowClass);
        action_SaveSimuPlane->setObjectName(QStringLiteral("action_SaveSimuPlane"));
        action_loadSimuPlane = new QAction(MainWindowClass);
        action_loadSimuPlane->setObjectName(QStringLiteral("action_loadSimuPlane"));
        action_showPoint = new QAction(MainWindowClass);
        action_showPoint->setObjectName(QStringLiteral("action_showPoint"));
        action_showPoint->setCheckable(true);
        QIcon icon2;
        icon2.addFile(QStringLiteral("Resources/point.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_showPoint->setIcon(icon2);
        action_showLine = new QAction(MainWindowClass);
        action_showLine->setObjectName(QStringLiteral("action_showLine"));
        action_showLine->setCheckable(true);
        QIcon icon3;
        icon3.addFile(QStringLiteral("Resources/line.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_showLine->setIcon(icon3);
        action_showFace = new QAction(MainWindowClass);
        action_showFace->setObjectName(QStringLiteral("action_showFace"));
        action_showFace->setCheckable(true);
        QIcon icon4;
        icon4.addFile(QStringLiteral("Resources/face.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_showFace->setIcon(icon4);
        action_GenerateModelPara = new QAction(MainWindowClass);
        action_GenerateModelPara->setObjectName(QStringLiteral("action_GenerateModelPara"));
        action_scatter = new QAction(MainWindowClass);
        action_scatter->setObjectName(QStringLiteral("action_scatter"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/MainWindow/Resources/scatter.ico"), QSize(), QIcon::Normal, QIcon::Off);
        action_scatter->setIcon(icon5);
        action_saveResult = new QAction(MainWindowClass);
        action_saveResult->setObjectName(QStringLiteral("action_saveResult"));
        action_loadResult = new QAction(MainWindowClass);
        action_loadResult->setObjectName(QStringLiteral("action_loadResult"));
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        tabWidget_Dispaly = new QTabWidget(centralWidget);
        tabWidget_Dispaly->setObjectName(QStringLiteral("tabWidget_Dispaly"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tabWidget_Dispaly->sizePolicy().hasHeightForWidth());
        tabWidget_Dispaly->setSizePolicy(sizePolicy);
        tabWidget_Dispaly->setBaseSize(QSize(0, 100));
        tabWidget_Dispaly->setTabPosition(QTabWidget::South);
        ModelView = new GLWidget();
        ModelView->setObjectName(QStringLiteral("ModelView"));
        tabWidget_Dispaly->addTab(ModelView, QString());
        simuArea = new scsLocalModelGLWidget();
        simuArea->setObjectName(QStringLiteral("simuArea"));
        tabWidget_Dispaly->addTab(simuArea, QString());
        simuPlane = new GLWidget();
        simuPlane->setObjectName(QStringLiteral("simuPlane"));
        tabWidget_Dispaly->addTab(simuPlane, QString());

        horizontalLayout->addWidget(tabWidget_Dispaly);

        MainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1280, 23));
        menuFILE = new QMenu(menuBar);
        menuFILE->setObjectName(QStringLiteral("menuFILE"));
        menuMesh = new QMenu(menuBar);
        menuMesh->setObjectName(QStringLiteral("menuMesh"));
        menuComputer = new QMenu(menuBar);
        menuComputer->setObjectName(QStringLiteral("menuComputer"));
        menuVisualize = new QMenu(menuBar);
        menuVisualize->setObjectName(QStringLiteral("menuVisualize"));
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName(QStringLiteral("menuAbout"));
        MainWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindowClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindowClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindowClass->setStatusBar(statusBar);
        dockWidget_outputLog = new QDockWidget(MainWindowClass);
        dockWidget_outputLog->setObjectName(QStringLiteral("dockWidget_outputLog"));
        dockWidget_outputLog->setMinimumSize(QSize(147, 187));
        dockWidget_outputLog->setFeatures(QDockWidget::DockWidgetFloatable);
        dockWidget_outputLog->setAllowedAreas(Qt::BottomDockWidgetArea|Qt::RightDockWidgetArea);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        verticalLayout_2 = new QVBoxLayout(dockWidgetContents);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        textBrowser = new QTextBrowser(dockWidgetContents);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));

        verticalLayout->addWidget(textBrowser);

        progressBar = new QProgressBar(dockWidgetContents);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setValue(0);

        verticalLayout->addWidget(progressBar);


        verticalLayout_2->addLayout(verticalLayout);

        dockWidget_outputLog->setWidget(dockWidgetContents);
        MainWindowClass->addDockWidget(static_cast<Qt::DockWidgetArea>(8), dockWidget_outputLog);
        dockWidget_outline = new QDockWidget(MainWindowClass);
        dockWidget_outline->setObjectName(QStringLiteral("dockWidget_outline"));
        dockWidget_outline->setMinimumSize(QSize(256, 300));
        dockWidget_outline->setFeatures(QDockWidget::NoDockWidgetFeatures);
        dockWidget_outline->setAllowedAreas(Qt::LeftDockWidgetArea);
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QStringLiteral("dockWidgetContents_2"));
        verticalLayout_5 = new QVBoxLayout(dockWidgetContents_2);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        dockWidget_outline->setWidget(dockWidgetContents_2);
        MainWindowClass->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget_outline);
        dockWidget_ShowPanel = new QDockWidget(MainWindowClass);
        dockWidget_ShowPanel->setObjectName(QStringLiteral("dockWidget_ShowPanel"));
        dockWidget_ShowPanel->setMinimumSize(QSize(220, 200));
        dockWidget_ShowPanel->setAllowedAreas(Qt::BottomDockWidgetArea|Qt::LeftDockWidgetArea);
        dockWidgetContents_6 = new QWidget();
        dockWidgetContents_6->setObjectName(QStringLiteral("dockWidgetContents_6"));
        verticalLayout_4 = new QVBoxLayout(dockWidgetContents_6);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        groupBox = new QGroupBox(dockWidgetContents_6);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout_3 = new QVBoxLayout(groupBox);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 1, 1, 1);

        horizontalSlider_Scene_Alpha = new QSlider(groupBox);
        horizontalSlider_Scene_Alpha->setObjectName(QStringLiteral("horizontalSlider_Scene_Alpha"));
        horizontalSlider_Scene_Alpha->setMaximum(100);
        horizontalSlider_Scene_Alpha->setPageStep(10);
        horizontalSlider_Scene_Alpha->setOrientation(Qt::Horizontal);
        horizontalSlider_Scene_Alpha->setTickPosition(QSlider::TicksAbove);

        gridLayout->addWidget(horizontalSlider_Scene_Alpha, 0, 2, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(28, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 1, 1, 1);

        horizontalSlider_Plane_Alpha = new QSlider(groupBox);
        horizontalSlider_Plane_Alpha->setObjectName(QStringLiteral("horizontalSlider_Plane_Alpha"));
        horizontalSlider_Plane_Alpha->setMaximum(100);
        horizontalSlider_Plane_Alpha->setOrientation(Qt::Horizontal);
        horizontalSlider_Plane_Alpha->setTickPosition(QSlider::TicksAbove);

        gridLayout->addWidget(horizontalSlider_Plane_Alpha, 1, 2, 1, 1);


        verticalLayout_3->addLayout(gridLayout);


        verticalLayout_4->addWidget(groupBox);

        groupBox_2 = new QGroupBox(dockWidgetContents_6);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_3 = new QGroupBox(groupBox_2);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(0, 0, 238, 73));
        layoutWidget = new QWidget(groupBox_3);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 30, 164, 22));
        horizontalLayout_3 = new QHBoxLayout(layoutWidget);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_3->addWidget(label_5);

        lineEdit_SimuPlane_siteID_2 = new QLineEdit(layoutWidget);
        lineEdit_SimuPlane_siteID_2->setObjectName(QStringLiteral("lineEdit_SimuPlane_siteID_2"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lineEdit_SimuPlane_siteID_2->sizePolicy().hasHeightForWidth());
        lineEdit_SimuPlane_siteID_2->setSizePolicy(sizePolicy1);
        lineEdit_SimuPlane_siteID_2->setMaximumSize(QSize(51, 16777215));
        lineEdit_SimuPlane_siteID_2->setReadOnly(true);

        horizontalLayout_3->addWidget(lineEdit_SimuPlane_siteID_2);

        label_6 = new QLabel(layoutWidget);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_3->addWidget(label_6);

        lineEdit_SimuPlane_PCI_2 = new QLineEdit(layoutWidget);
        lineEdit_SimuPlane_PCI_2->setObjectName(QStringLiteral("lineEdit_SimuPlane_PCI_2"));
        sizePolicy1.setHeightForWidth(lineEdit_SimuPlane_PCI_2->sizePolicy().hasHeightForWidth());
        lineEdit_SimuPlane_PCI_2->setSizePolicy(sizePolicy1);
        lineEdit_SimuPlane_PCI_2->setMaximumSize(QSize(51, 16777215));
        lineEdit_SimuPlane_PCI_2->setReadOnly(true);

        horizontalLayout_3->addWidget(lineEdit_SimuPlane_PCI_2);

        layoutWidget1 = new QWidget(groupBox_2);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(20, 30, 164, 22));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_3 = new QLabel(layoutWidget1);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_2->addWidget(label_3);

        lineEdit_SimuPlane_siteID = new QLineEdit(layoutWidget1);
        lineEdit_SimuPlane_siteID->setObjectName(QStringLiteral("lineEdit_SimuPlane_siteID"));
        sizePolicy1.setHeightForWidth(lineEdit_SimuPlane_siteID->sizePolicy().hasHeightForWidth());
        lineEdit_SimuPlane_siteID->setSizePolicy(sizePolicy1);
        lineEdit_SimuPlane_siteID->setMaximumSize(QSize(51, 16777215));
        lineEdit_SimuPlane_siteID->setReadOnly(true);

        horizontalLayout_2->addWidget(lineEdit_SimuPlane_siteID);

        label_4 = new QLabel(layoutWidget1);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_2->addWidget(label_4);

        lineEdit_SimuPlane_PCI = new QLineEdit(layoutWidget1);
        lineEdit_SimuPlane_PCI->setObjectName(QStringLiteral("lineEdit_SimuPlane_PCI"));
        sizePolicy1.setHeightForWidth(lineEdit_SimuPlane_PCI->sizePolicy().hasHeightForWidth());
        lineEdit_SimuPlane_PCI->setSizePolicy(sizePolicy1);
        lineEdit_SimuPlane_PCI->setMaximumSize(QSize(51, 16777215));
        lineEdit_SimuPlane_PCI->setReadOnly(true);

        horizontalLayout_2->addWidget(lineEdit_SimuPlane_PCI);


        verticalLayout_4->addWidget(groupBox_2);

        groupBox_4 = new QGroupBox(dockWidgetContents_6);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        layoutWidget_2 = new QWidget(groupBox_4);
        layoutWidget_2->setObjectName(QStringLiteral("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(20, 30, 164, 22));
        horizontalLayout_4 = new QHBoxLayout(layoutWidget_2);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_4->setContentsMargins(0, 0, 0, 0);
        label_7 = new QLabel(layoutWidget_2);
        label_7->setObjectName(QStringLiteral("label_7"));

        horizontalLayout_4->addWidget(label_7);

        lineEdit_ModelID = new QLineEdit(layoutWidget_2);
        lineEdit_ModelID->setObjectName(QStringLiteral("lineEdit_ModelID"));
        sizePolicy1.setHeightForWidth(lineEdit_ModelID->sizePolicy().hasHeightForWidth());
        lineEdit_ModelID->setSizePolicy(sizePolicy1);
        lineEdit_ModelID->setMaximumSize(QSize(51, 16777215));
        lineEdit_ModelID->setReadOnly(true);

        horizontalLayout_4->addWidget(lineEdit_ModelID);


        verticalLayout_4->addWidget(groupBox_4);

        dockWidget_ShowPanel->setWidget(dockWidgetContents_6);
        MainWindowClass->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget_ShowPanel);
        dockWidget_Property = new QDockWidget(MainWindowClass);
        dockWidget_Property->setObjectName(QStringLiteral("dockWidget_Property"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(dockWidget_Property->sizePolicy().hasHeightForWidth());
        dockWidget_Property->setSizePolicy(sizePolicy2);
        dockWidget_Property->setMinimumSize(QSize(200, 200));
        dockWidget_Property->setBaseSize(QSize(0, 200));
        dockWidget_Property->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        dockWidget_Property->setAllowedAreas(Qt::RightDockWidgetArea|Qt::TopDockWidgetArea);
        dockWidgetContents_4 = new QWidget();
        dockWidgetContents_4->setObjectName(QStringLiteral("dockWidgetContents_4"));
        dockWidget_Property->setWidget(dockWidgetContents_4);
        MainWindowClass->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget_Property);

        menuBar->addAction(menuFILE->menuAction());
        menuBar->addAction(menuMesh->menuAction());
        menuBar->addAction(menuComputer->menuAction());
        menuBar->addAction(menuVisualize->menuAction());
        menuBar->addAction(menuAbout->menuAction());
        menuFILE->addAction(action);
        menuFILE->addSeparator();
        menuFILE->addAction(action_matFile);
        menuFILE->addSeparator();
        menuFILE->addAction(action_3);
        menuFILE->addAction(action_obj);
        menuFILE->addSeparator();
        menuFILE->addAction(action_json);
        menuFILE->addSeparator();
        menuFILE->addAction(action_saveLocal);
        menuFILE->addSeparator();
        menuFILE->addAction(action_saveResult);
        menuFILE->addAction(action_loadResult);
        menuMesh->addAction(action_6);
        menuMesh->addAction(action_startMesh);
        menuMesh->addSeparator();
        menuMesh->addAction(action_9);
        menuMesh->addSeparator();
        menuMesh->addAction(action_GenerateModelPara);
        menuComputer->addAction(computerOption);
        menuComputer->addSeparator();
        menuComputer->addAction(action_loadPlugin);
        menuComputer->addAction(action_deletePlugin);
        menuComputer->addSeparator();
        menuComputer->addAction(action_run);
        menuVisualize->addAction(action_ShowAll);
        menuVisualize->addAction(action_localscene);
        menuVisualize->addSeparator();
        menuVisualize->addAction(action_showPoint);
        menuVisualize->addAction(action_showLine);
        menuVisualize->addAction(action_showFace);
        menuVisualize->addSeparator();
        menuVisualize->addAction(action_scatter);
        mainToolBar->addAction(action_showPoint);
        mainToolBar->addAction(action_showLine);
        mainToolBar->addAction(action_showFace);
        mainToolBar->addAction(action_loadPlugin);
        mainToolBar->addAction(action_run);

        retranslateUi(MainWindowClass);

        tabWidget_Dispaly->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "SCS", 0));
        action->setText(QApplication::translate("MainWindowClass", "\346\226\260\345\273\272", 0));
        action_3->setText(QApplication::translate("MainWindowClass", "\345\257\274\345\205\245\345\256\244\345\244\226\345\234\272\346\231\257", 0));
        action_saveLocal->setText(QApplication::translate("MainWindowClass", "\344\277\235\345\255\230\345\261\200\351\203\250\345\234\272\346\231\257", 0));
        action_6->setText(QApplication::translate("MainWindowClass", "\345\211\226\345\210\206\345\217\202\346\225\260", 0));
        action_startMesh->setText(QApplication::translate("MainWindowClass", "\345\211\226\345\210\206", 0));
        action_9->setText(QApplication::translate("MainWindowClass", "\346\235\220\346\226\231\351\200\211\351\241\271", 0));
        computerOption->setText(QApplication::translate("MainWindowClass", "\351\200\211\351\241\271", 0));
        action_8->setText(QApplication::translate("MainWindowClass", "\345\234\272\347\202\271", 0));
        action_11->setText(QApplication::translate("MainWindowClass", "\344\273\277\347\234\237\345\217\202\346\225\260", 0));
        action_ShowAll->setText(QApplication::translate("MainWindowClass", "\346\225\264\344\275\223\345\234\272\346\231\257\345\261\225\347\244\272", 0));
        action_obj->setText(QApplication::translate("MainWindowClass", "\345\257\274\345\205\245obj\346\250\241\345\236\213", 0));
        action_matFile->setText(QApplication::translate("MainWindowClass", "\345\257\274\345\205\245\346\235\220\346\226\231\346\226\207\344\273\266", 0));
        action_localscene->setText(QApplication::translate("MainWindowClass", "\344\273\277\347\234\237\345\214\272\345\237\237\345\261\225\347\244\272", 0));
        action_loadPlugin->setText(QApplication::translate("MainWindowClass", "\345\212\240\350\275\275\347\256\227\346\263\225", 0));
        action_deletePlugin->setText(QApplication::translate("MainWindowClass", "\345\215\270\350\275\275\347\256\227\346\263\225", 0));
        action_run->setText(QApplication::translate("MainWindowClass", "\350\256\241\347\256\227", 0));
        action_json->setText(QApplication::translate("MainWindowClass", "\345\277\253\351\200\237\345\257\274\345\205\245\345\234\272\346\231\257(.json)", 0));
        action_SaveSimuPlane->setText(QApplication::translate("MainWindowClass", "\344\277\235\345\255\230\344\273\277\347\234\237\351\235\242", 0));
        action_loadSimuPlane->setText(QApplication::translate("MainWindowClass", "\345\257\274\345\205\245\344\273\277\347\234\237\351\235\242", 0));
        action_showPoint->setText(QApplication::translate("MainWindowClass", "\346\230\276\347\244\272\347\202\271", 0));
        action_showLine->setText(QApplication::translate("MainWindowClass", "\346\230\276\347\244\272\347\272\277", 0));
        action_showFace->setText(QApplication::translate("MainWindowClass", "\346\230\276\347\244\272\351\235\242", 0));
        action_GenerateModelPara->setText(QApplication::translate("MainWindowClass", "\347\224\237\346\210\220\346\250\241\345\236\213\345\217\202\346\225\260", 0));
        action_scatter->setText(QApplication::translate("MainWindowClass", "\346\216\245\346\224\266\347\202\271\346\225\243\347\202\271\345\233\276", 0));
        action_saveResult->setText(QApplication::translate("MainWindowClass", "\344\277\235\345\255\230\344\273\277\347\234\237\346\225\260\346\215\256", 0));
        action_loadResult->setText(QApplication::translate("MainWindowClass", "\345\257\274\345\205\245\344\273\277\347\234\237\346\225\260\346\215\256", 0));
        tabWidget_Dispaly->setTabText(tabWidget_Dispaly->indexOf(ModelView), QApplication::translate("MainWindowClass", "\346\250\241\345\236\213\345\234\272\346\231\257", 0));
        tabWidget_Dispaly->setTabText(tabWidget_Dispaly->indexOf(simuArea), QApplication::translate("MainWindowClass", "\344\273\277\347\234\237\345\234\272\346\231\257", 0));
        tabWidget_Dispaly->setTabText(tabWidget_Dispaly->indexOf(simuPlane), QApplication::translate("MainWindowClass", "\344\273\277\347\234\237\351\235\242", 0));
        menuFILE->setTitle(QApplication::translate("MainWindowClass", "File", 0));
        menuMesh->setTitle(QApplication::translate("MainWindowClass", "Mesh", 0));
        menuComputer->setTitle(QApplication::translate("MainWindowClass", "Computer", 0));
        menuVisualize->setTitle(QApplication::translate("MainWindowClass", "Visualize", 0));
        menuAbout->setTitle(QApplication::translate("MainWindowClass", "About", 0));
        dockWidget_outputLog->setWindowTitle(QApplication::translate("MainWindowClass", "\346\227\245\345\277\227", 0));
        dockWidget_outline->setWindowTitle(QApplication::translate("MainWindowClass", "\347\233\256\345\275\225", 0));
        dockWidget_ShowPanel->setWindowTitle(QApplication::translate("MainWindowClass", "\344\273\277\347\234\237\345\217\257\350\247\206\345\214\226\350\256\276\347\275\256", 0));
        groupBox->setTitle(QApplication::translate("MainWindowClass", "\351\200\217\346\230\216\345\272\246", 0));
        label->setText(QApplication::translate("MainWindowClass", "\345\234\272\346\231\257\351\200\217\346\230\216\345\272\246", 0));
        label_2->setText(QApplication::translate("MainWindowClass", "\344\273\277\347\234\237\351\235\242\351\200\217\346\230\216\345\272\246", 0));
        groupBox_2->setTitle(QApplication::translate("MainWindowClass", "\344\273\277\347\234\237\351\235\242", 0));
        groupBox_3->setTitle(QApplication::translate("MainWindowClass", "\344\273\277\347\234\237\351\235\242", 0));
        label_5->setText(QApplication::translate("MainWindowClass", "Site", 0));
        label_6->setText(QApplication::translate("MainWindowClass", "PCI", 0));
        lineEdit_SimuPlane_PCI_2->setText(QString());
        label_3->setText(QApplication::translate("MainWindowClass", "Site", 0));
        label_4->setText(QApplication::translate("MainWindowClass", "PCI", 0));
        lineEdit_SimuPlane_PCI->setText(QString());
        groupBox_4->setTitle(QApplication::translate("MainWindowClass", "\346\250\241\345\236\213", 0));
        label_7->setText(QApplication::translate("MainWindowClass", "\346\250\241\345\236\213ID", 0));
        dockWidget_Property->setWindowTitle(QApplication::translate("MainWindowClass", "\346\250\241\345\236\213\345\261\236\346\200\247", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
