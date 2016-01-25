uniform sampler2D sea;
uniform sampler2D grass;
uniform sampler2D rock;
 
void main()
{
    vec4 color = texture2D(rock,gl_TexCoord[0].st);
    gl_FragColor = color;
}