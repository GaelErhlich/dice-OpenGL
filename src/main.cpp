#include <reactphysics3d/reactphysics3d.h> 
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
#include "makeTexture.hpp"

#include "Cube.h"
#include "Cylinder.h"

#include "logger.h"


using namespace reactphysics3d;
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



////////////////////////////////////////
//          Global variables
////////////////////////////////////////




int main(int argc, char* argv[])
{
    ////////////////////////////////////////
    //      Physics initialization : 
    ////////////////////////////////////////

    PhysicsCommon physicsCommon;
    PhysicsWorld::WorldSettings settings;
    settings.gravity = Vector3(0,-9.81, 0);
    PhysicsWorld* world = physicsCommon.createPhysicsWorld(settings);


    ////////////////////////////////////////
    //SDL2 / OpenGL Context initialization : 
    ////////////////////////////////////////

    SDL_Window* window;
    SDL_GLContext context;
    if (initializeOpenGL(window, context, WIDTH, HEIGHT) == false)
        return 0; // if initialization fails, the program can end here.


    if (OUTLINE_MODE_ENABLED)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);




    ///////////////////////////////////////////////////////////////////////
    //
    //           ---------  INITIALIZING ELEMENTS  ---------
    //
    ///////////////////////////////////////////////////////////////////////



    //////////////////////////////////////// */
    //              Textures
    ////////////////////////////////////////
    
    GLuint woodText = makeTexture("../textures/wood.jpg");
    GLuint wallTex = makeTexture("../textures/wall.jpg");
    GLuint diceTex = makeTexture("../textures/Dice.jpg");








    //////////////////////////////////////// */
    //              Cubes
    ////////////////////////////////////////

    Cube cube = Cube();
    GLuint vaoCube, vboCube;
    cube.getOneNewVAO(vaoCube, vboCube, GL_STATIC_DRAW);

    GLuint vaoPatronCube, vboPatronCube;
    cube.setUvArray("4x4-patron-down-left");
    cube.getOneNewVAO(vaoPatronCube, vboPatronCube, GL_STATIC_DRAW);
    


    //////////////////////////////////////// */
    //              Cylinder
    ////////////////////////////////////////

    Cylinder cylinder = Cylinder(128);
    GLuint vaoCylinder, vboCylinder;
    cylinder.getOneNewVAO(vaoCylinder, vboCylinder, GL_STATIC_DRAW);



   ////////////////////////////////////////
   //         Debug triangle                
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
    cpltShader = *cpltShader.loadFromFiles("../Shaders/cplt.vert", "../Shaders/cplt.frag");
    cpltShader.use();
    cpltShader.enableTextureField(0);



    ////////////////////////////////////////
    //      Declaring loop variables
    ////////////////////////////////////////
    bool isOpened = true;

    int DRAW_NUMBER = -1;

    Transform prevTransform;

    mat4 modelMat;
    mat4 viewMat;
    mat4 projectionMat;

    GLint location;


    ////////////////////////////////////////
    //        Compound : some cube
    ////////////////////////////////////////

    modelMat = mat4(1.0f);
    Compound cubeCompo = Compound(vaoPatronCube, cube.getNbVertices(), diceTex, &cpltShader, modelMat);


    ////////////////////////////////////////
    //          Compound : table
    ////////////////////////////////////////

    modelMat = mat4(1.0f);
    modelMat = glm::scale(modelMat, vec3(1.0f, 0.05f, 1.0f));
    Compound tableCompo = Compound(vaoCube, cube.getNbVertices(), woodText, &cpltShader, modelMat);

    modelMat = glm::scale(mat4(1.0f), vec3(0.1f, 0.5f, 0.1f));
    modelMat = glm::translate(modelMat, vec3(0.0f, -0.45f, 0.0f));

    Compound tableLeg1Compo = Compound(vaoCylinder, cylinder.getNbVertices(), wallTex, &cpltShader, modelMat);
    tableCompo.addChild(&tableLeg1Compo);







    Vector3 positionPCube(0.0, 0.0, 0.0);
    Quaternion orientationPCube = Quaternion::identity();
    Transform transformPCube(positionPCube, orientationPCube);
    RigidBody* pCube = world->createRigidBody(transformPCube);
    pCube->setMass(0.1);
    const Vector3 halfExtentsPCube(0.5, 0.5, 0.5);
    BoxShape* boxShapePCube = physicsCommon.createBoxShape(halfExtentsPCube);
    Transform identity = Transform::identity();
    Collider* colliderPCube;
    colliderPCube = pCube->addCollider(boxShapePCube, identity);

    Vector3 positionPTable(0.0, -2, 0.0);
    Quaternion orientationPTable = Quaternion::identity();
    Transform transformPTable(positionPTable, orientationPTable);
    RigidBody* pTable = world->createRigidBody(transformPTable);
    pTable->setType(BodyType::STATIC);
    const Vector3 halfExtentsPTable(10,0.1 , 10);
    BoxShape* boxShapePTable = physicsCommon.createBoxShape(halfExtentsPTable);
    Collider* colliderPTable;
    colliderPTable = pTable->addCollider(boxShapePTable, identity);







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

            ////////////////////////////////////////
            //    viewMat & projectionMat : cplt
            ////////////////////////////////////////
            
            cpltShader.use();
            viewMat = mat4(1.0f);
            viewMat = glm::translate(viewMat, vec3(0.5f, 0.5f, 0.5f));
            viewMat = glm::rotate(viewMat, glm::quarter_pi<float>() * ((float)SDL_GetTicks() / 1000), vec3(0.5f, 0.9f, 0.5f));
            projectionMat = mat4(1.0f);
            cpltShader.setMat4f("view", viewMat);
            cpltShader.setMat4f("projection", projectionMat);


            ////////////////////////////////////////
            //        Cube compound test
            ////////////////////////////////////////
            // Constant physics time step 
            const float timeStep = 1.0f / 60.0f; 
            world->update(timeStep); 
            Transform currTransformPCube = pCube->getTransform(); 
            float transformationMatrixPCube[16];
            currTransformPCube.getOpenGLMatrix(transformationMatrixPCube);
            const Vector3& tposition = currTransformPCube.getPosition(); 
            //std::cout << "Cube Position: (" << tposition.x << ", " << tposition.y << ", " << tposition.z << ")" << std::endl; 
            cubeCompo.calculateModelMatrix(glm::make_mat4(transformationMatrixPCube));
            cubeCompo.draw();


            //////////////////////////////////////// */
            //              Table
            ////////////////////////////////////////


            tableCompo.draw();


            ////////////////////////////////////////
            //          Other objects
            ////////////////////////////////////////




        
        ////////////////////////////////////////
        //             Loop end
        ////////////////////////////////////////

            // Display on screen (swap the buffer on screen and the buffer you are drawing on)
            SDL_GL_SwapWindow(window);
        }
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
