/**
 * Particle Generator
 * 4A IJV
 * BIHET Jonathan 
 * DESTRIBATS Thierry
 */

#define PI 3.14159265

struct Vertex
{
	vec3 position;
	vec2 texcoords;
};

struct Particle
{
	bool emited; //emited by the emiter
	bool living; //is the particle living
	vec2 position; //the starting position
	vec3 direction; //the direction where de particle go
	int type; //FireWork OR Water
	float ttlMax; //Starting time to live
	float ttlCurr; //Current time to live
};

struct Emiter
{
	vec2 position; //Emiter position
	int type; //FireWork OR Water
	float ttlMax; //Starting time to live
	float ttlCurr; //Current time to live
	float ttlPrev; //The last ttl Record (every tenth second)
	int nbParticle; //number of particles on the emiter
	std::vector<Particle> particles; //The particles
	int lastEmited; //The last emited particle
};

static const float WINDOW_WIDTH = 960.f;
static const float WINDOW_HEIGHT = 600.f;

static const int MAX_PARTICLE_FW = 1000;
static const int MIN_PARTICLE_FW = 500;

static const int MAX_PARTICLE_WATER = 10000;
static const int MIN_PARTICLE_WATER = 5000;

static const float MAX_TTL_FW = 5.0f;
static const float MIN_TTL_FW = 1.5f;

static const float MAX_TTL_WATER = 15.0f;
static const float MIN_TTL_WATER = 5.0f;

static const float MAX_SPEED_FW = 20.f;
static const float MIN_SPEED_FW = 5.f;

static const float MAX_SPEED_WATER = 20.f;
static const float MIN_SPEED_WATER = 20.f;

static const int MAX_EMITER_OPENING_ANGLE = 45;
static const int MIN_EMITER_OPENING_ANGLE = 30;