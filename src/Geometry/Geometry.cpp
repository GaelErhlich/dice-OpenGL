#include "Geometry.h"

Geometry::Geometry(){}

Geometry::~Geometry()
{
    if (m_vertices)
        free(m_vertices);
    if (m_normals)
        free(m_normals);
    if (m_uvs)
        free(m_uvs);

    glDeleteVertexArrays(VAOs.size(), &VAOs[0]);
}


void Geometry::getOneNewVAO(GLuint &VAO, GLuint &VBO, GLenum usage) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);


    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, (3 + 3 + 2) * sizeof(float) * m_nbVertices, NULL, usage);

    glBufferSubData(GL_ARRAY_BUFFER, 0                                  , 3*sizeof(float)*m_nbVertices  , m_vertices);
    glBufferSubData(GL_ARRAY_BUFFER, 3 * sizeof(float)*m_nbVertices     , 3*sizeof(float)*m_nbVertices  , m_normals);
    glBufferSubData(GL_ARRAY_BUFFER, (3+3) * sizeof(float)*m_nbVertices , 2*sizeof(float)*m_nbVertices  , m_uvs);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)( 3*sizeof(float)*m_nbVertices ));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)( 6*sizeof(float)*m_nbVertices ));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    VAOs.push_back(VAO);
}