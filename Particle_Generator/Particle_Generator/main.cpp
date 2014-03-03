//#include "../EsgiGL/EsgiGL.h"
//
//#include <cstdio>
//#include <cstdlib>
//#include <cmath>
//
//#include "../EsgiGL/Common/matrix.h"
//#include "../EsgiGL/EsgiShader.h"
//#include "../EsgiGL/common/EsgiTga.h"
//
//#include <iostream>
//
//bool loadTexture(const char* texPath);
//void drawSprite(GLuint textureID);
//
//EsgiShader shader;
//
//struct PointSprites
//{
//	vec3 *vertices;
//	unsigned int count;
//	GLuint textureId;
//};
//
//PointSprites sprites;
//
//float time = 0.f;
//
//void Update(float elapsedTime)
//{
//	time += elapsedTime;
//}
//
//void Draw()
//{
//	glClearColor(0.5f, 0.5f, 0.5f, 1.f);
//	glClear(GL_COLOR_BUFFER_BIT);
//
//#ifndef ESGI_GLES_20
//	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
//	glEnable(GL_POINT_SPRITE);
//#endif
//	glEnable(GL_TEXTURE_2D);
//
//	drawSprite(1);
//	//drawSprite(2);
//
//}
//
//bool Setup()
//{
//	loadTexture("fire.tga");
//	sprites.vertices[0] = vec3(0.f, 0.f, 0.f);
//	loadTexture("fire.tga");
//	sprites.vertices[1] = vec3(0.f, 0.8f, 0.f);
//
//	/*loadTexture("ice.tga");
//	sprites.vertices[1] = vec3(0.0f, 0.f, 0.f);*/
//
//	shader.LoadVertexShader("particle.vert");
//	shader.LoadFragmentShader("particle.frag");
//
//	shader.Create();
//
//
//
//	//shader.LoadVertexShader("particle.vert");
//	//shader.LoadFragmentShader("particle.frag");
//
//	//shader.Create();
//
//
//	return true;
//}
//
//void Clean()
//{
//	if(sprites.vertices)
//	{
//		delete[] sprites.vertices;
//		sprites.vertices = NULL;
//		sprites.count = 0;
//	}
//	if (sprites.textureId)
//	{
//		glDeleteTextures(1, &sprites.textureId);
//	}
//
//	shader.Destroy();
//}
//
//int main (int argc, char *argv[])
//{
//	EsgiGLApplication esgi;
//
//	esgi.InitWindowPosition(0, 0);
//	esgi.InitWindowSize(800, 600);
//	esgi.InitDisplayMode(ESGI_WINDOW_RGBA|ESGI_WINDOW_DOUBLEBUFFER);
//	esgi.CreateWindow("Particle Generator", ESGI_WINDOW_CENTERED);
//
//	esgi.IdleFunc(&Update);
//	esgi.DisplayFunc(&Draw);
//	esgi.InitFunc(&Setup);
//	esgi.CleanFunc(&Clean);
//
//	esgi.MainLoop();
//
//	return 0;
//}
//
//bool loadTexture(const char* texPath)
//{
//	EsgiTexture *Tex = esgiReadTGAFile(texPath);
//
//	if(Tex == NULL)
//	{
//		return false;
//	}
//	glGenTextures(1, &sprites.textureId);
//	glBindTexture(GL_TEXTURE_2D, sprites.textureId);
//	glTexImage2D(GL_TEXTURE_2D, 0, 
//		Tex->internalFormat, Tex->width, Tex->height
//		, 0, Tex->format, Tex->datatype, Tex->texels); 
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//
//	delete[] Tex->texels;
//	delete[] Tex;
//
//	sprites.count += 1;
//	sprites.vertices = new vec3[sprites.count];
//	return true;
//}
//
//void drawSprite(GLuint textureID)
//{
//	glBindTexture(GL_TEXTURE_2D, textureID);
//	glActiveTexture(GL_TEXTURE0);
//
//	GLuint programObject = shader.GetProgram();
//	glUseProgram(programObject);
//
//	GLuint timeUniform = glGetUniformLocation(programObject, "u_Time");
//	glUniform1f(timeUniform, time);
//
//	GLuint sampler_uniform = glGetUniformLocation(programObject, "u_Texture");
//	glUniform1i(sampler_uniform, 0);
//
//	GLint point_attribute = glGetAttribLocation(programObject, "a_Point");
//	glVertexAttribPointer(point_attribute, 3, GL_FLOAT, GL_FALSE, sizeof(PointSprites), sprites.vertices);
//
//	glEnableVertexAttribArray(point_attribute);
//	glDrawArrays(GL_POINTS, 0, 1);
//	glDisableVertexAttribArray(point_attribute);
//	glUseProgram(0);
//	glBindTexture(GL_TEXTURE_2D, 0);
//	glDisable(GL_TEXTURE_2D);
//}

// ---------------------------------------------------------------------------
//
// ESGI OpenGL (ES) 2.0 Framework
// Malek Bengougam, 2012							malek.bengougam@gmail.com
//
// ---------------------------------------------------------------------------

// --- Includes --------------------------------------------------------------
#include "../EsgiGL/EsgiGL.h"

#include <cstdio>
#include <cstdlib>
#include <cmath>

#include "../EsgiGL/Common/matrix.h"
#include "../EsgiGL/EsgiShader.h"
#include "../EsgiGL/common/EsgiTga.h"


// --- Globales --------------------------------------------------------------

struct Vertex
{
	vec3 position;
	vec2 texcoords;
};

