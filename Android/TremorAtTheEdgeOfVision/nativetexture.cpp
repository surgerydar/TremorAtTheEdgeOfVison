#include "nativetexture.h"
#include <QDebug>

NativeTexture::NativeTexture(QObject *parent) : QObject(parent) {
    m_source = nullptr;
    m_handle = 0;
}
NativeTexture::~NativeTexture() {
    qDebug() << "NativeTexture::~NativeTexture";
    if ( m_source ) {
        m_source->deleteLater();
        m_source = nullptr;
    }
}
void NativeTexture::create() {
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

void NativeTexture::bind() {
    if ( m_handle > 0 ) {
        glBindTexture(GL_TEXTURE_2D,m_handle);
    }
}

void NativeTexture::unbind() {
    glBindTexture(GL_TEXTURE_2D,0);
}

void NativeTexture::setData(GLint internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels) {
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
void NativeTexture::open(const QUrl& url) {
    if ( m_source == nullptr ) {
        qDebug() << "NativeTexture::open : creating video source";
        m_source = new VideoSource;
        /*
        connect(m_source,&VideoSource::playing,this,&NativeTexture::playing);
        connect(m_source,&VideoSource::paused,this,&NativeTexture::paused);
        connect(m_source,&VideoSource::stopped,this,&NativeTexture::stopped);
        */
    }
    qDebug() << "NativeTexture::open : openning : " << url;
    m_source->open(url);
}
void NativeTexture::close() {
    m_source->close();
}
//
//
//
void NativeTexture::play() {
    if ( m_source != nullptr ) m_source->play();
}
void NativeTexture::pause() {
    if ( m_source != nullptr ) m_source->pause();
}

bool NativeTexture::update() {
    bool success = false;
    if ( m_source != nullptr ) {
        int frameWidth = -1;
        int frameHeight = -1;
        QVideoFrame::PixelFormat frameFormat = QVideoFrame::Format_Invalid;
        m_source->lock();
        uchar* frameData = m_source->getData(frameWidth,frameHeight,frameFormat);
        if ( frameData ) {
            setData(GL_RGBA,frameWidth,frameHeight,GL_RGBA,GL_UNSIGNED_BYTE,frameData);
            success = true;
        }
        m_source->unlock();
    }
    return success;
}
