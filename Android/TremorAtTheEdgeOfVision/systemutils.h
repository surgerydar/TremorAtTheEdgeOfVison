#ifndef SYSTEMUTILS_H
#define SYSTEMUTILS_H

#include <QObject>
#include <QThread>


class SystemUtils : public QObject
{
    Q_OBJECT
public:
    //
    //
    //
    static SystemUtils& shared() {
        static SystemUtils instance;
        return instance;
    }

private:
    //
    // singleton
    //
    explicit SystemUtils(QObject *parent = nullptr);
    ~SystemUtils() = default;
    SystemUtils(const SystemUtils&) = delete;
    SystemUtils& operator=(const SystemUtils&) = delete;
    SystemUtils(SystemUtils&&) = delete;
    SystemUtils& operator=(SystemUtils&&) = delete;

public slots:
    bool isInstalled();
    void setInstalled(const bool installed=true);
    //
    //
    //
    QString applicationDirectory();
    QString documentDirectory();
    QString temporaryDirectory();
    //
    //
    //
    QString documentPath( const QString& filename );
    QString mediaPath( const QString& filename );
    QString toLocalPath( const QString& path );
    QString temporaryPath( const QString& filename );
    //
    //
    //
    bool fileExists( const QString& path );
    bool copyFile( const QString& from, const QString& to, bool force = false );
    bool moveFile( const QString& from, const QString& to, bool force = false );
    bool removeFile( const QString& path );
    QString readFile( const QString& path );
    //
    //
    //
    QString urlFilename( const QUrl& url );
    QString urlPath( const QUrl& url );
    QString urlHost( const QUrl& url );
    QString urlProtocol( const QUrl& url );
    //
    //
    //
    QString copyImageFromGallery( const QString& url );
    //
    //
    //
    QString mimeTypeForFile( QString filename );
    //
    //
    //
    qreal pointToPixel( qreal point );
    qreal pixelToPoint( qreal pixel );
    //
    //
    //
    int textHeight( const QString &text, const QFont &font, const int maxWidth );
    //
    //
    //
    bool rotateImage( const QString& path, const qreal& rotation );
    bool scaleImage( const QString& path, const qreal& hScale, const qreal& vScale );
    //
    //
    //
    int maxTextureSize();
    //
    //
    //
    void preventDeviceSleep();
    void allowDeviceSleep();
};

#endif // SYSTEMUTILS_H
