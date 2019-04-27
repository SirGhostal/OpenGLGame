#include "Texture.h"

bool Texture::loadFromFile(const char* filePath) {
	// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-5-a-textured-cube/

	FILE* file;
	errno_t err;

	// Data read from the header of the BMP file
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3
	// Actual RGB data
	unsigned char* data;

	if ((err = fopen_s(&file, filePath, "rb")) != 0) {
		// https://stackoverflow.com/questions/28691612/how-to-go-from-fopen-to-fopen-s
		// File could not be opened. filepoint was set to NULL
		// error code is returned in err.
		// error message can be retrieved with strerror(err);
		std::cout << "Image could not be opened" << std::endl;
		return false;
	}

	if (fread(header, 1, 54, file) != 54) { // If not 54 bytes read : problem
		std::cout << "Not a correct BMP file" << std::endl;
		return false;
	}

	if (header[0] != 'B' || header[1] != 'M') {
		std::cout << "Not a correct BMP file" << std::endl;
		return false;
	}

	// Read ints from the byte array according to their position in it
	dataPos = *(int*) & (header[10]);   // 0x0A
	imageSize = *(int*) & (header[34]); // 0x22
	width = *(int*) & (header[18]);     // 0x12
	height = *(int*) & (header[22]);    // 0x16

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)    imageSize = width * height * 3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

	// Create a buffer
	data = new unsigned char[imageSize];

	// Read the actual data from the file into the buffer
	fread(data, 1, imageSize, file);

	//Everything is in memory now, the file can be closed
	fclose(file);

	glGenTextures(1, &m_textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	return true;
}

/*
Texture::~Texture() {
	clear();
}*/

void Texture::clear() {
	glDeleteTextures(1, &m_textureID);
}

void Texture::bind() const {
	glBindTexture(GL_TEXTURE_2D, m_textureID);
}

void Texture::unbind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}