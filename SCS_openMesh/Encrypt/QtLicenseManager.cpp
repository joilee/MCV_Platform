#include "QtLicenseManager.h"
#include "messageHandler.h"
#include "sentinel/SentinelKeys.h"
#include "sentinel/SentinelKeysLicense.h"
#include <QApplication>
#include <QSettings>

#pragma comment(lib, "SentinelKeyW.lib")

LicenseCheckerThread* LicenseCheckerThread::m_pInstance = NULL;

LicenseCheckerThread* LicenseCheckerThread::instance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new LicenseCheckerThread;
	}
	return m_pInstance;
}

void LicenseCheckerThread::run()
{
	unsigned long status = LM_SUCCESS;
	char msg[256];
	while (true)
	{
		// 		static int count = 0;
		// 		printf("LicenseCheckerThread 's running %d\n", count++);
		status = LicenseManager::instance()->check(&msg[0]);
		if (status != LM_SUCCESS)
		{
			int ret = -1;
			error(QApplication::translate("QtLicenseManager", "Error", 0), QString::fromLocal8Bit(msg), &ret);
			// 等待对话框返回
			mutex.lock();
			msgDlgClosed.wait(&mutex);
			ret = -1;
			break;
			mutex.unlock();
		}
		else
			QThread::sleep(2);
	}
	if (status != LM_SUCCESS)
	{
		qApp->quit();
	}
	exec();
}

void LicenseCheckerThread::setMessageHandler(void* pHandler)
{
	connect(this, SIGNAL(message(int, const QString &, const QString &, int*, QMessageBox::StandardButtons, QMessageBox::StandardButton)),
		(ueesMessageHandler*)pHandler, SLOT(messageBox(int, const QString &, const QString &, int*, QMessageBox::StandardButtons, QMessageBox::StandardButton)));
}

void LicenseCheckerThread::message(const QString & title, const QString & text, int* ret)
{
	emit message(QMessageBox::Information, title, text, ret);
}

void LicenseCheckerThread::error(const QString & title, const QString & text, int* ret)
{
	emit message(QMessageBox::Critical, title, text, ret);
}

void LicenseCheckerThread::warning(const QString & title, const QString & text, int* ret)
{
	emit message(QMessageBox::Warning, title, text, ret);
}

//////////////////////////////////////////////////////////////////////////

void QtLicenseManager::init()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new QtLicenseManager;
	}
}

QtLicenseManager::QtLicenseManager(void)
{
	m_registerDialog = new ueesRegisterDialog;
	m_codeRegister = m_registerDialog->getCodeRegister();
	setManager(this);
}

QtLicenseManager::~QtLicenseManager(void)
{}
/**@brief modification for add the register code method
* @since 2014/4/30
*/
unsigned long QtLicenseManager::check(char* msg, const char* moduleName)
{
	QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "ZJU", "UEESIE");
	int RegisterMode = settings.value("RegisterMode", -1).toInt();
	//m_codeRegister->RegisterAndValidate();
	if (RegisterMode == 0)
	{
		if (m_codeRegister->RegisterAndValidate())
		{
			return LM_SUCCESS;
		}
		else{
			RegisterMode = -1;
		}
	}
	else{
		if (RegisterMode == 1)
		{
			if (checksentinel(msg, moduleName) == LM_SUCCESS)
			{
				return LM_SUCCESS;
			}
			else{
				RegisterMode = -1;
			}
		}
	}
	if (RegisterMode == -1)
	{
		if (m_registerDialog->exec() == QDialog::Accepted)
		{
			if (m_registerDialog->getStatus())
			{
				return LM_SUCCESS;
			}
		}
	}
	settings.remove("RegisterMode");
	return LM_FAIL;
}

