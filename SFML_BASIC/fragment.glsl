#version 120

uniform sampler2D sea;
uniform sampler2D sand;
uniform sampler2D grass;
uniform sampler2D rock;
uniform sampler2D snowrock; 

varying float height;
varying vec4 Ispec;
void main()
{	
	vec4 waterTex = texture2D(sea, fract(gl_TexCoord[0].st));
	vec4 sandTex = texture2D(sand, fract(gl_TexCoord[0].st));  
    vec4 grassTex = texture2D(grass, fract(gl_TexCoord[0].st));
	vec4 rockTex = texture2D(rock, fract(gl_TexCoord[0].st));   
	vec4 snowrockTex = texture2D(snowrock, fract(gl_TexCoord[0].st));  

	if (height >= 0.75)
		gl_FragColor = snowrockTex;
	else if (height >= 0.5)
		gl_FragColor = mix(snowrockTex, rockTex, (0.75 - height) / 0.25);
	else if(height >= 0.2)
		gl_FragColor = rockTex;	
	else if (height >= 0.175)
		gl_FragColor = mix(rockTex, grassTex, (0.2 - height) / 0.025);
	else if (height >= 0.05)
		gl_FragColor = grassTex;
	else if (height >= 0.03)
		gl_FragColor = mix(grassTex,sandTex, (0.05 - height) / 0.02);
	else if (height >= 0.008)
		gl_FragColor = sandTex;

	else if (height >= 0.001)
		gl_FragColor = mix(sandTex, waterTex, (0.008 - height) / 0.007);
	else
		gl_FragColor = waterTex +Ispec;

   // write Total Color:  
   gl_FragColor *= gl_Color;   
}