#ifndef SHADER_INCLUDE
#define SHADER_INCLUDE

#define ERROR_MAX_LENGTH 500

#include <string>
#include <iostream>

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <cstdlib>
#include "logger.h"

using std::string;


/** \brief A graphic program.*/
class Shader
{
    public:
        /** \brief the shader constructor. Should never be called alone (use loader functions)*/
        Shader();

        /* \brief Destructor. Destroy the shader component created */
        ~Shader();

        /* \brief changes the OpenGL context to use this shader program */
        void use();

        /** \brief get the program ID stored in the graphic memory of this shader.
         * \return the program ID */
        int getProgramID() const;

        /** \brief get the vertex ID stored in the graphic memory of this shader.
         * \return the vertex ID */
        int getVertexID() const;

        /** \brief get the fragment ID stored in the graphic memory of this shader.
         * \return the fragment ID */
        int getFragID() const;

        /** \brief create a shader from a vertex and a fragment file.
         * \param vertexFile the vertex file.
         * \param fragmentFile the fragment file.
         *
         * \return the Shader constructed or NULL if error*/
        static Shader* loadFromFiles(const char *vertexFilePath, const char *fragFilePath);

        /** \brief create a shader from a vertex and a fragment string.
         * \param vertexString the vertex string.
         * \param fragmentString the fragment string.
         *
         * \return the Shader constructed or NULL if error
         * */
        static Shader* loadFromStrings(const std::string& vertexString, const std::string& fragString);
        

        void Shader::setBool(const string& name, bool value) const;
        void Shader::setInt(const string& name, int value) const;
        void Shader::setFloat(const string& name, float value) const;
        void Shader::setVec2f(const string& name, glm::vec2 value) const;
        void Shader::setVec3f(const string& name, glm::vec3 value) const;
        void Shader::setVec4f(const string& name, glm::vec4 value) const;
        void Shader::setMat3f(const string& name, glm::mat3 value) const;
        void Shader::setMat4f(const string& name, glm::mat4 value) const;


    private:
        GLuint m_programID; /*!< The shader   program ID*/
        GLuint m_vertexID;  /*!< The vertex   shader  ID*/
        GLuint m_fragID;    /*!< The fragment shader  ID*/

        /* \brief Bind the attributes to known locations (vPosition to 0, vColor to 1 for example)*/
        virtual void bindAttributes();

        /** \brief Bind the attributes key string by an ID 
         * \param code the attribute name
         * \param type the type of this attribute (vertex, fragment, etc.)*/
        static int loadShader(const std::string& code, int type);
};

#endif
