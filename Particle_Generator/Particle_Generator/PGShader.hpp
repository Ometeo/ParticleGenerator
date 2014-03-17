/**
 * Particle Generator
 * 4A IJV
 * BIHET Jonathan 
 * DESTRIBATS Thierry
 */

#ifndef _PG_SHADER_H
#define _PG_SHADER_H

#include "../EsgiGL/EsgiGL.h"
#include "../EsgiGL/EsgiShader.h"

class PGShader
{
private:
	EsgiShader _shader; //the shader
	const char* _shaderName; //shader name
	const char* vertexShaderFile; //vertex shader file
	const char* fragmentShaderFile; //fragment shader file

public:
	PGShader();
	PGShader(const char* shaderName, const char* vertexShaderFile, const char* fragmentShaderFile);
	~PGShader(void);

	EsgiShader getShader();
	void setShader(EsgiShader shader);
	const char* getShaderName();

	GLuint UseShader(); //Use The shader on draw
	void UnUseShader(); //Unuse the shader on draw

};

#endif // _PG_SHADER_H