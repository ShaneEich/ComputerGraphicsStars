#version 150

in vec4 vPosition;
out vec4 vColor;


void
main()
{
	vColor = vPosition;
	vColor.rg = vColor.gb;
	//vColor.b = 1;
    gl_Position = vPosition;
}
