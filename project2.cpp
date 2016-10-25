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


///////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------- //
// Mouse Click Func   
// Info Window: Opens when specified area on screen is clicked
// ----------------------------------------------------------------------------- //
void OnMouseClick(){

}




///////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------- //
// ############################################################################# //
// ----------------------------------------------------------------------------- //

void myInit(){

	glClearColor(1, 1, 1, 0);			// specify a background clor: white 
	gluOrtho2D(-200, 200, -200, 200);  // specify a viewing area
}


///////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------- //
// ############################################################################# //
// ----------------------------------------------------------------------------- //

void myDisplayCallback(){

	glClear(GL_COLOR_BUFFER_BIT);	// draw the background
	glFlush(); // flush out the buffer contents
}

///////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------- //
// ############################################################################# //
// ----------------------------------------------------------------------------- //

void main(){

	glutInitWindowSize(800, 600);				// specify a window size
	glutInitWindowPosition(100, 0);			// specify a window position
	glutCreateWindow("Ijames-Wilson Text Editor");	// create a titled window

	myInit();									// setting up

	glutDisplayFunc(myDisplayCallback);		// register a callback

	glutM

	glutMainLoop();							// get into an infinite loop
}




