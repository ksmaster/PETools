
#include "Util.h"
#include <QChar>
#include <QString>
#include <QFile>
#include <QDateTime>



static const QStringList PE_EXT_LIST {
	".exe",
	".dll",
	//".sys",
};

QString uLongLongToHexStr(qulonglong num) {
	return QString("%1").arg(num, 16, 16, QChar('0')).toUpper();
}

QString uintToHexStr(unsigned int num) {
    return QString("%1").arg(num, 8, 16, QChar('0')).toUpper();
}

QString ushortToHexStr(unsigned short num) {
    return QString("%1").arg(num, 4, 16, QChar('0')).toUpper();
}

QString getCssContent(const QString& filePath)
{
	QFile f1;
	f1.setFileName(filePath);
	f1.open(QIODevice::ReadOnly);
	QString strQss = f1.readAll();
	f1.close();
	return strQss;
}

bool withPEExt(const QString& filePath)
{
	for (auto& ext : PE_EXT_LIST)
	{
		if (filePath.endsWith(ext))
		{
			return true;
		}
	}
	return false;
}

