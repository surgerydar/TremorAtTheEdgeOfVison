#ifndef VIDEOSOURCE_H
#define VIDEOSOURCE_H

#include <QObject>
#include <QMutex>
#include <QAbstractVideoSurface>
#include <QMediaPlayer>
#include <QDebug>

class VideoSource : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    explicit VideoSource(QObject *parent = nullptr);
    ~VideoSource();
    //
    //
    //
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType type = QAbstractVideoBuffer::NoHandle) const override;
    bool present(const QVideoFrame &frame) override;
    bool start(const QVideoSurfaceFormat &format) override {
        qDebug() << "VideoSource::start";
        return QAbstractVideoSurface::start(format);
    }
    void stop() override {
        qDebug() << "VideoSource::stop";
        QAbstractVideoSurface::stop();
    }
/*
signals:
    void playing();
    void paused();
    void stopped();
*/
public slots:
    void open(const QUrl& url);
    void close();
    void play();
    void pause();

    void lock() { m_bufferLock.lock(); }
    void unlock() { m_bufferLock.unlock(); }
    bool readFrame(int& width, int& height, QVideoFrame::PixelFormat& format, uchar** data);

    uchar* getData(int& width, int& height, QVideoFrame::PixelFormat& format);

private:
    QMutex                      m_bufferLock;
    int                         m_width;
    int                         m_height;
    QVideoFrame::PixelFormat    m_format;
    size_t                      m_dataSize;
    uchar*                      m_data;
    QMediaPlayer                m_mediaPlayer;
};

#endif // VIDEOSOURCE_H
