#ifndef NATIVEROTATIONSENSOR_H
#define NATIVEROTATIONSENSOR_H

#include <array>

class NativeRotationSensor {
public:
    static void open();
    static void start();
    static void stop();
    static std::array<float, 16> getRotationMatrix();
    static std::array<float, 4> getOrientation();
    static void setMapping( const int x, const int y );
};

#endif // NATIVEROTATIONSENSOR_H
