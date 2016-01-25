varying  float height;

void main()
{
	// Get the height
	height = gl_Vertex.y;

	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	gl_TexCoord[1] = gl_TextureMatrix[1] * gl_MultiTexCoord1;
	gl_TexCoord[2] = gl_TextureMatrix[2] * gl_MultiTexCoord2;
	gl_FrontColor = gl_Color; //pass the colour along to the next pahse (fragment shader);
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
}