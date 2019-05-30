/* sierpinski gasket using vertex buffer objects */

#include "Angel.h"

const int NumPoints = 100;
GLuint currentShader = 0;
GLuint red = 0;
GLuint green = 0;
GLuint blue = 0;
GLfloat rotateInDegrees = 0.0;
vec4 points[NumPoints];
vec3 colors[NumPoints];

GLfloat offsetY = 0.0;
GLfloat offsetX = 0.0;

GLfloat width = 512;
GLfloat height = 512;

GLfloat numRotate = 0.0, numStarPoint = 0.0;
mat4 transformation, rotationAboutZ, modelView, move;

GLfloat t = 0;
vec4 translation, goingTo, going, placement;
enum drawState{WAIT, MOVE, PLACE};

drawState inputState = WAIT;
//----------------------------------------------------------------------------
/* This function initializes an array of 3d vectors 
   and sends it to the graphics card along with shaders
   properly connected to them.
*/

void
init( void )
{

    // Specifiy the vertices for a triangle
	vec4 vertices[] = {
		vec4( 0.0, 0.0, 0.0, 1.0 ),
		vec4( -0.1, 0.1, 0.0, 1.0 ),
		vec4(  0.0, 0.5, 0.0, 1.0 ),
		vec4(  0.1, 0.1, 0.0, 1.0 )
	};
	// Create some colors
	vec3 base_colors[] = {
		vec3(0.0, 0.0, 0.0),
		vec3(0.0, 0.0, 1.0),
		vec3(0.0, 1.0, 0.0),
		vec3(0.0, 1.0, 1.0),
		vec3(1.0, 0.0, 0.0),
		vec3(1.0, 0.0, 1.0),
		vec3(1.0, 1.0, 0.0),
		vec3(1.0, 1.0, 1.0)
	};

    // Select an arbitrary initial point inside of the triangle
    points[0] = vertices[1];
	points[1] = vertices[0];
	points[2] = vertices[2];
	points[3] = vertices[3];

	//mat4 translation = Translate(0.0, 0.0, 0.0);
	//mat4 rotationAboutZ = RotateZ(120.0);
	//mat4 scaling = Scale(1.0, 1.0, 1.0);
	mat4 transformation =  rotationAboutZ;
	

	// Set the vertex colors
	colors[0] = base_colors[6];
	colors[1] = base_colors[6];
	colors[2] = base_colors[6];
	colors[3] = base_colors[6];
	colors[4] = base_colors[5];
	colors[5] = base_colors[2];
	colors[6] = base_colors[4];
	colors[7] = base_colors[5];


    // compute and store NumPoints - 1 new points
	/*
    for ( int i = 1; i < NumPoints; ++i ) {
        int j = rand() % 3;   // pick a vertex from the triangle at random

        // Compute the point halfway between the selected vertex
        //   and the previous point
        points[i] = ( points[i - 1] + vertices[j] ) / 2.0;
    }
	*/

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    //glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );
	// Setup the Array Buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), NULL, GL_STATIC_DRAW);

	// Fill the Array Buffer
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
    // Load shaders and use the resulting shader program
	red = InitShader("uniformColor.vert", "uniformColor.frag");
	green = InitShader("green.vert", "green.frag");
	blue = InitShader("blue.vert", "blue.frag");
	currentShader = red;
	// make these shaders the current shaders
    glUseProgram( currentShader );

    // Initialize the vertex position attribute from the vertex shader
    GLuint loc = glGetAttribLocation(currentShader, "vPosition" );
    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

	// Initialize the vertex position attribute from the vertex shader
	GLuint loc2 = glGetAttribLocation(currentShader, "vColor");
	glEnableVertexAttribArray(loc2);
	glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));

	glClearColor( 0.5, 0.5, 0.5, 1.0 ); // gray background
}

