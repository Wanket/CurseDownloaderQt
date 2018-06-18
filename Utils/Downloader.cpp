#include <QFile>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QStringBuilder>
#include <QDir>
#include <QProgressBar>
#include "Downloader.h"

QString Downloader::downloadFile(const QUrl &from, const QDir &to, QProgressBar *downloadProgress)
{
    QNetworkReply *reply = getReply(from, downloadProgress);

    if (QNetworkReply::NetworkError error = reply->error(); error)
    {
        throw DownloadException(error);
    }

    if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute) != 200)
    {
        QUrl redirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();

        QString redirectString = redirectUrl.toString();
        if (redirectString[0] == '/')
        {
            return downloadFile(QUrl("https://" % reply->url().host() % redirectString), to, downloadProgress);
        }
        else
        {
            return downloadFile(redirectUrl, to, downloadProgress);
        }
    }

    QString fileName = QUrl::fromPercentEncoding(reply->url().toString().split('/').last().toUtf8());
    QFile file(to.path() % '/' % fileName);
    file.open(QFile::WriteOnly);
    file.write(reply->readAll());

    return fileName;
}

QNetworkReply *Downloader::getReply(const QUrl &from, QProgressBar *downloadProgress)
{
    QNetworkRequest request(from);
    QNetworkReply *reply = networkAccessManager.get(request);

    connect(reply, &QNetworkReply::downloadProgress, [downloadProgress](qint64 value, qint64 total)
    {
        if (total == 0 || value == 0)
        {
            return;
        }

        int new_value = (int)(100 / ((long double)total / (long double)value));
        downloadProgress->setValue(new_value);
    });

    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    return reply;
}
