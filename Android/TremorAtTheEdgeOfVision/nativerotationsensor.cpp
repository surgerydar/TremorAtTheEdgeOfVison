#include "nativerotationsensor.h"
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>

void NativeRotationSensor::open() {
    QAndroidJniObject::callStaticMethod<void>("uk/co/soda/TremorAtTheEdge/RotationSensor", "open");
}
void NativeRotationSensor::start() {
    QAndroidJniObject::callStaticMethod<void>("uk/co/soda/TremorAtTheEdge/RotationSensor", "start");
}
void NativeRotationSensor::stop() {
    QAndroidJniObject::callStaticMethod<void>("uk/co/soda/TremorAtTheEdge/RotationSensor", "stop");
}
std::array<float, 16> NativeRotationSensor::getRotationMatrix() {
    std::array<float, 16> matrix;

    QAndroidJniObject object = QAndroidJniObject::callStaticObjectMethod("uk/co/soda/TremorAtTheEdge/RotationSensor", "getRotationMatrix","()[F");
    if ( object.isValid() ) {
        jfloatArray array = object.object<jfloatArray>();
        QAndroidJniEnvironment env;
        jfloat* value = env->GetFloatArrayElements(array,0);
        for ( int i = 0; i < 16; ++i ) {
            matrix[i] = value[i];
        }
    }

    return matrix;
}

std::array<float, 4> NativeRotationSensor::getOrientation() {
    std::array<float, 4> matrix;

    QAndroidJniObject object = QAndroidJniObject::callStaticObjectMethod("uk/co/soda/TremorAtTheEdge/RotationSensor", "getOrientation","()[F");
    if ( object.isValid() ) {
        jfloatArray array = object.object<jfloatArray>();
        QAndroidJniEnvironment env;
        jfloat* value = env->GetFloatArrayElements(array,0);
        for ( int i = 0; i < 4; ++i ) {
            matrix[i] = value[i];
        }
    }
    return matrix;
}

void NativeRotationSensor::setMapping( const int x, const int y ) {
    jint mappingX = x;
    jint mappingY = y;
    QAndroidJniObject::callStaticMethod<void>("uk/co/soda/TremorAtTheEdge/RotationSensor", "setMapping","(II)V",mappingX,mappingY);
}

