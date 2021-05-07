#include <GL/glew.h>

#include <glm/glm.hpp>

#include "Geometry.h"
#include "Shader.h"


using glm::mat4;

class Compound
{
public:
	// Constructor
	Compound(Geometry *shape, GLuint textureID, mat4 relativeTransformation, Shader shaderProgram);

	// Getters & setters
	Compound getChild(int i);
	void setChild(Compound *child, int i);

	Geometry* getShape();
	void setShape(Geometry *shape);

	void setRelativeTransf(mat4 relativeTransformation);


	// Use
	void draw();


protected:
	/* Updates the modelMatrix of this compound and all its children */
	void calculateModelMatrices();


private:
	Geometry *shape = NULL; // The Geometry object in the current node
	GLuint textureID = 0; // The OpenGL texture ID
	mat4 modelMatrix = mat4(); // The matrix placing this compound, relatively to the world (for storage purposes)
	
	// Indicates if relativeTransf has been updated but not modelMatrix.
	// If true, this compound's modelMatrix and all it's children's must be updated before the next render.
	bool mustUpdateModelMat = true;
	mat4 relativeTransf = mat4(); // The matrix placing this compound relatively to its parent compound
	Compound *childNode = NULL;
};