#version 330 core

/*
	cplt.vert
	IN :	3 pos coordinates, normal vector, 2 uv coordinates
	UNI :	model, model rotations, view, projection matrices
	OUT :	3 world coordinates, updated normal vector, 2 uv coordinates
*/


layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexture;
uniform mat4 model;
uniform mat4 rotModel;
uniform mat4 view;
uniform mat4 projection;

out vec3 worldCoord;
out vec3 normalVec;
out vec2 texCoord;

void main()
{
	worldCoord = vec3(model * vec4(aPos, 1.0));
	normalVec = normalize( vec3(rotModel * vec4(aNormal, 1.0)) );
	gl_Position = projection * view * vec4(worldCoord, 1.0f);
	texCoord = aTexture;
}