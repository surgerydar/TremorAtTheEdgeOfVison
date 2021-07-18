#include "sitelist.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include <QDebug>

SiteList::SiteList(QObject *parent) : QObject(parent) {
    QFile sitelist(":/data/sitelist.json");
    if ( sitelist.open(QIODevice::ReadOnly) ) {
        auto data = sitelist.readAll();
        QJsonParseError error;
        QJsonDocument document = QJsonDocument::fromJson(data,&error);
        if ( !document.isNull() ) {
            auto array = document.array();
            for ( auto site : array ) {
                QJsonObject siteObject = site.toObject();
                Site _site;
                _site.id = siteObject["id"].toString();
                auto coordinate = siteObject["coordinate"].toArray();
                auto latitude = coordinate[1].toDouble();
                auto longitude = coordinate[0].toDouble();
                _site.coordinate = QGeoCoordinate(latitude,longitude);
                _site.notes = siteObject["notes"].toString();
                _site.sequence = siteObject["sequence"].toString();
                _site.horizontalOffset = siteObject["horizontalOffset"].toDouble();
                _site.outro = siteObject["outro"].toString();
                m_siteList.append(_site);
            }
        } else {
            qDebug() << "SiteList::SiteList : error parsing json : " << error.errorString();
        }
    }
}

SiteList::~SiteList() {

}

int SiteList::siteCount() const {
    return m_siteList.count();
}
QVariantMap SiteList::getDisplaySite(const int index) const {
    QVariantMap site;
    if ( index >= 0 && index < m_siteList.count() ) {
        site["id"] = m_siteList[index].id;
        site["notes"] = m_siteList[index].notes;
        site["sequence"] = m_siteList[index].sequence;
        site["horizontalOffset"] = m_siteList[index].horizontalOffset;
    }
    return site;
}
QStringList SiteList::getFileList() const {
    QStringList fileList;
    for( auto& site : m_siteList ) {
        fileList.append(site.sequence);
    }
    return fileList;
}

#include "locationmanager.h"
#include <QUuid>
int siteNo = 1;

void SiteList::addSiteAtCurrentLocation() {
    Site site;
    site.id = QUuid::createUuid().toString();
    site.coordinate = LocationManager::shared().location();
    site.notes = QString("Test site %1").arg(siteNo);
    site.sequence = QString("Scene%1.mp4").arg(siteNo);
    site.outro = siteNo == 7 ? "Scene_07_Guitar_walk_home_AAC.m4a" : "In_Blue_background_track_AAC.m4a";
    siteNo++;
    if ( siteNo > 7 ) siteNo = 1;

    qDebug() << "SiteList::addSiteAtCurrentLocation : " << site.sequence;
    m_siteList.append(site);
}


