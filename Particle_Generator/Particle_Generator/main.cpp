/**
 * Particle Generator
 * 4A IJV
 * BIHET Jonathan 
 * DESTRIBATS Thierry
 */


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

#include "PGShader.hpp"
#include "PGShaderHandler.hpp"

#include "globals.h"

void mouseFunc(int button, int state, int mouseX, int mouseY); //Mouse handler
void drawParticle(vec2 pos, GLuint programObject, GLint &sampler_uniform, mat4 &worldMatrix, GLint &world_uniform, GLuint texture); //Draw particle method
void LoadTexture(const char* texName, const char* texFile); //Texture Loader
void setVertices(); //Set the vertices for the sprite

void createParticle(Emiter &em); //Create a single particle from a emiter
void createParticles(Emiter &em); //Create particles from emiter
Emiter createGenericEmiter(int type, vec2 pos); //Create a generic particle emiter
void createFireWorkEmiter(vec2 pos); //Create a FireWork particle emiter
void createWaterEmiter(vec2 pos); //Create a water fountain particle emiter

//Sprite's vertices
static const int VertexCount = 6;
Vertex vertices[VertexCount];

//Vector of particle emiters
std::vector<Emiter> ParticleEmiters; 

GLuint textureID;

bool click = false;

float t=0; 

// Map for texture loaded
std::map<const char*, GLuint> textures;

// Shader Handler
PGShaderHandler shaders;


GLuint programObject;

// Update method
void Update(float elapsedTime)
{
	t += elapsedTime;

	for(int i = 0; i < ParticleEmiters.size(); i++) //For each particle emiters
	{
		ParticleEmiters[i].ttlCurr -= elapsedTime; //subsract elapsed time from time to live
		
		for (std::vector<Particle>::iterator it = ParticleEmiters[i].particles.begin() ; it != ParticleEmiters[i].particles.end(); ++it) //for each particle
		{
			if(it->emited == true) //if the particle was emited
			{
				it->ttlCurr -= elapsedTime; //substract elapsed time from time to live
				if(it->ttlCurr <= 0)
					it->living = false; //kill particle if the time to live if equals to 0
			}
		}

		if((ParticleEmiters[i].ttlPrev - ParticleEmiters[i].ttlCurr) >= 0.1f) // each tenth secon
		{
			ParticleEmiters[i].ttlPrev = ParticleEmiters[i].ttlCurr;
			if(ParticleEmiters[i].ttlCurr >= ParticleEmiters[i].ttlMax /2 && ParticleEmiters[i].type == 2) //for water particle
			{
				for(int k = 0; k < ParticleEmiters[i].nbParticle / (ParticleEmiters[i].ttlMax * 100); k++)
				{
					if(ParticleEmiters[i].lastEmited + k < ParticleEmiters[i].particles.size())
					{
						ParticleEmiters[i].particles[ParticleEmiters[i].lastEmited + k].emited = true;
						ParticleEmiters[i].lastEmited = ParticleEmiters[i].lastEmited + k;
					}
				}
			}
		}
			
		if(ParticleEmiters[i].ttlCurr <= 0) //supress particle emiters of ttl < 0
			ParticleEmiters.erase(ParticleEmiters.begin() + i);
	}
}

