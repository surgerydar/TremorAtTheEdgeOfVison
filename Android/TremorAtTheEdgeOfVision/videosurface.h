#ifndef VIDEOSURFACE_H
#define VIDEOSURFACE_H

#include <QObject>
#include <QAbstractVideoSurface>
#include <QMutex>
#include <QDebug>

class VideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    explicit VideoSurface(QObject *parent = nullptr);
    ~VideoSurface();
    //
    //
    //
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType type = QAbstractVideoBuffer::NoHandle) const override;
    bool present(const QVideoFrame &frame) override;
    bool start(const QVideoSurfaceFormat &format) override {
        qDebug() << "VideoSurface::start";
        return QAbstractVideoSurface::start(format);
    }
    void stop() override {
        qDebug() << "VideoSurface::stop";
        QAbstractVideoSurface::stop();
    }
    //
    //
    //
    void lock() { m_bufferLock.lock(); }
    void unlock() { m_bufferLock.unlock(); }
    uchar* getData(int& width, int& height, QVideoFrame::PixelFormat& format);

private:
    QMutex                      m_bufferLock;
    int                         m_width;
    int                         m_height;
    QVideoFrame::PixelFormat    m_format;
    size_t                      m_dataSize;
    uchar*                      m_data;
};

#endif // VIDEOSURFACE_H
