// ---------------------------------------------------------------------------

// --- Includes --------------------------------------------------------------
#include "../EsgiGL/EsgiGL.h"

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>
#include <time.h> 
#include <map>
#include <string>

#include "../EsgiGL/Common/matrix.h"
#include "../EsgiGL/EsgiShader.h"
#include "../EsgiGL/common/EsgiTga.h"

#define PI 3.14159265

// --- Globales --------------------------------------------------------------

struct Vertex
{
	vec3 position;
	vec2 texcoords;
};



struct _Particle
{
	bool emited;
	vec2 position;
	vec3 direction;
	int type;
	float ttlMax;
	float ttlCurr;
};

struct Emiter
{
	float emissionTime;
	vec2 position;
	vec3 direction;
	int type;
	float ttlMax;
	float ttlCurr;
	int nbParticle;
	std::vector<_Particle> particles;
};

void mouseFunc(int button, int state, int mouseX, int mouseY);
void drawFire(vec2 pos, GLuint programObject, GLint &sampler_uniform, mat4 &worldMatrix, GLint &world_uniform);
void drawIce(vec2 pos, GLuint programObject, GLint &sampler_uniform, mat4 &worldMatrix, GLint &world_uniform);
void LoadTexture(const char* texName);
void setVertices();
void createShader(const char* shaderName, const char* vertexShaderPath, const char* fragmentShaderPath);

static const float WINDOW_WIDTH = 960.f;
static const float WINDOW_HEIGHT = 600.f;

static const int MAX_PARTICLE = 500;
static const int MIN_PARTICLE = 100;

static const int MAX_PARTICLE_WATER = 500;
static const int MIN_PARTICLE_WATER = 100;

static const int VertexCount = 6;
static const int MinSize = 5.f;
static const int MaxSize = 25.f;
Vertex vertices[VertexCount];

std::vector<Emiter> ParticleEmiters; 

GLuint textureID;

bool click = false;

float t=0; 

std::map<const char*, GLuint> textures;
std::map<const char*, EsgiShader> shaders;

