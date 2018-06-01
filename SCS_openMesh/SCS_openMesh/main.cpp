
#include <QtCore/QCoreApplication>

#include <gl/freeglut.h>
#include "ui/mainwindow.h"
#include <QtWidgets/QApplication>
#include <QSplashScreen>  
#include <QPixmap>  
#include <QDebug>
#include <QTextCodec>
#include <QElapsedTimer>  
#include <QDateTime>  
#include <QTreeView>
#include <iostream>
using namespace std;

#define USE_LICENSE_MANAGE
#ifdef USE_LICENSE_MANAGE
#include "../Encrypt/QtLicenseManager.h"
#include "../Encrypt/messageHandler.h"
#endif
#include "../Encrypt/registerDialog.h"
#include <DbgHelp.h>
#include <QTime>

globalContext *globalContext::ctx = NULL;
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));

	QApplication a(argc, argv);
	MainWindow w;
	
//封装代码-------------开始
	qRegisterMetaType<QItemSelection>("QItemSelection");
	qRegisterMetaType<QMessageBox::StandardButton>("QMessageBox::StandardButton");
	qRegisterMetaType<QMessageBox::StandardButtons>("QMessageBox::StandardButtons");

	//启动检测
#ifdef USE_LICENSE_MANAGE
	ueesMessageHandler* msgHandler = new ueesMessageHandler;
	QtLicenseManager::init(); // init a license manager
	LicenseManager::instance()->setMessageHandler(msgHandler);
	char errInfo[256];
	sprintf(errInfo, "%s", QApplication::translate("QtLicenseManager", "please contact State Key Lab of CAD&CG,ZheJiang University!", 0).toLocal8Bit().data());
	if (LicenseManager::instance()->check(errInfo) != LM_SUCCESS)
	{
		QMessageBox::critical(NULL, QApplication::translate("QtLicenseManager", "Error", 0), QString::fromLocal8Bit(errInfo));
		exit(0);
	}
#endif

	//一直检测
#ifdef USE_LICENSE_MANAGE
	LicenseManager::instance()->startLicenseThread(); // 启动授权检查线程
	msgHandler->setParent(&w);
#endif
//封装代码---------------结束

	a.setStyleSheet("QLineEdit{selection-color:yellow;selection-background-color:rgb(0,0,58);border:1px groove gray;border-radius:5px;}"); //background-color:rgb(255,255,241);color:rgb(127,0,63);
	QPixmap pixmap(":/MainWindow/image/StartExe/start.jpg");
	QSplashScreen screen(pixmap);
	screen.show();

	int delayTime = 2;
	QElapsedTimer timer;
	timer.start();
	while (timer.elapsed() < (delayTime * 500))
	{
		a.processEvents();
	}
	a.setWindowIcon(QIcon(":/MainWindow/image/appIcon/myapp.ico"));
	w.show();
	screen.finish(&w);
	return a.exec();

}
