#pragma once

#include "ui_MainWindow.h"
#include <QMainWindow>
#include <nlohmann/json.hpp>
#include <Utils/Downloader.h>

using Json = nlohmann::json;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();

private:
    Ui::MainWindow ui;

    Json json;

    Downloader downloader;

    void setupListeners();

    void setupInfoModPack();

private slots:
    void onPathButtonClick();

    void onDownloadButtonClick();
};
