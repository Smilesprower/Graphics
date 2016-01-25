uniform sampler2D sea;
uniform sampler2D grass;
uniform sampler2D rock;

varying float height;

void main()
{
	// Heights are between 0 - 18 I think ?
	// Because we got the vertex height and not the pixel height; 

   // vec4 color = texture2D(rock,gl_TexCoord[0].st);
   // gl_FragColor = color;

	if(height >= 0 && height <= 2)
		gl_FragColor = texture2D(sea, gl_TexCoord[0].st );  
	else if(height > 2 && height <= 8)
		gl_FragColor = texture2D(grass, gl_TexCoord[0].st );  

	else 
		gl_FragColor = texture2D(rock, gl_TexCoord[0].st );  

}