#ifndef TEXTUREVIDEOBUFFER_H
#define TEXTUREVIDEOBUFFER_H

#include <QAbstractVideoSurface>
#include <QVideoFrame>

class TextureVideoBuffer : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    explicit TextureVideoBuffer(QObject *parent = nullptr);
    //
    //
    //
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType type = QAbstractVideoBuffer::NoHandle) const override;
    bool present(const QVideoFrame &frame) override;

signals:
    void dataAvailable();

private:
    int                         m_width;
    int                         m_height;
    QVideoFrame::PixelFormat    m_format;
    uchar*                      m_data;

};

#endif // TEXTUREVIDEOBUFFER_H
