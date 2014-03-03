#version 110

uniform sampler2D u_Texture;

varying vec2 v_TexCoords;

void main()
{
	
	vec4 Texel = texture2D(u_Texture, v_TexCoords);
	
	//if(Texel.a < 1.0)
	//	discard;

	gl_FragColor = Texel;
}