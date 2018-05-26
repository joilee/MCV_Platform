#include "ueesCodeRegister.h"
#include <QSettings>
#include <QMessageBox>
#include <time.h>
#include "DESEncrypter.h"
#ifdef Q_OS_WIN32
#include <windows.h>
#endif // Q_OS_WIN32
#include "registerDialog.h"
#include <iostream>


bool ueesCodeRegister::RegisterAndValidate()
{
	QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "ZJU", "emXIE");
	// 用于存储已经用过的注册码的文件
	// 所有用户输入的有效注册码均记录在此文件中，防止一个注册码被多次使用
	//FILE* regFile = fopen("data.db", "ab+");
	fclose(regFile);
	regFile = fopen("data.db", "ab+");
	if (regFile == NULL)
	{
		//AfxMessageBox("emX软件数据文件打开错误，请检查程序目录及文件是否具有读写权限，或联系浙江大学CAD&CG国家重点实验室");
		int ret;
		QMessageBox::warning((QWidget*)(this->parent()), tr("Register Error"), tr("emX open the data file error,please check the whether the file and the directory has read and write permission,or contact State Key Lab of CAD&CG,ZheJiang University"));
		m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("emX open the data file error,please check the whether the file and the directory has read and write permission,or contact State Key Lab of CAD&CG,ZheJiang University"), &ret);
		//fclose(regFile);
		return false;
	}
	QString RegID, TrialRegID, LimitedRegID, FullRegID;
	if (!GenerateVolumeInfo(RegID, TrialRegID, LimitedRegID, FullRegID)) {
		// 无法获取本地C盘序列号
		int ret;
		//AfxMessageBox("读取注册信息失败，请联系浙江大学CAD&CG国家重点实验室");
		m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("emX read the register information error,please contact State Key Lab of CAD&CG,ZheJiang University"), &ret);
		return false;
	}
	// 注册码
	char regCode[255], regTime[255], regCodeGenID[255];
	memset(regCode, 0, sizeof(regCode));
	ULONG charNum = 255, regNum = 255;
	bool isValid = false;
	bool isRegisted = false;
	time_t regCodeGenTime = 0;
	//////////////////////////////////////////////////////////////////////////
	// 已注册则从注册表获取注册码，否则提示用户注册
	//////////////////////////////////////////////////////////////////////////
	do {
		if (settings.value("RegCode", -1).toInt() == -1) {
			isRegisted = false;
			return false;
			// 检查是否以前已安装，验证上次注册时间，已安装时间大于一个月不予使用
			//if(processReg.QueryStringValue("RegCodeBack", regTime, &regNum) == ERROR_SUCCESS) {
			//	regTime[regNum + 1] = '\0';
			//	CString RegTime(regTime);
			//	if(!IsValidTime(RegTime)) {
			//		AfxMessageBox("emX软件已过使用期，请联系浙江大学CAD&CG国家重点实验室");				
			//		break;
			//	}
			//}

			// 读取"键值"不成功, 说明软件尚未注册,弹出注册对话框提示用户注册
			//m_registerDialog->currentIdChanged(emxRegisterDialog::Page_Register);
			//m_registerDialog->setRegID(RegID);
			//			if (m_registerDialog->exec() == QDialog::Accepted) {
			//				if(m_registerDialog->getRegCode().length() < 255) 
			//				{
			//					QString ttregCode = m_registerDialog->getRegCode();
			//					QByteArray tregCode = ttregCode.toLatin1();
			//					tregCode.append("\0");
			//// 					while (*tregCode) {
			//// 						std::cout << "[" << *tregCode << "]" << std::endl;
			//// 						++tregCode;
			//// 					}
			//					strcpy(regCode,tregCode);
			//					//检查注册码是否已经用过
			//					char regCodeOld[255];
			//					bool sameCode = true;
			//					while(fread(regCodeOld, 255, 1, regFile) > 0)
			//					{
			//						sameCode = true;
			//						for (int i = 0; i < 64; ++i) // 64是注册码的长度
			//						{
			//							if(regCode[i] != regCodeOld[i])
			//							{
			//								sameCode = false;
			//								break;
			//							}
			//						}
			//						if (sameCode)
			//						{
			//							//AfxMessageBox("无效注册码，请重新输入注册码，或联系浙江大学CAD&CG国家重点实验室");
			//							int ret;
			//							m_handle->messageBox(QMessageBox::Critical, tr("Register Error"),tr("invalidate register code,please reenter register code again or contact State Key Lab of CAD&CG,ZheJiang University"),&ret);
			//							fclose(regFile);
			//							return false;
			//						}
			//					}
			//				}
			//			}
			//			else // 取消注册并退出
			//				break;
		}
		else {
			isRegisted = true;
			QByteArray tregCode = settings.value("RegCode", -1).toByteArray();
			tregCode.append("\0");
			strcpy(regCode, tregCode);
			QByteArray tregTime = settings.value("RegCodeBack", -1).toByteArray();
			tregTime.append("\0");
			strcpy(regTime, tregTime);
			// 成功读取"键值"
			//if(processReg.QueryStringValue("RegCodeBack", regTime, &regNum) != ERROR_SUCCESS) {
			//	AfxMessageBox("emX软件已过使用期，请联系浙江大学CAD&CG国家重点实验室");				
			//	break;
			//}
			//else {
			//	CString RegTime(regTime);
			//	if(!IsValidTime(RegTime)) {
			//		AfxMessageBox("emX软件已过使用期，请联系浙江大学CAD&CG国家重点实验室");				
			//		break;
			//	}
			//}
		}
		// else 成功读取"键值",判断注册码是否有效


		if (!DecodeRegCode(regCode, regCodeGenID, regCodeGenTime))
			return false;

		if (TrialRegID == QString(regCodeGenID)) { 	//注册码与硬盘地址匹配
			regType = 1;
			isValid = true;
		}
		else if (LimitedRegID == QString(regCodeGenID)) {
			regType = 2;
			isValid = true;
		}
		else if (FullRegID == QString(regCodeGenID)) {
			regType = 3;
			isValid = true;
		}
		else { //无效注册码，删除键值
			if (settings.value("RegCode", -1).toInt() != -1)
				settings.remove("RegCode");
			int ret;
			m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("invalidate register code,please reenter register code again or contact State Key Lab of CAD&CG,ZheJiang University"), &ret);
		}
	} while (!isValid);
	//////////////////////////////////////////////////////////////////////////
	// 注册码有效，检查注册时间和运行时间是否有效
	//////////////////////////////////////////////////////////////////////////
	if (isValid) {
		// 没有注册时检查注册码是否已经过期
		if (!isRegisted && !isCodeGenTimeValid(regCodeGenTime))
		{
			//AfxMessageBox("注册码已过期，请联系浙江大学CAD&CG国家重点实验室获取新的注册码");
			int ret;
			m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("register code is out of data,please contact State Key Lab of CAD&CG,ZheJiang University"), &ret);
			return false;//FullRegisterAndValidate(processReg, timeReg, RegID, LimitedRegID, FullRegID);
		}

		// 试用版和限制版检查注册时间
		if (regType == 1 || regType == 2)
		{
			regNum = 255;
			if (settings.value("RegCodeBack", -1).toInt() == -1) // 注册时间不存在
			{
				if (isRegisted) // 已注册，但注册时间已被删除
				{
					//AfxMessageBox("emX软件已过使用期，续订请联系浙江大学CAD&CG国家重点实验室");
					int ret;
					m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("emX software is out of data,please contact State Key Lab of CAD&CG,ZheJiang University if renew"), &ret);
					// 删除注册码
					if (settings.value("RegCode", -1).toInt() != -1)
						settings.remove("RegCode");
					return false;//FullRegisterAndValidate(processReg, timeReg, RegID, LimitedRegID, FullRegID);
				}
				else // 未注册，写入注册时间
				{
					GetCurrentRegTime(regTime);
					settings.setValue("RegCodeBack", regTime);
					getCurrentDateTime(regTime);
					settings.setValue("RegCodeBackBk", regTime);
				}
			}
			else // 注册时间已存在
			{
				QString RegTime(regTime);
				if (!IsValidTime(RegTime))
				{
					//AfxMessageBox("emX软件已过使用期，续订请联系浙江大学CAD&CG国家重点实验室");
					int ret;
					m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("emX software is out of data,please contact State Key Lab of CAD&CG,ZheJiang University if renew"), &ret);
					//删除注册码和注册时间
					if (settings.value("RegCode", -1).toInt() != -1)
						settings.remove("RegCode");
					settings.remove("RegCodeBack");
					settings.remove("RegCodeBackBk");
					return false;//FullRegisterAndValidate(processReg, timeReg, RegID, LimitedRegID, FullRegID);
				}
			}
		}

		//写入注册码
		settings.setValue("RegCode", regCode);
		fwrite(regCode, 255, 1, regFile);

		//////////////////////////////////////////////////////////////////////////
		// 试用版时间篡改的检查
		if (regType == 1 || regType == 2)
		{
			if (!timeCheatCheck(settings)) //如果存在时间篡改，删除注册码并提示用户重新注册
			{
				settings.remove("RegCode");
				settings.remove("RegCodeBack");
				settings.remove("RegCodeBackBk");
				return false;
				//return FullRegisterAndValidate(settings, RegID, TrialRegID, LimitedRegID, FullRegID);
			}
		}
		//////////////////////////////////////////////////////////////////////////
	}

	return isValid;
};

