#include "MyApp.h"
#include "GLUtils.hpp"
#include <math.h>


CMyApp::CMyApp(void) : m_programID(0), m_loc_mvp(0), m_loc_alpha(0), m_loc_colorFade(0), colStartTime(0) {
}

CMyApp::~CMyApp(void)
{
	Cube::destroy();
}

bool CMyApp::Init()
{
	// SETUP
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	
	// Load Geometries
	Cube::instance();
	//

	// Load Shaders
	setupShader();
	//

	// Load Matrices
	// Projection matrix
	m_matProj = glm::perspective( 45.0f, 640/480.0f, 1.0f, 1000.0f );

	// Uniforms
	m_loc_mvp   = glGetUniformLocation( m_programID, "MVP");
	m_loc_alpha = glGetUniformLocation(m_programID, "alpha");
	
	m_loc_colorFade = glGetUniformLocation(m_programID, "colorFade");
	glUniform1i(m_loc_colorFade, 0);


	return true;
}

void CMyApp::Clean()
{
	glDeleteProgram( m_programID );
}

void CMyApp::Update()
{
	// View Matrix
	m_matView = glm::lookAt(glm::vec3( 5,  10, 25),
							glm::vec3( 0,  0,  0 ),
							glm::vec3( 0,  1,  0 ));
}


void CMyApp::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float theta = SDL_GetTicks() / 2000.f;
	float phi = SDL_GetTicks() / 1650.f;

	m_matWorld =
		glm::rotate(theta, glm::vec3(cosf(phi), 0.f, sinf(phi))) *
		glm::translate(glm::vec3(R, 0, 0)) * glm::mat4(1.0f);

	renderComposition(m_programID,m_matWorld);
}

void CMyApp::setupShader()
{
	GLuint vs_ID = loadShader(GL_VERTEX_SHADER,   "myVert.vert");
	GLuint fs_ID = loadShader(GL_FRAGMENT_SHADER, "myFrag.frag");

	m_programID = glCreateProgram();

	//Attach
	glAttachShader(m_programID, vs_ID);
	glAttachShader(m_programID, fs_ID);

	//Bind attribs
	glBindAttribLocation(m_programID,0,"vs_in_pos");
	glBindAttribLocation(m_programID, 1, "vs_in_col");

	//Link
	glLinkProgram(m_programID);

	//Log
	GLint infoLogLength = 0, result = 0;

	glGetProgramiv(m_programID, GL_LINK_STATUS, &result);
	glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (GL_FALSE == result || infoLogLength != 0)
	{
		std::vector<char> error_massage(infoLogLength);
		glGetProgramInfoLog(m_programID, infoLogLength, NULL, error_massage.data());

		std::cout << "Hiba a shader letrehozasakor: " << error_massage.data() << std::endl;
	}
	glDeleteShader(vs_ID);
	glDeleteShader(fs_ID);
}

void CMyApp::renderTransformedCube(GLuint shaderID,const glm::mat4& mat)
{
	glm::mat4 matWorld = mat * glm::mat4(1.0f);
	glm::mat4 mvp = m_matProj * m_matView * matWorld;
	glUniformMatrix4fv(m_loc_mvp, 1, GL_FALSE, &(mvp[0][0]));

	float alpha = sinf((SDL_GetTicks() - colStartTime) * (2.f * M_PI / 4000.f));
	
	glUniform1f(m_loc_alpha, alpha);
	Cube::render(shaderID);
}

void CMyApp::renderWShape(GLuint shaderID,const glm::mat4& mat)
{
	renderTransformedCube(shaderID, mat * glm::mat4(1.f));
	renderTransformedCube(shaderID, mat * glm::translate(glm::vec3( 0,-1, 0)));
	renderTransformedCube(shaderID, mat * glm::translate(glm::vec3(-1,-1, 0)));
	renderTransformedCube(shaderID, mat * glm::translate(glm::vec3( 1, 0, 0)));
	renderTransformedCube(shaderID, mat * glm::translate(glm::vec3( 1, 1, 0)));
}

void CMyApp::renderComposition(GLuint shaderID, const glm::mat4& mat)
{
	renderWShape(shaderID, mat * glm::translate(glm::vec3(-dPos, -dPos, +dPos)));
	renderWShape(shaderID, mat * glm::translate(glm::vec3(+dPos, -dPos, +dPos)));
	renderWShape(shaderID, mat * glm::translate(glm::vec3(+dPos, +dPos, +dPos)));
	renderWShape(shaderID, mat * glm::translate(glm::vec3(-dPos, +dPos, +dPos)));
	renderWShape(shaderID, mat * glm::translate(glm::vec3(-dPos, -dPos, -dPos)));
	renderWShape(shaderID, mat * glm::translate(glm::vec3(+dPos, -dPos, -dPos)));
	renderWShape(shaderID, mat * glm::translate(glm::vec3(+dPos, +dPos, -dPos)));
	renderWShape(shaderID, mat * glm::translate(glm::vec3(-dPos, +dPos, -dPos)));
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	if (key.type == SDL_KEYDOWN)
	{
		glUseProgram(m_programID);

		GLint currColor;
		glGetUniformiv(m_programID, m_loc_colorFade, &currColor);

		switch (key.keysym.sym)
		{
		case SDLK_1:
			if (currColor != Color::RED)
			{
				colStartTime = SDL_GetTicks();
				glUniform1i(m_loc_colorFade, Color::RED);
			}
			break;
		case SDLK_2:
			if (currColor != Color::GREEN)
			{
				colStartTime = SDL_GetTicks();
				glUniform1i(m_loc_colorFade, Color::GREEN);
			}
			break;
		case SDLK_3:
			if (currColor != Color::BLUE)
			{
				colStartTime = SDL_GetTicks();
				glUniform1i(m_loc_colorFade, Color::BLUE);
			}
			break;
		default:
			if (currColor != Color::NOCOLOR)
			{
				colStartTime = SDL_GetTicks();
				glUniform1i(m_loc_colorFade, Color::NOCOLOR);
			}
			break;
		}
		glUseProgram(0);
	}
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key){}
void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse){}
void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse){}
void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse){}
void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel){}

void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);
	m_matProj = glm::perspective(45.0f,_w/(float)_h,0.01f,100.0f);
}
