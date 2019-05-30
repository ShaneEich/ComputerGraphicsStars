#version 150

in vec4 vPosition;
uniform vec4 uColor;
out vec4 color;
uniform mat4 modelView;



void
main()
{
	color = uColor;
    gl_Position = modelView * vPosition;

}