ueesCodeRegister::ueesCodeRegister(ueesMessageHandler * handle, ueesRegisterDialog* dialog)
{
	regFile = fopen("data.db", "ab+");
	if (regFile == NULL)
	{
		//AfxMessageBox("emX软件数据文件打开错误，请检查程序目录及文件是否具有读写权限，或联系浙江大学CAD&CG国家重点实验室");
		int ret;
		QMessageBox::warning((QWidget*)(this->parent()), tr("Register Error"), tr("emX open the data file error,please check the whether the file and the directory has read and write permission,or contact State Key Lab of CAD&CG,ZheJiang University"));
		m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("emX open the data file error,please check the whether the file and the directory has read and write permission,or contact State Key Lab of CAD&CG,ZheJiang University"), &ret);
		fclose(regFile);
		exit(0);
	}
	m_handle = handle;
	regType = 1;
	m_registerDialog = dialog;

}
#define REG_ID_LENGTH 21
bool ueesCodeRegister::GenerateVolumeInfo(QString& RegID, QString& TrialRegID, QString& LimitedRegID, QString& FullRegID)
{
#ifdef Q_OS_WIN32
	LPCTSTR lpRootPathName = L"c:\\";	// 取C盘
	LPTSTR volumeNameBuffer = new TCHAR[12];			// 磁盘卷标	
	DWORD nVolumeNameSize = 12;			// 卷标的字符串长度
	DWORD VolumeSerialNumber;			// 硬盘序列号
	DWORD MaximumComponentLength;		// 最大的文件长度
	LPTSTR fileSystemNameBuffer = new TCHAR[10];		// 存储所在盘符的分区类型的长指针变量
	DWORD nFileSystemNameSize = 10;		// 分区类型的长指针变量所指向的字符串长度
	DWORD FileSystemFlags;				// 文件系统的一此标志
	bool bget = ::GetVolumeInformation(lpRootPathName,
		volumeNameBuffer, nVolumeNameSize,
		&VolumeSerialNumber, &MaximumComponentLength,
		&FileSystemFlags,
		fileSystemNameBuffer, nFileSystemNameSize);
	//bool bget = true;
	if (!bget)
		return false;
#endif
	// 转换硬盘序列号为十六进制字符
	unsigned char serialNumber[20];
	unsigned char *pVolumeSerialNumber = (unsigned char*)&VolumeSerialNumber;
	for (int i = 0; i < 4; ++i) {	// sizeof(DWORD) == 4
		unsigned char low4 = (unsigned char)pVolumeSerialNumber[i] & (char)15;// & 00001111
		unsigned char high4 = (unsigned char)pVolumeSerialNumber[i] >> 4;
		if (low4 < 10)
			serialNumber[i * 2] = low4 + '0';
		else
			serialNumber[i * 2] = low4 - 10 + 'A';
		if (high4 < 10)
			serialNumber[i * 2 + 1] = high4 + '0';
		else
			serialNumber[i * 2 + 1] = high4 - 10 + 'A';
	}
	//generate RegisterID from volume serial number
	// 01234567
	// 2(3+7)'E'5('J'+3)04(1+'L')'M'(4+1)16(4+'D')'X'(2+0)3'0'(2+5)7(1+3)
	char regID[REG_ID_LENGTH];
	regID[0] = serialNumber[2];								  regID[1] = SumToChar(serialNumber[3], serialNumber[7]);
	regID[2] = 'E';											  regID[3] = serialNumber[5];
	regID[4] = SumToChar('J', serialNumber[3]);               regID[5] = serialNumber[0];
	regID[6] = serialNumber[4];                               regID[7] = SumToChar(serialNumber[1], 'L');
	regID[8] = 'M';                                           regID[9] = SumToChar(serialNumber[4], serialNumber[1]);
	regID[10] = serialNumber[1];                              regID[11] = serialNumber[6];
	regID[12] = SumToChar(serialNumber[4], 'D');              regID[13] = 'X';
	regID[14] = SumToChar(serialNumber[2], serialNumber[0]);  regID[15] = serialNumber[3];
	regID[16] = '0';                                          regID[17] = SumToChar(serialNumber[2], serialNumber[5]);
	regID[18] = serialNumber[7];                              regID[19] = SumToChar(serialNumber[1], serialNumber[3]);
	regID[20] = '\0';

	char regID1[REG_ID_LENGTH], regID2[REG_ID_LENGTH], regID3[REG_ID_LENGTH];
	RegIDToTrial(regID, regID1);
	RegIDToLimited(regID, regID2);
	RegIDToFull(regID, regID3);
	RegID = regID;
	TrialRegID = regID1;
	LimitedRegID = regID2;
	FullRegID = regID3;
	return true;
}

