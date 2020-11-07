#pragma once
#include <glm/glm.hpp>
#include <GL\glew.h>

#include <vector>

struct Vertex
{
	glm::vec3 pos;
	glm::vec3 color;
};

class Cube
{
private:
	// Typedefs
	using Point = glm::vec3;

private:
	// CTOR, DTOR
	Cube();

public:
	//Singleton getter
	static Cube* instance();

	// Methods
	static void render(GLuint shaderID);
	static void destroy();

private:
	
	//Singleton data
	static Cube* instance_;

	//IDs
	GLuint vaoID;
	GLuint vboID;
	GLuint ibID;

	//Private Methods
	glm::vec3 posToCol(const glm::vec3& pos);
};