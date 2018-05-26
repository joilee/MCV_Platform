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
	// ���ڴ洢�Ѿ��ù���ע������ļ�
	// �����û��������Чע�������¼�ڴ��ļ��У���ֹһ��ע���뱻���ʹ��
	//FILE* regFile = fopen("data.db", "ab+");
	fclose(regFile);
	regFile = fopen("data.db", "ab+");
	if (regFile == NULL)
	{
		//AfxMessageBox("emX��������ļ��򿪴����������Ŀ¼���ļ��Ƿ���ж�дȨ�ޣ�����ϵ�㽭��ѧCAD&CG�����ص�ʵ����");
		int ret;
		QMessageBox::warning((QWidget*)(this->parent()), tr("Register Error"), tr("emX open the data file error,please check the whether the file and the directory has read and write permission,or contact State Key Lab of CAD&CG,ZheJiang University"));
		m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("emX open the data file error,please check the whether the file and the directory has read and write permission,or contact State Key Lab of CAD&CG,ZheJiang University"), &ret);
		//fclose(regFile);
		return false;
	}
	QString RegID, TrialRegID, LimitedRegID, FullRegID;
	if (!GenerateVolumeInfo(RegID, TrialRegID, LimitedRegID, FullRegID)) {
		// �޷���ȡ����C�����к�
		int ret;
		//AfxMessageBox("��ȡע����Ϣʧ�ܣ�����ϵ�㽭��ѧCAD&CG�����ص�ʵ����");
		m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("emX read the register information error,please contact State Key Lab of CAD&CG,ZheJiang University"), &ret);
		return false;
	}
	// ע����
	char regCode[255], regTime[255], regCodeGenID[255];
	memset(regCode, 0, sizeof(regCode));
	ULONG charNum = 255, regNum = 255;
	bool isValid = false;
	bool isRegisted = false;
	time_t regCodeGenTime = 0;
	//////////////////////////////////////////////////////////////////////////
	// ��ע�����ע����ȡע���룬������ʾ�û�ע��
	//////////////////////////////////////////////////////////////////////////
	do {
		if (settings.value("RegCode", -1).toInt() == -1) {
			isRegisted = false;
			return false;
			// ����Ƿ���ǰ�Ѱ�װ����֤�ϴ�ע��ʱ�䣬�Ѱ�װʱ�����һ���²���ʹ��
			//if(processReg.QueryStringValue("RegCodeBack", regTime, &regNum) == ERROR_SUCCESS) {
			//	regTime[regNum + 1] = '\0';
			//	CString RegTime(regTime);
			//	if(!IsValidTime(RegTime)) {
			//		AfxMessageBox("emX����ѹ�ʹ���ڣ�����ϵ�㽭��ѧCAD&CG�����ص�ʵ����");				
			//		break;
			//	}
			//}

			// ��ȡ"��ֵ"���ɹ�, ˵�������δע��,����ע��Ի�����ʾ�û�ע��
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
			//					//���ע�����Ƿ��Ѿ��ù�
			//					char regCodeOld[255];
			//					bool sameCode = true;
			//					while(fread(regCodeOld, 255, 1, regFile) > 0)
			//					{
			//						sameCode = true;
			//						for (int i = 0; i < 64; ++i) // 64��ע����ĳ���
			//						{
			//							if(regCode[i] != regCodeOld[i])
			//							{
			//								sameCode = false;
			//								break;
			//							}
			//						}
			//						if (sameCode)
			//						{
			//							//AfxMessageBox("��Чע���룬����������ע���룬����ϵ�㽭��ѧCAD&CG�����ص�ʵ����");
			//							int ret;
			//							m_handle->messageBox(QMessageBox::Critical, tr("Register Error"),tr("invalidate register code,please reenter register code again or contact State Key Lab of CAD&CG,ZheJiang University"),&ret);
			//							fclose(regFile);
			//							return false;
			//						}
			//					}
			//				}
			//			}
			//			else // ȡ��ע�Ტ�˳�
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
			// �ɹ���ȡ"��ֵ"
			//if(processReg.QueryStringValue("RegCodeBack", regTime, &regNum) != ERROR_SUCCESS) {
			//	AfxMessageBox("emX����ѹ�ʹ���ڣ�����ϵ�㽭��ѧCAD&CG�����ص�ʵ����");				
			//	break;
			//}
			//else {
			//	CString RegTime(regTime);
			//	if(!IsValidTime(RegTime)) {
			//		AfxMessageBox("emX����ѹ�ʹ���ڣ�����ϵ�㽭��ѧCAD&CG�����ص�ʵ����");				
			//		break;
			//	}
			//}
		}
		// else �ɹ���ȡ"��ֵ",�ж�ע�����Ƿ���Ч


		if (!DecodeRegCode(regCode, regCodeGenID, regCodeGenTime))
			return false;

		if (TrialRegID == QString(regCodeGenID)) { 	//ע������Ӳ�̵�ַƥ��
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
		else { //��Чע���룬ɾ����ֵ
			if (settings.value("RegCode", -1).toInt() != -1)
				settings.remove("RegCode");
			int ret;
			m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("invalidate register code,please reenter register code again or contact State Key Lab of CAD&CG,ZheJiang University"), &ret);
		}
	} while (!isValid);
	//////////////////////////////////////////////////////////////////////////
	// ע������Ч�����ע��ʱ�������ʱ���Ƿ���Ч
	//////////////////////////////////////////////////////////////////////////
	if (isValid) {
		// û��ע��ʱ���ע�����Ƿ��Ѿ�����
		if (!isRegisted && !isCodeGenTimeValid(regCodeGenTime))
		{
			//AfxMessageBox("ע�����ѹ��ڣ�����ϵ�㽭��ѧCAD&CG�����ص�ʵ���һ�ȡ�µ�ע����");
			int ret;
			m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("register code is out of data,please contact State Key Lab of CAD&CG,ZheJiang University"), &ret);
			return false;//FullRegisterAndValidate(processReg, timeReg, RegID, LimitedRegID, FullRegID);
		}

		// ���ð�����ư���ע��ʱ��
		if (regType == 1 || regType == 2)
		{
			regNum = 255;
			if (settings.value("RegCodeBack", -1).toInt() == -1) // ע��ʱ�䲻����
			{
				if (isRegisted) // ��ע�ᣬ��ע��ʱ���ѱ�ɾ��
				{
					//AfxMessageBox("emX����ѹ�ʹ���ڣ���������ϵ�㽭��ѧCAD&CG�����ص�ʵ����");
					int ret;
					m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("emX software is out of data,please contact State Key Lab of CAD&CG,ZheJiang University if renew"), &ret);
					// ɾ��ע����
					if (settings.value("RegCode", -1).toInt() != -1)
						settings.remove("RegCode");
					return false;//FullRegisterAndValidate(processReg, timeReg, RegID, LimitedRegID, FullRegID);
				}
				else // δע�ᣬд��ע��ʱ��
				{
					GetCurrentRegTime(regTime);
					settings.setValue("RegCodeBack", regTime);
					getCurrentDateTime(regTime);
					settings.setValue("RegCodeBackBk", regTime);
				}
			}
			else // ע��ʱ���Ѵ���
			{
				QString RegTime(regTime);
				if (!IsValidTime(RegTime))
				{
					//AfxMessageBox("emX����ѹ�ʹ���ڣ���������ϵ�㽭��ѧCAD&CG�����ص�ʵ����");
					int ret;
					m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("emX software is out of data,please contact State Key Lab of CAD&CG,ZheJiang University if renew"), &ret);
					//ɾ��ע�����ע��ʱ��
					if (settings.value("RegCode", -1).toInt() != -1)
						settings.remove("RegCode");
					settings.remove("RegCodeBack");
					settings.remove("RegCodeBackBk");
					return false;//FullRegisterAndValidate(processReg, timeReg, RegID, LimitedRegID, FullRegID);
				}
			}
		}

		//д��ע����
		settings.setValue("RegCode", regCode);
		fwrite(regCode, 255, 1, regFile);

		//////////////////////////////////////////////////////////////////////////
		// ���ð�ʱ��۸ĵļ��
		if (regType == 1 || regType == 2)
		{
			if (!timeCheatCheck(settings)) //�������ʱ��۸ģ�ɾ��ע���벢��ʾ�û�����ע��
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
		//AfxMessageBox("emX��������ļ��򿪴����������Ŀ¼���ļ��Ƿ���ж�дȨ�ޣ�����ϵ�㽭��ѧCAD&CG�����ص�ʵ����");
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
	LPCTSTR lpRootPathName = L"c:\\";	// ȡC��
	LPTSTR volumeNameBuffer = new TCHAR[12];			// ���̾��	
	DWORD nVolumeNameSize = 12;			// �����ַ�������
	DWORD VolumeSerialNumber;			// Ӳ�����к�
	DWORD MaximumComponentLength;		// �����ļ�����
	LPTSTR fileSystemNameBuffer = new TCHAR[10];		// �洢�����̷��ķ������͵ĳ�ָ�����
	DWORD nFileSystemNameSize = 10;		// �������͵ĳ�ָ�������ָ����ַ�������
	DWORD FileSystemFlags;				// �ļ�ϵͳ��һ�˱�־
	bool bget = ::GetVolumeInformation(lpRootPathName,
		volumeNameBuffer, nVolumeNameSize,
		&VolumeSerialNumber, &MaximumComponentLength,
		&FileSystemFlags,
		fileSystemNameBuffer, nFileSystemNameSize);
	//bool bget = true;
	if (!bget)
		return false;
#endif
	// ת��Ӳ�����к�Ϊʮ�������ַ�
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
* ע��ʱ��
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
	regTime[4] = temptm->tm_mday % 10 + 80; // mday�ĸ�λ + 80
	regTime[5] = temptm->tm_mday / 10 + 90; // mday��ʮλ + 90
	regTime[6] = '\0';
}

