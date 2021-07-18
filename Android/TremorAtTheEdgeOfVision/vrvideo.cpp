#include "vrvideo.h"
#include <QOpenGLFramebufferObjectFormat>
#include <QMatrix4x4>
#include "motionmanager.h"
#include "nativerotationsensor.h"

SkyBox VRVideoRenderer::s_skybox = SkyBox(100,100);
QString VRVideoRenderer::s_vertexShader = QString(
    R"glsl(
        uniform mat4 u_MVP;
        uniform float u_offset;
        uniform float v_offset;
        attribute vec4 a_Position;
        attribute vec2 a_UV;

        varying vec2 v_UV;
        void main() {
          v_UV = vec2(1.0-(0.25+u_offset+a_UV.x),v_offset+(a_UV.y*0.5));
          gl_Position = u_MVP * a_Position;
        }
    )glsl"
);
QString VRVideoRenderer::s_fragmentShader = QString(
            R"glsl(
                precision mediump float;
                varying vec2 v_UV;
                uniform sampler2D u_Texture;
                void main() {
                gl_FragColor = vec4(texture2D(u_Texture, v_UV).xyz,1.0);
            })glsl");


VRVideoRenderer::VRVideoRenderer(const bool stereo, const qreal horizontalOffset) {
    m_stereo = stereo;
    m_horizontalOffset = horizontalOffset;
    setup();
}

VRVideoRenderer::~VRVideoRenderer() {
    //qDebug() << "VRVideoRenderer::~VRVideoRenderer";
}

void VRVideoRenderer::setup() {
    m_logger.initialize();
    m_program.addShaderFromSourceCode(QOpenGLShader::Vertex,s_vertexShader);
    m_program.addShaderFromSourceCode(QOpenGLShader::Fragment,s_fragmentShader);
    if ( m_program.link() ) {
        m_program.bind();
        m_positionAttribute = m_program.attributeLocation("a_Position");
        m_uvAttribute = m_program.attributeLocation("a_UV");
        m_modelViewUniform = m_program.uniformLocation("u_MVP");
        m_textureBoundsUniform = m_program.uniformLocation("u_textureBounds");
        m_textureUniform = m_program.uniformLocation("u_Texture");
        m_textureUOffsetUniform = m_program.uniformLocation("u_offset");
        m_textureVOffsetUniform = m_program.uniformLocation("v_offset");
        glGenBuffers(1, &positionBuffer);
        glGenBuffers(1, &uvBuffer);
        glGenBuffers(1, &indexBuffer);
    } else {
        qDebug() << "Error linking shader:\n" << m_program.log();
    }
}

//
//
//
QOpenGLFramebufferObject *VRVideoRenderer::createFramebufferObject(const QSize &size) {
    m_size = size;
    //
    //
    //
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil); // this may result in nothing being rendered
    // optionally enable multisampling by doing format.setSamples(4);
    return new QOpenGLFramebufferObject(size, format);
}

float wrap(float value, float min, float max ) {

    if ( value > max ) {
        return value - max;
    } else if ( value < min ) {
        return value + min;
    }
    return value;
}

