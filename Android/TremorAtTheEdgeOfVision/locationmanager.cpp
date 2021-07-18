#include "locationmanager.h"
#include <QtAndroid>
#include <QDebug>

//#define TEST_LOCATION 1

#ifdef TEST_LOCATION
#include "sitelist.h"

int test_site_index = 0;
QGeoCoordinate test_location;

#endif

LocationManager::LocationManager(QObject *parent) : QObject(parent) {
    m_tracking = false;
    m_permissionRequired = true;

#ifdef TEST_LOCATION
    double count = 0.0;
    test_location = QGeoCoordinate(0.0,0.0);
    for ( auto& site : SiteList::shared().m_siteList ) {
        test_location.setLatitude( test_location.latitude() + site.coordinate.latitude() );
        test_location.setLongitude( test_location.longitude() + site.coordinate.longitude() );
        count++;
    }
    test_location.setLatitude( test_location.latitude() / count );
    test_location.setLongitude( test_location.longitude() / count );
#endif

}

LocationManager::~LocationManager() {

}
void LocationManager::start() {
    //
    // set up location services
    //
    if ( setup() ) {
        m_source->startUpdates();
        m_tracking = true;
    } else {
        m_tracking = false;
    }
    emit trackingChanged(m_tracking);
}

void LocationManager::stop() {
    if ( m_source ) {
        m_source->stopUpdates();
    }
    m_tracking = false;
    emit trackingChanged(m_tracking);
}

bool LocationManager::setup() {
    if ( !m_source ) {
        m_permissionRequired = false;
        m_source = QGeoPositionInfoSource::createDefaultSource(this);
        if ( m_source ) {
            qDebug() << "LocationManager::setup : position source created";
            //qDebug() << "minimum update interval : " << m_source->minimumUpdateInterval();
            //m_source->setUpdateInterval(3000);
            m_source->setUpdateInterval( m_source->minimumUpdateInterval() );
            //
            //
            //
            connect( m_source, static_cast<void(QGeoPositionInfoSource::*)(QGeoPositionInfoSource::Error)>(&QGeoPositionInfoSource::error), this, &LocationManager::error);
            connect( m_source, &QGeoPositionInfoSource::positionUpdated, this, &LocationManager::positionUpdated);
            connect( m_source, &QGeoPositionInfoSource::updateTimeout, this, &LocationManager::updateTimeout);
            emit permissionRequiredChanged(m_permissionRequired);
            return true;
        } else {
            qDebug() << "LocationManager::setup : unable to create position source";
            m_permissionRequired = true;
            emit permissionRequiredChanged(m_permissionRequired);
        }
        return false;
    }
    return true;
}

void LocationManager::requestPermissions() {
    QStringList permissions;
    if ( QtAndroid::checkPermission("android.permission.ACCESS_FINE_LOCATION") != QtAndroid::PermissionResult::Granted ) {
        permissions.push_back("android.permission.ACCESS_FINE_LOCATION");
    }
    if ( permissions.size() > 0 ) {
        qDebug() << "requesting permissions : " << permissions;
        QtAndroid::requestPermissions(permissions,[](const QtAndroid::PermissionResultMap & results) {
            for ( auto& result : results ) {
                qDebug() << "permissions results : " << (int) result;
            }
        });
    }
}

void LocationManager::error(QGeoPositionInfoSource::Error positioningError) {
    QString error = "unknown";
    switch ( positioningError ) {
    case QGeoPositionInfoSource::Error::AccessError :
        error = "AccessError";
        m_permissionRequired = true;
        emit permissionRequiredChanged( m_permissionRequired );
        break;
    case QGeoPositionInfoSource::Error::ClosedError :
        error = "ClosedError";
        break;
    case QGeoPositionInfoSource::Error::NoError :
        return;
    case QGeoPositionInfoSource::Error::UnknownSourceError :
        error = "UnknownSourceError";
        break;
    }
    qDebug() << "LocationManager::error : " << error;
}
void LocationManager::positionUpdated(const QGeoPositionInfo &update) {
    m_location = update.coordinate();
    QGeoPositionInfo filteredUpdate = update;
    m_filteredLocation = m_kalman.process(filteredUpdate).coordinate();
    //qDebug() << "LocationManager::positionUpdated : raw " << update;
    //qDebug() << "LocationManager::positionUpdated : filtered " << m_filteredLocation;
#ifdef TEST_LOCATION
    qreal distance = test_location.distanceTo(SiteList::shared().m_siteList[test_site_index].coordinate);
    int previous_index = test_site_index==0?6:test_site_index-1;
    qreal previous_distance = test_location.distanceTo(SiteList::shared().m_siteList[previous_index].coordinate);
    qDebug() << "LocationManager::positionUpdated : distance to target " << test_site_index << " = " << distance << " distance to previous " << previous_index << " = " << previous_distance;
    if ( distance < 2.5 /*5.0*/ ) { // activation distance
        test_site_index++;
        if ( test_site_index >= SiteList::shared().m_siteList.count() ) {
            test_site_index = 0;
        }
        return;
    }
    QGeoCoordinate vector = QGeoCoordinate(
                SiteList::shared().m_siteList[test_site_index].coordinate.latitude() - test_location.latitude(),
                SiteList::shared().m_siteList[test_site_index].coordinate.longitude() - test_location.longitude() );
    vector.setLatitude( vector.latitude() / distance );
    vector.setLongitude( vector.longitude() / distance );
    //
    // slow down in range
    //
    qreal scale = 1.0;
    if ( previous_distance < 5.0 ) {
        scale = 0.1;
    }
    test_location.setLatitude( test_location.latitude() + vector.latitude() * scale );
    test_location.setLongitude( test_location.longitude() + vector.longitude() * scale );
    emit locationChanged( test_location );
#else
    emit locationChanged( m_location );
#endif
}
void LocationManager::updateTimeout() {
    qDebug() << "LocationManager::updateTimeout";
}
qreal LocationManager::distanceTo(QGeoCoordinate& location) {
#ifdef TEST_LOCATION
    return test_location.distanceTo(location);
 #else
    if ( m_location.isValid() ) {
        return m_location.distanceTo(location);
    }
    return -1.0;
#endif
}
qreal LocationManager::filteredDistanceTo(QGeoCoordinate& location) {
#ifdef TEST_LOCATION
    return distanceTo(location);
#else
    if ( m_filteredLocation.isValid() ) {
        return m_filteredLocation.distanceTo(location);
    }
    return -1.0;
#endif
}




