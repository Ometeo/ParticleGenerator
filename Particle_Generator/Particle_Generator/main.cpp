// ---------------------------------------------------------------------------

// --- Includes --------------------------------------------------------------
#include "../EsgiGL/EsgiGL.h"

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>
#include <time.h> 

#include "../EsgiGL/Common/matrix.h"
#include "../EsgiGL/EsgiShader.h"
#include "../EsgiGL/common/EsgiTga.h"


// --- Globales --------------------------------------------------------------

struct Vertex
{
	vec3 position;
	vec2 texcoords;
};

struct Emiter
{
	vec2 position;
	int type;
	float ttlMax;
	float ttlCurr;
	int maxNbParticle;
	int nbParticle;

};

struct Particle
{
	vec2 position;
	int type;
	float ttlMax;
	float ttlCurr;

};


void mouseFunc(int button, int state, int mouseX, int mouseY);
void drawFire(vec2 pos, GLuint programObject, GLint &sampler_uniform, mat4 &worldMatrix, GLint &world_uniform);
void drawIce(vec2 pos, GLuint programObject, GLint &sampler_uniform, mat4 &worldMatrix, GLint &world_uniform);

static const int VertexCount = 6;
static const int MinSize = 5.f;
static const int MaxSize = 25.f;
Vertex vertices[VertexCount];

std::vector<Emiter> ParticleEmiters; 

GLuint textureID;

bool click = false;

//variables for high performance timer
LARGE_INTEGER frequency;        // ticks per second
LARGE_INTEGER t1, t2;           // ticks
double frameTimeQP=0;
float frameTime =0 ;

//for fps calculation
float startTime =0, fps=0 ;
int totalFrames=0;

float t=0; 

EsgiShader shaderObject;
// --- Fonctions -------------------------------------------------------------

// ---

void Update(float elapsedTime)
{
	t += elapsedTime;
	for(int i = 0; i < ParticleEmiters.size(); i++)
	{
		ParticleEmiters[i].ttlCurr -= elapsedTime;
		if(ParticleEmiters[i].ttlCurr <= 0)
			ParticleEmiters.erase(ParticleEmiters.begin() + i);
	}
}

