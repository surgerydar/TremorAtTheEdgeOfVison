#include "videosource.h"

VideoSource::VideoSource(QObject *parent) : QAbstractVideoSurface(parent), m_mediaPlayer(this,QMediaPlayer::VideoSurface) {
    m_width = m_height = -1;
    m_format = QVideoFrame::Format_Invalid;
    m_dataSize = 0;
    m_data = nullptr;

    m_mediaPlayer.setVideoOutput((QAbstractVideoSurface*)this);
    connect(&m_mediaPlayer,&QMediaPlayer::mediaStatusChanged,[=](QMediaPlayer::MediaStatus status ){
        qDebug() << "VideoSource : mediaStatusChanged=" << status;
        if ( status == QMediaPlayer::LoadedMedia ) { // TODO: should be controlled by autoplay flag
            play();
        }
        if ( m_mediaPlayer.error() ) {
            qDebug() << m_mediaPlayer.errorString();
        }
    });
    connect(&m_mediaPlayer,&QMediaPlayer::stateChanged,[=](QMediaPlayer::State state ){
        qDebug() << "VideoSource : stateChanged=" << state;
        /*
        switch( state ) {
        case QMediaPlayer::PlayingState :
            emit playing();
            break;
        case QMediaPlayer::PausedState :
            emit paused();
            break;
        case QMediaPlayer::StoppedState :
            emit stopped();
        }
        */
    });
}
VideoSource::~VideoSource() {
    qDebug() << "VideoSource::~VideoSource";
    m_mediaPlayer.stop();
}

QList<QVideoFrame::PixelFormat> VideoSource::supportedPixelFormats( QAbstractVideoBuffer::HandleType /*type*/) const {
    qDebug() << "VideoSource::supportedPixelFormats";
    return QList<QVideoFrame::PixelFormat>() << QVideoFrame::Format_ARGB32;
}

bool VideoSource::present(const QVideoFrame&frame) {
    qDebug() << "VideoSource::present";
    QVideoFrame _frame(frame);
    if ( _frame.map(QAbstractVideoBuffer::ReadOnly) ) {
        qDebug() << "VideoSource::present : width=" << _frame.width() << " height=" << _frame.height() << " format=" << _frame.pixelFormat();
        if ( m_bufferLock.tryLock() ) {
            m_data = (uchar*)realloc(m_data,_frame.mappedBytes());
            if (m_data != nullptr) {
                m_width = _frame.width();
                m_height = _frame.height();
                m_format = _frame.pixelFormat();
                m_dataSize = _frame.mappedBytes();
                memcpy(m_data,_frame.bits(),_frame.mappedBytes());
            } else {
                qDebug() << "VideoSource::present :unable to allocate buffer " << _frame.mappedBytes();
            }
            m_bufferLock.unlock();
        } else {
            qDebug() << "VideoSource::present :unable to lock buffer";
        }
        _frame.unmap();
    } else {
        qDebug() << "VideoSource::present :unable to map buffer";
    }
    return true;
}

void VideoSource::open(const QUrl& url) {
    m_mediaPlayer.setMedia(url);
}
void VideoSource::close() {
    m_mediaPlayer.setMedia(nullptr);
}
void VideoSource::play() {
    m_mediaPlayer.play();
}
void VideoSource::pause() {
    m_mediaPlayer.pause();
}
bool VideoSource::readFrame(int& width, int& height, QVideoFrame::PixelFormat& format, uchar** data) {
    if ( m_data != nullptr ) {
        *data = (uchar*)malloc(m_dataSize);
        if ( *data != nullptr ) {
            width = m_width;
            height = m_height;
            format = m_format;
            memcpy(*data,m_data,m_dataSize);
            return true;
        }
    }
    return false;
}
uchar* VideoSource::getData(int& width, int& height, QVideoFrame::PixelFormat& format) {
    width = m_width;
    height = m_height;
    format = m_format;
    return m_data;
}

