#pragma once

#include <QString>
#include <QFile>
#include <QMap>
#include <QSettings>

class ModsListFile
{
public:
    explicit ModsListFile(const QString &path);

    bool addMod(int mod, const QString &fileName);

    bool existMod(int mod);

    QString getMod(int mod);

private:
    QSettings settings;
};
