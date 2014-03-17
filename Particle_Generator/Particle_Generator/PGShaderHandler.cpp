/**
 * Particle Generator
 * 4A IJV
 * BIHET Jonathan 
 * DESTRIBATS Thierry
 */

#include "PGShaderHandler.hpp"


PGShaderHandler::PGShaderHandler(void)
{
}

PGShaderHandler::~PGShaderHandler(void)
{
	this->clear();
}

void PGShaderHandler::addShader(PGShader shader)
{
	_shaders.insert(std::pair<const char*,PGShader>(shader.getShaderName(),shader));
}

EsgiShader PGShaderHandler::getShader(const char* shaderName)
{
	return this->_shaders[shaderName].getShader();
}

void PGShaderHandler::clear()
{
	for (std::map<const char*,PGShader>::iterator it=_shaders.begin(); it!=_shaders.end(); ++it)
		it->second.getShader().Destroy();
	_shaders.clear();
}

GLuint PGShaderHandler::UseShader(const char* shaderName)
{
	return _shaders[shaderName].UseShader();
}

void PGShaderHandler::UnUseShader(const char* shaderName)
{
	_shaders[shaderName].UnUseShader();
}
