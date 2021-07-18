#include "kalmanfilter.h"
#include <QDebug>

KalmanFilter::KalmanFilter(QObject *parent) : QObject(parent) {

}
QGeoPositionInfo KalmanFilter::process(QGeoPositionInfo& measurement) {
    QGeoPositionInfo updatedEstimate = m_locationEstimate.isValid() ? predict(measurement.timestamp()) : initialEstimate(measurement);
    if ( measurement.coordinate().isValid() && m_locationEstimate.isValid() ) {
        updatedEstimate = correct(measurement,m_locationEstimate);
    }
    m_locationEstimate = updatedEstimate;
    return updatedEstimate;
}
QGeoPositionInfo KalmanFilter::initialEstimate(QGeoPositionInfo& measurement) {
    variance = pow(measurement.attribute(QGeoPositionInfo::HorizontalAccuracy), 2);
    return measurement;
}
QGeoPositionInfo KalmanFilter::predict(QDateTime time) {
    //qDebug() << "KalmanFilter::predict";
    qreal timeDelta = time.toSecsSinceEpoch() - m_locationEstimate.timestamp().toSecsSinceEpoch();
    if ( timeDelta > 0 ) {
        variance = timeDelta * pow(m_timeUncertainty,2);
    }
    QGeoPositionInfo predicted = m_locationEstimate;
    predicted.setAttribute(QGeoPositionInfo::HorizontalAccuracy,uncertainty());
    predicted.setTimestamp(time);
    return predicted;
}
QGeoPositionInfo KalmanFilter::correct(QGeoPositionInfo& measurement, QGeoPositionInfo& estimate) {
    qreal gain = variance / (variance + pow(measurement.attribute(QGeoPositionInfo::HorizontalAccuracy), 2) + epsilon);

    variance = (1 - gain) * variance;

    qreal latitude = estimate.coordinate().latitude() + gain * (measurement.coordinate().latitude() - estimate.coordinate().latitude());
    qreal longitude = estimate.coordinate().longitude() + gain * (measurement.coordinate().longitude() - estimate.coordinate().longitude());

    QGeoPositionInfo corrected = m_locationEstimate;
    corrected.setCoordinate(QGeoCoordinate(latitude,longitude));
    return corrected;
}
