#ifndef UEESMESSAGEHANDLER_H
#define UEESMESSAGEHANDLER_H

#include <QObject>
#include <QMessageBox>

// 用于与线程同步的信号量。当线程需要等message box关闭再继续执行时可使用该信号量进行等待
#include <QWaitCondition>
#include <QMutex>
extern QMutex mutex;
extern QWaitCondition msgDlgClosed;

class ueesMessageHandler : public QObject
{
	Q_OBJECT
	public slots:
	void messageBox(int icon, const QString & title, const QString & text, int* ret, QMessageBox::StandardButtons buttons = QMessageBox::Ok, QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);
};

#endif // EMXMESSAGEHANDLER_H