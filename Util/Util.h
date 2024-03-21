#ifndef _UTIL_H_
#define _UTIL_H_
#include <QString>
QString uLongLongToHexStr(qulonglong num);
QString uintToHexStr(unsigned int num);
QString ushortToHexStr(unsigned short num);
QString getCssContent(const QString& filePath);
#endif
