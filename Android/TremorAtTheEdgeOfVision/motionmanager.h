#ifndef MOTIONMANAGER_H
#define MOTIONMANAGER_H

#include <QObject>
#include <QMatrix4x4>
#include "mathutils.h"

#include "nativerotationsensor.h"

class MotionManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(qreal xRotation READ xRotation WRITE setxRotation NOTIFY xRotationChanged)
    Q_PROPERTY(qreal yRotation READ yRotation WRITE setyRotation NOTIFY yRotationChanged)
    Q_PROPERTY(qreal zRotation READ zRotation WRITE setzRotation NOTIFY zRotationChanged)

public:
    //
    //
    //
    static MotionManager& shared() {
        static MotionManager instance;
        return instance;
    }
    //
    //
    //
    qreal xRotation() const {
        return _xRotation;
    }
    void setxRotation( const qreal rotation) {
        _xRotation = rotation;
    }
    qreal yRotation() const {
        return _yRotation;
    }
    void setyRotation( const qreal rotation) {
        _yRotation = rotation;
    }
    qreal zRotation() const {
        return _zRotation;
    }
    void setzRotation( const qreal rotation) {
        _zRotation = rotation;
    }
signals:
    void xRotationChanged(qreal x);
    void yRotationChanged(qreal y);
    void zRotationChanged(qreal x);

public slots:
    //
    //
    //
    void start();
    void stop();
    Matrix4x4 getRotationMatrix();
    std::array<float, 4> getOrientation();
    //
    //
    //
    void setMapping(const int x, const int y ) {
        NativeRotationSensor::setMapping(x,y);
    }

private:
    //
    // singleton
    //
    explicit MotionManager(QObject *parent = nullptr);
    ~MotionManager();
    MotionManager(const MotionManager&) = delete;
    MotionManager& operator=(const MotionManager&) = delete;
    MotionManager(MotionManager&&) = delete;
    MotionManager& operator=(MotionManager&&) = delete;
    //
    //
    //
    qreal _xRotation = 0.0;
    qreal _yRotation = 0.0;
    qreal _zRotation = 0.0;
};

#endif // MOTIONMANAGER_H
