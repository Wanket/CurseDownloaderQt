#pragma once

#include <QString>
#include <QFile>
#include <QMap>
#include <QSettings>

class ModsListFile
{
public:
    explicit ModsListFile(const QString &path);

    bool addMod(const int mod, const QString &fileName);

    bool existMod(const int mod);

    QString getMod(const int mod);

private:
    QSettings settings;
};
