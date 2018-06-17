#include "ModsListFile.h"

ModsListFile::ModsListFile(const QString &path)
        : settings(path, QSettings::NativeFormat)
{}

bool ModsListFile::addMod(const int mod, const QString &fileName)
{
    if (existMod(mod))
    {
        return false;
    }

    settings.setValue(QString::number(mod), fileName);

    return true;
}

bool ModsListFile::existMod(const int mod)
{
    return !settings.value(QString::number(mod)).isNull();
}

QString ModsListFile::getMod(const int mod)
{
    return settings.value(QString::number(mod)).toString();
}
