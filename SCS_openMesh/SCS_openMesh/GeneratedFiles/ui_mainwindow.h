/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <glWidget.h>
#include "./ui/tree/scsTreeWidget.h"

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
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QTabWidget *tabWidget_Dispaly;
    GLWidget *ModelView;
    GLWidget *simuArea;
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
    QTreeWidget *treeWidget_project;
    QDockWidget *dockWidget;
    scs_GUI::scsTreeWidget *dockWidgetContents_3;
    QDockWidget *dockWidget_Property;
    QWidget *dockWidgetContents_6;
    QVBoxLayout *verticalLayout_4;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QStringLiteral("MainWindowClass"));
        MainWindowClass->resize(1024, 864);
        MainWindowClass->setMinimumSize(QSize(1024, 768));
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
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        tabWidget_Dispaly = new QTabWidget(centralWidget);
        tabWidget_Dispaly->setObjectName(QStringLiteral("tabWidget_Dispaly"));
        tabWidget_Dispaly->setBaseSize(QSize(0, 100));
        tabWidget_Dispaly->setTabPosition(QTabWidget::South);
        ModelView = new GLWidget();
        ModelView->setObjectName(QStringLiteral("ModelView"));
        tabWidget_Dispaly->addTab(ModelView, QString());
        simuArea = new GLWidget();
        simuArea->setObjectName(QStringLiteral("simuArea"));
        tabWidget_Dispaly->addTab(simuArea, QString());
        simuPlane = new GLWidget();
        simuPlane->setObjectName(QStringLiteral("simuPlane"));
        tabWidget_Dispaly->addTab(simuPlane, QString());

        horizontalLayout->addWidget(tabWidget_Dispaly);

        MainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1024, 23));
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
        treeWidget_project = new QTreeWidget(dockWidgetContents_2);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        treeWidget_project->setHeaderItem(__qtreewidgetitem);
        treeWidget_project->setObjectName(QStringLiteral("treeWidget_project"));
        treeWidget_project->setMinimumSize(QSize(0, 200));

        verticalLayout_5->addWidget(treeWidget_project);

        dockWidget_outline->setWidget(dockWidgetContents_2);
        MainWindowClass->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget_outline);
        dockWidget = new QDockWidget(MainWindowClass);
        dockWidget->setObjectName(QStringLiteral("dockWidget"));
        dockWidget->setAllowedAreas(Qt::BottomDockWidgetArea|Qt::LeftDockWidgetArea|Qt::TopDockWidgetArea);
        dockWidgetContents_3 = new scs_GUI::scsTreeWidget();
        dockWidgetContents_3->setObjectName(QStringLiteral("dockWidgetContents_3"));
        dockWidgetContents_3->setStyleSheet(QLatin1String("*{\n"
"    border-style: outset;\n"
"    border-width: 5px;\n"
"\n"
"    border-color: beige;\n"
"}"));
        dockWidget->setWidget(dockWidgetContents_3);
        MainWindowClass->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget);
        dockWidget_Property = new QDockWidget(MainWindowClass);
        dockWidget_Property->setObjectName(QStringLiteral("dockWidget_Property"));
        dockWidget_Property->setMinimumSize(QSize(220, 200));
        dockWidget_Property->setAllowedAreas(Qt::BottomDockWidgetArea|Qt::LeftDockWidgetArea);
        dockWidgetContents_6 = new QWidget();
        dockWidgetContents_6->setObjectName(QStringLiteral("dockWidgetContents_6"));
        verticalLayout_4 = new QVBoxLayout(dockWidgetContents_6);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        dockWidget_Property->setWidget(dockWidgetContents_6);
        MainWindowClass->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget_Property);

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
        menuVisualize->addAction(action_SaveSimuPlane);
        menuVisualize->addAction(action_loadSimuPlane);
        menuVisualize->addSeparator();
        menuVisualize->addAction(action_showPoint);
        menuVisualize->addAction(action_showLine);
        menuVisualize->addAction(action_showFace);
        menuVisualize->addSeparator();
        mainToolBar->addAction(action_showPoint);
        mainToolBar->addAction(action_showLine);
        mainToolBar->addAction(action_showFace);
        mainToolBar->addAction(action_loadPlugin);
        mainToolBar->addAction(action_run);

        retranslateUi(MainWindowClass);

        tabWidget_Dispaly->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "SCS", Q_NULLPTR));
        action->setText(QApplication::translate("MainWindowClass", "\346\226\260\345\273\272", Q_NULLPTR));
        action_3->setText(QApplication::translate("MainWindowClass", "\345\257\274\345\205\245\345\256\244\345\244\226\345\234\272\346\231\257", Q_NULLPTR));
        action_saveLocal->setText(QApplication::translate("MainWindowClass", "\344\277\235\345\255\230\345\261\200\351\203\250\345\234\272\346\231\257", Q_NULLPTR));
        action_6->setText(QApplication::translate("MainWindowClass", "\345\211\226\345\210\206\345\217\202\346\225\260", Q_NULLPTR));
        action_startMesh->setText(QApplication::translate("MainWindowClass", "\345\211\226\345\210\206", Q_NULLPTR));
        action_9->setText(QApplication::translate("MainWindowClass", "\346\235\220\346\226\231\351\200\211\351\241\271", Q_NULLPTR));
        computerOption->setText(QApplication::translate("MainWindowClass", "\351\200\211\351\241\271", Q_NULLPTR));
        action_8->setText(QApplication::translate("MainWindowClass", "\345\234\272\347\202\271", Q_NULLPTR));
        action_11->setText(QApplication::translate("MainWindowClass", "\344\273\277\347\234\237\345\217\202\346\225\260", Q_NULLPTR));
        action_ShowAll->setText(QApplication::translate("MainWindowClass", "\346\225\264\344\275\223\345\234\272\346\231\257\345\261\225\347\244\272", Q_NULLPTR));
        action_obj->setText(QApplication::translate("MainWindowClass", "\345\257\274\345\205\245obj\346\250\241\345\236\213", Q_NULLPTR));
        action_matFile->setText(QApplication::translate("MainWindowClass", "\345\257\274\345\205\245\346\235\220\346\226\231\346\226\207\344\273\266", Q_NULLPTR));
        action_localscene->setText(QApplication::translate("MainWindowClass", "\344\273\277\347\234\237\345\214\272\345\237\237\345\261\225\347\244\272", Q_NULLPTR));
        action_loadPlugin->setText(QApplication::translate("MainWindowClass", "\345\212\240\350\275\275\347\256\227\346\263\225", Q_NULLPTR));
        action_deletePlugin->setText(QApplication::translate("MainWindowClass", "\345\215\270\350\275\275\347\256\227\346\263\225", Q_NULLPTR));
        action_run->setText(QApplication::translate("MainWindowClass", "\350\256\241\347\256\227", Q_NULLPTR));
        action_json->setText(QApplication::translate("MainWindowClass", "\345\277\253\351\200\237\345\257\274\345\205\245\345\234\272\346\231\257(.json)", Q_NULLPTR));
        action_SaveSimuPlane->setText(QApplication::translate("MainWindowClass", "\344\277\235\345\255\230\344\273\277\347\234\237\351\235\242", Q_NULLPTR));
        action_loadSimuPlane->setText(QApplication::translate("MainWindowClass", "\345\257\274\345\205\245\344\273\277\347\234\237\351\235\242", Q_NULLPTR));
        action_showPoint->setText(QApplication::translate("MainWindowClass", "\346\230\276\347\244\272\347\202\271", Q_NULLPTR));
        action_showLine->setText(QApplication::translate("MainWindowClass", "\346\230\276\347\244\272\347\272\277", Q_NULLPTR));
        action_showFace->setText(QApplication::translate("MainWindowClass", "\346\230\276\347\244\272\351\235\242", Q_NULLPTR));
        action_GenerateModelPara->setText(QApplication::translate("MainWindowClass", "\347\224\237\346\210\220\350\256\241\347\256\227\345\217\202\346\225\260", Q_NULLPTR));
        tabWidget_Dispaly->setTabText(tabWidget_Dispaly->indexOf(ModelView), QApplication::translate("MainWindowClass", "\346\250\241\345\236\213\345\234\272\346\231\257", Q_NULLPTR));
        tabWidget_Dispaly->setTabText(tabWidget_Dispaly->indexOf(simuArea), QApplication::translate("MainWindowClass", "\344\273\277\347\234\237\345\234\272\346\231\257", Q_NULLPTR));
        tabWidget_Dispaly->setTabText(tabWidget_Dispaly->indexOf(simuPlane), QApplication::translate("MainWindowClass", "\344\273\277\347\234\237\351\235\242", Q_NULLPTR));
        menuFILE->setTitle(QApplication::translate("MainWindowClass", "File", Q_NULLPTR));
        menuMesh->setTitle(QApplication::translate("MainWindowClass", "Mesh", Q_NULLPTR));
        menuComputer->setTitle(QApplication::translate("MainWindowClass", "Computer", Q_NULLPTR));
        menuVisualize->setTitle(QApplication::translate("MainWindowClass", "Visualize", Q_NULLPTR));
        menuAbout->setTitle(QApplication::translate("MainWindowClass", "About", Q_NULLPTR));
        dockWidget_outputLog->setWindowTitle(QApplication::translate("MainWindowClass", "\346\227\245\345\277\227", Q_NULLPTR));
        dockWidget_outline->setWindowTitle(QApplication::translate("MainWindowClass", "\347\233\256\345\275\225", Q_NULLPTR));
        dockWidget_Property->setWindowTitle(QApplication::translate("MainWindowClass", "\345\261\236\346\200\247", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
