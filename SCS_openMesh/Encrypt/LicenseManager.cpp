#include "LicenseManager.h"

LicenseManager* LicenseManager::m_pInstance = 0;

LicenseManager::LicenseManager()
: m_licenseInfo("")
{}

LicenseManager::~LicenseManager()
{}

void LicenseManager::setManager(LicenseManager* pManager)
{
	m_pInstance = pManager;
}

LicenseManager* LicenseManager::instance()
{
	return m_pInstance;
}

void LicenseManager::destroy()
{
	if (m_pInstance != 0)
	{
		delete m_pInstance;
		m_pInstance = 0;
	}
}

std::string LicenseManager::getLicenseInfo() const
{
	return m_licenseInfo;
}
