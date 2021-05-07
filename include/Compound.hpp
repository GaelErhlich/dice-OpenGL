#include <GL/glew.h>

#include <glm/glm.hpp>

#include "Geometry.h"


using glm::mat4;

class Compound
{
public:
	Compound(Geometry *shape, GLuint textureID, mat4 relativeTransformation);
	

private:
	Geometry *shape = NULL; // The Geometry object in the current node
	GLuint textureID[4]; // The OpenGL texture ID
	mat4 relativeTransf = mat4(); // The matrix placing this compound relatively to its parent compound
	mat4 modelMatrix = mat4(); // The matrix placing this compound, relatively to the world (for storage purposes)
	
	// Indicates if relativeTransf has been updated but not modelMatrix.
	// If true, this compound's modelMatrix and all it's children's must be updated before the next render.
	bool mustUpdateModelMat = true;
};