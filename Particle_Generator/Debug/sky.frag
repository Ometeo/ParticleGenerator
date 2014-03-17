#version 110

uniform sampler2D u_Texture;

varying vec2 v_TexCoords;

void main()
{	
	vec4 texel = texture2D(u_Texture, v_TexCoords);
	gl_FragColor = texel;//v_VertexColor;
}