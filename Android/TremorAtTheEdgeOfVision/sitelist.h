#ifndef SITELIST_H
#define SITELIST_H

#include <QObject>
#include <QtPositioning/QGeoCoordinate>
#include <QVariantMap>
#include <QList>
#include <QStringList>

class SiteList : public QObject
{
    Q_OBJECT
public:
    //
    //
    //
    static SiteList& shared() {
        static SiteList instance;
        return instance;
    }
    struct Site {
        QString         id;
        QGeoCoordinate  coordinate;
        QString         notes;
        QString         sequence;
        QString         outro;
        qreal           horizontalOffset;
    };
    //
    //
    //
    QList<Site> m_siteList;

public slots:
    int siteCount() const;
    QVariantMap getDisplaySite(const int index) const;
    QStringList getFileList() const;
    void addSiteAtCurrentLocation();
signals:

private:
    //
    // singleton
    //
    explicit SiteList(QObject *parent = nullptr);
    ~SiteList();
    SiteList(const SiteList&) = delete;
    SiteList& operator=(const SiteList&) = delete;
    SiteList(SiteList&&) = delete;
    SiteList& operator=(SiteList&&) = delete;
};

#endif // SITELIST_H