// Draw Method
void Draw()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glClearColor(0.3f, 0.3f, 0.3f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	GLint projection_uniform;
	GLint world_uniform;
	mat4 projectionMatrix = esgiOrtho(0.f, 800.f, 600.f, 0.f, 0.f, 1.f);
	
	GLint position_attr;
	GLint texcoord_attr;

	GLint sampler_uniform;

	float angle = 0.f;

	mat4 worldMatrix;
	worldMatrix.Identity();

	worldMatrix = esgiRotateZ(angle);
	worldMatrix.I.x = 800.f / 1.6f;
	worldMatrix.J.y = 600.f /1.6f;

	/**
	 * Draw Sky - Start
	 */
	programObject = shaders.UseShader("Sky");

	position_attr = glGetAttribLocation(programObject, "a_Position");
	texcoord_attr = glGetAttribLocation(programObject, "a_TexCoords");

	projection_uniform = glGetUniformLocation(programObject, "u_ProjectionMatrix");
	world_uniform = glGetUniformLocation(programObject, "u_WorldMatrix");


	glVertexAttribPointer(position_attr, 3, GL_FLOAT, false, sizeof(Vertex), vertices);
	glVertexAttribPointer(texcoord_attr, 2, GL_FLOAT, false, sizeof(Vertex), &vertices[0].texcoords);

	glEnableVertexAttribArray(position_attr);
	glEnableVertexAttribArray(texcoord_attr);
	
	glUniformMatrix4fv(projection_uniform, 1, false, &projectionMatrix.I.x);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures["Sky"]);

	sampler_uniform = glGetUniformLocation(programObject, "u_Texture");
	glUniform1i(sampler_uniform, 0);

	worldMatrix.T.set(400.f, 300.f, 0.f, 1.0f);

	glUniformMatrix4fv(world_uniform, 1, false, &worldMatrix.I.x);

	glDrawArrays(GL_TRIANGLES, 0, VertexCount);

	glDisableVertexAttribArray(position_attr);
	glDisableVertexAttribArray(texcoord_attr);
	shaders.UnUseShader("sky");
	/**
	 * Draw Sky - End
	 */

	/**
	 * Draw Clouds - Start
	 */
	programObject = shaders.UseShader("Clouds");

	position_attr = glGetAttribLocation(programObject, "a_Position");
	texcoord_attr = glGetAttribLocation(programObject, "a_TexCoords");
		
	projection_uniform = glGetUniformLocation(programObject, "u_ProjectionMatrix");
	world_uniform = glGetUniformLocation(programObject, "u_WorldMatrix");
	GLint time_uniform = glGetUniformLocation(programObject, "u_Time");

	glVertexAttribPointer(position_attr, 3, GL_FLOAT, false, sizeof(Vertex), vertices);
	glVertexAttribPointer(texcoord_attr, 2, GL_FLOAT, false, sizeof(Vertex), &vertices[0].texcoords);
	
	glUniformMatrix4fv(projection_uniform, 1, false, &projectionMatrix.I.x);

	glEnableVertexAttribArray(position_attr);
	glEnableVertexAttribArray(texcoord_attr);

	glUniform1f(time_uniform, t);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures["Clouds"]);

	sampler_uniform = glGetUniformLocation(programObject, "u_Texture");
	glUniform1i(sampler_uniform, 0);

	worldMatrix.T.set(400.f, 300.f, 0.f, 1.0f);
	glUniformMatrix4fv(world_uniform, 1, false, &worldMatrix.I.x);
	glDrawArrays(GL_TRIANGLES, 0, VertexCount);

	glDisableVertexAttribArray(position_attr);
	glDisableVertexAttribArray(texcoord_attr);

	shaders.UnUseShader("Clouds");
	/**
	 * Draw Clouds - End
	 */

	/**
	 * Draw Particle - Start
	 */
	programObject = shaders.UseShader("Particle");

	position_attr = glGetAttribLocation(programObject, "a_Position");
	texcoord_attr = glGetAttribLocation(programObject, "a_TexCoords");

	projection_uniform = glGetUniformLocation(programObject, "u_ProjectionMatrix");
	world_uniform = glGetUniformLocation(programObject, "u_WorldMatrix");

	glVertexAttribPointer(position_attr, 3, GL_FLOAT, false, sizeof(Vertex), vertices);
	glVertexAttribPointer(texcoord_attr, 2, GL_FLOAT, false, sizeof(Vertex), &vertices[0].texcoords);

	GLint ttl_uniform = glGetUniformLocation(programObject, "u_ttl");
	GLint ttl_max_uniform = glGetUniformLocation(programObject, "u_ttl_max");

	GLint dir_uniform = glGetUniformLocation(programObject, "u_dir");
	
	glUniformMatrix4fv(projection_uniform, 1, false, &projectionMatrix.I.x);

	glEnableVertexAttribArray(position_attr);
	glEnableVertexAttribArray(texcoord_attr);

	for(int i = 0; i < ParticleEmiters.size(); i++)
	{
		for(int j = 0; j < ParticleEmiters[i].nbParticle; j++)
		{
			if(ParticleEmiters[i].particles[j].living == true && ParticleEmiters[i].particles[j].emited == true )
			{
				glUniform1f(ttl_uniform, ParticleEmiters[i].particles[j].ttlCurr);
				glUniform1f(ttl_max_uniform, ParticleEmiters[i].particles[j].ttlMax);
				glUniform3f(dir_uniform, ParticleEmiters[i].particles[j].direction.x, ParticleEmiters[i].particles[j].direction.y, ParticleEmiters[i].particles[j].direction.z);
				
				if(ParticleEmiters[i].particles[j].type == 1)
					drawParticle(ParticleEmiters[i].particles[j].position, programObject, sampler_uniform, worldMatrix, world_uniform, textures["Fireworks"]);
				if(ParticleEmiters[i].particles[j].type == 2)
					drawParticle(ParticleEmiters[i].particles[j].position, programObject, sampler_uniform, worldMatrix, world_uniform, textures["Water"]);
			}
		}
	}

	glDisableVertexAttribArray(position_attr);
	glDisableVertexAttribArray(texcoord_attr);

	shaders.UnUseShader("Particle");
	/**
	 * Draw Clouds - End
	 */
}

// Initialise Mesh, Shaders & Textures.
bool Setup()
{	
	setVertices();

	//Create all the shaders
	shaders.addShader(PGShader("Particle", "particle.vert", "particle.frag"));
	shaders.addShader(PGShader("Sky", "sky.vert", "sky.frag"));
	shaders.addShader(PGShader("Clouds", "clouds.vert", "clouds.frag"));

	//Load all textures
	LoadTexture("Fireworks", "fire.tga");
	LoadTexture("Water", "ice.tga");
	LoadTexture("Sky", "night_sky.tga");
	LoadTexture("Clouds", "clouds.tga");

	return true;
}

/**
 * Free memory
 */
