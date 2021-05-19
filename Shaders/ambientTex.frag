#version 330 core

/*
	ambientTex.frag
	IN	: 3 world coordinates, 2 uv coordinates
	UNI	: 1 texture, 1 ambient intensity, 1 light color
	OUT	: point color/texture (RGB1)
*/

in vec3 worldCoord;
in vec2 texCoord;

uniform sampler2D texture1;
uniform float ambientIntens;
uniform vec3 lightColor;

out vec4 FragColor;

void main()
{
	FragColor = texture(texture1, texCoord) * vec4(lightColor, 1.0) + vec4(ambientIntens,ambientIntens,ambientIntens,1.0);
}