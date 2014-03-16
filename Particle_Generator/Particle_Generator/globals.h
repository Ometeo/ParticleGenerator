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
	bool living;
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
	float ttlPrev;
	int nbParticle;
	std::vector<_Particle> particles;
	int lastEmited;
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
static const float MIN_SPEED_WATER = 5.f;

static const int MAX_EMITER_OPENING_ANGLE = 90;
static const int MIN_EMITER_OPENING_ANGLE = 45;