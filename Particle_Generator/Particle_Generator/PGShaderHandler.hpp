/**
 * Particle Generator
 * 4A IJV
 * BIHET Jonathan 
 * DESTRIBATS Thierry
 */

#ifndef _PG_SHADER_HANDLER_H
#define _PG_SHADER_HANDLER_H

#include <map>
#include "PGShader.hpp"

class PGShaderHandler
{
private:
	std::map<const char*, PGShader> _shaders;

public:
	PGShaderHandler(void);
	~PGShaderHandler(void);

	void addShader(PGShader);
	EsgiShader getShader(const char* shaderName);

	GLuint UseShader(const char* shaderName);
	void UnUseShader(const char* shaderName);
	void clear();
};

#endif