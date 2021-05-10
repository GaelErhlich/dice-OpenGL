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


using std::cout;

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;


#define WIDTH     800
#define HEIGHT    600
#define FRAMERATE 60
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

    //TODO
    //From here you can load your OpenGL objects, like VBO, Shaders, etc.
    //TODO

    ////////////////////////////////////////
    //      Declaring loop variables
    ////////////////////////////////////////

    bool isOpened = true;

    mat4 modelMat;
    mat4 viewMat;
    mat4 projectionMat;


    ///////////////////////////////////////////////////////////////////////
    //
    //           ---------  INITIALIZING ELEMENTS  ---------
    //
    ///////////////////////////////////////////////////////////////////////
    
    ////////////////////////////////////////
    //             Cube VAO
    ////////////////////////////////////////

    GLuint vaoCube;
    GLuint vboCube[3];
    glGenVertexArrays(1, &vaoCube);
    glGenBuffers(3, vboCube);

    Cube cube = Cube();

    glBindVertexArray(vaoCube);
    glBindBuffer(GL_ARRAY_BUFFER, vboCube[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube.getVertices()), cube.getVertices(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vboCube[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube.getNormals()), cube.getNormals(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vboCube[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube.getUVs()), cube.getUVs(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    

    ////////////////////////////////////////
    //       Complete program (cplt)
    ////////////////////////////////////////

    Shader cpltShader;
    //shader.loadFromFiles("C:\\Users\\user\\Documents\\Polytech\\ET3 (S2)\\Spé Informatique graphique\\Dice Project\\Shaders\\cplt.vert", "Initialization.cpp");
    cpltShader.loadFromFiles("Shaders/cplt.vert", "Shaders/cplt.frag");



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
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glClearColor(0.25f, 0.23f, 0.40f, 1.0f);

        ////////////////////////////////////////
        //            Test cube
        ////////////////////////////////////////
        cpltShader.use();
        glBindVertexArray(vaoCube);
        glDrawArrays(GL_STATIC_DRAW, 0, cube.getNbVertices());


        ////////////////////////////////////////
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
        SDL_GL_SwapWindow(window);

        // Time in ms telling us when this frame ended. Useful for keeping a fix framerate
        uint32_t timeEnd = SDL_GetTicks();

        // We want FRAMERATE FPS
        if (timeEnd - timeBegin < TIME_PER_FRAME_MS)
            SDL_Delay(TIME_PER_FRAME_MS - (timeEnd - timeBegin));
    }

    //Free everything
    if (context != NULL)
        SDL_GL_DeleteContext(context);
    if (window != NULL)
        SDL_DestroyWindow(window);

    return 0;
}