char ueesCodeRegister::SumToChar(unsigned char a, unsigned char b)
{
	//Used only in function GenerateRegId(...)
	//convert a+b to char(a and b are HEX)
	if (a >= '0' && a <= '9')
		a = a - '0';
	else
		a = a - 'A';
	if (b >= '0' && b <= '9')
		b = b - '0';
	else
		b = b - 'A';

	return a + b + 'A';
}
void ueesCodeRegister::RegIDToTrial(char regID[21], char trialRegID[21])
{
	for (int i = 0; i < 20; ++i)
		trialRegID[i] = regID[i] + 1;
	trialRegID[20] = '\0';
}

void ueesCodeRegister::RegIDToLimited(char regID[21], char limitedRegID[21])
{
	for (int i = 0; i < 10; ++i) {
		limitedRegID[i] = regID[i * 2] + 2;
		limitedRegID[10 + i] = regID[i * 2 + 1] + 2;
	}
	limitedRegID[20] = '\0';
}

void ueesCodeRegister::RegIDToFull(char regID[21], char fullRegID[21])
{
	for (int i = 0; i < 10; ++i) {
		fullRegID[(i + 7) % 20] = regID[i * 2] + 3;
		fullRegID[(i + 17) % 20] = regID[i * 2 + 1] + 3;
	}
	fullRegID[20] = '\0';
}
/*
* 注册时间
*/

