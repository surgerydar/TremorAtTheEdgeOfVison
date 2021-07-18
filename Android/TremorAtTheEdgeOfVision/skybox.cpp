#include "skybox.h"

SkyBox::SkyBox(quint32 meridians, quint32 parallels ) {
    generateVertices(meridians,parallels);
    generateIndices(meridians,parallels);
}

void SkyBox::generateVertices(quint32 meridians, quint32 parallels) {
    /*
    addVertex(0.0f, 1.0f, 0.0f);
    addUv(0.0f, 0.0f);
    for (uint32_t j = 0; j <= parallels; ++j) {
        float const polar = M_PI * float(j+1) / float(parallels);
        float const sp = std::sin(polar);
        float const cp = std::cos(polar);

        for (uint32_t i = 0; i <= meridians; ++i)
        {
            float const azimuth = 2.0 * M_PI * float(i) / float(meridians);
            float const sa = std::sin(azimuth);
            float const ca = std::cos(azimuth);
            float const x = sp * ca;
            float const y = cp;
            float const z = sp * sa;
            addVertex(x, y, z);

            // TODO: UV 1.0 /
            //float const u = float(i) / float(meridians-1);
            //float const v = float(j) / float(parallels-1);
            float const u = float(i) / float(meridians);
            float const v = float(j) / float(parallels);
            addUv(u, v);
        }
    }
    addVertex(0.0f, -1.0f, 0.0f);
    //addUv(1.0f, 1.0f);
    addUv(0.0f, 1.0f);
    */
    float polarInc = M_PI/(parallels); // parallel
    float azimInc = (M_PI*2.0)/(meridians); // segAngle //

    for(float i = 0; i <= parallels; i++) {

        float tr = sin( M_PI-i * polarInc );
        float y = cos( M_PI-i * polarInc );

        float v = 1.0 - (i / parallels);

        for(float j = 0; j <= meridians; j++) {

            float x = tr * sin(j * azimInc);
            float z = tr * cos(j * azimInc);
            addVertex(x, y, z);

            float u = j / (meridians);
            addUv(u, v);
        }
    }
}
void SkyBox::generateIndices(quint32 meridians, quint32 parallels) {
    /*
    for (uint32_t i = 0; i < meridians; ++i)
    {
        uint32_t const a = i + 1;
        uint32_t const b = (i + 1) % meridians + 1;
        addTriangle(0, b, a);
    }

    for (uint32_t j = 0; j < parallels - 2; ++j)
    {
        uint32_t aStart = j * meridians + 1;
        uint32_t bStart = (j + 1) * meridians + 1;
        for (uint32_t i = 0; i < meridians; ++i)
        {
            const uint32_t a = aStart + i;
            const uint32_t a1 = aStart + (i + 1) % meridians;
            const uint32_t b = bStart + i;
            const uint32_t b1 = bStart + (i + 1) % meridians;
            addQuad(a, a1, b1, b);
        }
    }

    for (uint32_t i = 0; i < meridians; ++i)
    {
        uint32_t const a = i + meridians * (parallels - 2) + 1;
        uint32_t const b = (i + 1) % meridians + meridians * (parallels - 2) + 1;
        addTriangle(m_vertex.size() - 1, a, b);
    }
    */
    m_index.clear();
    quint32 nr = meridians+1;
    for(quint32 y = 0; y < parallels; y++) {
        for(quint32 x = 0; x <= meridians; x++) {
            m_index.push_back( (y)*nr + x );
            m_index.push_back( (y+1)*nr + x );
        }
    }

}

void SkyBox::addVertex(float x, float y, float z) {
    m_vertex.push_back(x);
    m_vertex.push_back(y);
    m_vertex.push_back(z);
}
void SkyBox::addUv(float u, float v) {
    m_uv.push_back(u);
    m_uv.push_back(v);
}


void SkyBox::addTriangle(quint32 a, quint32 b, quint32 c) {
    m_index.push_back(a);
    m_index.push_back(b);
    m_index.push_back(c);
}

void SkyBox::addQuad(quint32 a, quint32 b, quint32 c, quint32 d) {
    m_index.push_back(a);
    m_index.push_back(b);
    m_index.push_back(c);
    m_index.push_back(a);
    m_index.push_back(c);
    m_index.push_back(d);
}

