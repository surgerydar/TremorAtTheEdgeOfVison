#ifndef NATIVETEXTURE_H
#define NATIVETEXTURE_H

#include <QObject>
#include <QOpenGLFunctions>
#include "videosource.h"

class NativeTexture : public QObject
{
    Q_OBJECT
public:
    explicit NativeTexture(QObject *parent = nullptr);
    ~NativeTexture();
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
    VideoSource* m_source;
};

#endif // NATIVETEXTURE_H