void ueesCodeRegister::GetCurrentRegTime(char regTime[255])
{
	time_t temptime = time(0);
	tm *temptm = localtime(&temptime);
	if (temptm->tm_year < 113)
		temptm->tm_year = 113;
	regTime[0] = temptm->tm_year;
	regTime[1] = temptm->tm_yday % 100 + 10;
	regTime[2] = temptm->tm_yday / 100 + 100;
	regTime[3] = temptm->tm_mon * 3 + 30; // month * 3 + 30
	regTime[4] = temptm->tm_mday % 10 + 80; // mday的个位 + 80
	regTime[5] = temptm->tm_mday / 10 + 90; // mday的十位 + 90
	regTime[6] = '\0';
}

bool ueesCodeRegister::DecodeRegCode(char regCode[255], char codeGenID[255], time_t& codeGenTime)
{
	// 判断注册码与注册标识符是否匹配
	char key[] = { 'b', 'F', 'd', 'E', 'G', '/', '!', 'T', '5', '~', '#', 'p', 'U', 'W', 'r', 'e' };
	char buf[255], outbuf[255];
	memset(buf, 0, sizeof(buf));
	memset(outbuf, 0, 255);
	// 将注册码恢复为密文
	int regCodeLength = static_cast<int>(strlen(regCode)) / 2;
	for (int j = 0; j < regCodeLength; ++j){
		char ch8 = 0;
		unsigned char high4 = CharToHex(regCode[j * 2]);
		unsigned char low4 = CharToHex(regCode[j * 2 + 1]);
		ch8 = ch8 | (high4 << 4) | low4;
		buf[j] = ch8;
	}

	//解密还原
	CDESEncrypter des;
	des.Des_Go(outbuf, buf, regCodeLength, key, sizeof(key), CDESEncrypter::DECRYPT);

	if (strlen(outbuf) != 26)
	{
		int ret;
		m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("invalidate register code,please reenter register code again or contact State Key Lab of CAD&CG,ZheJiang University"), &ret);
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	//在注册码后缀添加注册码生成日期，用来防止一个注册码被多次使用
	char regCodeGenDate[REG_TIME_LENGTH];
	memset(regCodeGenDate, 0, REG_TIME_LENGTH);
	memcpy(regCodeGenDate, outbuf + 20, REG_TIME_LENGTH); // 从注册码中取出注册日期的编码
	struct tm regCodeGenTime_tm = regTimeStr2TM(regCodeGenDate); // 将编码恢复为注册日期
	codeGenTime = mktime(&(regCodeGenTime_tm));
	//printf("Reg Code Generation Date:%s\n", ctime(&regCodeGenTime));
	outbuf[20] = 0;//前面20位为注册码
	//////////////////////////////////////////////////////////////////////////
	memset(codeGenID, 0, 255);
	memcpy(codeGenID, outbuf, strlen(outbuf)); // 取前20位机器码放到codeGenID里

	return true;
}

unsigned char ueesCodeRegister::CharToHex(unsigned char ch)
{
	//convert	A----P to 0---15
	if (ch >= 'A' && ch <= ('A' + 15))
		return (ch - 'A');
	return -1;
}

struct tm ueesCodeRegister::regTimeStr2TM(char regTime[REG_TIME_LENGTH])
{
	struct tm regTime_tm;
	memset(&regTime_tm, 0, sizeof(struct tm));
	regTime_tm.tm_year = regTime[0];
	regTime_tm.tm_mon = (regTime[3] - 30) / 3;
	regTime_tm.tm_mday = regTime[4] - 80 + (regTime[5] - 90) * 10;
	return regTime_tm;
}

bool ueesCodeRegister::isCodeGenTimeValid(time_t regTime)
{
	time_t currentTime = time(0);
	int keyUseLimit = 10;			// 限制注册码生成后在10天之内使用有效
	time_t keyDeadLine = regTime + keyUseLimit * 24 * 3600;
	// 注册码在编译时间之后生成 && 注册码生成时间小于当前时间 && 当前时间在注册码生成后10天之内
	if (EMX_BUILD_TIME < regTime && regTime < currentTime && currentTime < keyDeadLine)
		return true;
	else
		return false;
}

