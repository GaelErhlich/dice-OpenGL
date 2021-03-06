#include "Cube.h"

#include <iostream>

using std::cout;

Cube::Cube() : Geometry()
{
    m_vertices = (float*)malloc(sizeof(float)*3*36);
    m_normals  = (float*)malloc(sizeof(float)*3*36);
    m_uvs      = (float*)malloc(sizeof(float)*2*36);

    float vertices[3*36] = {
                            //Front
                            -0.5, -0.5, -0.5,
                             0.5, -0.5, -0.5,
                             0.5,  0.5, -0.5,
                            -0.5, -0.5, -0.5,
                             0.5,  0.5, -0.5,
                            -0.5,  0.5, -0.5,

                            //Back
                            -0.5, -0.5,  0.5,
                             0.5,  0.5,  0.5,
                             0.5, -0.5,  0.5,
                            -0.5, -0.5,  0.5,
                            -0.5,  0.5,  0.5,
                             0.5,  0.5,  0.5,

                            //Left
                            -0.5, -0.5,  0.5,
                            -0.5, -0.5, -0.5,
                            -0.5,  0.5, -0.5,
                            -0.5, -0.5,  0.5,
                            -0.5,  0.5, -0.5,
                            -0.5,  0.5,  0.5,

                            //Right
                             0.5, -0.5,  0.5,
                             0.5,  0.5, -0.5,
                             0.5, -0.5, -0.5,
                             0.5, -0.5,  0.5,
                             0.5,  0.5,  0.5,
                             0.5,  0.5, -0.5,

                            //Top
                             0.5,  0.5, -0.5,
                            -0.5,  0.5, -0.5,
                            -0.5,  0.5,  0.5,
                             0.5,  0.5, -0.5,
                            -0.5,  0.5,  0.5,
                             0.5,  0.5,  0.5,

                            //Bottom
                             0.5, -0.5, -0.5,
                            -0.5, -0.5,  0.5,
                            -0.5, -0.5, -0.5,
                             0.5, -0.5, -0.5,
                             0.5, -0.5,  0.5,
                            -0.5, -0.5,  0.5};

    float uvs[2*36] = {
                            //Front
                            0.0, 0.0,
                             1.0, 0.0,
                             1.0,  1.0,
                            0.0, 0.0,
                             1.0,  1.0,
                            0.0,  1.0,

                            //Back
                            0.0, 0.0,
                             1.0,  1.0,
                             1.0, 0.0,
                            0.0, 0.0,
                            0.0,  1.0,
                             1.0,  1.0,

                            //Left
                            0.0,  1.0,
                            0.0, 0.0,
                             1.0, 0.0,
                            0.0,  1.0,
                             1.0, 0.0,
                             1.0,  1.0,

                            //Right
                            0.0,  1.0,
                             1.0, 0.0,
                            0.0, 0.0,
                            0.0,  1.0,
                             1.0,  1.0,
                             1.0, 0.0,

                            //Top
                             1.0, 0.0,
                            0.0, 0.0,
                            0.0,  1.0,
                             1.0, 0.0,
                            0.0,  1.0,
                             1.0,  1.0,

                            //Bottom
                             1.0, 0.0,
                            0.0,  1.0,
                            0.0, 0.0,
                             1.0, 0.0,
                             1.0,  1.0,
                            0.0,  1.0};

    float normals[3*36] = {//Front
                           0.0, 0.0, -1.0,
                           0.0, 0.0, -1.0,
                           0.0, 0.0, -1.0,
                           0.0, 0.0, -1.0,
                           0.0, 0.0, -1.0,
                           0.0, 0.0, -1.0,

                           //Back
                           0.0, 0.0, 1.0,
                           0.0, 0.0, 1.0,
                           0.0, 0.0, 1.0,
                           0.0, 0.0, 1.0,
                           0.0, 0.0, 1.0,
                           0.0, 0.0, 1.0,

                           //Left
                           -1.0, 0.0, 0.0,
                           -1.0, 0.0, 0.0,
                           -1.0, 0.0, 0.0,
                           -1.0, 0.0, 0.0,
                           -1.0, 0.0, 0.0,
                           -1.0, 0.0, 0.0,

                           //Right
                            1.0, 0.0, 0.0,
                            1.0, 0.0, 0.0,
                            1.0, 0.0, 0.0,
                            1.0, 0.0, 0.0,
                            1.0, 0.0, 0.0,
                            1.0, 0.0, 0.0,

                           //Top
                            0.0, 1.0, 0.0,
                            0.0, 1.0, 0.0,
                            0.0, 1.0, 0.0,
                            0.0, 1.0, 0.0,
                            0.0, 1.0, 0.0,
                            0.0, 1.0, 0.0,

                           //Bottom
                            0.0, -1.0, 0.0,
                            0.0, -1.0, 0.0,
                            0.0, -1.0, 0.0,
                            0.0, -1.0, 0.0,
                            0.0, -1.0, 0.0,
                            0.0, -1.0, 0.0};

    for(uint32_t i = 0; i < 3*36; i++)
    {
        m_vertices[i] = vertices[i];
        m_normals[i] = normals[i];
    }
    for(uint32_t i = 0; i < 2*36; i++)
        m_uvs[i] = uvs[i];
    m_nbVertices = 36;
}


void Cube::setUvArray(string name) {
    

    if(name == "4x4-patron-down-left") {
        float uvs[2 * 36] = {
            //Front (5)
            0.0, 0.0,
            0.25, 0.0,
            0.25, 0.25,
            0.0, 0.0,
            0.25, 0.25,
            0.0, 0.25,

            //Back (2)
            0.5, 0.0,
            0.75,  0.25,
            0.75, 0.0,
            0.5, 0.0,
            0.5,  0.25,
            0.75,  0.25,
            
             //Left (4)
             0.25,  1.0,
             0.25, 0.75,
              0.5, 0.75,
             0.25,  1.0,
             0.5, 0.75,
             0.5,  1.0,

              //Right (3)
              0.25,  0.5,
               0.5, 0.25,
              0.25, 0.25,
              0.25,  0.5,
               0.5,  0.5,
               0.5, 0.25,

               //Top (1)
               0.5, 0.5,
               0.25, 0.5,
               0.25,  0.75,
               0.5, 0.5,
               0.25,  0.75,
               0.5,  0.75,

                //Bottom (6)
                0.5, 0.0,
                0.25,  0.25,
                0.25, 0.0,
                0.5, 0.0,
                0.5,  0.25,
                0.25,  0.25 };
        for (uint32_t i = 0; i < 2 * 36; i++)
            m_uvs[i] = uvs[i];
    }
    else {
        cout << "Wrong name for cube UV array : " << name << "\n";
    }
}