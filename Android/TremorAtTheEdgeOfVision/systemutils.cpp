#include <QStandardPaths>
#include <QFile>
#include <QDir>
#include <QCoreApplication>
#include <QDebug>
#include <QMimeDatabase>
#include <QMimeType>
#include <QGuiApplication>
#include <QScreen>
#include <QFontMetrics>
#include <QImage>
#include <QMatrix>
#include <QUrl>
#include <QUuid>
#include <QOpenGLTexture>

#ifdef Q_OS_ANDROID
#include <QtAndroidExtras>
#include <QtAndroidExtras/QAndroidJniObject>
#include <jni.h>
#include <QSettings>

QString _getAndroidPath(const QString path) {
    QAndroidJniObject jniPath = QAndroidJniObject::fromString(path);
    QAndroidJniObject uri = QAndroidJniObject::callStaticObjectMethod("uk/co/soda/QSharePathResolver", "getRealPathFromURI", "(Ljava/lang/String;)Landroid/net/Uri;", jniPath.object<jstring>());

}
#endif
#include "systemutils.h"

SystemUtils::SystemUtils(QObject *parent) : QObject(parent)
{

}
//
//
//
bool SystemUtils::isInstalled() {
    QSettings settings;
    return settings.value("installed", false ).toBool();
}
void SystemUtils::setInstalled(const bool installed) {
    QSettings settings;
    settings.setValue("installed", installed );
}
//
//
//
QString SystemUtils::applicationDirectory() {
    return QCoreApplication::applicationDirPath();
}

QString SystemUtils::documentDirectory() {
    QString documentPath;
#ifdef Q_OS_ANDROID
    documentPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
#else
    documentPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
#endif
    QDir dir(documentPath);
    QString appName = QCoreApplication::applicationName();
    if ( dir.mkpath(appName) ) { // ensure directory exists
        dir.cd(appName);
        if ( !dir.exists("media") ) { // TODO: move this somewhere more sensible
            dir.mkdir("media");
        }
    }
    return dir.canonicalPath();
}

QString SystemUtils::temporaryDirectory() {
    return QDir::tempPath();
}

QString SystemUtils::documentPath( const QString& filename ) {
    return documentDirectory().append("/").append(filename);
}

QString SystemUtils::mediaPath( const QString& filename ) {
    return documentDirectory().append("/media/").append(filename);
}

QString SystemUtils::toLocalPath( const QString& path ) {
    QUrl url( path );
    return url.toLocalFile();
}

QString SystemUtils::temporaryPath( const QString& filename ) {
    return temporaryDirectory().append("/").append(filename);
}
//
//
//
bool SystemUtils::fileExists( const QString& path ) {
    return QFile::exists(path);
}

bool SystemUtils::copyFile( const QString& from, const QString& to, bool force ) {
    if ( force ) {
        if ( QFile::exists(to) ) {
            QFile::remove(to);
        }
    }
    return QFile::copy( from, to );
}
bool SystemUtils::moveFile( const QString& from, const QString& to, bool force ) {
    if ( force ) {
        if ( QFile::exists(to) ) {
            QFile::remove(to);
        }
    }
    return QFile::rename( from, to );
}
bool SystemUtils::removeFile( const QString& path ) {
    if ( QFile::exists(path) ) {
        qDebug() << "SystemUtils::removeFile : " << path;
        if ( QFile::remove(path) ) {
            qDebug() << "SystemUtils::removeFile : file removed";
            return true;
        }
    }
    return false;
}
QString SystemUtils::readFile( const QString& path ) {
    QFile textfile(path);
    if ( textfile.open(QIODevice::ReadOnly) ) {
        auto data = textfile.readAll();
        return QString(data);
    }
    return "";
}
//
//
//
QString SystemUtils::urlFilename( const QUrl& url ) {
    return url.fileName();
}
QString SystemUtils::urlPath( const QUrl& url ) {
    return url.path();
}
QString SystemUtils::urlHost( const QUrl& url ) {
    return url.host();
}
QString SystemUtils::urlProtocol( const QUrl& url ) {
    return url.scheme();
}
//
//
//
#if defined(Q_OS_IOS) || defined(Q_OS_ANDROID)
extern bool _copyImageFromGallery(const QString& source, const QString& target );
#endif
QString SystemUtils::copyImageFromGallery( const QString& url ) {
    QString targetFile = QString("%1.jpg").arg(QUuid::createUuid().toString());
    QString targetPath = documentPath(targetFile);
    qDebug() << "SystemUtils::copyImageFromGallery : " << url << " >> " << targetPath;
#if defined(Q_OS_IOS)
    if( !_copyImageFromGallery(url, targetPath) ) {
        return "";
    }
#elif defined(Q_OS_ANDROID)
    qDebug() << "source : " << url;
    /*
    if( !_copyImageFromGallery(url, targetPath) ) {
        return "";
    }
    */
    return "";
#endif
    return targetPath;
}
//
//
//
QString SystemUtils::mimeTypeForFile( QString filename ) {
    QMimeDatabase mimeDb;
    QMimeType mimeType = mimeDb.mimeTypeForFile(filename);
    return mimeType.name();
}
//
//
//
//
//
//
qreal SystemUtils::pointToPixel( qreal point ) {
    qreal dpi = QGuiApplication::primaryScreen()->physicalDotsPerInch();
    return point/72*dpi;
}

qreal SystemUtils::pixelToPoint( qreal pixel ) {
    qreal dpi = QGuiApplication::primaryScreen()->physicalDotsPerInch();
    return pixel*72/dpi;
}

int SystemUtils::textHeight( const QString &text, const QFont &font, const int maxWidth ) {
    QFontMetrics metrics(font);
    return metrics.boundingRect(0,0,maxWidth,10000, Qt::TextWordWrap, text).height();
}
//
//
//
bool SystemUtils::rotateImage(const QString& path, const qreal& rotation ) {
    QImage image( path );
    if ( !image.isNull() ) {
        QPoint center = image.rect().center();
        QMatrix transform;
        transform.translate(center.x(),center.y());
        transform.rotate(rotation);
        QImage rotated = image.transformed(transform);
        if ( !rotated.isNull() ) {
            return rotated.save(path);
        }
    }
    return false;
}
bool SystemUtils::scaleImage( const QString& path, const qreal& sx, const qreal& sy ) {
    QImage image( path );
    if ( !image.isNull() ) {
        //QPoint center = image.rect().center();
        QMatrix transform;
        transform.scale(sx,sy);
        QImage rotated = image.transformed(transform);
        if ( !rotated.isNull() ) {
            return rotated.save(path);
        }
    }
    return false;
}
//
//
//
int SystemUtils::maxTextureSize() {
    GLint size;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE,&size);
    return int(size);
}
//
//
//
void SystemUtils::preventDeviceSleep() {
#ifdef Q_OS_ANDROID
    qDebug() << "SystemUtils::preventDeviceSleep";
    QAndroidJniObject::callStaticMethod<void>("uk/co/soda/TremorAtTheEdge/PowerManagement", "preventSleep");
#endif
}
void SystemUtils::allowDeviceSleep() {
#ifdef Q_OS_ANDROID
    qDebug() << "SystemUtils::allowDeviceSleep";
    QAndroidJniObject::callStaticMethod<void>("uk/co/soda/TremorAtTheEdge/PowerManagement", "allowSleep");
#endif
}
