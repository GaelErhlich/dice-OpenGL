#version 330 core

/*
	simpTex.vert
	IN :	3 pos coordinates, normal vector, 2 uv coordinates
	UNI :	model, view, projection matrices
	OUT :	-
*/


layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexture;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}