bool ueesCodeRegister::DecodeRegCode(char regCode[255], char codeGenID[255], time_t& codeGenTime)
{
	// �ж�ע������ע���ʶ���Ƿ�ƥ��
	char key[] = { 'b', 'F', 'd', 'E', 'G', '/', '!', 'T', '5', '~', '#', 'p', 'U', 'W', 'r', 'e' };
	char buf[255], outbuf[255];
	memset(buf, 0, sizeof(buf));
	memset(outbuf, 0, 255);
	// ��ע����ָ�Ϊ����
	int regCodeLength = static_cast<int>(strlen(regCode)) / 2;
	for (int j = 0; j < regCodeLength; ++j){
		char ch8 = 0;
		unsigned char high4 = CharToHex(regCode[j * 2]);
		unsigned char low4 = CharToHex(regCode[j * 2 + 1]);
		ch8 = ch8 | (high4 << 4) | low4;
		buf[j] = ch8;
	}

	//���ܻ�ԭ
	CDESEncrypter des;
	des.Des_Go(outbuf, buf, regCodeLength, key, sizeof(key), CDESEncrypter::DECRYPT);

	if (strlen(outbuf) != 26)
	{
		int ret;
		m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("invalidate register code,please reenter register code again or contact State Key Lab of CAD&CG,ZheJiang University"), &ret);
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	//��ע�����׺���ע�����������ڣ�������ֹһ��ע���뱻���ʹ��
	char regCodeGenDate[REG_TIME_LENGTH];
	memset(regCodeGenDate, 0, REG_TIME_LENGTH);
	memcpy(regCodeGenDate, outbuf + 20, REG_TIME_LENGTH); // ��ע������ȡ��ע�����ڵı���
	struct tm regCodeGenTime_tm = regTimeStr2TM(regCodeGenDate); // ������ָ�Ϊע������
	codeGenTime = mktime(&(regCodeGenTime_tm));
	//printf("Reg Code Generation Date:%s\n", ctime(&regCodeGenTime));
	outbuf[20] = 0;//ǰ��20λΪע����
	//////////////////////////////////////////////////////////////////////////
	memset(codeGenID, 0, 255);
	memcpy(codeGenID, outbuf, strlen(outbuf)); // ȡǰ20λ������ŵ�codeGenID��

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
	int keyUseLimit = 10;			// ����ע�������ɺ���10��֮��ʹ����Ч
	time_t keyDeadLine = regTime + keyUseLimit * 24 * 3600;
	// ע�����ڱ���ʱ��֮������ && ע��������ʱ��С�ڵ�ǰʱ�� && ��ǰʱ����ע�������ɺ�10��֮��
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
	regTime[0] = temptm->tm_min + 41; // 41~100(ȡֵ��Χ)
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
		// ����ֹ��������
		struct tm end_tm;
		memset(&end_tm, 0, sizeof(struct tm));
		end_tm.tm_year = 115; // 2015��
		end_tm.tm_mon = 9; //10��
		end_tm.tm_mday = 31; //31��
		endTime = mktime(&end_tm);
	}
	else if (regType == 2)
	{
		//////////////////////////////////////////////////////////////////////////
		// ����������
		int licenseValidDayCount = 180;//֤����Ч����
		endTime = regTime + licenseValidDayCount * 24 * 3600;
	}
	return endTime;
}

