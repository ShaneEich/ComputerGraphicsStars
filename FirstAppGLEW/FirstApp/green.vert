#version 150

in vec4 vPosition;
out vec4 vColor;


void
main()
{
	vColor = vec4(0, 1, 0, 1);
    gl_Position = vPosition;
}
