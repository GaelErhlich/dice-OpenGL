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


#define WIDTH     600
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



    ///////////////////////////////////////////////////////////////////////
    //
    //           ---------  INITIALIZING ELEMENTS  ---------
    //
    ///////////////////////////////////////////////////////////////////////
    
    ////////////////////////////////////////
    //             Cube VAO                ----------------------------------------------------------------- Partie probablement problématique
    ////////////////////////////////////////

    GLuint vaoCube;
    GLuint vboCube;
    glGenVertexArrays(1, &vaoCube);
    glGenBuffers(1, &vboCube);

    Cube cube = Cube();
    unsigned int nbVertices = cube.getNbVertices();



    glBindBuffer(GL_ARRAY_BUFFER, vboCube);
    glBufferData(GL_ARRAY_BUFFER, (3 + 3 + 2) * sizeof(float) * cube.getNbVertices(), NULL, GL_STATIC_DRAW);

    // De ce que je comprends, ces 3 lignes vont remplir le buffer "vboCube" en mettant toutes les positions, puis toutes les normales, puis tous les UVs.
    glBufferSubData(GL_ARRAY_BUFFER, 0                          , 3 * sizeof(float)*nbVertices, cube.getVertices());
    glBufferSubData(GL_ARRAY_BUFFER, 3 * sizeof(float)*nbVertices, 3 * sizeof(float)*nbVertices, cube.getNormals());
    glBufferSubData(GL_ARRAY_BUFFER, 6 * sizeof(float)*nbVertices, 2 * sizeof(float)*nbVertices, cube.getUVs());

    
    glBindVertexArray(vaoCube);
    
    // Je définis que l'emplacement mémoire 0 (celui utilisé explicitement dans cplt.vert) est pour les 3 positions, 1 pour les 3 normales, et 2 pour les 2 UVs.
    // J'indique qu'ils sont en 3 "blocs homogènes" en disant par exemple que les normales sont 3, qu'il y en a un ensemble toutes les 3 cases ( 3*sizeof(float) ), et que la première est
    // au début du bloc des normales après le bloc des positions ( 3*nbVertices * sizeof(float) ).
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3*sizeof(float)*nbVertices));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(6*sizeof(float)*nbVertices));
    glEnableVertexAttribArray(2);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    


    ////////////////////////////////////////
    //          Imported cube VAO         ---------------- Un autre VBO de cube pour le test, qui range les attributs par point et non par type d'attribut. Le contraire du précédent.
    ////////////////////////////////////////

    float vertices4[] = {
        // positions		// normals			// texture coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);


    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices4), vertices4, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    ////////////////////////////////////////
    //       Complete program (cplt)
    ////////////////////////////////////////

    Shader cpltShader;
    cpltShader = *cpltShader.loadFromFiles("Shaders/cplt.vert", "Shaders/cplt.frag");



    ////////////////////////////////////////
    //      Declaring loop variables
    ////////////////////////////////////////
    bool isOpened = true;
    bool mustDraw = true;

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
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glClearColor(0.25f, 0.23f, 0.40f, 1.0f);

        ////////////////////////////////////////
        //            Test cube
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
        glBindVertexArray(vaoCube);
        if (mustDraw && true) { // J'ai mis ça pour que l'affichage se fasse une seule fois pour voir. Ca peut être retiré en remplaçant true par false ici.
            glDrawArrays(GL_TRIANGLES, 0, cube.getNbVertices());
            mustDraw = false;
        }
        
        ////////////////////////////////////////
        //            Test cube 2
        ////////////////////////////////////////
        
        modelMat = mat4(1.0f);
        modelMat = glm::translate(modelMat, vec3(0.5f, 0.5f, 0.5f));
        modelMat = glm::rotate(modelMat, 1.0f, vec3(1.0f, 0.0f, 1.0f));
        modelMat = glm::scale(modelMat, vec3(0.1f, 0.1f, 0.1f));
        viewMat = mat4(1.0f);
        projectionMat = mat4(1.0f);
        location = glGetUniformLocation(cpltShader.getProgramID(), "model");
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMat));
        location = glGetUniformLocation(cpltShader.getProgramID(), "view");
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMat));
        location = glGetUniformLocation(cpltShader.getProgramID(), "projection");
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMat));

        cpltShader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        

        glBindVertexArray(0);

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

    glDeleteBuffers(1, &vboCube);

    //Free everything
    if (context != NULL)
        SDL_GL_DeleteContext(context);
    if (window != NULL)
        SDL_DestroyWindow(window);

    return 0;
}