//----------------------------------------------------------------------------
/* This function handles the display and it is automatically called by GLUT
   once it is declared as the display function. The application should not
   call it directly.
*/


void
display( void )
{/*
	GLuint uColorLoc = glGetUniformLocation(currentShader, "uColor");
	vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
	mat4 identity = mat4(1.0);
    glClear( GL_COLOR_BUFFER_BIT );	// clear the window
	glUseProgram(currentShader);
	//glUseProgram(red);
	glUniform4fv(uColorLoc, 1, color);
	glUniformMatrix4fv(glGetUniformLocation(currentShader, "modelView"), 1, true, transformation);
    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );    // draw the points
	//glUseProgram(blue);
	color = vec4(1.0, 1.0, 0.0, 1.0);
	glUniform4fv(uColorLoc, 1, color);
	glUniformMatrix4fv(glGetUniformLocation(currentShader, "modelView"), 1, true, transformation);
	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
	glUniform4fv(uColorLoc, 1, color);
	glUniformMatrix4fv(glGetUniformLocation(currentShader, "modelView"), 1, true, transformation);
	glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
    glFlush();									// flush the buffer
	*/
	GLuint offsetParamX = glGetUniformLocation(currentShader, "offsetX");
	GLuint offsetParamY = glGetUniformLocation(currentShader, "offsetY");

	GLuint uColorLoc = glGetUniformLocation(currentShader, "uColor");
	vec4 color = vec4(1.0, 0.0, 0.0, 1.0);
	mat4 identity = mat4(1.0);
	glClear(GL_COLOR_BUFFER_BIT);	// clear the window

	switch (inputState)
	{
		GLint count;

	case WAIT:
		break;
	case MOVE:
		glUniform4fv(uColorLoc, 1, color);

		count = 0;
		while (count < numStarPoint)
		{
			modelView = transformation * RotateZ(numRotate * count);
			glUniformMatrix4fv(glGetUniformLocation(currentShader, "modelView"), 1, true, modelView);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			count++;
			
		}
		break;
	case PLACE:
		glUniform4fv(uColorLoc, 1, color);

		count = 0;
		while (count < numStarPoint)
		{
			modelView = move * transformation * RotateZ(numRotate * count);
			glUniformMatrix4fv(glGetUniformLocation(currentShader, "modelView"), 1, true, modelView);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			count++;
		}
		
		break;
	}
	//glFlush();									// flush the buffer
	glutSwapBuffers();
}



void
idle()
{
	if (inputState != WAIT) {
		rotateInDegrees += 0.01;
		mat4 rotationAboutZ = RotateZ(rotateInDegrees);
		if (inputState == PLACE) {
			if (t <= 0.00095) {
				goingTo = goingTo + t * (going - goingTo);
				t += 0.0000001;
			}
			else if (t > 0.00095) {
				inputState = WAIT;
				t = 0;
			}
		}
		translation = goingTo;
		transformation = Translate(translation) * rotationAboutZ;
	}
	glutPostRedisplay();
}

//----------------------------------------------------------------------------
/* This function handles the keyboard and it is called by GLUT once it is 
   declared as the keyboard function. The application should not call it
   directly.
*/

