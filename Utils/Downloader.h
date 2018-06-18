#pragma once

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QtWidgets/QProgressBar>

class Downloader : public QObject
{
Q_OBJECT
public:
    QString downloadFile(const QUrl &from, const QDir &to, QProgressBar *downloadProgress);

private:
    QNetworkAccessManager networkAccessManager;

    QNetworkReply *getReply(const QUrl &from, QProgressBar *downloadProgress);
};

class DownloadException : public std::runtime_error
{
public:
    explicit DownloadException(QNetworkReply::NetworkError code)
            : std::runtime_error::runtime_error("Network error"),
              error(code)
    {}

    const QNetworkReply::NetworkError error;
};
