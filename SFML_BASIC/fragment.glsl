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
		gl_FragColor = mix(rockTex, grassTex, (0.75 - height) / 0.25);
	else if (height >= 0.1)
		gl_FragColor = grassTex;
	else if (height >= 0.01)
		gl_FragColor = mix(grassTex, waterTex, (0.1 - height) / 0.09);
	else
		gl_FragColor = waterTex;
}