bool ueesCodeRegister::timeCheatCheck(QSettings &setting)
{
	bool ret = false;
	char regTime[255];
	ULONG regNum = 255;
	// �����ϴ�����ʱ�䣬�鿴ϵͳʱ���Ƿ񱻴۸ģ���ֵRegCodeBackBk���ڴ洢ÿ��emx������ʱ��
	QByteArray tregTime = setting.value("RegCodeBackBk", -1).toByteArray();
	tregTime.append("\0");
	strcpy(regTime, tregTime);
	if (setting.value("RegCodeBackBk", -1).toInt() == -1)
	{
		//ע������ϴ�����ʱ�䱻ɾ��������Error Code:TC001  (Time Cheat 001)
		//AfxMessageBox("emX���ע����Ϣ���ʧ�ܣ�����������ע���룬����ϵ�㽭��ѧCAD&CG�����ص�ʵ����(Error Code:TC001)");
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
			//ʱ��۸���ƭ��ע����е��ϴ�����ʱ�䱻�޸�����Чʱ�䣬����Error Code:TC003  (Time Cheat 003)
			//AfxMessageBox("emX���ע����Ϣ���ʧ�ܣ�����������ע���룬����ϵ�㽭��ѧCAD&CG�����ص�ʵ����(Error Code:TC003)");
			int re;
			m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("check emX software register information error,please reenter the register code or contact State Key Lab of CAD&CG,ZheJiang University(Error Code:TC001)"), &re);
			ret = false;
		}
		else{
			if (currentTime < lastRunTime) //��ǰʱ�����ϴ�����ʱ��֮ǰ
			{
				//ʱ��۸���ƭ��ϵͳʱ�䱻�޸����ϴ�����֮ǰ������Error Code:TC002  (Time Cheat 002)
				//AfxMessageBox("emX���ע����Ϣ���ʧ�ܣ�����������ע���룬����ϵ�㽭��ѧCAD&CG�����ص�ʵ����(Error Code:TC002)");
				int re;
				m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("check emX software register information error,please reenter the register code or contact State Key Lab of CAD&CG,ZheJiang University(Error Code:TC001)"), &re);
				ret = false;
			}
			else
			{
				// д�뱾������ʱ��
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
	//������֤ǰ��4���ֽڵ���Ϣ
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
	// ע��
	m_registerDialog->setRegID(RegID);
	if (m_registerDialog->exec() == QDialog::Rejected)
		return false;

	// ע����
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
			//AfxMessageBox("ע�����ѹ��ڣ�����������ע���룬����ϵ�㽭��ѧCAD&CG�����ص�ʵ����");
			int ret;
			m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("register code is out of data,please contact State Key Lab of CAD&CG,ZheJiang University"), &ret);
			return FullRegisterAndValidate(setting, RegID, LimitedRegID, FullRegID);
		}
		else { //ע������Ч��д��ע��ʱ�估�״�����ʱ��
			GetCurrentRegTime(regTime);
			setting.setValue("RegCodeBack", regTime);
			getCurrentDateTime(regTime);
			setting.setValue("RegCodeBackBk", regTime);
			setting.setValue("RegCode", regCode);
			int ret;
			m_handle->messageBox(QMessageBox::Information, tr("Register Success"), tr("The trial version upgrade successfully"), &ret);
			//AfxMessageBox("���ð������ɹ���");
		}
	}
	else if (FullRegID == QString(regCodeGenID)) {
		regType = 3;
		if (!isCodeGenTimeValid(regCodeGenTime))
		{
			//AfxMessageBox("ע�����ѹ��ڣ�����������ע���룬����ϵ�㽭��ѧCAD&CG�����ص�ʵ����");
			int ret;
			m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("register code is out of data,please contact State Key Lab of CAD&CG,ZheJiang University"), &ret);
			return FullRegisterAndValidate(setting, RegID, LimitedRegID, FullRegID);
		}
		else { //ע������Ч��д��ע��ʱ�估�״�����ʱ��
			GetCurrentRegTime(regTime);
			setting.setValue("RegCodeBack", regTime);
			setting.setValue("RegCodeBackBk", regTime);
			setting.setValue("RegCode", regCode);
			//AfxMessageBox("�����������ɹ�");
			int ret;
			m_handle->messageBox(QMessageBox::Information, tr("Register Success"), tr("The full version upgrade successfully"), &ret);
		}
	}
	else {
		int ret;
		m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("invalidate register code,please contact State Key Lab of CAD&CG,ZheJiang University"), &ret);
		//AfxMessageBox("��Чע���룬����ϵ�㽭��ѧCAD&CG�����ص�ʵ����");
		return false;
	}
	return true;
}

