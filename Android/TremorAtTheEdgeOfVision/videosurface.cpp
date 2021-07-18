#include "videosurface.h"

VideoSurface::VideoSurface(QObject *parent) : QAbstractVideoSurface(parent) {
    m_width = m_height = -1;
    m_format = QVideoFrame::Format_Invalid;
    m_dataSize = 0;
    m_data = nullptr;
}
VideoSurface::~VideoSurface() {
    qDebug() << "VideoSurface::~VideoSurface";
}

QList<QVideoFrame::PixelFormat> VideoSurface::supportedPixelFormats( QAbstractVideoBuffer::HandleType /*type*/) const {
    //qDebug() << "VideoSurface::supportedPixelFormats";
    return QList<QVideoFrame::PixelFormat>() << QVideoFrame::Format_ARGB32;
}

bool VideoSurface::present(const QVideoFrame&frame) {
    //qDebug() << "VideoSurface::present";
    QVideoFrame _frame(frame);
    if ( _frame.map(QAbstractVideoBuffer::ReadOnly) ) {
        //qDebug() << "VideoSurface::present : width=" << _frame.width() << " height=" << _frame.height() << " format=" << _frame.pixelFormat();
        if ( m_bufferLock.tryLock() ) {
            m_data = (uchar*)realloc(m_data,_frame.mappedBytes());
            if (m_data != nullptr) {
                m_width = _frame.width();
                m_height = _frame.height();
                m_format = _frame.pixelFormat();
                m_dataSize = _frame.mappedBytes();
                memcpy(m_data,_frame.bits(),_frame.mappedBytes());
            } else {
                qDebug() << "VideoSurface::present :unable to allocate buffer " << _frame.mappedBytes();
            }
            m_bufferLock.unlock();
        } else {
            qDebug() << "VideoSurface::present :unable to lock buffer";
        }
        _frame.unmap();
    } else {
        qDebug() << "VideoSurface::present :unable to map buffer";
    }
    return true;
}

uchar* VideoSurface::getData(int& width, int& height, QVideoFrame::PixelFormat& format) {
    width = m_width;
    height = m_height;
    format = m_format;
    return m_data;
}

