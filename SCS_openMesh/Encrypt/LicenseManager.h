#ifndef LICENSEMANAGER_H
#define LICENSEMANAGER_H

#include "apibase.h"
#include <string>
#define  LM_SUCCESS 0
#define  LM_FAIL  1

class DLLEXPORT LicenseManager
{
public:
	static LicenseManager* instance();
	static void destroy();
	virtual unsigned long check(char* msg = 0, const char* moduleName = 0) = 0;
	virtual void startLicenseThread() = 0;
	virtual void setMessageHandler(void* pHandler){};
	//virtual void endLicenseThread() = 0;
	std::string getLicenseInfo() const;

protected:
	LicenseManager();	/// do nothing
	~LicenseManager();	/// do nothing
	void setManager(LicenseManager* pManager);

protected:
	static LicenseManager* m_pInstance;	/// a static pointer to subclass object which implements license management.
	std::string m_licenseInfo;
};

#endif // LICENSEMANAGER_H