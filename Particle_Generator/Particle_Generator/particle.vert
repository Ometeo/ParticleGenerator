#version 110

attribute vec4 a_Position;
attribute vec2 a_TexCoords;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_WorldMatrix;

uniform float u_ttl;
uniform float u_ttl_max;

uniform vec3 u_dir;

varying vec2 v_TexCoords;

void main()
{
	vec4 pos = a_Position;
	
	pos.x += ((u_ttl_max - u_ttl)  * 3.0);
	pos.y += ((u_ttl_max - u_ttl)  * 3.0);

	//pos.x *= u_ttl;
	//pos.y *= u_ttl;
	gl_Position = u_ProjectionMatrix * u_WorldMatrix * pos ;
	v_TexCoords = a_TexCoords;	
}