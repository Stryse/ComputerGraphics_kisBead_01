#include "MyApp.h"
#include "GLUtils.hpp"

#include <math.h>

CMyApp::CMyApp(void) : m_programID(0), cube(nullptr), m_loc_mvp(0) {
}

CMyApp::~CMyApp(void)
{
	delete cube;
}

bool CMyApp::Init()
{
	// SETUP
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)
	glCullFace(GL_BACK); // GL_BACK: a kamerától "elfelé" nézõ lapok, GL_FRONT: a kamera felé nézõ lapok
	
	// Load Geometry
	cube = new Cube();
	//

	// shaderek betöltése
	setupShader();
	//

	//
	// egyéb inicializálás
	// vetítési mátrix létrehozása
	m_matProj = glm::perspective( 45.0f, 640/480.0f, 1.0f, 1000.0f );

	// shader-beli transzformációs mátrixok címének lekérdezése
	m_loc_mvp = glGetUniformLocation( m_programID, "MVP");

	return true;
}

void CMyApp::Clean()
{
	glDeleteProgram( m_programID );
}

void CMyApp::Update()
{
	// nézeti transzformáció beállítása
	m_matView = glm::lookAt(glm::vec3( 5,  10, 20),		// honnan nézzük a színteret
							glm::vec3( 0,  0,  0),		// a színtér melyik pontját nézzük
							glm::vec3( 0,  1,  0));		// felfelé mutató irány a világban
}


void CMyApp::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram( m_programID );

	float theta = SDL_GetTicks() / 1500.f;
	float phi   = SDL_GetTicks() / 2500.f;

	m_matWorld = 
		glm::rotate(theta, glm::vec3(cosf(phi), 0.f, sinf(phi))) * 
		glm::translate(glm::vec3(R,0,0)) * glm::mat4(1.0f);

	renderComposition(m_matWorld);
}

void CMyApp::setupShader()
{
	GLuint vs_ID = loadShader(GL_VERTEX_SHADER,   "myVert.vert");
	GLuint fs_ID = loadShader(GL_FRAGMENT_SHADER, "myFrag.frag");

	m_programID = glCreateProgram();

	//Attach
	glAttachShader(m_programID, vs_ID);
	glAttachShader(m_programID, fs_ID);

	// VAO-beli attribútumok hozzárendelése a shader változókhoz
	// FONTOS: linkelés elõtt kell ezt megtenni!
	glBindAttribLocation(m_programID,	// shader azonosítója, amibõl egy változóhoz szeretnénk hozzárendelést csinálni
		0,				// a VAO-beli azonosító index
		"vs_in_pos");	// a shader-beli változónév
	glBindAttribLocation(m_programID, 1, "vs_in_col");

	// illesszük össze a shadereket (kimenõ-bemenõ változók összerendelése stb.)
	glLinkProgram(m_programID);

	// linkeles ellenorzese
	GLint infoLogLength = 0, result = 0;

	glGetProgramiv(m_programID, GL_LINK_STATUS, &result);
	glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (GL_FALSE == result || infoLogLength != 0)
	{
		std::vector<char> error_massage(infoLogLength);
		glGetProgramInfoLog(m_programID, infoLogLength, NULL, error_massage.data());

		std::cout << "Hiba a shader letrehozasakor: " << error_massage.data() << std::endl;
	}
	// mar nincs ezekre szukseg
	glDeleteShader(vs_ID);
	glDeleteShader(fs_ID);
}

void CMyApp::renderTransformedCube(const Cube& cube, const glm::mat4& mat)
{
	glm::mat4 matWorld = mat * glm::mat4(1.0f);
	glm::mat4 mvp = m_matProj * m_matView * matWorld;
	glUniformMatrix4fv(m_loc_mvp, 1, GL_FALSE, &(mvp[0][0]));
	cube.render();
}

void CMyApp::renderWShape(const glm::mat4& mat)
{
	renderTransformedCube(*cube, mat * glm::mat4(1.f));
	renderTransformedCube(*cube, mat * glm::translate(glm::vec3( 0,-1, 0)));
	renderTransformedCube(*cube, mat * glm::translate(glm::vec3(-1,-1, 0)));
	renderTransformedCube(*cube, mat * glm::translate(glm::vec3( 1, 0, 0)));
	renderTransformedCube(*cube, mat * glm::translate(glm::vec3( 1, 1, 0)));
}

void CMyApp::renderComposition(const glm::mat4& mat)
{
	renderWShape(mat * glm::translate(glm::vec3(-dPos, -dPos, +dPos)));
	renderWShape(mat * glm::translate(glm::vec3(+dPos, -dPos, +dPos)));
	renderWShape(mat * glm::translate(glm::vec3(+dPos, +dPos, +dPos)));
	renderWShape(mat * glm::translate(glm::vec3(-dPos, +dPos, +dPos)));
	renderWShape(mat * glm::translate(glm::vec3(-dPos, -dPos, -dPos)));
	renderWShape(mat * glm::translate(glm::vec3(+dPos, -dPos, -dPos)));
	renderWShape(mat * glm::translate(glm::vec3(+dPos, +dPos, -dPos)));
	renderWShape(mat * glm::translate(glm::vec3(-dPos, +dPos, -dPos)));
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{

}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

// a két paraméterbe az új ablakméret szélessége (_w) és magassága (_h) található
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);

	m_matProj = glm::perspective(  45.0f,		// 90 fokos nyilasszog
									_w/(float)_h,	// ablakmereteknek megfelelo nezeti arany
									0.01f,			// kozeli vagosik
									100.0f);		// tavoli vagosik
}
