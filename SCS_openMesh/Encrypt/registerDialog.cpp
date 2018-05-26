#include "registerDialog.h"
#include<QVBoxLayout> 
#include<QHBoxLayout> 
#include<QLabel> 
#include<QRadioButton> 
#include<QButtonGroup> 
#include<QLineEdit> 
#include<QGridLayout> 
#include <QSettings>
#include <QTimerEvent>
#include "QtLicenseManager.h"
#include "sentinel/SentinelKeys.h"
#ifdef Q_OS_WIN32
#include <windows.h>
#endif // Q_OS_WIN32
#define  DEVELOPERID                                         0xA97BF285

#define  LICENSEID                                           0x209
//Q_DECLARE_METATYPE(void*)//QVariant declare void pointer type for convert
extern unsigned char SOFTWARE_KEY[SP_SOFTWARE_KEY_LEN];


ueesRegisterDialog::ueesRegisterDialog(QWidget * parent) :QWizard(parent)
{
	
	settings = new QSettings(QSettings::NativeFormat, QSettings::UserScope, "ZJU", "UEESIE");
	m_status = false;
	msgHandler = new ueesMessageHandler;
	m_codeRegister = new ueesCodeRegister(msgHandler);
	m_codeRegister->setRegisterDialog(this);
	//setField("codeRegister", m_codeRegister);
	init();
	connect(button(QWizard::NextButton), SIGNAL(clicked()), this, SLOT(sentinelDetect()));
}

ueesRegisterDialog::~ueesRegisterDialog()
{
	delete msgHandler;
	delete m_codeRegister;
}

void ueesRegisterDialog::setRegID(QString regID)
{
	setField("regID", regID);
}

QString ueesRegisterDialog::getRegCode()
{
	return field("regCodeEdit").value<QString>();
}

void ueesRegisterDialog::accept()
{
	if (field("codeRadioButton").value<bool>())
	{
		settings->setValue("RegisterMode", 0);
		if (codecheck())
		{
			m_status = true;
		}
		else{
			m_status = false;
		}
	}
	else
	{
		settings->setValue("RegisterMode", 1);
		char errInfo[256];
		if (LicenseManager::instance()->check(errInfo) != LM_SUCCESS)
		{
			QMessageBox::critical(NULL, tr("Error"), QString::fromLocal8Bit(errInfo));
			m_status = false;
		}
		else{
			m_status = true;
		}
	}
	QDialog::accept();
}

void ueesRegisterDialog::reject()
{
	QDialog::reject();
}

void ueesRegisterDialog::init()
{
	setPage(Page_Intro, new IntroPage);
	setPage(Page_RegisterMode, new registerationModePage);
	setPage(Page_Register, new registerationPage);
	setPage(Page_SentinelCheck, new sentinelCheckPage);
	m_RegID = m_codeRegister->GetRegID(m_RegID, m_TrialRegID, m_LimitedRegID, m_FullRegID);
	setRegID(m_RegID);
}

bool ueesRegisterDialog::codecheck()
{
	return m_codeRegister->FullRegisterAndValidate(m_RegID, m_TrialRegID, m_LimitedRegID, m_FullRegID);
}

bool ueesRegisterDialog::getStatus()
{
	return m_status;
}


QString ueesRegisterDialog::getRegID()
{
	if (m_RegID.length() == 0)
	{
		if (!m_codeRegister->GenerateVolumeInfo(m_RegID, m_TrialRegID, m_LimitedRegID, m_FullRegID)) {
			QMessageBox::critical(this, tr("Register Error"), tr("UEES read the register information error,please contact State Key Lab of CAD&CG,ZheJiang University"));
			exit(0);
		}
	}
	return m_RegID;
}

ueesCodeRegister* ueesRegisterDialog::getCodeRegister()
{
	return m_codeRegister;
}

void ueesRegisterDialog::sentinelDetect()
{
	if (currentId() == ueesRegisterDialog::Page_SentinelCheck)
	{
		button(QWizard::FinishButton)->setDisabled(true);
		settings->setValue("RegisterMode", 1);
		timeid = startTimer(1000);
		//emit accept();
	}
}

void ueesRegisterDialog::timerEvent(QTimerEvent *event)
{
	char errInfo[256];
	if (event->timerId() == timeid)
	{
		SP_DWORD  flags = SP_STANDALONE_MODE;	/* flags for License  */
		SP_HANDLE licHandle;
		if (SFNTGetLicense(DEVELOPERID, SOFTWARE_KEY, LICENSEID, flags, &licHandle) == SP_SUCCESS)
		{
			killTimer(timeid);
			timeid = 0;
			emit accept();
		}
	}
	else
	{
		QWizard::timerEvent(event);
	}

}


