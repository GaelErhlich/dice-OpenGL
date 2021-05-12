#include "Compound.hpp"

#include <glm/gtc/type_ptr.hpp>


// Constructor
Compound::Compound(GLuint vaoID, size_t nbPoints, GLuint textureID, Shader* shader, mat4 relativeTransformation)
{
	this->vaoID = vaoID;
	this->nbPoints = nbPoints;
	this->textureID = textureID;
	this->relativeTransf = relativeTransformation;
	this->shaderProgram = shader;
}

// Getters & setters
Compound Compound::getChild(int i) {
	return childNodes[i];
}
void Compound::setChild(Compound* child, int i) {
	childNodes[i] = *child;
}

GLuint Compound::getVAO() {
	return vaoID;
}
void Compound::setVAO(GLuint vaoID) {
	this->vaoID = vaoID;
}

void Compound::setRelativeTransf(mat4 relativeTransformation) {
	this->relativeTransf = relativeTransf;
	this->mustUpdateModelMat = true;
}
bool Compound::isModelMatUpToDate() {
	return this->mustUpdateModelMat;
}

void Compound::calculateModelMatrix(mat4 parentModelMatrix) {
	modelMatrix = parentModelMatrix * this->relativeTransf;
	this->mustUpdateModelMat = false;
	this->calculateChildModelMatrices();
}


void Compound::calculateChildModelMatrices() {
	for (Compound child : childNodes) {
		child.calculateModelMatrix(this->modelMatrix);
	}
}



void Compound::draw() {

	// First : checking if the model matrix must be updated (Only applies if this IS the tree root)
	// If it must be updated, since it's the tree root, model matrix = relative transformation.
	// In other words, the virtual parent node is the world, making this task easy.
	if (this->mustUpdateModelMat) {
		this->modelMatrix = this->relativeTransf;
	}


	// Secondly : the compound is actually drawn

	shaderProgram->use();
	glBindVertexArray(vaoID);
	if (textureID != 0) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE0, textureID);
	}
	shaderProgram->setMat4f("model", modelMatrix);
	glDrawArrays(GL_TRIANGLES, 0, nbPoints);


	// Thirdly : checking if the child nodes' model matrices must be edited.
	// Since they are not at the root, they need their parent's model matrix.
	// This is why the calculateModelMatrix() recursion is initialized here.

	if (this->mustUpdateModelMat) { // Case in which this node has been edited
		this->calculateChildModelMatrices(); // Then all its children must be edited too.
		this->mustUpdateModelMat = false;
	}
	else { // Case in which this node hasn't been edited, but maybe a child needs to be updated
		for (Compound child : childNodes) {
			if (child.mustUpdateModelMat)
				child.calculateModelMatrix(this->modelMatrix);
		}
	}

	// Lastly : The child compounds are also drawn
	for (Compound child : childNodes) {
		child.draw();
	}
	

	
}





Compound::~Compound() {
}