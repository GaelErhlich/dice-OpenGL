#version 330 core

/*
	simpTex.frag
	IN	: 3 world coordinates, 2 uv coordinates
	UNI	: 1 texture
	OUT	: point color/texture (RGB1)
*/

in vec3 worldCoord;
in vec2 texCoord;

uniform sampler2D texture1;

out vec4 FragColor;

void main()
{
	FragColor = texture(texture1, texCoord);
}