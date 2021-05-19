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
#include "Sphere.h"
#include "Cone.h"
#include <time.h>

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
    //            Color list
    ////////////////////////////////////////

    vec3 colors[] = {
        vec3(1.0f, 1.0f, 1.0f), // White
        vec3(1.0f, 0.0f, 0.0f), // Red
        vec3(0.0f, 1.0f, 0.0f), // Green
        vec3(0.0f, 0.0f, 1.0f), // Blue
        vec3(1.0f, 1.0f, 0.0f), // Yellow
        vec3(1.0f, 0.0f, 1.0f), // Magenta
        vec3(0.0f, 1.0f, 1.0f), // Cyan
    };




    //////////////////////////////////////// */
    //              Textures
    ////////////////////////////////////////
    
    GLuint alTex = makeTexture("../textures/aluminium.jpg");
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
   //         Sphere             
   ////////////////////////////////////////

   Sphere sphere = Sphere(64, 64);
   GLuint vaoSphere, vboSphere;
   sphere.getOneNewVAO(vaoSphere, vboSphere, GL_STATIC_DRAW);


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
    //          Shader programs
    ////////////////////////////////////////

    Shader cpltShader;
    cpltShader = *cpltShader.loadFromFiles("../Shaders/cplt.vert", "../Shaders/cplt.frag");
    cpltShader.use();
    cpltShader.enableTextureField(0);

    Shader texShader;
    texShader = *texShader.loadFromFiles("../Shaders/simpTex.vert", "../Shaders/simpTex.frag");
    texShader.use();
    texShader.enableTextureField(0);

    Shader ambientShader;
    ambientShader = *ambientShader.loadFromFiles("../Shaders/simpTex.vert", "../Shaders/ambientTex.frag");
    texShader.use();
    texShader.enableTextureField(0);

    Shader unicolorShader;
    unicolorShader = *unicolorShader.loadFromFiles("../Shaders/positions.vert", "../Shaders/colorUnif.frag");



    ////////////////////////////////////////
    //      Declaring loop variables
    ////////////////////////////////////////
    
    
    bool isOpened = true;
    float upDownParameter = 1.0f;
    float leftRightParameter = 1.0f;
    unsigned int numberAmong7 = 0;
    
    int timeLastLoop = 0;

    int DRAW_NUMBER = -1;
    int DICE_FORCE_TIME = 5000;

    float currentAngle = 0;

    Transform prevTransform;
    int timeLastVelocityStop = 0;

    mat4 modelMat, rotModelMat, viewMat, projectionMat;
    vec3 cameraPosition;
    vec3 lightSourcePosition = vec3(0.6, 1.0, 0.5);
    vec3 lightColor;
    float ambientIntensity;


    // Temporarily used but many times
    GLint location;


    ////////////////////////////////////////
    //        Compound : Dice
    ////////////////////////////////////////

    modelMat = mat4(1.0f);
    modelMat = glm::scale(modelMat, vec3(0.1f, 0.1f, 0.1f));
    rotModelMat = mat4(1.0f);
    Compound cubeCompo = Compound(vaoPatronCube, cube.getNbVertices(), diceTex, &ambientShader, modelMat, rotModelMat);


    ////////////////////////////////////////
    //          Compound : table
    ////////////////////////////////////////

    modelMat = mat4(1.0f);
    rotModelMat = mat4(1.0f);
    modelMat = glm::scale(modelMat, vec3(2.5f, 0.05f, 2.5f));
    Compound tableCompo = Compound(vaoCube, cube.getNbVertices(), woodText, &cpltShader, modelMat, rotModelMat);

    modelMat = mat4(1.0f);
    rotModelMat = mat4(1.0f);
    modelMat = glm::translate(modelMat, vec3(0.0f, -1.25f, 0.0f));
    modelMat = glm::scale(modelMat, vec3(0.1f, 2.5f, 0.1f));
    modelMat = glm::rotate(modelMat, glm::half_pi<float>(), vec3(1.0f, 0.0f, 0.0f));
        rotModelMat = glm::rotate(rotModelMat, glm::half_pi<float>(), vec3(1.0f, 0.0f, 0.0f)); // A chaque fois qu'on fait une rotation sur une modelMat, on doit aussi la faire sur la rotModelMat

    Compound tableLeg1Compo = Compound(vaoCylinder, cylinder.getNbVertices(), woodText, &cpltShader, modelMat, rotModelMat);
    tableCompo.addChild(&tableLeg1Compo);

    ////////////////////////////////////////
    //        Compound : Lightbulb
    ////////////////////////////////////////
    
    modelMat = mat4(1.0);
    rotModelMat = mat4(1.0);
    modelMat = glm::translate(modelMat, lightSourcePosition);
    modelMat = glm::scale(modelMat, vec3(0.14, 0.14, 0.14));
    Compound bulbCompo = Compound(vaoSphere, sphere.getNbVertices(), 0, &unicolorShader, modelMat, rotModelMat);
    
    modelMat = mat4(1.0);
    rotModelMat = mat4(1.0);
    modelMat = glm::translate(modelMat, lightSourcePosition);
    modelMat = glm::rotate(modelMat, glm::quarter_pi<float>(), vec3(0.0f, 1.0f, 0.0f));
            rotModelMat = glm::rotate(rotModelMat, glm::quarter_pi<float>(), vec3(0.0f, 1.0f, 0.0f));
    modelMat = glm::rotate(modelMat, -glm::quarter_pi<float>(), vec3(1.0f, 0.0f, 0.0f) );
            rotModelMat = glm::rotate(rotModelMat, glm::quarter_pi<float>(), vec3(1.0f, 0.0f, 0.0f));
    modelMat = glm::translate(modelMat, vec3(0.0f, 0.0f, -0.03f));
    modelMat = glm::scale(modelMat, vec3(0.15, 0.15, 0.2));
    
    Compound bulbCylinderCompo = Compound(vaoCylinder, cylinder.getNbVertices(), alTex, &cpltShader, modelMat, rotModelMat);


    modelMat = mat4(1.0);
    rotModelMat = mat4(1.0);
    modelMat = glm::translate(modelMat, lightSourcePosition);
    modelMat = glm::translate(modelMat, vec3(0.0f, -0.05f, 0.0f));
    modelMat = glm::rotate(modelMat, glm::quarter_pi<float>(), vec3(0.0f, 1.0f, 0.0f));
            rotModelMat = glm::rotate(rotModelMat, glm::quarter_pi<float>(), vec3(0.0f, 1.0f, 0.0f));
    modelMat = glm::translate(modelMat, vec3(0.0f, 0.0f, 0.12f));
    modelMat = glm::rotate(modelMat, glm::quarter_pi<float>(), vec3(1.0f, 0.0f, 0.0f));
            rotModelMat = glm::rotate(rotModelMat, glm::quarter_pi<float>(), vec3(1.0f, 0.0f, 0.0f));
    modelMat = glm::scale(modelMat, vec3(0.05, 0.04, 0.6));

    Compound firstBranchCompound = Compound(vaoCube, cube.getNbVertices(), alTex, &cpltShader, modelMat, rotModelMat);


    modelMat = mat4(1.0);
    rotModelMat = mat4(1.0);
    modelMat = glm::translate(modelMat, vec3(0.25f, -0.3f, 0.25f));
    modelMat = glm::translate(modelMat, lightSourcePosition);
    modelMat = glm::scale(modelMat, vec3(0.1, 0.1, 0.1));
    
    Compound interbranchBallCompo = Compound(vaoSphere, sphere.getNbVertices(), alTex, &cpltShader, modelMat, rotModelMat);


    modelMat = mat4(1.0);
    rotModelMat = mat4(1.0);
    modelMat = glm::translate(modelMat, lightSourcePosition);
    modelMat = glm::translate(modelMat, vec3(0.15f, -0.6f, 0.15f));
    modelMat = glm::rotate(modelMat, glm::quarter_pi<float>(), vec3(0.0f, 1.0f, 0.0f));
            rotModelMat = glm::rotate(rotModelMat, glm::quarter_pi<float>(), vec3(0.0f, 1.0f, 0.0f));
    modelMat = glm::rotate(modelMat, -glm::quarter_pi<float>()*1.5f, vec3(1.0f, 0.0f, 0.0f));
            rotModelMat = glm::rotate(rotModelMat, -glm::quarter_pi<float>()*1.5f, vec3(1.0f, 0.0f, 0.0f));
    modelMat = glm::scale(modelMat, vec3(0.05, 0.04, 0.6));

    Compound secondBranchCompound = Compound(vaoCube, cube.getNbVertices(), alTex, &cpltShader, modelMat, rotModelMat);
   
    
    modelMat = mat4(1.0f);
    rotModelMat = mat4(1.0);
    modelMat = glm::translate(modelMat, lightSourcePosition);
    modelMat = glm::translate(modelMat, vec3(0.1f, -0.9f, 0.1f));
    modelMat = glm::scale(modelMat, vec3(0.2f, 0.2f, 0.2f));

    Compound lampSupportCompo = Compound(vaoCube, cube.getNbVertices(), alTex, &cpltShader, modelMat, rotModelMat);




    secondBranchCompound.addChild(&lampSupportCompo);
    interbranchBallCompo.addChild(&secondBranchCompound);
    firstBranchCompound.addChild(&interbranchBallCompo);
    bulbCylinderCompo.addChild(&firstBranchCompound);
    bulbCompo.addChild(&bulbCylinderCompo);






    Vector3 positionPCube(0.0, 2.0, 0.0);
    Quaternion orientationPCube = Quaternion::identity();
    Transform transformPCube(positionPCube, orientationPCube);
    RigidBody* pCube = world->createRigidBody(transformPCube);
    pCube->setMass(0.05);
    const Vector3 halfExtentsPCube(0.05, 0.05, 0.05);
    BoxShape* boxShapePCube = physicsCommon.createBoxShape(halfExtentsPCube);
    Transform identity = Transform::identity();
    Collider* colliderPCube;
    colliderPCube = pCube->addCollider(boxShapePCube, identity);
    Material& materialPCube = colliderPCube->getMaterial();
    // materialPCube = colliderPCube->getMaterial();
    materialPCube.setBounciness(0.1);
    materialPCube.setRollingResistance(0.5);
    materialPCube.setFrictionCoefficient(0.3);

    Vector3 positionPTable(0.0, -0.05, 0.0);
    Quaternion orientationPTable = Quaternion::identity();
    Transform transformPTable(positionPTable, orientationPTable);
    RigidBody* pTable = world->createRigidBody(transformPTable);
    pTable->setType(BodyType::STATIC);
    const Vector3 halfExtentsPTable(10,0.075 , 10);
    BoxShape* boxShapePTable = physicsCommon.createBoxShape(halfExtentsPTable);
    Collider* colliderPTable;
    colliderPTable = pTable->addCollider(boxShapePTable, identity);







    ///////////////////////////////////////////////////////////////////////
    //
    //           ---------  APPLICATION MAIN LOOP  ---------
    //
    ///////////////////////////////////////////////////////////////////////
    srand (time(NULL));
    Vector3 force(rand()%10-3, 0, rand()%10-3);
    Vector3 point(0.1, 0.1, 0.1);
    // // Apply a force to the center of the cube 
    pCube->applyForceAtLocalPosition(force, point);

    Vector3 torque(rand()%100, rand()%100,rand()%100); 
    // // Apply a torque to the cube
    pCube->applyTorque(torque);
    
    while (isOpened)
    {
        ////////////////////////////////////////
        //          Loop beginning
        ////////////////////////////////////////

        // Time in ms telling us when this frame started. Useful for keeping a fix framerate
        uint32_t timeBegin = SDL_GetTicks();

        // Treat inputs
        manageEvents(isOpened, upDownParameter, leftRightParameter, numberAmong7);

        // Clear the screen : the depth buffer and the color buffer
        if (DRAW_NUMBER != 0) {
            glClearColor(0.02f, 0.02f, 0.05f, 1.0f);
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);


            ////////////////////////////////////////
            //      Updating loop variables
            ////////////////////////////////////////


            ambientIntensity = 0.2;
            lightColor = colors[numberAmong7] * upDownParameter;

            currentAngle += (SDL_GetTicks() - timeLastLoop)*leftRightParameter/1000;
            cameraPosition = vec3(-sin(currentAngle), 2.0, cos(currentAngle) );
            

            viewMat = mat4(1.0f);
            viewMat = glm::translate(viewMat, vec3(0.0f, 0.0f, -4.0f));
            viewMat = glm::rotate(viewMat, glm::quarter_pi<float>()*0.75f, vec3(1.0f, 0.0f, 0.0f));
            viewMat = glm::rotate(viewMat, currentAngle, vec3(0.0f, 1.0f, 0.0f));
            projectionMat = mat4(1.0f);
            projectionMat = glm::perspective(glm::quarter_pi<float>(), 800.0f / 800.0f, 0.1f, 100.0f);


            ////////////////////////////////////////
            //          Uniforms : cplt
            ////////////////////////////////////////
            
            cpltShader.use();
            
            cpltShader.setMat4f("view", viewMat);
            cpltShader.setMat4f("projection", projectionMat);

            // Light
            cpltShader.setFloat("ambientIntens", ambientIntensity);
            cpltShader.setVec3f("lightColor", lightColor);
            cpltShader.setVec3f("lightPos", lightSourcePosition);
            cpltShader.setVec3f("cameraCoord", cameraPosition); 
            cpltShader.setFloat("specularStrength", 0.8);
            

            ////////////////////////////////////////
            //        Uniforms : simpTex
            ////////////////////////////////////////

            texShader.use();

            texShader.setMat4f("view", viewMat);
            texShader.setMat4f("projection", projectionMat);


            ////////////////////////////////////////
            //      Uniforms : ambientTex
            ////////////////////////////////////////

            ambientShader.use();

            ambientShader.setMat4f("view", viewMat);
            ambientShader.setMat4f("projection", projectionMat);

            ambientShader.setFloat("ambientLight", ambientIntensity);
            ambientShader.setVec3f("lightColor", lightColor);


            ////////////////////////////////////////
            //      Uniforms : ambientTex
            ////////////////////////////////////////

            unicolorShader.use();

            unicolorShader.setMat4f("view", viewMat);
            unicolorShader.setMat4f("projection", projectionMat);

            ////////////////////////////////////////
            //                Dice
            ////////////////////////////////////////
            
            if (SDL_GetTicks() > DICE_FORCE_TIME && SDL_GetTicks() < DICE_FORCE_TIME+15000 && SDL_GetTicks() > timeLastVelocityStop+500) {
                pCube->setAngularVelocity( pCube->getAngularVelocity() * 0.75 );
                pCube->setLinearVelocity( pCube->getLinearVelocity() * 0.75 );
                timeLastVelocityStop = SDL_GetTicks();
            }

            const float timeStep = 1.0f / 60.0f; // Constant physics time step 
            world->update(timeStep);
            Transform currTransformPCube = pCube->getTransform(); 
            float transformationMatrixPCube[16];
            currTransformPCube.getOpenGLMatrix(transformationMatrixPCube);
            const Vector3& tposition = currTransformPCube.getPosition(); 
            cubeCompo.calculateModelMatrix(glm::make_mat4(transformationMatrixPCube), mat4(1.0f) ); // On ne peut pas donner la matrice de rotation � cubeCompo pour mettre � jour ses normales, donc on ne les met pas � jour pour l'instant
            cubeCompo.draw();


            //////////////////////////////////////// */
            //              Table
            ////////////////////////////////////////


            tableCompo.draw();


            ////////////////////////////////////////
            //              Lamp
            ////////////////////////////////////////


            unicolorShader.use();
            unicolorShader.setVec3f("unicolor", lightColor);
            bulbCompo.draw();


        
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


        timeLastLoop = timeEnd;


    }


    glDeleteBuffers(1, &vboCube);

    //Free everything
    if (context != NULL)
        SDL_GL_DeleteContext(context);
    if (window != NULL)
        SDL_DestroyWindow(window);

    return 0;
}
