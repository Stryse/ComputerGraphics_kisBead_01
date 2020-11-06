#include "cube.h"
#include <algorithm>

Cube::Cube() : edgeWidth(1.f),vaoID(0), vboID(0)
{
	GLfloat halfEdge = edgeWidth * 0.5f;
	//SETUP VERTICES

	//POS -- Center is origo (0,0,0)
	glm::vec3 center(0, 0, 0);

	vertices.resize(8);
	vertices[0].pos = { center.x - halfEdge, center.y - halfEdge, center.z + halfEdge }; //Front bottom left
	vertices[1].pos = { center.x + halfEdge, center.y - halfEdge, center.z + halfEdge }; //Front bottom right
	vertices[2].pos = { center.x + halfEdge, center.y + halfEdge, center.z + halfEdge }; // Front top right
	vertices[3].pos = { center.x - halfEdge, center.y + halfEdge, center.z + halfEdge }; // Front top left
									  				
	vertices[4].pos = { center.x - halfEdge, center.y - halfEdge, center.z - halfEdge }; // Back bottom left
	vertices[5].pos = { center.x + halfEdge, center.y - halfEdge, center.z - halfEdge }; // Back bottom right
	vertices[6].pos = { center.x + halfEdge, center.y + halfEdge, center.z - halfEdge }; // Back top right
	vertices[7].pos = { center.x - halfEdge, center.y + halfEdge, center.z - halfEdge }; // Back top left

	indices = {
		//FACES Index buffer
		// front face
		0, 1, 2,
		2, 3, 0,
		// right face
		1, 5, 6,
		6, 2, 1,
		// back face
		7, 6, 5,
		5, 4, 7,
		// left face
		4, 0, 3,
		3, 7, 4,
		// bottom face 
		4, 5, 1,
		1, 0, 4,
		// top face
		3, 2, 6,
		6, 7, 3
	};

	//COL
	for (int i = 0; i < vertices.size(); ++i)
		vertices[i].color = posToCol(vertices[i].pos);

	//SETUP VAO & VBO
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);

	//Populate VBO
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	//Setup Vertex attribs in VAO
	//_POS_
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	//_COLOR_
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));

	//Index buffer
	glGenBuffers(1, &ibID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);
	
	//Disables
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

Cube::~Cube()
{
	glDeleteBuffers(1, &vboID);
	glDeleteBuffers(1, &ibID);
	glDeleteVertexArrays(1, &vaoID);
}

void Cube::render() const
{
	glBindVertexArray(vaoID);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);

	//Disables
	glBindVertexArray(0);
}

glm::vec3 Cube::posToCol(const glm::vec3& pos)
{
	return (glm::normalize(pos) + glm::vec3(1, 1, 1)) * 0.5f;
}