void ueesCodeRegister::getCurrentDateTime(char regTime[255])
{
	time_t temptime = time(0);
	tm *temptm = localtime(&temptime);
	if (temptm->tm_year < 113)
		temptm->tm_year = 113;

	// 	regTime[0] = (unsigned char)(temptime & 0x000000FF);
	// 	temptime >>= 8;
	// 	regTime[3] = (unsigned char)(temptime & 0x000000FF);
	// 	temptime >>= 8;
	// 	regTime[1] = (unsigned char)(temptime & 0x000000FF);
	// 	temptime >>= 8;
	// 	regTime[2] = (unsigned char)(temptime & 0x000000FF);
	regTime[0] = temptm->tm_min + 41; // 41~100(取值范围)
	regTime[1] = temptm->tm_sec + 33; // 33~92
	regTime[2] = temptm->tm_hour + 84;// 84~107
	regTime[3] = '@' + temptm->tm_sec;// 64~124
	regTime[4] = temptm->tm_year - 21;// 113-21 == 92 ~
	regTime[5] = temptm->tm_yday % 64 + 38; // 38~101
	regTime[6] = temptm->tm_yday / 64 + 68; // 68~73
	regTime[7] = temptm->tm_mon * 3 + 38;   // 38~71
	regTime[8] = temptm->tm_mday % 10 + 76; // 76~85
	regTime[9] = temptm->tm_mday / 10 + 67; // 67~70
	regTime[10] = '\0';
}

bool ueesCodeRegister::IsValidTime(QString& RegTime)
{
	if (RegTime.length() != REG_TIME_LENGTH)
		return false;
	//QByteArray tregTime = setting.value("RegCodeBackBk",-1).toByteArray();
	//tregTime.append("\0");
	//strcpy(regTime,tregTime);
	// Get register time
	int year = RegTime[0].unicode();
	int day = RegTime[4].unicode() - 80 + (RegTime[5].unicode() - 90) * 10;
	int month = (RegTime[3].unicode() - 30) / 3;

	struct tm regTime_tm;
	memset(&regTime_tm, 0, sizeof(struct tm));
	regTime_tm.tm_year = year;
	regTime_tm.tm_mon = month;
	regTime_tm.tm_mday = day;
	time_t regTime = mktime(&regTime_tm);

	// Get current time
	time_t currentTime = time(0);

	time_t endTime = getLicenseEndTime(regTime);

	if (regTime > EMX_BUILD_TIME && currentTime > EMX_BUILD_TIME && currentTime < endTime)
		return true;
	else
		return false;
}

time_t ueesCodeRegister::getLicenseEndTime(time_t regTime)
{
	time_t endTime = 0;
	if (regType == 1)
	{
		//////////////////////////////////////////////////////////////////////////
		// 按截止日期试用
		struct tm end_tm;
		memset(&end_tm, 0, sizeof(struct tm));
		end_tm.tm_year = 115; // 2015年
		end_tm.tm_mon = 9; //10月
		end_tm.tm_mday = 31; //31日
		endTime = mktime(&end_tm);
	}
	else if (regType == 2)
	{
		//////////////////////////////////////////////////////////////////////////
		// 按天数试用
		int licenseValidDayCount = 180;//证书有效天数
		endTime = regTime + licenseValidDayCount * 24 * 3600;
	}
	return endTime;
}

bool ueesCodeRegister::timeCheatCheck(QSettings &setting)
{
	bool ret = false;
	char regTime[255];
	ULONG regNum = 255;
	// 读出上次运行时间，查看系统时间是否被篡改，键值RegCodeBackBk用于存储每次emx的启动时间
	QByteArray tregTime = setting.value("RegCodeBackBk", -1).toByteArray();
	tregTime.append("\0");
	strcpy(regTime, tregTime);
	if (setting.value("RegCodeBackBk", -1).toInt() == -1)
	{
		//注册表中上次运行时间被删除，返回Error Code:TC001  (Time Cheat 001)
		//AfxMessageBox("emX软件注册信息检查失败，请重新输入注册码，或联系浙江大学CAD&CG国家重点实验室(Error Code:TC001)");
		int re;
		m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("check emX software register information error,please reenter the register code or contact State Key Lab of CAD&CG,ZheJiang University(Error Code:TC001)"), &re);
		ret = false;
	}
	else
	{
		time_t lastRunTime = decodeCurrentDateTime(regTime);
		time_t currentTime = time(0);
		if ((regType == 1 || regType == 2) && !isValidRunTime(lastRunTime))
		{
			//时间篡改欺骗，注册表中的上次运行时间被修改至无效时间，返回Error Code:TC003  (Time Cheat 003)
			//AfxMessageBox("emX软件注册信息检查失败，请重新输入注册码，或联系浙江大学CAD&CG国家重点实验室(Error Code:TC003)");
			int re;
			m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("check emX software register information error,please reenter the register code or contact State Key Lab of CAD&CG,ZheJiang University(Error Code:TC001)"), &re);
			ret = false;
		}
		else{
			if (currentTime < lastRunTime) //当前时间在上次运行时间之前
			{
				//时间篡改欺骗，系统时间被修改至上次运行之前，返回Error Code:TC002  (Time Cheat 002)
				//AfxMessageBox("emX软件注册信息检查失败，请重新输入注册码，或联系浙江大学CAD&CG国家重点实验室(Error Code:TC002)");
				int re;
				m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("check emX software register information error,please reenter the register code or contact State Key Lab of CAD&CG,ZheJiang University(Error Code:TC001)"), &re);
				ret = false;
			}
			else
			{
				// 写入本次运行时间
				getCurrentDateTime(regTime);
				setting.setValue("RegCodeBackBk", regTime);
				ret = true;
			}
		}
	}
	return ret;
}

