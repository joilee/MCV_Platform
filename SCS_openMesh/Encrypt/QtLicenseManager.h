#ifndef QTLICENSEMANAGER_H
#define QTLICENSEMANAGER_H

#include "LicenseManager.h"
#include <QThread>
#include <QMessageBox>
#include "registerDialog.h"


class LicenseCheckerThread : public QThread
{
	Q_OBJECT
public:
	void run();
	static LicenseCheckerThread* instance();
	void setMessageHandler(void* pHandler);
	void message(const QString & title, const QString & text, int* ret);
	void error(const QString & title, const QString & text, int* ret);
	void warning(const QString & title, const QString & text, int* ret);

signals:
	void message(int icon, const QString & title, const QString & text, int* ret, QMessageBox::StandardButtons buttons = QMessageBox::Ok, QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);
	void exitApp();

private:
	static LicenseCheckerThread* m_pInstance;
};

class QtLicenseManager :public LicenseManager
{
public:
	virtual unsigned long check(char* msg = 0, const char* moduleName = 0);
	virtual unsigned long checksentinel(char* msg = 0, const char* moduleName = 0);
	virtual void startLicenseThread();
	virtual void setMessageHandler(void* pHandler);
	static void init();

private:
	QtLicenseManager(void);
	~QtLicenseManager(void);
	unsigned long isAppValid(char* msg);
	ueesRegisterDialog* m_registerDialog;
	ueesCodeRegister* m_codeRegister;
};

#endif // QTLICENSEMANAGER_H