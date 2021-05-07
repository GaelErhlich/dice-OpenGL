#include "Compound.hpp"

// Constructor
Compound::Compound(Geometry* shape, GLuint textureID, mat4 relativeTransformation, Shader shaderProgram)
{
	this->shape = shape;
	this->textureID = textureID;
	this->relativeTransf = relativeTransformation;
}

// Getters & setters
Compound Compound::getChild(int i) {
	return childNode[i];
}
void Compound::setChild(Compound* child, int i) {
	childNode[i] = *child;
}

Geometry* Compound::getShape() {
	return shape;
}
void Compound::setShape(Geometry* shape) {
	this->shape = shape;
}

void Compound::setRelativeTransf(mat4 relativeTransformation) {
	this->relativeTransf = relativeTransf;
	this->mustUpdateModelMat = true;
}



void Compound::calculateModelMatrices() {
	//TODO
}


// Use
void Compound::draw() {
	//TODO
}