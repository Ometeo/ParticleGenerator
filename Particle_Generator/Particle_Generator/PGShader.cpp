/**
 * Particle Generator
 * 4A IJV
 * BIHET Jonathan 
 * DESTRIBATS Thierry
 */

#include "PGShader.hpp"
#include "PGShaderHandler.hpp"

PGShader::PGShader()
{
}


PGShader::PGShader(const char* shaderName, const char* vertexShaderFile, const char* fragmentShaderFile)
{
	_shaderName = shaderName;

	_shader.LoadVertexShader(vertexShaderFile);
	_shader.LoadFragmentShader(fragmentShaderFile);
	_shader.Create();
}


PGShader::~PGShader(void)
{
}

EsgiShader PGShader::getShader()
{
	return _shader;
}


const char* PGShader::getShaderName()
{
	return _shaderName;
}

//Use The shader on draw
GLuint PGShader::UseShader()
{
	GLuint programObject = _shader.GetProgram();
	glUseProgram(programObject);
	return programObject;
}

//Unuse the shader on draw
void PGShader::UnUseShader()
{
	_shader.Unbind();
}