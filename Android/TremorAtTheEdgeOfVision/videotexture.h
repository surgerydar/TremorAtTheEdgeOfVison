#ifndef VIDEOTEXTURE_H
#define VIDEOTEXTURE_H
#include <QObject>
#include <QOpenGLFunctions>
#include <QMediaPlayer>
#include <QDebug>
#include "videosurface.h"

class VideoTexture : public QObject
{
    Q_OBJECT
public:
    explicit VideoTexture(QObject *parent = nullptr);
    ~VideoTexture();
    //
    //
    //
    //
    //
    //
    void create();
    void bind();
    void unbind();
    void setData(GLint internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
    //
    //
    //
    void open(const QUrl& url);
    void close();
    //
    //
    //
    void play();
    void pause();
    //
    //
    //
    bool update();

signals:
    void playing();
    void paused();
    void stopped();

private:
    GLuint   m_handle;
    VideoSurface m_surface;
    QMediaPlayer m_player;
};

#endif // VIDEOTEXTURE_H
