#pragma once

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

//MyHeaders
#include "cube.h"

enum Color {
	NOCOLOR,
	RED,
	GREEN,
	BLUE
};

class CMyApp
{
public:
	CMyApp(void);
	~CMyApp(void);

	bool Init();
	void Clean();

	void Update();
	void Render();

	void KeyboardDown(SDL_KeyboardEvent&);
	void KeyboardUp(SDL_KeyboardEvent&);
	void MouseMove(SDL_MouseMotionEvent&);
	void MouseDown(SDL_MouseButtonEvent&);
	void MouseUp(SDL_MouseButtonEvent&);
	void MouseWheel(SDL_MouseWheelEvent&);
	void Resize(int, int);

private:
	void setupShader();
	void renderTransformedCube(GLuint shaderID, const glm::mat4& mat = glm::mat4(1.f));
	void renderWShape(GLuint shaderID,const glm::mat4& mat = glm::mat4(1.f));
	void renderComposition(GLuint shaderID, const glm::mat4& mat = glm::mat4(1.f));

protected:

	const float dPos = 2.5f;  // Half of formation cube edge
	const float R = 8.0f;	  // Rotation sphere radius
	GLuint colStartTime;

	// IDS
	GLuint m_programID;

	// Matrices
	glm::mat4 m_matWorld;
	glm::mat4 m_matView;
	glm::mat4 m_matProj;

	// Uniforms
	GLuint m_loc_mvp;
	GLuint m_loc_alpha;
	GLuint m_loc_colorFade;
};
