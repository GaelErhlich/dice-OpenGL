#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>

#include "Geometry.h"
#include "Shader.h"

using std::vector;
using glm::mat4;
using std::size_t;

class Compound
{
public:

	// Constructor
	Compound(GLuint vaoI, size_t nbPoints, GLuint textureID, Shader* shader, mat4 relativeTransformation, mat4 relativeRotation);

	// Destructor
	~Compound();

	// Getters & setters
	Compound getChild(int i);
	void addChild(Compound* child);
	void setChild(Compound *child, int i);

	GLuint getVAO();
	void setVAO(GLuint vaoID);

	void setRelativeTransf(mat4 relativeTransformation, mat4 relativeRotation);
	mat4 getRelativeTransf();
	bool isModelMatUpToDate();

	/* Updates the (non-relative) model matrix of this compound based on its parent's model matrix */
	void calculateModelMatrix(mat4 parentModelMatrix, mat4 parentRotModelMatrix);


	/*	Draws the current compound and its children, giving GPU the model matrix.
	View and projection matrix must however still be set manually
	Model matrices aren't updated till this function is executed. */
	void draw();


protected:
	/*	Updates the modelMatrix of all this compound's children
		by initializing the calculateModelMatrix() recursion. */
	void calculateChildModelMatrices();


private:
	GLuint vaoID = 0; // The Geometry object in the current node
	size_t nbPoints = 0;
	GLuint textureID = 0; // The OpenGL texture ID
	mat4 modelMatrix = mat4(); // The matrix placing this compound, relatively to the world (could be calculated, but better stored)
	mat4 rotModelMatrix = mat4(); // Same as above, but only rotations
	Shader *shaderProgram = NULL;
	
	// Indicates if relativeTransf has been updated but not modelMatrix.
	// If true, this compound's modelMatrix and all it's children's must be updated before the next render.
	bool mustUpdateModelMat = true;
	mat4 relativeTransf = mat4(1.0f); // The matrix placing this compound relatively to its parent compound
	mat4 relativeRot = mat4(1.0f); // Same as above, but only rotations
	vector<Compound> childNodes;
};