bool ueesCodeRegister::FullRegisterAndValidate(QSettings& setting, QString& RegID, QString& LimitedRegID, QString& FullRegID)
{
	// ע��
	// ע��
	//emxRegisterDialog regDlg;
	m_registerDialog->setRegID(RegID);
	if (m_registerDialog->exec() == QDialog::Rejected)
		return false;

	// ע����
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
			//AfxMessageBox("ע�����ѹ��ڣ�����������ע���룬����ϵ�㽭��ѧCAD&CG�����ص�ʵ����");
			int ret;
			m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("register code is out of data,please contact State Key Lab of CAD&CG,ZheJiang University"), &ret);
			return FullRegisterAndValidate(setting, RegID, LimitedRegID, FullRegID);
		}
		else { //ע������Ч��д��ע��ʱ�估�״�����ʱ��
			GetCurrentRegTime(regTime);
			setting.setValue("RegCodeBack", regTime);
			getCurrentDateTime(regTime);
			setting.setValue("RegCodeBackBk", regTime);
			setting.setValue("RegCode", regCode);
			int ret;
			m_handle->messageBox(QMessageBox::Information, tr("Register Success"), tr("The trial version upgrade successfully"), &ret);
			//AfxMessageBox("���ð������ɹ���");
		}
	}
	else if (FullRegID == QString(regCodeGenID)) {
		regType = 3;
		if (!isCodeGenTimeValid(regCodeGenTime))
		{
			int ret;
			m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("register code is out of data,please contact State Key Lab of CAD&CG,ZheJiang University"), &ret);
			//AfxMessageBox("ע�����ѹ��ڣ�����������ע���룬����ϵ�㽭��ѧCAD&CG�����ص�ʵ����");
			return FullRegisterAndValidate(setting, RegID, LimitedRegID, FullRegID);
		}
		else { //ע������Ч��д��ע��ʱ�估�״�����ʱ��
			GetCurrentRegTime(regTime);
			setting.setValue("RegCodeBack", regTime);
			setting.setValue("RegCodeBackBk", regTime);
			setting.setValue("RegCode", regCode);
			int ret;
			m_handle->messageBox(QMessageBox::Information, tr("Register Success"), tr("The full version upgrade successfully"), &ret);
			//AfxMessageBox("�����������ɹ�");
		}
	}
	else {
		int ret;
		m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("invalidate register code,please contact State Key Lab of CAD&CG,ZheJiang University"), &ret);
		//AfxMessageBox("��Чע���룬����ϵ�㽭��ѧCAD&CG�����ص�ʵ����");
		return false;
	}
	return true;
}

