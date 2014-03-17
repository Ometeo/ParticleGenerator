#version 110

uniform sampler2D u_Texture;

uniform float u_ttl;
uniform float u_ttl_max;

varying vec2 v_TexCoords;

void main()
{
	vec4 Texel = texture2D(u_Texture, v_TexCoords);
	
	float temp = u_ttl / u_ttl_max;
	if(Texel.a > temp)
		Texel.a = temp;

	gl_FragColor = Texel;
}