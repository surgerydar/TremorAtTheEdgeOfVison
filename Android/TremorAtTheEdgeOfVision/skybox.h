#ifndef SKYBOX_H
#define SKYBOX_H
#include <QtCore>

class SkyBox
{
public:
    SkyBox(quint32 meridians, quint32 parallels);
    //
    //
    //
    size_t getVertexCount() const { return m_vertex.size() / 3; }
    float *getVertices() const { return const_cast<float*>(m_vertex.data()); }
    size_t getUvCount() const { return m_uv.size() / 2; }
    float *getUvs() const { return const_cast<float*>(m_uv.data()); }
    size_t getIndexCount() const { return m_index.size(); }
    int *getIndices() const { return const_cast<int*>(m_index.data()); }


private:
    void generateVertices(quint32 meridians, quint32 parallels);
    void generateIndices(quint32 meridians, quint32 parallels);
    void addVertex(float x, float y, float z);
    void addUv(float u, float v);
    void addTriangle(quint32 a, quint32 b, quint32 c);
    void addQuad(quint32 a, quint32 b, quint32 c, quint32 d);

    std::vector<int>    m_index;
    std::vector<float>  m_vertex;
    std::vector<float>  m_uv;
};

#endif // SKYBOX_H
