#ifndef KALMANFILTER_H
#define KALMANFILTER_H

#include <QObject>
#include <QGeoPositionInfo>

class KalmanFilter : public QObject
{
    Q_OBJECT
public:
    explicit KalmanFilter(QObject *parent = nullptr);
    //
    //
    //
    QGeoPositionInfo process(QGeoPositionInfo& measurement);

signals:

private:
    const qreal epsilon = 0.000001;
    qreal m_timeUncertainty = 1.4;
    QGeoPositionInfo m_locationEstimate;
    qreal variance = 0.0;
    //
    //
    //
    double uncertainty() {
        return sqrt(variance);
    }
    QGeoPositionInfo initialEstimate(QGeoPositionInfo& measurement);
    QGeoPositionInfo predict(QDateTime time);
    QGeoPositionInfo correct(QGeoPositionInfo& measurement, QGeoPositionInfo& estimate);

};

#endif // KALMANFILTER_H
