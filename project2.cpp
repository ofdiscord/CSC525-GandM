/*==================================================================================================
PROGRAMMERS: Matthew Wilson and Garren Ijames
COURSE: CSC 525/625
MODIFIED BY: Matthew Wilson and Garren Ijames
LAST MODIFIED DATE: 10/25/2016
CONTRIBUTIONS:
	Matt - 50%
		
	Garren - 50%


DESCRIPTION: Project 2 - Text Editor

NOTES:	


	Extra Features:


FILES: project2.cpp
	
IDE/COMPILER: Visual Studio 2013
INSTRUCTION FOR COMPILATION AND EXECUTION:
1.		Double click on projProject.sln		to OPEN the project
2.		Press Ctrl+F7						to COMPILE
3.		Press Ctrl+Shift+B					to BUILD (COMPILE+LINK)
4.		Press Ctrl+F5						to EXECUTE
==================================================================================================*/
#include <GL/glut.h>				// include GLUT library
#include <iostream>
#include <cmath>

using std::cout;

// Globals

// program constants

const int WINDOW_HEIGHT = 600;

const int WINDOW_WIDTH  = 800;

const int LINE_HEIGHT   = 18; // will likely change

const int MARGIN        = 10; 

const int MAX_LINES     = 30;

const int CURSOR_SIZE   = 10;

// global access variables

int linePosition = 1;


void drawCursor(){

	glColor3f(1, 0, 0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	int x0 = (-WINDOW_WIDTH / 2) + MARGIN;
	int y0 = (WINDOW_HEIGHT / 2) - ((linePosition - 1) * LINE_HEIGHT) - MARGIN;
	int x1 = (-WINDOW_WIDTH / 2) + MARGIN + CURSOR_SIZE;
	int y1 = (WINDOW_HEIGHT / 2) - ((linePosition - 1) * LINE_HEIGHT) - MARGIN;
	int x3 = (-WINDOW_WIDTH / 2) + MARGIN;
	int y3 = (WINDOW_HEIGHT / 2) - ((linePosition - 1) * LINE_HEIGHT) - MARGIN - LINE_HEIGHT;
	int x2 = (-WINDOW_WIDTH / 2) + MARGIN + CURSOR_SIZE;
	int y2 = (WINDOW_HEIGHT / 2) - ((linePosition - 1) * LINE_HEIGHT) - MARGIN - LINE_HEIGHT;

	glBegin(GL_POLYGON);
		glVertex2i(x0, y0);
		glVertex2i(x1, y1);
		glVertex2i(x2, y2);
		glVertex2i(x3, y3);
	glEnd();
}


void drawDebug(){

	// If user clicks on a line, place the cursor there

	if (linePosition > 0 && linePosition <= 30)
		drawCursor();

	// Draw margin lines and others just to see layout of page

	glPointSize(1);

	glBegin(GL_LINES);
		glColor3f(0, 0, 0);
		glVertex2i(-WINDOW_WIDTH / 2, (WINDOW_HEIGHT / 2) - MARGIN); // margin line
		glVertex2i(WINDOW_WIDTH / 2, (WINDOW_HEIGHT / 2) - MARGIN);
		glVertex2i((-WINDOW_WIDTH / 2) + MARGIN, WINDOW_HEIGHT / 2);
		glVertex2i((-WINDOW_WIDTH / 2) + MARGIN, -WINDOW_HEIGHT / 2);
		glColor3f(0.8, 0.8, 0.8);
		for(int i = 1; i < MAX_LINES + 1; i++) {
			glVertex2i(-WINDOW_WIDTH / 2, ((WINDOW_HEIGHT / 2) - MARGIN) - i*LINE_HEIGHT);
			glVertex2i(WINDOW_WIDTH / 2, ((WINDOW_HEIGHT / 2) - MARGIN) - i*LINE_HEIGHT);
		}
	glEnd();
}

///////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------- //
// Mouse Click Func   
// Info Window: Opens when specified area on screen is clicked
// ----------------------------------------------------------------------------- //
void OnMouseClick(int button, int state, int x, int y){
}


///////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------- //
// ############################################################################# //
// ----------------------------------------------------------------------------- //

void Init(){

	glClearColor(1, 1, 1, 0);			// specify a background clor: white 
	gluOrtho2D(-400, 400, -300, 300);  // specify a viewing area
}


///////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------- //
// ############################################################################# //
// ----------------------------------------------------------------------------- //

void myDisplayCallback(){

	glClear(GL_COLOR_BUFFER_BIT);	// draw the background
	drawDebug();
	glFlush(); // flush out the buffer contents
}

///////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------- //
// ############################################################################# //
// ----------------------------------------------------------------------------- //
void mouseCallback(int button, int state, int x, int y){

	linePosition = std::ceil((y + MARGIN) / 18);
	myDisplayCallback();
}


///////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------- //
// ############################################################################# //
// ----------------------------------------------------------------------------- //

int main(int argc, char **argv){

	glutInit(&argc, argv);
	glutInitWindowSize(800, 600);				// specify a window size
	glutInitWindowPosition(100, 0);			// specify a window position
	glutCreateWindow("Ijames-Wilson Text Editor");	// create a titled window

	Init();									// setting up

	glutDisplayFunc(myDisplayCallback);		// register a callback

	glutMouseFunc(mouseCallback);

	glutMainLoop();							// get into an infinite loop

	return 0;
}




