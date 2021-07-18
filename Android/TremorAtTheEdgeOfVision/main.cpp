#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QAndroidJniEnvironment>
#include <QtAndroid>
#include "vrvideo.h"
#include "motionmanager.h"
#include "systemutils.h"
#include "downloader.h"
#include "sitelist.h"
#include "locationmanager.h"
#include "siteproximitymanager.h"
#include "audioplayer.h"


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setOrganizationName("soda");
    QCoreApplication::setOrganizationDomain("soda.co.uk");
    QCoreApplication::setApplicationName("TremorOnTheEdge");
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QCoreApplication::setOrganizationName("soda");
    QCoreApplication::setOrganizationDomain("soda.co.uk");
    QCoreApplication::setApplicationName("TremorOnTheEdge");

    qmlRegisterType<VRVideo>("uk.co.soda.VR", 1, 0, "VRVideo");

    engine.rootContext()->setContextProperty("MotionManager", &MotionManager::shared());
    engine.rootContext()->setContextProperty("SystemUtils", &SystemUtils::shared());
    engine.rootContext()->setContextProperty("Downloader", &Downloader::shared());
    engine.rootContext()->setContextProperty("SiteList", &SiteList::shared());
    engine.rootContext()->setContextProperty("LocationManager", &LocationManager::shared());
    engine.rootContext()->setContextProperty("SiteProximityManager", &SiteProximityManager::shared());
    engine.rootContext()->setContextProperty("AudioPlayer", &AudioPlayer::shared());

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