time_t ueesCodeRegister::decodeCurrentDateTime(char regTime[255])
{
	//用于验证前面4个字节的信息
	tm temptm;
	memset(&temptm, 0, sizeof(tm));
	temptm.tm_min = regTime[0] - 41;
	temptm.tm_sec = regTime[1] - 33;
	temptm.tm_hour = regTime[2] - 84;
	temptm.tm_year = regTime[4] + 21;
	temptm.tm_mon = (regTime[7] - 38) / 3;
	temptm.tm_mday = regTime[8] - 76 + (regTime[9] - 67) * 10;
	temptm.tm_yday = regTime[5] - 38 + (regTime[6] - 68) * 64;
	time_t temptime = mktime(&temptm);

	printf("Last Run:%d-%d-%d %d:%d:%d\n", temptm.tm_year, temptm.tm_mon, temptm.tm_mday, temptm.tm_hour, temptm.tm_min, temptm.tm_sec);
	// 	if (test->tm_year != temptm.tm_year || test->tm_mon != temptm.tm_mon || test->tm_mday != temptm.tm_mday || test->tm_yday != temptm.tm_yday)
	// 	{
	// 		AfxMessageBox("Error Time!");
	// 	}
	return temptime;
}

bool ueesCodeRegister::isValidRunTime(time_t t)
{
	QSettings settings(QSettings::NativeFormat, QSettings::UserScope, "ZJU", "emXIE");
	char regTime[255];
	ULONG regNum = 255;
	QByteArray tregTime = settings.value("RegCodeBackBk", -1).toByteArray();
	tregTime.append("\0");
	strcpy(regTime, tregTime);
	if (settings.value("RegCodeBack", -1).toInt() != -1)
	{
		tm regTime_tm = regTimeStr2TM(regTime);
		time_t regTime = mktime(&regTime_tm);
		time_t endTime = getLicenseEndTime(regTime);
		if (t > EMX_BUILD_TIME && t > regTime && t < endTime)
		{
			return true;
		}
	}
	return false;
}

bool ueesCodeRegister::FullRegisterAndValidate(QSettings& setting, QString& RegID, QString& TrialRegID, QString& LimitedRegID, QString& FullRegID)
{
	// 注册
	m_registerDialog->setRegID(RegID);
	if (m_registerDialog->exec() == QDialog::Rejected)
		return false;

	// 注册码
	char regCode[255], regTime[255], regCodeGenID[255];
	ULONG charNum = 255, regNum = 255;

	if (m_registerDialog->getRegCode().length() < 255)
	{
		QString ttregCode = m_registerDialog->getRegCode();
		QByteArray tregCode = ttregCode.toLatin1();
		tregCode.append("\0");
		strcpy(regCode, tregCode);
	}

	time_t regCodeGenTime;
	if (!DecodeRegCode(regCode, regCodeGenID, regCodeGenTime))
		return false;

	if (regType == 1 && LimitedRegID == QString(regCodeGenID)) {
		regType = 2;
		if (!isCodeGenTimeValid(regCodeGenTime))
		{
			//AfxMessageBox("注册码已过期，请重新输入注册码，或联系浙江大学CAD&CG国家重点实验室");
			int ret;
			m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("register code is out of data,please contact State Key Lab of CAD&CG,ZheJiang University"), &ret);
			return FullRegisterAndValidate(setting, RegID, LimitedRegID, FullRegID);
		}
		else { //注册码有效，写入注册时间及首次运行时间
			GetCurrentRegTime(regTime);
			setting.setValue("RegCodeBack", regTime);
			getCurrentDateTime(regTime);
			setting.setValue("RegCodeBackBk", regTime);
			setting.setValue("RegCode", regCode);
			int ret;
			m_handle->messageBox(QMessageBox::Information, tr("Register Success"), tr("The trial version upgrade successfully"), &ret);
			//AfxMessageBox("试用版升级成功！");
		}
	}
	else if (FullRegID == QString(regCodeGenID)) {
		regType = 3;
		if (!isCodeGenTimeValid(regCodeGenTime))
		{
			//AfxMessageBox("注册码已过期，请重新输入注册码，或联系浙江大学CAD&CG国家重点实验室");
			int ret;
			m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("register code is out of data,please contact State Key Lab of CAD&CG,ZheJiang University"), &ret);
			return FullRegisterAndValidate(setting, RegID, LimitedRegID, FullRegID);
		}
		else { //注册码有效，写入注册时间及首次运行时间
			GetCurrentRegTime(regTime);
			setting.setValue("RegCodeBack", regTime);
			setting.setValue("RegCodeBackBk", regTime);
			setting.setValue("RegCode", regCode);
			//AfxMessageBox("完整版升级成功");
			int ret;
			m_handle->messageBox(QMessageBox::Information, tr("Register Success"), tr("The full version upgrade successfully"), &ret);
		}
	}
	else {
		int ret;
		m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("invalidate register code,please contact State Key Lab of CAD&CG,ZheJiang University"), &ret);
		//AfxMessageBox("无效注册码，请联系浙江大学CAD&CG国家重点实验室");
		return false;
	}
	return true;
}

