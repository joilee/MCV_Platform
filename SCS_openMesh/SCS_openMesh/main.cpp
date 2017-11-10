
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

globalContext *globalContext::ctx = NULL;

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));

	QApplication a(argc, argv);
	a.setStyleSheet("QLineEdit{selection-color:yellow;selection-background-color:rgb(0,0,58);border:1px groove gray;border-radius:5px;}"); //background-color:rgb(255,255,241);color:rgb(127,0,63);

	QPixmap pixmap("ImageFIle\\StartExe\\start.png");
	QSplashScreen screen(pixmap);
	screen.show();
	int delayTime =0.5;
	QElapsedTimer timer;
	timer.start();
	while (timer.elapsed() < (delayTime * 1000))
	{
		a.processEvents();
	}

	MainWindow w;
	w.show();
	screen.finish(&w);
	return a.exec();

}
