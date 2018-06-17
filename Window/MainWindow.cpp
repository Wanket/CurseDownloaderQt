#include "MainWindow.h"
#include "Utils/Constants.h"
#include <QFileDialog>
#include <QMessageBox>
#include <fstream>
#include <QStringBuilder>
#include <Utils/ModsListFile.h>

MainWindow::MainWindow()
{
    ui.setupUi(this);
    setupListeners();
}

void MainWindow::setupListeners()
{
    connect(ui.pathButton, SIGNAL(clicked(bool)), SLOT(onPathButtonClick()));
    connect(ui.downloadButton, SIGNAL(clicked(bool)), SLOT(onDownloadButtonClick()));
}

void MainWindow::onPathButtonClick()
{
    QString path = QFileDialog::getOpenFileName(this, Constants::choiceManifest, "~", "*.json");

    if (path.isEmpty())
    {
        return;
    }

    try
    {
        json = Json::parse(std::ifstream(path.toStdString()));
        setupInfoModPack();
        ui.pathLine->setText(path);
    }
    catch (...)
    {
        QMessageBox::critical(this, "Error", Constants::errorOpenManifest);
    }
}

void MainWindow::setupInfoModPack()
{
    if (json["manifestType"] != "minecraftModpack" || json["manifestVersion"] != 1)
    {
        QMessageBox::information(this, "Error", Constants::unsupportedManifest);
        return;
    }

    ui.nameModPack->setText(static_cast<std::string>(json["name"]).c_str());
    ui.versionModPack->setText(static_cast<std::string>(json["version"]).c_str());
    ui.authorModPack->setText(static_cast<std::string>(json["author"]).c_str());
    ui.overridesPath->setText(static_cast<std::string>(json["overrides"]).c_str());

    Json minecraft = json["minecraft"];
    ui.gameVersion->setText(static_cast<std::string>(minecraft["version"]).c_str());

    Json modLoaders = minecraft["modLoaders"][0];
    ui.forgeVersion->setText(static_cast<std::string>(modLoaders["id"]).c_str());
}

void MainWindow::onDownloadButtonClick()
{
    if (ui.pathLine->text().isEmpty())
    {
        return;
    }

    if (!ui.pathButton->isEnabled())
    {
        ui.pathButton->setEnabled(true);
        ui.downloadButton->setText(Constants::downloadMods);
        return;
    }

    ui.pathButton->setEnabled(false);
    ui.downloadButton->setText("Stop");
    ui.modsList->clear();

    try
    {
        Json files = json["files"];

        QString path = ui.pathLine->text().remove("manifest.json");
        QDir modsDir = path;
        modsDir.rename(path + ui.overridesPath->text(), path + "minecraft");
        modsDir.mkpath("minecraft/mods");
        modsDir.cd("minecraft/mods");

        ModsListFile modsListFile(modsDir.path() + "/curse_downloader_qt.cache");

        int i = 0;
        for (Json &file : files)
        {
            ++i;
            try
            {
                if (ui.pathButton->isEnabled())
                {
                    break;
                }

                if (int id = file["projectID"]; modsListFile.existMod(id))
                {
                    ui.modsList->addItem(modsListFile.getMod(id) + " (cached)");
                    ui.modsList->scrollToBottom();

                    continue;
                }

                QString url = QString("https://minecraft.curseforge.com/projects/") %
                              QString::number(static_cast<int>(file["projectID"])) % "/files/" %
                              QString::number(static_cast<int>(file["fileID"])) % "/download";
                QString fileName = downloader.downloadFile(url, modsDir);

                modsListFile.addMod(file["projectID"], fileName);

                ui.modsList->addItem(fileName % " (" % QString::number(i) % '/' % QString::number(files.size()) % ')');
                ui.modsList->scrollToBottom();
            }
            catch (DownloadException &downloadException)
            {
                QMessageBox::critical(this, "Network error",
                                      Constants::networkErrorCode % QString::number(downloadException.error));
            }
        }
    }
    catch (std::exception &exception)
    {
        QMessageBox::critical(this, "Manifest error", exception.what());
    }

    ui.pathButton->setEnabled(true);
    ui.downloadButton->setText(Constants::downloadMods);
}
