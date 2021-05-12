//SDL Libraries
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

//OpenGL Libraries
#include <GL/glew.h>
#include <GL/gl.h>

//GML libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include "Initialization.hpp"
#include "EventsManager.hpp"

#include "Shader.h"
#include "Compound.hpp"

#include "Cube.h"

#include "logger.h"





// Temporary
#include "glm/gtc/type_ptr.hpp"

using std::cout;

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;


#define WIDTH                   600
#define HEIGHT                  600
#define FRAMERATE               60
#define OUTLINE_MODE_ENABLED    false
#define TIME_PER_FRAME_MS  (1.0f/FRAMERATE * 1e3)
#define INDICE_TO_PTR(x) ((void*)(x))

int main(int argc, char* argv[])
{
    ////////////////////////////////////////
    //SDL2 / OpenGL Context initialization : 
    ////////////////////////////////////////

    SDL_Window* window;
    SDL_GLContext context;
    if (initializeOpenGL(window, context, WIDTH, HEIGHT) == false)
        return 0; // if initialization fails, the program can end here.



    
    if(OUTLINE_MODE_ENABLED)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



    ///////////////////////////////////////////////////////////////////////
    //
    //           ---------  INITIALIZING ELEMENTS  ---------
    //
    ///////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////
    //             Cube VAO                ----------------------------------------------------------------- Partie probablement problématique
    ////////////////////////////////////////

    Cube cube = Cube();
    GLuint* VBAO = cube.getOneNewVAO(GL_STATIC_DRAW);
    GLuint vaoCube = VBAO[0];
    GLuint vboCube = VBAO[1];

   ////////////////////////////////////////
   //             Triangle                
   ////////////////////////////////////////

    GLuint vaoTri;
    GLuint vboTri;
    glGenVertexArrays(1, &vaoTri);
    glGenBuffers(1, &vboTri);
    
    float posTri[] = {
        -0.5f, 0.5f, 0.0f,  0.5f, 0.5f, 0.0f,   0.0f, -0.5f, 0.0f      // Positions
    };
    float normTri[] = {
        0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f,       // Normales
    };
    float uvTri[] = {
        0.0f, 0.0f,         1.0f, 0.0f,         1.0f, 1.0f              // UVs
    };



    glBindBuffer(GL_ARRAY_BUFFER, vboTri);
    glBufferData(GL_ARRAY_BUFFER, (3 + 3 + 2) * sizeof(float) * 3, NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, 9*sizeof(float), &posTri);
    glBufferSubData(GL_ARRAY_BUFFER, 9*sizeof(float), 9*sizeof(float), &normTri);
    glBufferSubData(GL_ARRAY_BUFFER, 18*sizeof(float), 6*sizeof(float), &uvTri);


    glBindVertexArray(vaoTri);

    // Je définis que l'emplacement mémoire 0 (celui utilisé explicitement dans cplt.vert) est pour les 3 positions, 1 pour les 3 normales, et 2 pour les 2 UVs.
    // J'indique qu'ils sont en 3 "blocs homogènes" en disant par exemple que les normales sont 3, qu'il y en a un ensemble toutes les 3 cases ( 3*sizeof(float) ), et que la première est
    // au début du bloc des normales après le bloc des positions ( 3*nbVertices * sizeof(float) ).
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)( 9*sizeof(float) ));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)( 18*sizeof(float) ));
    glEnableVertexAttribArray(2);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);




    ////////////////////////////////////////
    //       Complete program (cplt)
    ////////////////////////////////////////

    Shader cpltShader;
    cpltShader = *cpltShader.loadFromFiles("Shaders/cplt.vert", "Shaders/cplt.frag");



    ////////////////////////////////////////
    //             Compounds
    ////////////////////////////////////////


    Compound cubeCompo = Compound(vaoCube, cube.getNbVertices(), 0, cpltShader.getProgramID(), mat4(1.0f));


    ////////////////////////////////////////
    //      Declaring loop variables
    ////////////////////////////////////////
    bool isOpened = true;

    int DRAW_NUMBER = -1;

    mat4 modelMat;
    mat4 viewMat;
    mat4 projectionMat;

    GLint location;



    ///////////////////////////////////////////////////////////////////////
    //
    //           ---------  APPLICATION MAIN LOOP  ---------
    //
    ///////////////////////////////////////////////////////////////////////

    while (isOpened)
    {
        ////////////////////////////////////////
        //          Loop beginning
        ////////////////////////////////////////

        // Time in ms telling us when this frame started. Useful for keeping a fix framerate
        uint32_t timeBegin = SDL_GetTicks();
        
        // Treat inputs
        manageEvents(isOpened);
        
        // Clear the screen : the depth buffer and the color buffer
        if (DRAW_NUMBER != 0) {
            glClearColor(0.25f, 0.23f, 0.40f, 1.0f);
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        }

        ////////////////////////////////////////
        //    viewMat & projectionMat : cplt
        ////////////////////////////////////////
        
        viewMat = mat4(1.0f);
        viewMat = glm::rotate(viewMat, glm::quarter_pi<float>(), vec3(0.5f, 0.9f, 1.0f));
        projectionMat = mat4(1.0f);
        cpltShader.setMat4f("view", viewMat);
        cpltShader.setMat4f("projection", projectionMat);


        ////////////////////////////////////////
        //        Cube compound test
        ////////////////////////////////////////

        cubeCompo.draw();



        ////////////////////////////////////////
        //            Test cube
        ////////////////////////////////////////
        /*/
        modelMat = mat4(1.0f);
        viewMat = mat4(1.0f);
        viewMat = glm::rotate(viewMat, glm::quarter_pi<float>(), vec3(0.5f, 0.9f, 1.0f));
        projectionMat = mat4(1.0f);
        location = glGetUniformLocation(cpltShader.getProgramID(), "model");
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMat));
        location = glGetUniformLocation(cpltShader.getProgramID(), "view");
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMat));
        location = glGetUniformLocation(cpltShader.getProgramID(), "projection");
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMat));
        
        cpltShader.use();
        glBindVertexArray(vaoCube);
        if (DRAW_NUMBER != 0) {
            glDrawArrays(GL_TRIANGLES, 0, cube.getNbVertices());
        }
        

        //////////////////////////////////////// */
        //            Triangle
        ////////////////////////////////////////
        
        modelMat = mat4(1.0f);
        viewMat = mat4(1.0f);
        projectionMat = mat4(1.0f);
        location = glGetUniformLocation(cpltShader.getProgramID(), "model");
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMat));
        location = glGetUniformLocation(cpltShader.getProgramID(), "view");
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMat));
        location = glGetUniformLocation(cpltShader.getProgramID(), "projection");
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMat));

        cpltShader.use();
        glBindVertexArray(vaoTri);
        if (DRAW_NUMBER != 0) {
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
        //////////////////////////////////////// */
        //              Table
        ////////////////////////////////////////


        //TODO rendering


        ////////////////////////////////////////
        //          Other objects
        ////////////////////////////////////////




        ////////////////////////////////////////
        //             Loop end
        ////////////////////////////////////////

        // Display on screen (swap the buffer on screen and the buffer you are drawing on)
        if(DRAW_NUMBER != 0)
            SDL_GL_SwapWindow(window);

        if (DRAW_NUMBER > 0)
            DRAW_NUMBER--;

        // Time in ms telling us when this frame ended. Useful for keeping a fix framerate
        uint32_t timeEnd = SDL_GetTicks();

        // We want FRAMERATE FPS
        if (timeEnd - timeBegin < TIME_PER_FRAME_MS)
            SDL_Delay(TIME_PER_FRAME_MS - (timeEnd - timeBegin));


    }

    glDeleteBuffers(1, &vboCube);

    //Free everything
    if (context != NULL)
        SDL_GL_DeleteContext(context);
    if (window != NULL)
        SDL_DestroyWindow(window);

    return 0;
}
