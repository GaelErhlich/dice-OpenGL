#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using std::string;
using std::ifstream;



Shader::Shader() : m_programID(0), m_vertexID(0), m_fragID(0)
{}

Shader::~Shader()
{
    glDeleteProgram(m_programID);
    glDeleteShader(m_vertexID);
    glDeleteShader(m_fragID);
}

Shader* Shader::loadFromFiles(const char *vertexFilePath, const char *fragFilePath)
{

    // ---------- Code from LearnOpenGL (modified) ----------

    // 1. retrieve the vertex/fragment source code from filePath
    string vertexCode;
    string fragmentCode;
    ifstream vShaderFile;
    ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
    fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexFilePath, ifstream::out);
        fShaderFile.open(fragFilePath);
        std::stringstream vShaderStream, fShaderStream;
        // read file’s buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        perror("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ");
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // ---------- END code from LearnOpenGL ----------


    /* Return the shader and free everything*/
    Shader* s = loadFromStrings(vShaderCode, fShaderCode);

    return s;
}

Shader* Shader::loadFromStrings(const std::string& vertexString, const std::string& fragString)
{
    Shader* shader = new Shader();

    /* Create a program and compile each shader component (vertex, fragment) */
    shader->m_programID = glCreateProgram();
    shader->m_vertexID = loadShader(vertexString, GL_VERTEX_SHADER);
    shader->m_fragID = loadShader(fragString, GL_FRAGMENT_SHADER);

    /* Attach the shader components to the program */
    glAttachShader(shader->m_programID, shader->m_vertexID);
    glAttachShader(shader->m_programID, shader->m_fragID);

    /* Do the attributes binding */
    shader->bindAttributes();

    /* Link the program. */
    glLinkProgram(shader->m_programID);

    /* Check for errors and print error message */
    int linkStatus;
    glGetProgramiv(shader->m_programID, GL_LINK_STATUS, &linkStatus);
    if(linkStatus == GL_FALSE)
    {
        char* error = (char*) malloc(ERROR_MAX_LENGTH * sizeof(char));
        int length=0;
        glGetProgramInfoLog(shader->m_programID, ERROR_MAX_LENGTH, &length, error);
        ERROR("Could not link shader-> : \n %s", error);

        delete shader;
        return NULL;
    }

    return shader;
}

int Shader::loadShader(const std::string& code, int type)
{
    /* Create a shader component and compile it */
    int shader = glCreateShader(type);
    const GLchar* s = code.c_str();
    glShaderSource(shader, 1, &s, 0);
    glCompileShader(shader);

    /* Check for errors and print error message */
    int compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if(compiled == GL_FALSE)
    {
        char* error = (char*) malloc(ERROR_MAX_LENGTH * sizeof(char));
        int length=0;
        glGetShaderInfoLog(shader, ERROR_MAX_LENGTH, &length, error);

        ERROR("Could not compile shader %d : \n %s", type, error);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

void Shader::use() {
    glUseProgram(this->getProgramID());
}

int Shader::getProgramID() const
{
    return m_programID;
}

int Shader::getVertexID() const
{
    return m_vertexID;
}

int Shader::getFragID() const
{
    return m_fragID;
}

void Shader::bindAttributes()
{
}
