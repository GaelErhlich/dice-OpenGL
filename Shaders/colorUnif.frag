#version 330 core

/*
	ambientTex.frag
	IN	: 3 world coordinates
	UNI	: 1 light color (RGB)
	OUT	: Uniform color (RGB1)
*/

uniform vec3 unicolor;

out vec4 FragColor;

void main()
{
	FragColor = vec4(unicolor, 1.0);
}