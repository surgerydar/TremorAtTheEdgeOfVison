#include "downloader.h"
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QDebug>
#include "systemutils.h"

Downloader::Downloader(QObject *parent) : QObject(parent), m_manager( this ) {

}

void Downloader::download(const QString &url, const QString &filepath) {
    QString message = "downloading";
    QString temporaryPath = filepath + QString(".temp");
    //
    // delete existing
    //
    if ( QFile::exists(temporaryPath) ) {
        qDebug() << "Downloader::download : removing existing temporary archive : " << temporaryPath;
        QFile::remove(temporaryPath);
    }
    //
    // download new
    //
    qDebug() << "Downloader::download : requesting file from : " << url;
    QNetworkReply *reply = m_manager.get(QNetworkRequest(QUrl(url)));
    //
    //
    //
    connect(reply, &QNetworkReply::readyRead,[=]() {
        QFile file( temporaryPath );
        if ( file.open(QFile::WriteOnly|QFile::Append) ) {
            QByteArray data = reply->readAll();
            file.write(data);
            file.close();
        }
    } );
    connect( reply, &QNetworkReply::downloadProgress,[=](qint64 bytesReceived, qint64 bytesTotal){
        qDebug() << "Downloader::download : QNetworkReply::downloadProgress : " << bytesReceived << " of " << bytesTotal;
        emit progress(url,filepath,bytesTotal,bytesReceived, message);
    });
    connect( reply, static_cast<void (QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error),[=](QNetworkReply::NetworkError errorNo ) {
        if ( QFile::exists(temporaryPath) ) {
            QFile::remove(temporaryPath);
            emit error( url, filepath, message );
            qDebug() << "Downloader::download : QNetworkReply::error : " << errorNo;
        }
    });
    connect(reply, &QNetworkReply::sslErrors,[=](const QList<QSslError>& /*errors*/) {
        if ( QFile::exists(temporaryPath) ) {
            QFile::remove(temporaryPath);
            QString message = "ssl error";
            emit error( url, filepath, message );
        }
    });
    connect(reply, &QNetworkReply::finished,[=]() {
        if ( QFile::exists(temporaryPath) ) {
            SystemUtils::shared().moveFile(temporaryPath,filepath,true);
            emit done( url, filepath );
            qDebug() << "Downloader::download : QNetworkReply::finished";
        } else {
            QString message = "file did not download";
            emit error( url, filepath, message );
            qDebug() << "Downloader::download : QNetworkReply::finished : " << message;

        }
        reply->deleteLater();
    });
}


