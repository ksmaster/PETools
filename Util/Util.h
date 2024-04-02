#ifndef _UTIL_H_
#define _UTIL_H_
#include <QString>
QString uLongLongToHexStr(qulonglong num);
QString uintToHexStr(unsigned int num);
QString ushortToHexStr(unsigned short num);
QString getCssContent(const QString& filePath);
bool withPEExt(const QString& filePath);


class CBinaryStateGuard
{
public:
	CBinaryStateGuard::CBinaryStateGuard(bool& bState, bool bInitState = true) :m_bInitState(bInitState), m_bState(m_bInitState)
	{
		
	}
	CBinaryStateGuard::~CBinaryStateGuard()
	{
		m_bState = !m_bInitState;
	}
private:
	bool& m_bState;
	bool m_bInitState;
};

#endif