bool ueesCodeRegister::FullRegisterAndValidate(QSettings& setting, QString& RegID, QString& LimitedRegID, QString& FullRegID)
{
	// 注册
	// 注册
	//emxRegisterDialog regDlg;
	m_registerDialog->setRegID(RegID);
	if (m_registerDialog->exec() == QDialog::Rejected)
		return false;

	// 注册码
	char regCode[255], regTime[255], regCodeGenID[255];
	ULONG charNum = 255, regNum = 255;

	if (m_registerDialog->getRegCode().length() < 255)
	{
		QString ttregCode = m_registerDialog->getRegCode();
		QByteArray tregCode = ttregCode.toLatin1();
		tregCode.append("\0");
		strcpy(regCode, tregCode);
	}

	time_t regCodeGenTime;
	if (!DecodeRegCode(regCode, regCodeGenID, regCodeGenTime))
		return false;

	if (regType == 1 && LimitedRegID == QString(regCodeGenID)) {
		regType = 2;
		if (!isCodeGenTimeValid(regCodeGenTime))
		{
			//AfxMessageBox("注册码已过期，请重新输入注册码，或联系浙江大学CAD&CG国家重点实验室");
			int ret;
			m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("register code is out of data,please contact State Key Lab of CAD&CG,ZheJiang University"), &ret);
			return FullRegisterAndValidate(setting, RegID, LimitedRegID, FullRegID);
		}
		else { //注册码有效，写入注册时间及首次运行时间
			GetCurrentRegTime(regTime);
			setting.setValue("RegCodeBack", regTime);
			getCurrentDateTime(regTime);
			setting.setValue("RegCodeBackBk", regTime);
			setting.setValue("RegCode", regCode);
			int ret;
			m_handle->messageBox(QMessageBox::Information, tr("Register Success"), tr("The trial version upgrade successfully"), &ret);
			//AfxMessageBox("试用版升级成功！");
		}
	}
	else if (FullRegID == QString(regCodeGenID)) {
		regType = 3;
		if (!isCodeGenTimeValid(regCodeGenTime))
		{
			int ret;
			m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("register code is out of data,please contact State Key Lab of CAD&CG,ZheJiang University"), &ret);
			//AfxMessageBox("注册码已过期，请重新输入注册码，或联系浙江大学CAD&CG国家重点实验室");
			return FullRegisterAndValidate(setting, RegID, LimitedRegID, FullRegID);
		}
		else { //注册码有效，写入注册时间及首次运行时间
			GetCurrentRegTime(regTime);
			setting.setValue("RegCodeBack", regTime);
			setting.setValue("RegCodeBackBk", regTime);
			setting.setValue("RegCode", regCode);
			int ret;
			m_handle->messageBox(QMessageBox::Information, tr("Register Success"), tr("The full version upgrade successfully"), &ret);
			//AfxMessageBox("完整版升级成功");
		}
	}
	else {
		int ret;
		m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("invalidate register code,please contact State Key Lab of CAD&CG,ZheJiang University"), &ret);
		//AfxMessageBox("无效注册码，请联系浙江大学CAD&CG国家重点实验室");
		return false;
	}
	return true;
}

