#include "motionmanager.h"
#include <QDebug>
#include "mathutils.h"

MotionManager::MotionManager(QObject *parent) : QObject(parent) {
    NativeRotationSensor::open();
}
MotionManager::~MotionManager() {
}
void MotionManager::start() {
    NativeRotationSensor::start();
}
void MotionManager::stop() {
    NativeRotationSensor::stop();
}
Matrix4x4 MotionManager::getRotationMatrix() {
    std::array<float,16> matrix = NativeRotationSensor::getRotationMatrix();
    return GetMatrixFromGlArray(matrix.data());
}
std::array<float, 4> MotionManager::getOrientation() {
    return NativeRotationSensor::getOrientation();
}

