#include "texturevideobuffer.h"

TextureVideoBuffer::TextureVideoBuffer(QObject *parent) : QAbstractVideoSurface(parent) {

}
//
//
//
QList<QVideoFrame::PixelFormat> TextureVideoBuffer::supportedPixelFormats(QAbstractVideoBuffer::HandleType type) const {
    if (type == QAbstractVideoBuffer::NoHandle) {
        return QList<QVideoFrame::PixelFormat>() << QVideoFrame::Format_RGB24;
    } else {
        return QList<QVideoFrame::PixelFormat>();
    }
}
bool TextureVideoBuffer::present(const QVideoFrame &frame) {
    QVideoFrame _frame(frame);
    if ( _frame.map(QAbstractVideoBuffer::MapMode::ReadOnly) ) {
        // store data
        _frame.unmap();
    }
    // fire signal

    return true;
}
