#version 330 core

/*
	cplt.frag
	IN	: 3 world coordinates, normal vector, 2 uv coordinates
	UNI	: 1 texture
	OUT	: point color/texture (RGB1)
*/

in vec3 worldCoord;
in vec3 normalVec;
in vec2 texCoord;

//uniform sampler2D texture;

out vec4 FragColor;

void main()
{
	//FragColor = texture(texture, texCoord);
	FragColor = vec4(0.4, 0.4, 0.4, 1.0);
}