bool ueesCodeRegister::FullRegisterAndValidate(QString& RegID, QString& TrialRegID, QString& LimitedRegID, QString& FullRegID)
{
	QSettings setting(QSettings::NativeFormat, QSettings::UserScope, "ZJU", "emXIE");
	// ע����
	char regCode[255], regTime[255], regCodeGenID[255];
	ULONG charNum = 255, regNum = 255;

	if (m_registerDialog->getRegCode().length() < 255)
	{
		QString ttregCode = m_registerDialog->getRegCode();
		QByteArray tregCode = ttregCode.toLatin1();
		tregCode.append("\0");
		strcpy(regCode, tregCode);
		strcpy(regCode, tregCode);
		//���ע�����Ƿ��Ѿ��ù�
		char regCodeOld[255];
		bool sameCode = true;
		while (fread(regCodeOld, 255, 1, regFile) > 0)
		{
			sameCode = true;
			for (int i = 0; i < 64; ++i) // 64��ע����ĳ���
			{
				if (regCode[i] != regCodeOld[i])
				{
					sameCode = false;
					break;
				}
			}
			if (sameCode)
			{
				//AfxMessageBox("��Чע���룬����������ע���룬����ϵ�㽭��ѧCAD&CG�����ص�ʵ����");
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
			//AfxMessageBox("ע�����ѹ��ڣ�����������ע���룬����ϵ�㽭��ѧCAD&CG�����ص�ʵ����");
			int ret;
			m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("register code is out of data,please contact State Key Lab of CAD&CG,ZheJiang University"), &ret);
			return false;//FullRegisterAndValidate(setting, RegID, LimitedRegID, FullRegID);
		}
		else { //ע������Ч��д��ע��ʱ�估�״�����ʱ��
			GetCurrentRegTime(regTime);
			setting.setValue("RegCodeBack", regTime);
			getCurrentDateTime(regTime);
			setting.setValue("RegCodeBackBk", regTime);
			setting.setValue("RegCode", regCode);
			int ret;
			m_handle->messageBox(QMessageBox::Information, tr("Register Success"), tr("The trial version upgrade successfully"), &ret);
			//AfxMessageBox("���ð������ɹ���");
			return true;
		}
	}
	if (regType == 1 && LimitedRegID == QString(regCodeGenID)) {
		regType = 2;
		if (!isCodeGenTimeValid(regCodeGenTime))
		{
			//AfxMessageBox("ע�����ѹ��ڣ�����������ע���룬����ϵ�㽭��ѧCAD&CG�����ص�ʵ����");
			int ret;
			m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("register code is out of data,please contact State Key Lab of CAD&CG,ZheJiang University"), &ret);
			return false;//FullRegisterAndValidate(setting, RegID, LimitedRegID, FullRegID);
		}
		else { //ע������Ч��д��ע��ʱ�估�״�����ʱ��
			GetCurrentRegTime(regTime);
			setting.setValue("RegCodeBack", regTime);
			getCurrentDateTime(regTime);
			setting.setValue("RegCodeBackBk", regTime);
			setting.setValue("RegCode", regCode);
			int ret;
			m_handle->messageBox(QMessageBox::Information, tr("Register Success"), tr("The trial version upgrade successfully"), &ret);
			//AfxMessageBox("���ð������ɹ���");
			return true;
		}
	}
	else if (FullRegID == QString(regCodeGenID)) {
		regType = 3;
		if (!isCodeGenTimeValid(regCodeGenTime))
		{
			//AfxMessageBox("ע�����ѹ��ڣ�����������ע���룬����ϵ�㽭��ѧCAD&CG�����ص�ʵ����");
			int ret;
			m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("register code is out of data,please contact State Key Lab of CAD&CG,ZheJiang University"), &ret);
			return false;//FullRegisterAndValidate(setting, RegID, LimitedRegID, FullRegID);
		}
		else { //ע������Ч��д��ע��ʱ�估�״�����ʱ��
			GetCurrentRegTime(regTime);
			setting.setValue("RegCodeBack", regTime);
			setting.setValue("RegCodeBackBk", regTime);
			setting.setValue("RegCode", regCode);
			//AfxMessageBox("�����������ɹ�");
			int ret;
			m_handle->messageBox(QMessageBox::Information, tr("Register Success"), tr("The full version upgrade successfully"), &ret);
		}
	}
	else {
		int ret;
		m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("invalidate register code,please contact State Key Lab of CAD&CG,ZheJiang University"), &ret);
		//AfxMessageBox("��Чע���룬����ϵ�㽭��ѧCAD&CG�����ص�ʵ����");
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
	// ���ڴ洢�Ѿ��ù���ע������ļ�
	// �����û��������Чע�������¼�ڴ��ļ��У���ֹһ��ע���뱻���ʹ��
	//fclose(regFile);
	//regFile = fopen("data.db", "ab+");
	//if(regFile == NULL)
	//{
	//	//AfxMessageBox("emX��������ļ��򿪴����������Ŀ¼���ļ��Ƿ���ж�дȨ�ޣ�����ϵ�㽭��ѧCAD&CG�����ص�ʵ����");
	//int ret;
	//	//QMessageBox::warning((QWidget*)(this->parent()),tr("Register Error"),tr("emX open the data file error,please check the whether the file and the directory has read and write permission,or contact State Key Lab of CAD&CG,ZheJiang University"));
	//m_handle->messageBox(QMessageBox::Critical, tr("Register Error"),tr("emX open the data file error,please check the whether the file and the directory has read and write permission,or contact State Key Lab of CAD&CG,ZheJiang University"),&ret);
	//	exit(0);
	//}
	if (!GenerateVolumeInfo(RegID, TrialRegID, LimitedRegID, FullRegID)) {
		// �޷���ȡ����C�����к�
		int ret;
		//AfxMessageBox("��ȡע����Ϣʧ�ܣ�����ϵ�㽭��ѧCAD&CG�����ص�ʵ����");
		m_handle->messageBox(QMessageBox::Critical, tr("Register Error"), tr("emX read the register information error,please contact State Key Lab of CAD&CG,ZheJiang University"), &ret);
		exit(0);
	}
	return RegID;
}

ueesCodeRegister::~ueesCodeRegister()
{
	fclose(regFile);
}
