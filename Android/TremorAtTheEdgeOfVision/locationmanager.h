#ifndef LOCATIONMANAGER_H
#define LOCATIONMANAGER_H

#include <QObject>
#include <QtPositioning/QGeoPositionInfoSource>
#include "kalmanfilter.h"

class LocationManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool permissionRequired READ permissionRequired NOTIFY permissionRequiredChanged )
    Q_PROPERTY(bool tracking READ tracking NOTIFY trackingChanged )
    Q_PROPERTY(QGeoCoordinate location READ location NOTIFY locationChanged)
public:
    //
    //
    //
    static LocationManager& shared() {
        static LocationManager instance;
        return instance;
    }
    bool permissionRequired() const {
        return m_permissionRequired;
    }
    bool tracking() const {
        return m_tracking;
    }
    QGeoCoordinate location() const {
        return m_location;
    }
    bool denied() const {
        return m_denied;
    }
public slots:
    void start();
    void stop();
    void requestPermissions();
    qreal distanceTo(QGeoCoordinate& location);
    qreal filteredDistanceTo(QGeoCoordinate& location);
private slots:
    void error(QGeoPositionInfoSource::Error positioningError);
    void positionUpdated(const QGeoPositionInfo &update);
    void updateTimeout();

signals:
    void permissionRequiredChanged(const bool permissionChanged);
    void trackingChanged(bool tracking);
    void locationChanged(QGeoCoordinate& location);
    void deniedChanged(const bool denied);
private:
    //
    // singleton
    //
    explicit LocationManager(QObject *parent = nullptr);
    ~LocationManager();
    LocationManager(const LocationManager&) = delete;
    LocationManager& operator=(const LocationManager&) = delete;
    LocationManager(LocationManager&&) = delete;
    LocationManager& operator=(LocationManager&&) = delete;
    //
    //
    //
    bool setup();
    //
    //
    //
    QGeoPositionInfoSource* m_source;
    QGeoCoordinate          m_location;
    QGeoCoordinate          m_filteredLocation;
    bool                    m_permissionRequired;
    bool                    m_tracking = false;
    bool                    m_denied = false;
    KalmanFilter            m_kalman;
};

#endif // LOCATIONMANAGER_H