registerationModePage::registerationModePage(QWizard *parent /*=0*/) :QWizardPage(parent)
{
	setTitle(tr("Registration Method"));
	setSubTitle(tr("Select a method to register UEES. We offer two methods. You can register UEES with Registration code or Sentinel Dog"));
	verticalLayout = new QVBoxLayout();
	label = new QLabel();
	verticalLayout->addWidget(label);
	horizontalLayout = new QHBoxLayout();
	codeRadioButton = new QRadioButton();
	horizontalLayout->addWidget(codeRadioButton);
	dogRadioButton = new QRadioButton();
	horizontalLayout->addWidget(dogRadioButton);
	verticalLayout->addLayout(horizontalLayout);
	this->setLayout(verticalLayout);
	label->setText(tr("Please Select Register Mode"));
	codeRadioButton->setText(tr("Registration Code"));
	dogRadioButton->setText(tr("Sentinel Dog"));
	codeRadioButton->setChecked(true);
	//registerModeGroup = new QButtonGroup();
	// 	registerModeGroup->addButton(codeRadioButton,0);
	// 	registerModeGroup->addButton(dogRadioButton,1);
	registerField("codeRadioButton", codeRadioButton);
	registerField("dogRadioButton", dogRadioButton);
}

int registerationModePage::nextId() const
{
	if (codeRadioButton->isChecked())
	{
		return ueesRegisterDialog::Page_Register;
	}
	else
	{
		if (dogRadioButton->isChecked())
		{
			return ueesRegisterDialog::Page_SentinelCheck;
		}
		else
		{
			QMessageBox::information(NULL, tr("Registration Error"), tr("please select registration method."));
			return ueesRegisterDialog::Page_RegisterMode;
		}

	}
}

registerationPage::registerationPage(QWizard *parent /*=0*/) :QWizardPage(parent)
{
	setTitle(tr("Registration Code"));
	setSubTitle(tr("Please enter the registration code below.If you haven't registration code,please contact State Key Lab of CAD&CG,ZheJiang University."));
	gridlayout = new QGridLayout();
	label = new QLabel();
	label->setText(tr("RegID"));
	gridlayout->addWidget(label, 0, 0);
	//QSpacerItem *horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	//horizontalLayout->addItem(horizontalSpacer);
	regIDEdit = new QLineEdit();
	gridlayout->addWidget(regIDEdit, 0, 1);
	regIDEdit->setReadOnly(true);

	label_2 = new QLabel();
	label_2->setText(tr("RegCode"));
	gridlayout->addWidget(label_2, 1, 0);
	//QSpacerItem * horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	//horizontalLayout_2->addItem(horizontalSpacer_2);
	regCodeEdit = new QLineEdit();
	gridlayout->addWidget(regCodeEdit, 1, 1);
	//gridlayout->setRowStretch()
	this->setLayout(gridlayout);
	registerField("regID", regIDEdit);
	registerField("regCodeEdit", regCodeEdit);
}

int registerationPage::nextId() const
{
	return -1;
}

sentinelCheckPage::sentinelCheckPage(QWizard *parent /*=0*/)
{
	setTitle(tr("Sentinel Dog"));
	setSubTitle(tr("Insert Sentinel Dog into this computer."));
	verticalLayout = new QVBoxLayout();
	label = new QLabel;
	label->setText(tr("Sentinel Dog is checking..."));
	verticalLayout->addWidget(label);
	this->setLayout(verticalLayout);
	//registerField("SentinelErrorMsg",label);
}

int sentinelCheckPage::nextId() const
{
	return -1;
}

IntroPage::IntroPage(QWizard *parent /*=0*/)
{
	setTitle(tr("UEES Introduction"));
	setSubTitle(tr("UEES is an urban electromagnetic environment simulation system designed by State Key Lab of CAD&CG,ZheJiang University."));
	verticalLayout = new QVBoxLayout();
	label = new QLabel("");
	QFont ft;
	ft.setPointSize(14);
	label->setFont(ft);
	verticalLayout->addWidget(label);
	this->setLayout(verticalLayout);
}

int IntroPage::nextId() const
{
	return ueesRegisterDialog::Page_RegisterMode;
}