EsgiShader shaderObject;
EsgiShader skyShader;
EsgiShader cloudsShader;
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
	glClearColor(0.3f, 0.3f, 0.3f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	

	GLuint programObject = shaders["Sky"].GetProgram();
	glUseProgram(programObject);

	GLint position_attr = glGetAttribLocation(programObject, "a_Position");
	glVertexAttribPointer(position_attr, 3, GL_FLOAT, false, sizeof(Vertex), vertices);

	GLint texcoord_attr = glGetAttribLocation(programObject, "a_TexCoords");
	glVertexAttribPointer(texcoord_attr, 2, GL_FLOAT, false, sizeof(Vertex), &vertices[0].texcoords);

	glEnableVertexAttribArray(position_attr);
	glEnableVertexAttribArray(texcoord_attr);

	//Projection Orthogonale
	mat4 projectionMatrix = esgiOrtho(0.f, 800.f, 600.f, 0.f, 0.f, 1.f);
	GLint projection_uniform = glGetUniformLocation(programObject, "u_ProjectionMatrix");
	
	glUniformMatrix4fv(projection_uniform, 1, false, &projectionMatrix.I.x);

	float angle = 0.f;
	// Transformation local -> monde (model to world)
	mat4 worldMatrix;
	worldMatrix.Identity();

	worldMatrix = esgiRotateZ(angle);

	GLint world_uniform = glGetUniformLocation(programObject, "u_WorldMatrix");

	// activer le transfert de l'attribut
	glEnableVertexAttribArray(position_attr);
	glEnableVertexAttribArray(texcoord_attr);

	worldMatrix.I.x = 800.f / 1.6f;
	worldMatrix.J.y = 600.f /1.6f;

	GLint sampler_uniform;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 3);
	sampler_uniform = glGetUniformLocation(programObject, "u_Texture");
	glUniform1i(sampler_uniform, 0);
	worldMatrix.T.set(400.f, 300.f, 0.f, 1.0f);
	glUniformMatrix4fv(world_uniform, 1, false, &worldMatrix.I.x);
	glDrawArrays(GL_TRIANGLES, 0, VertexCount);

	glDisableVertexAttribArray(position_attr);
	glDisableVertexAttribArray(texcoord_attr);
	shaders["Sky"].Unbind();

	programObject = shaders["Clouds"].GetProgram();
	glUseProgram(programObject);

	GLint time_uniform = glGetUniformLocation(programObject, "u_Time");

	position_attr = glGetAttribLocation(programObject, "a_Position");
	glVertexAttribPointer(position_attr, 3, GL_FLOAT, false, sizeof(Vertex), vertices);

	texcoord_attr = glGetAttribLocation(programObject, "a_TexCoords");
	glVertexAttribPointer(texcoord_attr, 2, GL_FLOAT, false, sizeof(Vertex), &vertices[0].texcoords);

	projectionMatrix = esgiOrtho(0.f, 800.f, 600.f, 0.f, 0.f, 1.f);
	projection_uniform = glGetUniformLocation(programObject, "u_ProjectionMatrix");
	
	glUniformMatrix4fv(projection_uniform, 1, false, &projectionMatrix.I.x);

	angle = 0.f;
	// Transformation local -> monde (model to world)
	worldMatrix;
	worldMatrix.Identity();

	worldMatrix = esgiRotateZ(angle);

	world_uniform = glGetUniformLocation(programObject, "u_WorldMatrix");

	glEnableVertexAttribArray(position_attr);
	glEnableVertexAttribArray(texcoord_attr);

	glUniform1f(time_uniform, t);

	worldMatrix.I.x = 800.f / 1.6f;
	worldMatrix.J.y = 600.f /1.6f;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 4);
	sampler_uniform = glGetUniformLocation(programObject, "u_Texture");
	glUniform1i(sampler_uniform, 0);
	worldMatrix.T.set(400.f, 300.f, 0.f, 1.0f);
	glUniformMatrix4fv(world_uniform, 1, false, &worldMatrix.I.x);
	glDrawArrays(GL_TRIANGLES, 0, VertexCount);

	glDisableVertexAttribArray(position_attr);
	glDisableVertexAttribArray(texcoord_attr);
	shaders["Clouds"].Unbind();

	programObject = shaders["Particle"].GetProgram();
	glUseProgram(programObject);


	// affecter les valeurs des vertices au vertex shader
	position_attr = glGetAttribLocation(programObject, "a_Position");
	glVertexAttribPointer(position_attr, 3, GL_FLOAT, false, sizeof(Vertex), vertices);

	texcoord_attr = glGetAttribLocation(programObject, "a_TexCoords");
	glVertexAttribPointer(texcoord_attr, 2, GL_FLOAT, false, sizeof(Vertex), &vertices[0].texcoords);

	GLint ttl_uniform = glGetUniformLocation(programObject, "u_ttl");
	GLint ttl_max_uniform = glGetUniformLocation(programObject, "u_ttl_max");

	GLint dir_uniform = glGetUniformLocation(programObject, "u_dir");
	


	//affectation de l'unite de texture
	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 1);*/
	/*GLint sampler_uniform = glGetUniformLocation(programObject, "u_Texture");
	glUniform1i(sampler_uniform, 0);*/

	//GLint sampler_uniform;


	//Projection Orthogonale
	projectionMatrix = esgiOrtho(0.f, 800.f, 600.f, 0.f, 0.f, 1.f);
	projection_uniform = glGetUniformLocation(programObject, "u_ProjectionMatrix");
	
	glUniformMatrix4fv(projection_uniform, 1, false, &projectionMatrix.I.x);

//	float angle = 0.f;
	// Transformation local -> monde (model to world)
//	mat4 worldMatrix;
	worldMatrix.Identity();

	worldMatrix = esgiRotateZ(angle);

	world_uniform = glGetUniformLocation(programObject, "u_WorldMatrix");

	// activer le transfert de l'attribut
	glEnableVertexAttribArray(position_attr);
	glEnableVertexAttribArray(texcoord_attr);

	for(int i = 0; i < ParticleEmiters.size(); i++)
	{
		glUniform1f(ttl_uniform, ParticleEmiters[i].ttlCurr);
		glUniform1f(ttl_max_uniform, ParticleEmiters[i].ttlMax);

		if(ParticleEmiters[i].type == 1)
		{
			for(int j = 0; j < ParticleEmiters[i].nbParticle; j++)
			{
				glUniform3f(dir_uniform, ParticleEmiters[i].particles[j].direction.x, ParticleEmiters[i].particles[j].direction.y, ParticleEmiters[i].particles[j].direction.z);
				drawFire(ParticleEmiters[i].particles[j].position, programObject, sampler_uniform, worldMatrix, world_uniform);
			}
		}
		if(ParticleEmiters[i].type == 2)
		{
			glUniform3f(dir_uniform, ParticleEmiters[i].direction.x, ParticleEmiters[i].direction.y, ParticleEmiters[i].direction.z);
			drawIce(ParticleEmiters[i].position, programObject, sampler_uniform, worldMatrix, world_uniform);
		}
	}

	//termine l'usage de l'attribut
	glDisableVertexAttribArray(position_attr);
	glDisableVertexAttribArray(texcoord_attr);

	shaders["Particle"].Unbind();	
}

