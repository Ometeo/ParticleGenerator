#version 110

attribute vec4 a_Position;
attribute vec2 a_TexCoords;

uniform mat4 u_ProjectionMatrix;
uniform mat4 u_WorldMatrix;


varying vec2 v_TexCoords;

void main()
{
	gl_Position = u_ProjectionMatrix * u_WorldMatrix * a_Position;
	v_TexCoords = a_TexCoords;	
}