bool ueesCodeRegister::FullRegisterAndValidate(QString& RegID, QString& TrialRegID, QString& LimitedRegID, QString& FullRegID)
{
	QSettings setting(QSettings::NativeFormat, QSettings::UserScope, "ZJU", "emXIE");
	// 注册码
	char regCode[255], regTime[255], regCodeGenID[255];
	ULONG charNum = 255, regNum = 255;

	if (m_registerDialog->getRegCode().length() < 255)
	{
		QString ttregCode = m_registerDialog->getRegCode();
		QByteArray tregCode = ttregCode.toLatin1();
		tregCode.append("\0");
		strcpy(regCode, tregCode);
		strcpy(regCode, tregCode);
		//检查注册码是否已经用过
		char regCodeOld[255];
		bool sameCode = true;
		while (fread(regCodeOld, 255, 1, regFile) > 0)
		{
			sameCode = true;
			for (int i = 0; i < 64; ++i) // 64是注册码的长度
			{
				if (regCode[i] != regCodeOld[i])
				{
					sameCode = false;
					break;
				}
			}
			if (sameCode)
			{
				//AfxMessageBox("无效注册码，请重新输入注册码，或联系浙江大学CAD&CG国家重点实验室");
				int ret;
				m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("invalidate register code,please reenter register code again or contact State Key Lab of CAD&CG,ZheJiang University"), &ret);
				return false;
			}
		}
	}

	time_t regCodeGenTime;
	if (!DecodeRegCode(regCode, regCodeGenID, regCodeGenTime))
		return false;
	if (regType == 1 && TrialRegID == QString(regCodeGenID)) {
		if (!isCodeGenTimeValid(regCodeGenTime))
		{
			//AfxMessageBox("注册码已过期，请重新输入注册码，或联系浙江大学CAD&CG国家重点实验室");
			int ret;
			m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("register code is out of data,please contact State Key Lab of CAD&CG,ZheJiang University"), &ret);
			return false;//FullRegisterAndValidate(setting, RegID, LimitedRegID, FullRegID);
		}
		else { //注册码有效，写入注册时间及首次运行时间
			GetCurrentRegTime(regTime);
			setting.setValue("RegCodeBack", regTime);
			getCurrentDateTime(regTime);
			setting.setValue("RegCodeBackBk", regTime);
			setting.setValue("RegCode", regCode);
			int ret;
			m_handle->messageBox(QMessageBox::Information, tr("Register Success"), tr("The trial version upgrade successfully"), &ret);
			//AfxMessageBox("试用版升级成功！");
			return true;
		}
	}
	if (regType == 1 && LimitedRegID == QString(regCodeGenID)) {
		regType = 2;
		if (!isCodeGenTimeValid(regCodeGenTime))
		{
			//AfxMessageBox("注册码已过期，请重新输入注册码，或联系浙江大学CAD&CG国家重点实验室");
			int ret;
			m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("register code is out of data,please contact State Key Lab of CAD&CG,ZheJiang University"), &ret);
			return false;//FullRegisterAndValidate(setting, RegID, LimitedRegID, FullRegID);
		}
		else { //注册码有效，写入注册时间及首次运行时间
			GetCurrentRegTime(regTime);
			setting.setValue("RegCodeBack", regTime);
			getCurrentDateTime(regTime);
			setting.setValue("RegCodeBackBk", regTime);
			setting.setValue("RegCode", regCode);
			int ret;
			m_handle->messageBox(QMessageBox::Information, tr("Register Success"), tr("The trial version upgrade successfully"), &ret);
			//AfxMessageBox("试用版升级成功！");
			return true;
		}
	}
	else if (FullRegID == QString(regCodeGenID)) {
		regType = 3;
		if (!isCodeGenTimeValid(regCodeGenTime))
		{
			//AfxMessageBox("注册码已过期，请重新输入注册码，或联系浙江大学CAD&CG国家重点实验室");
			int ret;
			m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("register code is out of data,please contact State Key Lab of CAD&CG,ZheJiang University"), &ret);
			return false;//FullRegisterAndValidate(setting, RegID, LimitedRegID, FullRegID);
		}
		else { //注册码有效，写入注册时间及首次运行时间
			GetCurrentRegTime(regTime);
			setting.setValue("RegCodeBack", regTime);
			setting.setValue("RegCodeBackBk", regTime);
			setting.setValue("RegCode", regCode);
			//AfxMessageBox("完整版升级成功");
			int ret;
			m_handle->messageBox(QMessageBox::Information, tr("Register Success"), tr("The full version upgrade successfully"), &ret);
		}
	}
	else {
		int ret;
		m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("invalidate register code,please contact State Key Lab of CAD&CG,ZheJiang University"), &ret);
		//AfxMessageBox("无效注册码，请联系浙江大学CAD&CG国家重点实验室");
		return false;
	}
	return true;
}

void ueesCodeRegister::setRegisterDialog(ueesRegisterDialog * dialog)
{
	m_registerDialog = dialog;
}

QString ueesCodeRegister::GetRegID(QString& RegID, QString& TrialRegID, QString& LimitedRegID, QString& FullRegID)
{
	// 用于存储已经用过的注册码的文件
	// 所有用户输入的有效注册码均记录在此文件中，防止一个注册码被多次使用
	//fclose(regFile);
	//regFile = fopen("data.db", "ab+");
	//if(regFile == NULL)
	//{
	//	//AfxMessageBox("emX软件数据文件打开错误，请检查程序目录及文件是否具有读写权限，或联系浙江大学CAD&CG国家重点实验室");
	//int ret;
	//	//QMessageBox::warning((QWidget*)(this->parent()),tr("Register Error"),tr("emX open the data file error,please check the whether the file and the directory has read and write permission,or contact State Key Lab of CAD&CG,ZheJiang University"));
	//m_handle->messageBox(QMessageBox::Critical, tr("Register Error"),tr("emX open the data file error,please check the whether the file and the directory has read and write permission,or contact State Key Lab of CAD&CG,ZheJiang University"),&ret);
	//	exit(0);
	//}
	if (!GenerateVolumeInfo(RegID, TrialRegID, LimitedRegID, FullRegID)) {
		// 无法获取本地C盘序列号
		int ret;
		//AfxMessageBox("读取注册信息失败，请联系浙江大学CAD&CG国家重点实验室");
		m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("emX read the register information error,please contact State Key Lab of CAD&CG,ZheJiang University"), &ret);
		exit(0);
	}
	return RegID;
}

ueesCodeRegister::~ueesCodeRegister()
{
	fclose(regFile);
}