void Draw()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// efface le color buffer
	glClearColor(0.5f, 0.5f, 0.5f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	GLuint programObject = shaderObject.GetProgram();
	glUseProgram(programObject);


	// affecter les valeurs des vertices au vertex shader
	GLint position_attr = glGetAttribLocation(programObject, "a_Position");
	glVertexAttribPointer(position_attr, 3, GL_FLOAT, false, sizeof(Vertex), vertices);

	GLint texcoord_attr = glGetAttribLocation(programObject, "a_TexCoords");
	glVertexAttribPointer(texcoord_attr, 2, GL_FLOAT, false, sizeof(Vertex), &vertices[0].texcoords);

	GLint ttl_uniform = glGetUniformLocation(programObject, "u_ttl");
	GLint ttl_max_uniform = glGetUniformLocation(programObject, "u_ttl_max");

	GLint dir_uniform = glGetUniformLocation(programObject, "u_dir");
	


	//affectation de l'unite de texture
	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 1);*/
	/*GLint sampler_uniform = glGetUniformLocation(programObject, "u_Texture");
	glUniform1i(sampler_uniform, 0);*/

	GLint sampler_uniform;


	//Projection Orthogonale
	mat4 projectionMatrix = esgiOrtho(0.f, 800.f, 600.f, 0.f, 0.f, 1.f);
	GLint projection_uniform = glGetUniformLocation(programObject, "u_ProjectionMatrix");
	
	glUniformMatrix4fv(projection_uniform, 1, false, &projectionMatrix.I.x);

	float angle = 45.f;
	// Transformation local -> monde (model to world)
	mat4 worldMatrix;
	worldMatrix.Identity();

	worldMatrix = esgiRotateZ(angle);

	GLint world_uniform = glGetUniformLocation(programObject, "u_WorldMatrix");

	// activer le transfert de l'attribut
	glEnableVertexAttribArray(position_attr);
	glEnableVertexAttribArray(texcoord_attr);

	for(int i = 0; i < ParticleEmiters.size(); i++)
	{
		glUniform1f(ttl_uniform, ParticleEmiters[i].ttlCurr);
		glUniform1f(ttl_max_uniform, ParticleEmiters[i].ttlMax);

		if(ParticleEmiters[i].type == 1)
		{
			drawFire(ParticleEmiters[i].position, programObject, sampler_uniform, worldMatrix, world_uniform);
		}
		if(ParticleEmiters[i].type == 2)
		{
			drawIce(ParticleEmiters[i].position, programObject, sampler_uniform, worldMatrix, world_uniform);
		}
	}

	/*worldMatrix.T.set(50.f, 50.f, 0.f, 1.f);

	glUniformMatrix4fv(world_uniform, 1, false, &worldMatrix.I.x);
	glDrawArrays(GL_TRIANGLES, 0, VertexCount);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 2);
	sampler_uniform = glGetUniformLocation(programObject, "u_Texture");
	glUniform1i(sampler_uniform, 0);

	worldMatrix.T.set(200.f, 200.f, 0.f, 1.f);

	glUniformMatrix4fv(world_uniform, 1, false, &worldMatrix.I.x);
	glDrawArrays(GL_TRIANGLES, 0, VertexCount);*/

	//drawFire(400.f, 400.f, programObject, sampler_uniform, worldMatrix, world_uniform);

	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 1);
	sampler_uniform = glGetUniformLocation(programObject, "u_Texture");
	glUniform1i(sampler_uniform, 0);
	worldMatrix.T.set(400.f, 400.f, 0.f, 1.f);
	glUniformMatrix4fv(world_uniform, 1, false, &worldMatrix.I.x);
	glDrawArrays(GL_TRIANGLES, 0, VertexCount);*/

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
	esgi.MouseFunc(&mouseFunc);

    
	esgi.MainLoop();
    
    return 0;
}

void mouseFunc(int button, int state, int mouseX, int mouseY)
{
	Emiter temp;
	temp.position.x = mouseX;
	temp.position.y = mouseY;
	temp.ttlMax = 5.0f;
	temp.ttlCurr = temp.ttlMax;
	switch (button)
	{
		case ESGI_LEFT_BUTTON :
				if(false == click)
				{
				temp.type = 1;
				ParticleEmiters.push_back(temp);
				}

				break;
		case ESGI_RIGHT_BUTTON :
			if(false == click)
				{
				temp.type = 2;
				ParticleEmiters.push_back(temp);
				}
			break;
	}
	click = !click;
}

void drawFire(vec2 pos, GLuint programObject, GLint &sampler_uniform, mat4 &worldMatrix, GLint &world_uniform)
{	
	worldMatrix.I.x = 10.f;
	worldMatrix.J.y = 10.f;
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 1);
	sampler_uniform = glGetUniformLocation(programObject, "u_Texture");
	glUniform1i(sampler_uniform, 0);
	worldMatrix.T.set(pos.x, pos.y, 0.f, 1.0f);
	glUniformMatrix4fv(world_uniform, 1, false, &worldMatrix.I.x);
	glDrawArrays(GL_TRIANGLES, 0, VertexCount);
}

void drawIce(vec2 pos, GLuint programObject, GLint &sampler_uniform, mat4 &worldMatrix, GLint &world_uniform)
{
	worldMatrix.I.x = 10.f;
	worldMatrix.J.y = 10.f;
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 2);
	sampler_uniform = glGetUniformLocation(programObject, "u_Texture");
	glUniform1i(sampler_uniform, 0);
	worldMatrix.T.set(pos.x, pos.y, 0.f, 1.f);
	glUniformMatrix4fv(world_uniform, 1, false, &worldMatrix.I.x);
	glDrawArrays(GL_TRIANGLES, 0, VertexCount);
}