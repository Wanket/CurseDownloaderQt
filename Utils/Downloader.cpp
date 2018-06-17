#include <QFile>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QStringBuilder>
#include <QDir>
#include "Downloader.h"

QString Downloader::downloadFile(const QUrl &from, const QDir &to)
{
    QNetworkReply *reply = getReply(from);

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
            return downloadFile(QUrl("https://" % reply->url().host() % redirectString), to);
        }
        else
        {
            return downloadFile(redirectUrl, to);
        }
    }

    QString fileName = QUrl::fromPercentEncoding(reply->url().toString().split('/').last().toUtf8());
    QFile file(to.path() % '/' % fileName);
    file.open(QFile::WriteOnly);
    file.write(reply->readAll());

    return fileName;
}

QNetworkReply *Downloader::getReply(const QUrl &from)
{
    QNetworkRequest request(from);
    QNetworkReply *reply = networkAccessManager.get(request);

    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    return reply;
}
