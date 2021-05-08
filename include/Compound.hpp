#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>

#include "Geometry.h"
#include "Shader.h"

using std::vector;
using glm::mat4;

class Compound
{
public:
	// TODO :	On devrait remplacer l'élement Geometry par un VAO, sans quoi on peut pas tracer.
	//			Mais du coup, on devra générer le VAO à l'extérieur de la classe. Je sais pas si c'est une bonne idée.

	// Constructor
	Compound(Geometry* shape, GLuint textureID, GLuint shaderProgramID, mat4 relativeTransformation);

	// Getters & setters
	Compound getChild(int i);
	void setChild(Compound *child, int i);

	Geometry* getShape();
	void setShape(Geometry *shape);

	void setRelativeTransf(mat4 relativeTransformation);
	bool isModelMatUpToDate();

	/* Updates the (non-relative) model matrix of this compound based on its parent's model matrix */
	void calculateModelMatrix(mat4 parentModelMatrix);


	/*	Draws the current compound and its children, giving GPU the model matrix.
	View and projection matrix must however still be set manually
	Model matrices aren't updated till this function is executed. */
	void draw();


protected:
	/*	Updates the modelMatrix of all this compound's children
		by initializing the calculateModelMatrix() recursion. */
	void calculateChildModelMatrices();


private:
	Geometry *shape = NULL; // The Geometry object in the current node
	GLuint textureID = 0; // The OpenGL texture ID
	mat4 modelMatrix = mat4(); // The matrix placing this compound, relatively to the world (could be calculated, but better stored)
	GLuint shaderProgramID = 0;
	
	// Indicates if relativeTransf has been updated but not modelMatrix.
	// If true, this compound's modelMatrix and all it's children's must be updated before the next render.
	bool mustUpdateModelMat = true;
	mat4 relativeTransf = mat4(1.0f); // The matrix placing this compound relatively to its parent compound
	vector<Compound> childNodes;
};