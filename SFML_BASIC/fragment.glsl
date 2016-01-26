#version 120

uniform sampler2D sea;
uniform sampler2D sand;
uniform sampler2D grass;
uniform sampler2D rock;
uniform sampler2D snowrock;

varying float height;

void main()
{	
	vec4 waterTex = texture2D(sea, fract(gl_TexCoord[0].st));
	vec4 sandTex = texture2D(sand, fract(gl_TexCoord[0].st));  
    vec4 grassTex = texture2D(grass, fract(gl_TexCoord[0].st));
	vec4 rockTex = texture2D(rock, fract(gl_TexCoord[0].st));   
	vec4 snowrockTex = texture2D(snowrock, fract(gl_TexCoord[0].st));  

	if (height >= 0.75)
		gl_FragColor = snowrockTex;
	else if (height >= 0.625)
		gl_FragColor = mix(rockTex, snowrockTex, (0.75 - height) / 0.25);
	else if (height >= 0.5)
		gl_FragColor = mix(snowrockTex, rockTex, (0.75 - height) / 0.25);
	else if(height >= 0.2)
		gl_FragColor = rockTex;	
	else if (height >= 0.175)
		gl_FragColor = mix(grassTex, rockTex, (0.5 - height) / 0.5);
	else if (height >= 0.15)
		gl_FragColor = mix(rockTex, grassTex, (0.5 - height) / 0.5);
	else if (height >= 0.05)
		gl_FragColor = grassTex;
	else if (height >= 0.04)
		gl_FragColor = mix(grassTex,sandTex, (0.035 - height) / 0.035);
	else if (height >= 0.03)
		gl_FragColor = mix(sandTex,grassTex, (0.035 - height) / 0.035);
	else if (height >= 0.02)
		gl_FragColor = sandTex;
	else if (height >= 0.0125)
		gl_FragColor = mix(waterTex, sandTex, (0.5 - height) / 0.6);
	else if (height >= 0.005)
		gl_FragColor = mix(sandTex, waterTex, (0.5 - height) / 0.6);
	else
		gl_FragColor = waterTex;

}