static const int VertexCount = 6;
Vertex vertices[VertexCount];

GLuint textureID;

EsgiShader shaderObject;
// --- Fonctions -------------------------------------------------------------

// ---

void Update(float elapsedTime)
{
}

void Draw()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// efface le color buffer
	glClearColor(0.5f, 0.5f, 0.5f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	//GLuint code erreur 0
	//GLint code erreur -1
	//GLuint programObject = shaderObject.Bind();

	GLuint programObject = shaderObject.GetProgram();
	glUseProgram(programObject);


	// affecter les valeurs des vertices au vertex shader
	GLint position_attr = glGetAttribLocation(programObject, "a_Position");
	glVertexAttribPointer(position_attr, 3, GL_FLOAT, false, sizeof(Vertex), vertices);

	GLint texcoord_attr = glGetAttribLocation(programObject, "a_TexCoords");
	glVertexAttribPointer(texcoord_attr, 2, GL_FLOAT, false, sizeof(Vertex), &vertices[0].texcoords);


	//affectation de l'unite de texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 1);
	GLint sampler_uniform = glGetUniformLocation(programObject, "u_Texture");
	glUniform1i(sampler_uniform, 0);

	//Projection Orthogonale
	mat4 projectionMatrix = esgiOrtho(0.f, 800.f, 600.f, 0.f, 0.f, 1.f);
	GLint projection_uniform = glGetUniformLocation(programObject, "u_ProjectionMatrix");
	
	glUniformMatrix4fv(projection_uniform, 1, false, &projectionMatrix.I.x);

	float angle = 45.f;
	// Transformation local -> monde (model to world)
	mat4 worldMatrix;
	worldMatrix.Identity();

	worldMatrix = esgiRotateZ(angle);
	
	worldMatrix.I.x *= 100.f;
	worldMatrix.J.y *= 100.f;

	GLint world_uniform = glGetUniformLocation(programObject, "u_WorldMatrix");

	// activer le transfert de l'attribut
	glEnableVertexAttribArray(position_attr);
	glEnableVertexAttribArray(texcoord_attr);

	worldMatrix.T.set(0.f, 0.f, 0.f, 1.f);

	glUniformMatrix4fv(world_uniform, 1, false, &worldMatrix.I.x);
	glDrawArrays(GL_TRIANGLES, 0, VertexCount);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 2);
	sampler_uniform = glGetUniformLocation(programObject, "u_Texture");
	glUniform1i(sampler_uniform, 0);

	worldMatrix.T.set(100.f, 100.f, 0.f, 1.f);

	glUniformMatrix4fv(world_uniform, 1, false, &worldMatrix.I.x);
	glDrawArrays(GL_TRIANGLES, 0, VertexCount);

	//termine l'usage de l'attribut
	glDisableVertexAttribArray(position_attr);
	glDisableVertexAttribArray(texcoord_attr);

	shaderObject.Unbind();
	//glUseProgram(0);
}

//
// Initialise les shader & mesh
//
bool Setup()
{	
	//triangle bas
	vertices[0].position = vec3(-0.8f, -0.8f, 0.0f);
	vertices[0].texcoords = vec2(0.0f, 0.0f);
	vertices[1].position = vec3(0.8f, -0.8f, 0.0f);
	vertices[1].texcoords = vec2(1.0f, 0.0f);
	vertices[2].position = vec3(-0.8f, 0.8f, 0.0f);
	vertices[2].texcoords = vec2(0.0f, 1.0f);
	//triangle haut
	vertices[3].position = vec3(-0.8f, 0.8f, 0.0f);
	vertices[3].texcoords = vec2(0.0f, 1.0f);
	vertices[4].position = vec3(0.8f, -0.8f, 0.0f);
	vertices[4].texcoords = vec2(1.0f, 0.0f);
	vertices[5].position = vec3(0.8f, 0.8f, 0.0f);
	vertices[5].texcoords = vec2(1.0f, 1.0f);



	shaderObject.LoadVertexShader("particle.vert");
	shaderObject.LoadFragmentShader("particle.frag");
	shaderObject.Create();

	EsgiTexture *fireTGA = esgiReadTGAFile("fire.tga");
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, /* Lod */
		fireTGA->internalFormat, fireTGA->width, fireTGA->height,
		0, fireTGA->format, GL_UNSIGNED_BYTE, fireTGA->texels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		EsgiTexture *iceTGA = esgiReadTGAFile("ice.tga");
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, /* Lod */
		iceTGA->internalFormat, iceTGA->width, iceTGA->height,
		0, iceTGA->format, GL_UNSIGNED_BYTE, iceTGA->texels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	return true;
}

//
// Libere la memoire occupee
//
void Clean()
{
	glDeleteTextures(1, &textureID);
	shaderObject.Destroy();

}

// 
//
//
int main(int argc, char *argv[])
{
	EsgiGLApplication esgi;
    
	esgi.InitWindowPosition(0, 0);
	esgi.InitWindowSize(800, 600);
	esgi.InitDisplayMode(ESGI_WINDOW_RGBA|ESGI_WINDOW_DEPTH|ESGI_WINDOW_DOUBLEBUFFER);
	esgi.CreateWindow("Particle Generator", ESGI_WINDOW_CENTERED);
	
    esgi.IdleFunc(&Update);
	esgi.DisplayFunc(&Draw);
    esgi.InitFunc(&Setup);
    esgi.CleanFunc(&Clean);
    
	esgi.MainLoop();
    
    return 0;
}