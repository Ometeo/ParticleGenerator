#pragma once

#include "../EsgiGL/EsgiGL.h"

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>
#include <time.h> 


enum ParticleType{FIRE = 1, WATER = 2};

class Particle
{
private:
	bool _emited; //Does the particle have been emited by the emetor
	vec2 _position; //Position of the particle
	vec3 _direction; //Particle Direction and velocity
	ParticleType _particleType; //FIRE or WATER
	float ttlStart; //Starting particle's time to live
	float ttlCurrent; //Current particle's time to live


public:
	Particle(void);
	~Particle(void);
	void Draw();
};

