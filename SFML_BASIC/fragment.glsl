#version 120

uniform sampler2D sea;
uniform sampler2D grass;
uniform sampler2D rock;

varying float height;

void main()
{	
	vec4 waterTex = texture2D(sea, gl_TexCoord[0].st);
    vec4 grassTex = texture2D(grass, gl_TexCoord[0].st);
	vec4 rockTex = texture2D(rock, gl_TexCoord[0].st);    

	if(height >= 0.75)
		gl_FragColor = rockTex;
	else if (height >= 0.6)
		gl_FragColor = mix(rockTex, grassTex, grassTex.a);
	else if (height >= 1)
		gl_FragColor = grassTex;
	else if (height >= 0.05)
		gl_FragColor = mix(waterTex, grassTex, grassTex.a);
	else
		gl_FragColor = waterTex;
}