void VRVideoRenderer::render() {
    //
    //
    //
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //
    //
    //
    m_program.bind();
    //
    // set mesh, TODO: move to private method
    //
    glBindBuffer(GL_ARRAY_BUFFER,positionBuffer);
    glBufferData(
                GL_ARRAY_BUFFER,
                s_skybox.getVertexCount() * sizeof(float) * 3,
                s_skybox.getVertices(),
                GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,uvBuffer);
    glBufferData(
                GL_ARRAY_BUFFER,
                s_skybox.getUvCount() * sizeof(float) * 2,
                s_skybox.getUvs(),
                GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,indexBuffer);
    glBufferData(
                GL_ARRAY_BUFFER,
                s_skybox.getIndexCount() * sizeof(int),
                s_skybox.getIndices(),
                GL_STATIC_DRAW);
    //
    // activate mesh
    //
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glVertexAttribPointer(
                m_positionAttribute,
                3,  // 3 components per vertex
                GL_FLOAT,
                false,
                0,  // Stride and offset 0, as we are using different vbos.
                0);
    glEnableVertexAttribArray(m_positionAttribute);

    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glVertexAttribPointer(
                m_uvAttribute,
                2,  // 2 components per uv
                GL_FLOAT,
                false,
                0,
                0);
    glEnableVertexAttribArray(m_uvAttribute);

    if ( m_videoTexture.update() ) {
        //qDebug() << "VRVideoRenderer::render : drawing skybox";

        glActiveTexture(GL_TEXTURE0);
        m_videoTexture.bind();
        glUniform1i(m_textureUniform,0);

        int eyes = m_stereo ? 2 : 1;
        for ( int eye = 0; eye < eyes; eye++ ) {
            // model view matrix
            float height = m_size.height() / float(eyes);
            float offset = height * eye;
            float aspectRatio   = float(m_size.width()) / height;
            float verticalFOV   = (aspectRatio > 1) ? 60.0 : 90.0;
            float near          = 0.1;
            float far           = 100.0;

            //
            Matrix4x4 projection = GetPerspectiveProjectionMatrix(verticalFOV*(M_PI / 180.0),aspectRatio,near,far);

            // sensor axis mapping should be set to AXIS_X ( 0 ), AXIS_Z ( 2 )
            std::array<float, 4> orientation = MotionManager::shared().getOrientation();

            float roll = orientation[1];
            Matrix4x4 rollMatrix = {{
                    {1.0, 0.0, 0.0, 0.0},
                    {0.0, cos(roll), -sin(roll), 0.0},
                    {0.0, sin(roll), cos(roll), 0.0},
                    {0.0, 0.0, 0.0, 1.0}
                }};
            // pitch, rotation around y
            // this is yaw as far as the sensor is concerned, relative to magnetic north
            float pitch = -orientation[0];
            Matrix4x4 pitchMatrix = {{
                    {cos(pitch), 0.0, sin(pitch), 0.0},
                    {0.0, 1.0, 0.0, 0.0},
                    {-sin(pitch), 0.0, cos(pitch), 0.0},
                    {0.0, 0.0, 0.0, 1.0}
                }};
            // yaw, rotation around z
            // this is roll from the sensor
            float yaw = orientation[2];
            Matrix4x4 yawMatrix = {{
                    {cos(yaw), -sin(yaw), 0.0, 0.0},
                    {sin(yaw), cos(yaw), 0.0, 0.0},
                    {0.0, 0.0, 1.0, 0.0},
                    {0.0, 0.0, 0.0, 1.0}
                }};

            Matrix4x4 model = yawMatrix * rollMatrix * pitchMatrix;//* pitchMatrix;// // * rollMatrix;// * yawMatrix;
            std::array<float, 16> modelView = ( projection * model ).ToGlArray();
            glUniformMatrix4fv(m_modelViewUniform, 1, GL_FALSE, modelView.data());

            // set texture offset
            glUniform1f(m_textureUOffsetUniform,m_horizontalOffset);
            glUniform1f(m_textureVOffsetUniform,0.5*eye);

            // clip
            glViewport(0, offset*eye, m_size.width(), height);
            //glScissor(0, offset*eye, m_size.width(), height);

            // Draw with indices
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
            glDrawElements(GL_TRIANGLE_STRIP, s_skybox.getIndexCount(), GL_UNSIGNED_INT, 0);
        }
    }
    const QList<QOpenGLDebugMessage> messages = m_logger.loggedMessages();
    for (const QOpenGLDebugMessage &message : messages)
        qDebug() << message;

}

VRVideo::VRVideo(QQuickItem *parent) : QQuickFramebufferObject(parent) {
    m_renderer = nullptr;
    m_stereo = false;
    connect(this,&VRVideo::createdRenderer,[=](VRVideoRenderer* renderer) {
        qDebug() << "VRVideo : createdRenderer";
        this->m_renderer = renderer;
        this->m_renderer->connect(SIGNAL(playing()),this,SIGNAL(playing()));
        this->m_renderer->connect(SIGNAL(paused()),this,SIGNAL(paused()));
        this->m_renderer->connect(SIGNAL(stopped()),this,SIGNAL(stopped()));
    });
}

VRVideo::~VRVideo() {

}
//
//
//
QQuickFramebufferObject::Renderer *VRVideo::createRenderer() const {
    VRVideoRenderer* renderer = new VRVideoRenderer(m_stereo,m_horizontalOffset);
    emit createdRenderer(renderer);
    return renderer;
}
void VRVideo::timerEvent(QTimerEvent *) {
    update();
}
