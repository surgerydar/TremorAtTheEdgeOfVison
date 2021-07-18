#include "videotexture.h"
#include <QVideoFrame>
#include <QDebug>

VideoTexture::VideoTexture(QObject *parent) : QObject(parent), m_player(this,QMediaPlayer::VideoSurface) {
    m_handle = 0;
    //
    //
    //
    connect(&m_surface,&QAbstractVideoSurface::activeChanged,[=](bool active){
        //qDebug() << "VideoTexture : activeChanged=" << active;
    });
    //
    //
    //
    connect(&m_player,&QMediaPlayer::mediaStatusChanged,[=](QMediaPlayer::MediaStatus status ){
        //qDebug() << "VideoTexture : mediaStatusChanged=" << status;
        if ( status == QMediaPlayer::LoadedMedia ) { // TODO: should be controlled by autoplay flag
            play();
        }
        if ( m_player.error() ) {
            qDebug() << m_player.errorString();
        }
    });
    connect(&m_player,&QMediaPlayer::stateChanged,[=](QMediaPlayer::State state ){
        //qDebug() << "VideoTexture : stateChanged=" << state;
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
    });

    m_player.setVideoOutput((QAbstractVideoSurface *)&m_surface);
}
VideoTexture::~VideoTexture() {
    qDebug() << "VideoTexture::~VideoTexture";
}
void VideoTexture::create() {
    glGenTextures(1, &m_handle);
    if ( m_handle > 0 ) {
        bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        unbind();
    }
}

void VideoTexture::bind() {
    if ( m_handle > 0 ) {
        glBindTexture(GL_TEXTURE_2D,m_handle);
    }
}

void VideoTexture::unbind() {
    glBindTexture(GL_TEXTURE_2D,0);
}

void VideoTexture::setData(GLint internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels) {
    if ( m_handle == 0 ) {
        create();
    }
    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, type, pixels);
    unbind();
}
//
//
//
void VideoTexture::open(const QUrl& url) {
    qDebug() << "VideoTexture::open : openning : " << url;
    m_player.setMedia(url);
}
void VideoTexture::close() {
    m_player.setMedia(QMediaContent());
}
//
//
//
void VideoTexture::play() {
    m_player.play();
}
void VideoTexture::pause() {
    //m_player.pause();
    m_player.stop();
}

bool VideoTexture::update() {
    bool success = false;
    int frameWidth = -1;
    int frameHeight = -1;
    QVideoFrame::PixelFormat frameFormat = QVideoFrame::Format_Invalid;
    m_surface.lock();
    uchar* frameData = m_surface.getData(frameWidth,frameHeight,frameFormat);
    if ( frameData ) {
        if ( m_player.state() == QMediaPlayer::StoppedState ) {
            memset(frameData,0,4*frameWidth*frameHeight);
        } else {
            setData(GL_RGBA,frameWidth,frameHeight,GL_RGBA,GL_UNSIGNED_BYTE,frameData);
        }
        success = true;
    }
    m_surface.unlock();
    return success;
}
