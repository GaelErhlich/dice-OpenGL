#version 330 core

/*
	cplt.frag
	IN	: 3 world coordinates, normal vector, 2 uv coordinates
	UNI	: 1 texture, 3 light source coords, 3 camera coords, light source color (RGB), ambient light intensity, specular light intensity
	OUT	: point color/texture (RGB1)
*/

in vec3 worldCoord;
in vec3 normalVec;
in vec2 texCoord;

uniform sampler2D texture1;
uniform vec3 lightPos;
uniform vec3 cameraCoord;
uniform vec3 lightColor;
uniform float ambientIntens;
uniform float specularStrength;

out vec4 FragColor;

void main()
{
	// Diffusion
	vec3 lightOriginDir = normalize(lightPos - worldCoord);
	float diffusion = max(0.0, dot(lightOriginDir, normalVec));
	
	// Specular light
	vec3 cameraDir = normalize(cameraCoord - worldCoord);
	float specularLight = max(0.0, dot( cameraDir, reflect(-lightOriginDir, normalVec) ) );
	specularLight = pow(specularLight, 256);

	// Finally
	vec4 color = vec4( lightColor * (diffusion + specularStrength*specularLight), 1.0f) + vec4(ambientIntens,ambientIntens,ambientIntens,1.0);
	FragColor = texture(texture1, texCoord) * color * vec4(lightColor, 1.0);
}