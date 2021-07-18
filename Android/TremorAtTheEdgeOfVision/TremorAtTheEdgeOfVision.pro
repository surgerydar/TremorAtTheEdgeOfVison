QT += quick multimedia androidextras network positioning

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    audiochannel.h \
    audioplayer.h \
    downloader.h \
    kalmanfilter.h \
    locationmanager.h \
    mathutils.h \
    motionmanager.h \
    nativerotationsensor.h \
    nativetexture.h \
    sitelist.h \
    siteproximitymanager.h \
    skybox.h \
    systemutils.h \
    texturevideobuffer.h \
    videosource.h \
    videosurface.h \
    videotester.h \
    videotexture.h \
    vrvideo.h

SOURCES += \
        audiochannel.cpp \
        audioplayer.cpp \
        downloader.cpp \
        kalmanfilter.cpp \
        locationmanager.cpp \
        main.cpp \
        mathutils.cpp \
        motionmanager.cpp \
        nativerotationsensor.cpp \
        nativetexture.cpp \
        sitelist.cpp \
        siteproximitymanager.cpp \
        skybox.cpp \
        systemutils.cpp \
        texturevideobuffer.cpp \
        videosource.cpp \
        videosurface.cpp \
        videotester.cpp \
        videotexture.cpp \
        vrvideo.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

android {

    INCLUDEPATH += $$PWD/libs/arcore_native

    contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
        ANDROID_VERSION_CODE = 12
        # ANDROID_EXTRA_LIBS += $$PWD/libs/openssl/arm7/libcrypto.so
        # ANDROID_EXTRA_LIBS += $$PWD/libs/openssl/arm7/libssl.so
    }
    contains(ANDROID_TARGET_ARCH,arm64-v8a) {
        ANDROID_VERSION_CODE = 13
        # ANDROID_EXTRA_LIBS += $$PWD/libs/openssl/aarch64/libcrypto.so
        # ANDROID_EXTRA_LIBS += $$PWD/libs/openssl/aarch64/libssl.so
    }
    message (ANDROID_TARGET_ARCH:$$ANDROID_TARGET_ARCH)
    message (ANDROID_VERSION_CODE:$$ANDROID_VERSION_CODE)

    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android

    DISTFILES += \
        android/AndroidManifest.xml \
        android/build.gradle \
        android/gradle.properties \
        android/gradle/wrapper/gradle-wrapper.jar \
        android/gradle/wrapper/gradle-wrapper.properties \
        android/gradlew \
        android/gradlew.bat \
        android/res/values/libs.xml \
        android/src/uk/co/soda/TremorAtTheEdge/RotationSensor.java

    include(/Users/jons/Library/Android/sdk/android_openssl/openssl.pri)
}

DISTFILES += \
    android/src/uk/co/soda/TremorAtTheEdge/PowerManagement.java