void Clean()
{
	glDeleteTextures(1, &textureID);

	textures.clear();
}

/**
 * Main function
 */
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

/**
 * mouseFunc
 */
void mouseFunc(int button, int state, int mouseX, int mouseY)
{
	vec2 pos(mouseX, mouseY);
	if(false == click)
	{
		int type = 0;

		if(button == ESGI_LEFT_BUTTON)
			type = 1;
		if(button == ESGI_RIGHT_BUTTON)
			type = 2;
		
		if(type == 1)
			createFireWorkEmiter(pos);
		if(type == 2)
			createWaterEmiter(pos);	
	}
	click = !click;
}

/**
 * Draw Single Particle
 */
void drawParticle(vec2 pos, GLuint programObject, GLint &sampler_uniform, mat4 &worldMatrix, GLint &world_uniform, GLuint texture)
{
	worldMatrix.I.x = 5.f;
	worldMatrix.J.y = 5.f;
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	sampler_uniform = glGetUniformLocation(programObject, "u_Texture");
	glUniform1i(sampler_uniform, 0);
	worldMatrix.T.set(pos.x, pos.y, 0.f, 1.0f);
	glUniformMatrix4fv(world_uniform, 1, false, &worldMatrix.I.x);
	glDrawArrays(GL_TRIANGLES, 0, VertexCount);
}

/**
 * LoadTexture
 */
void LoadTexture(const char* texName, const char* texFile)
{
	EsgiTexture *tex = esgiReadTGAFile(texFile);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, /* Lod */
		tex->internalFormat, tex->width, tex->height,
		0, tex->format, GL_UNSIGNED_BYTE, tex->texels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	textures.insert(std::pair<const char*,GLuint>(texName, textureID));
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

/**
 * Create Single Particle
 */
void createParticle(Emiter &em)
{
	float speed_max;
	float speed_min;
	float speed;
	
	int angle = 0;
	int emiterOpeningAngle = 0;

	Particle part;
	part.living = true;
	part.position = em.position;
	part.type = em.type;

	if(em.type == 1)
	{
		speed_max = MAX_SPEED_FW;
		speed_min = MIN_SPEED_FW;

		angle = rand() % 360;

		part.emited = true;
		part.ttlMax = ((float)rand()/(float)(RAND_MAX)) * em.ttlMax;
	}
	else if(em.type == 2)
	{
		speed_max = MAX_SPEED_WATER;
		speed_min = MIN_SPEED_WATER;

		emiterOpeningAngle = rand() %(MAX_EMITER_OPENING_ANGLE - MIN_EMITER_OPENING_ANGLE) + MIN_EMITER_OPENING_ANGLE;
		angle = rand() % ((270 + emiterOpeningAngle/2) - (270 - emiterOpeningAngle/2)) + (270 - emiterOpeningAngle/2);
	
		part.emited = false;
		part.ttlMax = ((float)rand()/(float)(RAND_MAX)) * em.ttlMax/2;
	}
	
	speed = ((float)rand()/(float)(RAND_MAX)) * (speed_max - speed_min) + speed_min;
	
	float x = 2 * cos((float)angle * PI / 180.0);
	float y = 2 * sin((float)angle * PI / 180.0);
	vec3 dir(x, y, speed);

	part.direction = dir;
	part.ttlCurr = part.ttlMax;
	em.particles.push_back(part);
}

/**
 * Create Particles for Emiter em
 */
void createParticles(Emiter &em)
{
	for(int i = 0; i < em.nbParticle; i++)
		createParticle(em);
}

/**
 * Create Generic Emiter
 */
Emiter createGenericEmiter(int type, vec2 pos)
{
	Emiter temp;
	temp.position.x = pos.x;
	temp.position.y = pos.y;
	temp.lastEmited = 0;
	temp.type = type;

	return temp;
}

/**
 * Create FireWork Emiter
 */
void createFireWorkEmiter(vec2 pos)
{
	Emiter temp = createGenericEmiter(1, pos);
	temp.ttlMax =  ((float)rand()/(float)(RAND_MAX)) * (MAX_TTL_FW - MIN_TTL_FW) + MIN_TTL_FW;
	temp.nbParticle = rand() % (MAX_PARTICLE_FW - MIN_PARTICLE_FW) + MIN_PARTICLE_FW;
	createParticles(temp);
	temp.ttlCurr = temp.ttlMax;
	temp.ttlPrev = temp.ttlMax;
	ParticleEmiters.push_back(temp);
}

/**
 * Create Water Generic Emiter
 */
void createWaterEmiter(vec2 pos)
{
	Emiter temp = createGenericEmiter(2, pos);	
	temp.ttlMax =  ((float)rand()/(float)(RAND_MAX)) * (MAX_TTL_WATER - MIN_TTL_WATER) + MIN_TTL_WATER;
	temp.nbParticle = rand() % (MAX_PARTICLE_WATER - MIN_PARTICLE_WATER) + MIN_PARTICLE_WATER;
	createParticles(temp);
	temp.ttlCurr = temp.ttlMax;
	temp.ttlPrev = temp.ttlMax;
	ParticleEmiters.push_back(temp);
}
