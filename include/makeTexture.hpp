#include <iostream>

#include "GL/glew.h"
#include <stb_image_implem.h>


using std::cout;


GLuint makeTexture(char* path) {
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* imageData = stbi_load(path, &width, &height, &nrChannels, 0);
	if (!imageData) // Checking if the picture was successfully loaded
		cout << "Failed to load image from " << path << "\n";

	// Creating an OpenGL texture object
	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1); // On dit qu'on travaille sur texture1 maintenant, donc GL_TEXTURE_2D est texture1

	// Paramètres de la texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	// Mise des données en OpenGL texture object
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData); // On met réellement l'image dans texture1
	glGenerateMipmap(GL_TEXTURE_2D); // On génère automatiquement la mipmap (les différentes résolutions d'images)


	stbi_image_free(imageData);

	return texture1;
}