void
keyboard( unsigned char key, int x, int y )
{
	GLfloat width;
	GLfloat height;
	width = glutGet(GLUT_WINDOW_WIDTH);
	height = glutGet(GLUT_WINDOW_HEIGHT);
	y = height - y;
    switch ( key ) {
    case 033:					// escape key
        exit( EXIT_SUCCESS );	// terminates the program
        break;
	case '2':
		if (inputState == WAIT) {
			GLfloat xWorld = (GLfloat)x / width * 2 - 1;
			GLfloat yWorld = (GLfloat)y / height * 2 - 1;
			offsetX = xWorld;
			offsetY = yWorld;
			numRotate = 180;
			move = Translate(offsetX, offsetY, 0.0);
			numStarPoint = 2;
			inputState = PLACE;
		}
		break;
	case '3':
		if (inputState == WAIT) {
			GLfloat xWorld = (GLfloat)x / width * 2 - 1;
			GLfloat yWorld = (GLfloat)y / height * 2 - 1;
			offsetX = xWorld;
			offsetY = yWorld;
			numRotate = 120;
			move = Translate(offsetX, offsetY, 0.0);
			numStarPoint = 3;
			inputState = PLACE;
		}
		
		break;
	case '4':
		if (inputState == WAIT) {
			GLfloat xWorld = (GLfloat)x / width * 2 - 1;
			GLfloat yWorld = (GLfloat)y / height * 2 - 1;
			offsetX = xWorld;
			offsetY = yWorld;
			numRotate = 90;
			move = Translate(offsetX, offsetY, 0.0);
			numStarPoint = 4;
			inputState = PLACE;
		}
		break;
	case '5':
		if (inputState == WAIT) {
			GLfloat xWorld = (GLfloat)x / width * 2 - 1;
			GLfloat yWorld = (GLfloat)y / height * 2 - 1;
			offsetX = xWorld;
			offsetY = yWorld;
			numRotate = 72;
			move = Translate(offsetX, offsetY, 0.0);
			numStarPoint = 5;
			inputState = PLACE;
		}
		break;
    }
	glutPostRedisplay();
}
void 
mouse(GLint button, GLint state, GLint x, GLint y) {
	GLfloat width;
	GLfloat height;
	width = glutGet(GLUT_WINDOW_WIDTH);
	height = glutGet(GLUT_WINDOW_HEIGHT);
	GLfloat xWorld = ((GLfloat)x) / width * 2 - 1;
	GLfloat yWorld = ((GLfloat)y) / height * 2 - 1;

	if (state == GLUT_DOWN)
	{
		printf("x: %i - y: %i || xWorld: %f - yWorld %f\n", x, y, xWorld, -yWorld);
		going = vec4(xWorld, -yWorld, 0.0, 1.0);
		inputState = PLACE;
		glutPostRedisplay();
	}
}

void simpleMenu(GLint choice)
{
	switch (choice)
	{
		case 0:
			currentShader = red;
			break;
		case 1:
			currentShader = green;
			break;
		case 2:
			currentShader = blue;
			break;
		default:
			break;
	}
	glutPostRedisplay();

}

//----------------------------------------------------------------------------
/* This is the main function that calls all the functions to initialize
   and setup the OpenGL environment through GLUT and GLEW.
*/

int
main( int argc, char **argv )
{
	// Initialize GLUT
    glutInit( &argc, argv );
	// Initialize the display mode to a buffer with Red, Green, Blue and Alpha channels
    glutInitDisplayMode( GLUT_RGBA );
	// Set the window size
    glutInitWindowSize( 512, 512 );
	// Here you set the OpenGL version
    glutInitContextVersion( 3, 2 );
	//Use only one of the next two lines
    //glutInitContextProfile( GLUT_CORE_PROFILE );
	glutInitContextProfile( GLUT_COMPATIBILITY_PROFILE );
    glutCreateWindow( "Simple GLSL example" );

    // Uncomment if you are using GLEW
	glewInit(); 

    // initialize the array and send it to the graphics card
	init();

	// create a simple menu
	GLint menu = glutCreateMenu(simpleMenu);
	glutAddMenuEntry("Red", 0);
	glutAddMenuEntry("Green", 1);
	glutAddMenuEntry("Blue", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	//glutAddSubMenu("Colors", 1);

    // provide the function that handles the display
	glutDisplayFunc( display );
	// provide the functions that handles the keyboard
    glutKeyboardFunc( keyboard );
	// provide the function that handles the idle state
	glutIdleFunc(idle);
	glutMouseFunc( mouse ); 

    // Wait for input from the user (the only meaningful input is the key escape)
	glutMainLoop();
    return 0;
}