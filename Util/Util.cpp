#ifndef _UTIL_H_
#define _UTIL_H_
#include "Util.h"
#include <QChar>
#include <QString>

QString uintToHexStr(unsigned int num) {
    return QString("%1").arg(num, 8, 16, QChar('0')).toUpper();
}

QString ushortToHexStr(unsigned short num) {
    return QString("%1").arg(num, 4, 16, QChar('0')).toUpper();
}

#endif