//unsigned long QtLicenseManager::check( char* msg, const char* moduleName )
//{
//	unsigned long status = LM_SUCCESS;
//	bool flag = false;
//	if (moduleName == 0)
//	{
//		if (msg == NULL)
//		{
//			msg = new char[256];
//			flag = true;
//		}
//		status = isAppValid(msg);
//		if (flag)
//		{
//			printf("ERROR:%s\n", msg);
//			delete []msg;
//		}
//	}
//	return status;
//}
unsigned long QtLicenseManager::checksentinel(char* msg /*= 0*/, const char* moduleName /*= 0*/)
{
	unsigned long status = LM_SUCCESS;
	bool flag = false;
	if (moduleName == 0)
	{
		if (msg == NULL)
		{
			msg = new char[256];
			flag = true;
		}
		status = isAppValid(msg);
		if (flag)
		{
			printf("ERROR:%s\n", msg);
			delete[]msg;
		}
	}
	return status;
}
unsigned long QtLicenseManager::isAppValid(char* msg)
{
	SP_DWORD  status;						/* The Sentinel Keys API return codes */
	SP_DWORD  flags = SP_STANDALONE_MODE;	/* flags for License  */
	SP_HANDLE licHandle;					/* Handle for License  */
	SP_DWORD  flag_AES = SP_SIMPLE_QUERY;	/* To set the counter flag on/off */
	SP_FEATURE_INFO FeatureInfo_AES;		/* A structure for Feature Info */
	//获取License
	status = SFNTGetLicense(DEVELOPERID, SOFTWARE_KEY, LICENSEID, flags, &licHandle);
	if (status != SP_SUCCESS)
	{
		sprintf(msg, "%s", QApplication::translate("QtLicenseManager", "Error accessing license! Please check whether the USB Key is plugged into computer.", 0).toLocal8Bit().data());
		return status;
	}

	//查询License是否有效
	status = SFNTQueryFeature(licHandle, SP_APP_EXPDATE_AES, flag_AES, QUERY_SP_APP_EXPDATE_AES_SP_SIMPLE_QUERY[0], SP_LEN_OF_QR, RESPONSE_SP_APP_EXPDATE_AES_SP_SIMPLE_QUERY[0], SP_LEN_OF_QR);
	if (status != SP_SUCCESS)
	{
		if (status == SP_ERR_DEMO_EXPIRED)
		{
			sprintf(msg, "%s", QApplication::translate("QtLicenseManager", "Error[%1]:License has been expired!", 0).arg(status).toLocal8Bit().data());
		}
		else if (status == SP_ERR_TIME_CHEAT)
		{
			sprintf(msg, "%s", QApplication::translate("QtLicenseManager", "Error[%1]:The time setting of this computer is abnormal. Please correct it before trying to start UEES again, or it'll be locked forever.", 0).arg(status).toLocal8Bit().data());
		}
		else
		{
			sprintf(msg, "%s", QApplication::translate("QtLicenseManager", "Error[%1]:Failed to get AES information.", 0).arg(status).toLocal8Bit().data());
		}

		SFNTReleaseLicense(licHandle);
		return status;
	}

	//获取License具体信息
	status = SFNTGetFeatureInfo(licHandle, SP_APP_EXPDATE_AES, &FeatureInfo_AES);
	if (status == SP_SUCCESS)
	{
		SP_TIME_CONTROL timeCtrl = FeatureInfo_AES.timeControl;
		QString licmsg = QApplication::translate("QtLicenseManager", "License to %1-%2-%3", 0).arg(timeCtrl.stopTime.year).arg(timeCtrl.stopTime.month).arg(timeCtrl.stopTime.dayOfMonth);

		m_licenseInfo = licmsg.toLocal8Bit().data();
	}

	SFNTReleaseLicense(licHandle);
	return status;
}

void QtLicenseManager::startLicenseThread()
{
	QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "ZJU", "UEESIE");
	int RegisterMode = settings.value("RegisterMode", -1).toInt();
	if (RegisterMode != 0)
	{
		LicenseCheckerThread::instance()->start();
	}

}

void QtLicenseManager::setMessageHandler(void* pHandler)
{
	LicenseCheckerThread::instance()->setMessageHandler(pHandler);
}



//////////////////////////////////////////////////////////////////////////
//#define LICENSE_CHECKER_ID 1
// void checkLicense()
// {
// 	if (!isAppValid())
// 	{
// 		KillTimer(NULL,LICENSE_CHECKER_ID);
// 		CString strMsg;
// 		strMsg.Format("Error:程序运行时请不要移除加密狗!\n");
// 		AfxMessageBox(strMsg);
// 		exit(0);
// 	}
// }
// 
// void CALLBACK TimerProc(HWND hWnd,UINT nMsg,UINT nTimerid,DWORD dwTime)
// {
// 	//checkLicense();
// 	static bool licenseCheckFailed = false;
// 	if (licenseCheckFailed)
// 	{
// 		return;
// 	}
// 
// 	SP_DWORD  status;              /* The Sentinel Keys API return codes */
// 	SP_DWORD  flags = SP_STANDALONE_MODE; /* flags for License  */
// 	SP_HANDLE licHandle;            /* Handle for License  */
// 	SP_DWORD  flag_AES = SP_SIMPLE_QUERY;	    /* To set the counter flag on/off */
// 	SP_FEATURE_INFO FeatureInfo_AES;			/* A structure for Feature Info */
// 
// 	//获取License
// 	status = SFNTGetLicense(DEVELOPERID, SOFTWARE_KEY, LICENSEID, flags, &licHandle );
// 	if (status != SP_SUCCESS)
// 	{
// 		licenseCheckFailed = true;
// 		KillTimer(NULL, LICENSE_CHECKER_ID);
// 		AfxMessageBox("License获取错误!请检查是否已经插入加密狗!程序运行时请不要移除加密狗，否则程序无法正常运行!\n");
// 		exit(0);
// 	}
// }
//////////////////////////////////////////////////////////////////////////
