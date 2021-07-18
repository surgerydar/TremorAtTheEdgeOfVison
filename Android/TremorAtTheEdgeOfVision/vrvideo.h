#ifndef VRVIDEO_H
#define VRVIDEO_H

#include <QQuickFramebufferObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QBasicTimer>
#include <QDebug>
#include <QOpenGLDebugLogger>
#include "skybox.h"
#include "motionmanager.h"
#include "videotexture.h"
#include "nativetexture.h"

class VRVideoRenderer : public QQuickFramebufferObject::Renderer {
public:
    VRVideoRenderer( const bool stereo, const qreal horizontalOffset );
    ~VRVideoRenderer();
    //
    //
    //
    void setup(); // Should be called with valid OpenGL context in RenderThread
    //
    //
    //
    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size);
    void render();
    void play(const QUrl& url) {
        m_videoTexture.open(url); // TODO: this will autoplay, this should be an option
    }
    void pause() {
        m_videoTexture.pause();
    }
    void close() {
        m_videoTexture.close();
    }
    //
    //
    //
    void connect(const char* signal, QObject*target, const char* member) {
        QObject::connect(&m_videoTexture, signal, target, member);
    }

private:
    bool m_stereo;
    qreal m_horizontalOffset;
    static SkyBox s_skybox;
    static QString s_vertexShader;
    static QString s_fragmentShader;

    bool                    m_isSetup;
    QSize                   m_size;
    QOpenGLShaderProgram    m_program;

    VideoTexture           m_videoTexture;

    int m_positionAttribute;
    int m_uvAttribute;
    int m_modelViewUniform;
    int m_textureBoundsUniform;
    int m_textureUniform;
    int m_textureUOffsetUniform;
    int m_textureVOffsetUniform;

    GLuint positionBuffer;
    GLuint uvBuffer;
    GLuint indexBuffer;

    QOpenGLDebugLogger m_logger;
};

class VRVideo : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(bool stereo READ stereo WRITE setStereo)
    Q_PROPERTY(qreal horizontalOffset READ horizontalOffset WRITE setHorizontalOffset)
public:
    VRVideo(QQuickItem *parent = nullptr);
    ~VRVideo();
    //
    //
    //
    bool stereo() const {
        return m_stereo;
    }
    void setStereo( const bool stereo ) {
        m_stereo = stereo;
    }
    qreal horizontalOffset() const {
        return m_horizontalOffset;
    }
    void setHorizontalOffset( const qreal horizontalOffset ) {
        m_horizontalOffset = horizontalOffset;
    }
    //
    //
    //
    QQuickFramebufferObject::Renderer *createRenderer() const override;
    //
    //
    //
    void timerEvent(QTimerEvent *) override;
    //
    //
    //
signals:
    void playing();
    void paused();
    void stopped();

public slots:
    void play(const QUrl& url) {
        qDebug() << "VRVideo::play";
        if ( m_renderer ) {
            start();
            m_renderer->play(url);
        }
    }
    void pause() {
        if ( m_renderer ) {
            m_renderer->pause();
        }
    }
    void close() {
        if ( m_renderer ) {
            m_renderer->close();
        }
    }
    void start() {
        MotionManager::shared().start();
        m_timer.start(12,this);
    }
    void stop() {
        m_timer.stop();
        if ( m_renderer ) {
            m_renderer->pause();
        }
        MotionManager::shared().stop();
    }

signals:
    void createdRenderer(VRVideoRenderer* renderer) const;

private:
    QBasicTimer         m_timer;
    VRVideoRenderer*    m_renderer;
    bool                m_stereo;
    qreal               m_horizontalOffset;
};

#endif // VRVIDEO_H