//
// Initialise les shader & mesh
//
bool Setup()
{	
	setVertices();

	createShader("Particle", "particle.vert", "particle.frag");
	createShader("Sky", "sky.vert", "sky.frag");
	createShader("Clouds", "clouds.vert", "clouds.frag");

	LoadTexture("fire.tga");
	LoadTexture("ice.tga");
	LoadTexture("night_sky.tga");
	LoadTexture("clouds.tga");

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
	srand(time(0));
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
	temp.ttlMax = rand() % (3 - 1) + 1;
	std::cout << temp.ttlMax << std::endl;
	temp.ttlCurr = temp.ttlMax;
	switch (button)
	{
		case ESGI_LEFT_BUTTON :
				if(false == click)
				{
					std::cout << "click" << std::endl;
					int nbParticle = rand() % (MAX_PARTICLE - MIN_PARTICLE) + MIN_PARTICLE;
					temp.nbParticle = nbParticle;
					std::cout << temp.nbParticle << std::endl;
					for(int i = 0; i < temp.nbParticle; i++)
					{
						int speed = rand() % (20 - 3) + 3;
						int angle = rand() % 360 ;
						float x = 2 * cos((float)angle * PI / 180.0);
						float y = 2 * sin((float)angle * PI / 180.0);
						vec3 dir(x, y, (float)speed);
						_Particle part;
						part.position = temp.position;
						part.direction = dir;
						part.ttlMax = rand() % (3 - 1) + 1;
						part.ttlCurr = part.ttlMax;
						part.type = 1;
						part.emited = false;
						temp.particles.push_back(part);
					}

					//temp.direction = dir; 
					temp.type = 1;
					ParticleEmiters.push_back(temp);
				}

				break;
		case ESGI_RIGHT_BUTTON :
			if(false == click)
				{
				int speed = rand() % 20;
				int angle = rand() % 45;
				std::cout << angle << std::endl;
				std::cout << (float)angle << std::endl;
				float x = 2 * cos((float)angle * PI / 180.0);
				float y = 2 * sin((float)angle * PI / 180.0);
				vec3 dir(x, y, (float)speed);
				std::cout << dir.x << ", " << dir.y << std::endl;
				temp.direction = dir;
				temp.type = 2;
				ParticleEmiters.push_back(temp);
				}
			break;
	}
	click = !click;
}

void drawFire(vec2 pos, GLuint programObject, GLint &sampler_uniform, mat4 &worldMatrix, GLint &world_uniform)
{	
	worldMatrix.I.x = 5.f;
	worldMatrix.J.y = 5.f;
	
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
	worldMatrix.I.x = 5.f;
	worldMatrix.J.y = 5.f;
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 2);
	sampler_uniform = glGetUniformLocation(programObject, "u_Texture");
	glUniform1i(sampler_uniform, 0);
	worldMatrix.T.set(pos.x, pos.y, 0.f, 1.f);
	glUniformMatrix4fv(world_uniform, 1, false, &worldMatrix.I.x);
	glDrawArrays(GL_TRIANGLES, 0, VertexCount);
}

void LoadTexture(const char* texName)
{
	EsgiTexture *tex = esgiReadTGAFile(texName);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, /* Lod */
		tex->internalFormat, tex->width, tex->height,
		0, tex->format, GL_UNSIGNED_BYTE, tex->texels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void setVertices()
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
}

void createShader(const char* shaderName, const char* vertexShaderPath, const char* fragmentShaderPath)
{
	EsgiShader shader;

	shader.LoadVertexShader(vertexShaderPath);
	shader.LoadFragmentShader(fragmentShaderPath);
	shader.Create();

	shaders.insert(std::pair<const char*,EsgiShader>(shaderName,shader));

	//shaders.emplace(shaderName, shader);
}