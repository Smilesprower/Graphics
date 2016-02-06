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
	
	
	// How Blending works Craig - I think
	//////////////////////////////////
	// Mix (1st text = highest text, 2nd text = lowest tex, 
	//(start height of the texture above - current height ) divided by the difference between both)

	else if (height >= 0.008)
		gl_FragColor = sandTex;

	else if (height >= 0.001)
		gl_FragColor = mix(sandTex, waterTex, (0.008 - height) / 0.007);
	else
		gl_FragColor = waterTex;
}