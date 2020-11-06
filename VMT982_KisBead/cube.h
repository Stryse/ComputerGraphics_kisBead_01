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

public:
	// CTOR, DTOR
	Cube();
	~Cube();

	// Methods
	glm::vec3 posToCol(const glm::vec3& pos);
	void render() const;

private:

	//Props
	const GLfloat edgeWidth;

	//BUFFERS
	//Vertex
	std::vector<Vertex> vertices;
	//Index
	std::vector<GLushort> indices;

	//IDs
	GLuint vaoID;
	GLuint vboID;
	GLuint ibID;
};