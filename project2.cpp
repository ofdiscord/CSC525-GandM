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
//#include <stdlib.h>
#include <GL/glut.h>				// include GLUT library
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using std::vector;
using std::string;
using std::cout;

// program constants
const int WINDOW_HEIGHT = 600;
const int WINDOW_WIDTH  = 800;
const int LINE_HEIGHT   = 18; // will likely change
const int MARGIN        = 20;
const int MAX_LINES     = 30;
const int CURSOR_SIZE   = 10;

// global access variables: typing related
int lastLinePosition    = 1;
int textLinePosition    = 1;
int currentLinePosition = 1;
int totalTextWidth      = 0;

// string to contain all text for different utilities
string text = "";

// partition sections
struct Partition {
	float color[3];
	void *font;
	string text;
};

// vector containing partitions
vector<Partition> partitions;


void drawCursor(){

	// set polygon mode to fill
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// clear old cursor
	glColor3f(1, 1, 1);

	int x0 = (-WINDOW_WIDTH / 2) + MARGIN;
	int y0 = (WINDOW_HEIGHT / 2) - (lastLinePosition * LINE_HEIGHT) - (MARGIN) - 1;
	int x1 = (-WINDOW_WIDTH / 2) + MARGIN + CURSOR_SIZE;
	int y1 = (WINDOW_HEIGHT / 2) - (lastLinePosition * LINE_HEIGHT) - (MARGIN) - 1;
	int x3 = (-WINDOW_WIDTH / 2) + MARGIN;
	int y3 = (WINDOW_HEIGHT / 2) - (lastLinePosition * LINE_HEIGHT) - (MARGIN) - LINE_HEIGHT + 1;
	int x2 = (-WINDOW_WIDTH / 2) + MARGIN + CURSOR_SIZE;
	int y2 = (WINDOW_HEIGHT / 2) - (lastLinePosition * LINE_HEIGHT) - (MARGIN) - LINE_HEIGHT + 1;

	glBegin(GL_POLYGON);
		glVertex2i(x0, y0);
		glVertex2i(x1, y1);
		glVertex2i(x2, y2);
		glVertex2i(x3, y3);
	glEnd();

	// draw new cursor
	glColor3f(1, 0, 0);

	x0 = (-WINDOW_WIDTH / 2) + MARGIN;
	y0 = (WINDOW_HEIGHT / 2) - (currentLinePosition * LINE_HEIGHT) - MARGIN - 1;
	x1 = (-WINDOW_WIDTH / 2) + MARGIN + CURSOR_SIZE;
	y1 = (WINDOW_HEIGHT / 2) - (currentLinePosition * LINE_HEIGHT) - MARGIN - 1;
	x3 = (-WINDOW_WIDTH / 2) + MARGIN;
	y3 = (WINDOW_HEIGHT / 2) - (currentLinePosition * LINE_HEIGHT) - MARGIN - LINE_HEIGHT + 2;
	x2 = (-WINDOW_WIDTH / 2) + MARGIN + CURSOR_SIZE;
	y2 = (WINDOW_HEIGHT / 2) - (currentLinePosition * LINE_HEIGHT) - MARGIN - LINE_HEIGHT + 2;

	glBegin(GL_POLYGON);
		glVertex2i(x0, y0);
		glVertex2i(x1, y1);
		glVertex2i(x2, y2);
		glVertex2i(x3, y3);
	glEnd();

	glFlush();
}


void drawPartitions(){

	int lastPartitionWidth = 0;

	for (int i = 0; i < partitions.size(); i++)
	{
		int lineHeight = (WINDOW_HEIGHT / 2) - (currentLinePosition * 18) - MARGIN;

		glColor3fv(partitions[i].color);
		glRasterPos2i(lastPartitionWidth - (WINDOW_WIDTH / 2) + MARGIN, lineHeight);

		for (int j = 0; j < partitions[i].text.size(); j++) {
			// draw over the last character
			glutBitmapCharacter(partitions[i].font, partitions[i].text[j]);
			lastPartitionWidth += glutBitmapWidth(partitions[i].font, partitions[i].text[j]);
		}
	}
}


void drawLines(){

	// draw margin lines and others just to see layout of page
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
// ############################################################################# //
// ----------------------------------------------------------------------------- //

void Init(){

	// Initialize partitions to a default font and color
	Partition partition;
	partition.color[0] = 0;
	partition.color[1] = 0;
	partition.color[2] = 0;
	partition.font = GLUT_BITMAP_HELVETICA_18;
	partition.text = "";
	partitions.push_back(partition);

	glClearColor(1, 1, 1, 0);			// specify a background color: white
	gluOrtho2D(-400, 400, -300, 300);  // specify a viewing area
}


///////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------- //
// ############################################################################# //
// ----------------------------------------------------------------------------- //

void myDisplayCallback(){

	glClear(GL_COLOR_BUFFER_BIT);	// draw the background
	drawLines();
	drawPartitions();
	glFlush(); // flush out the buffer contents
}


///////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------- //
// ############################################################################# //
// ----------------------------------------------------------------------------- //

void mouseCallback(int button, int state, int x, int y){

}


///////////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------- //
// ############################################################################# //
// ----------------------------------------------------------------------------- //

void keyboardCallback(unsigned char key, int x, int y){

	if (text.size() == 0) {
		// initialize raster position
		glRasterPos2i(MARGIN - (WINDOW_WIDTH / 2), (WINDOW_HEIGHT / 2) - MARGIN - LINE_HEIGHT);
	}

	if (key == 8) // if backspace key
	{
			// remove the last character from the current partition
			int textSize = partitions[partitions.size() - 1].text.size();

			if (textSize == 0) {
				if (partitions.size() > 1) {
					partitions.pop_back();
				}
			}
			else {
				string newText = partitions[partitions.size() - 1].text.substr(0, textSize - 1);
				partitions[partitions.size() - 1].text = newText;
			}

			// draw the text
			myDisplayCallback();
	}
	else
	{
		// add the key to the text of the current partition
		partitions[partitions.size() - 1].text += key;

		// draw the text
		myDisplayCallback();
	}
}


//
void colorMenuCallback(int entryId)
{
	float red, green, blue;

	switch(entryId)
	{
		case 1:
			red = 1; green = 0; blue = 0;
			break;
		case 2:
			red = 0; green = 1; blue = 0;
			break;
		case 3:
			red = 0; green = 0; blue = 1;
			break;
		case 4:
			red = 0; green = 0; blue = 0;
			break;
	}

	float color[3] {red, green, blue};
	void *font = partitions[partitions.size() - 1].font;
	Partition partition;
	partition.color[0] = color[0];
	partition.color[1] = color[1];
	partition.color[2] = color[2];
	partition.font = font;
	partition.text = "";
	partitions.push_back(partition);
}


//
void fontMenuCallback(int entryId)
{
	void *font;

	switch(entryId)
	{
		case 1:
			font = GLUT_BITMAP_TIMES_ROMAN_24;
			break;
		case 2:
			font = GLUT_BITMAP_HELVETICA_18;
	}

	Partition partition;
	partition.color[0] = partitions[partitions.size() - 1].color[0];
	partition.color[1] = partitions[partitions.size() - 1].color[1];
	partition.color[2] = partitions[partitions.size() - 1].color[2];
	partition.font = font;
	partition.text = "";
	partitions.push_back(partition);
}


//
void topMenuCallback(int entryId)
{
	if (entryId == 3) {
		exit(0);
	}
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

	// color sub-menu
	int colorSubMenuId = glutCreateMenu(colorMenuCallback);
	glutAddMenuEntry("RED", 1);
	glutAddMenuEntry("GREEN", 2);
	glutAddMenuEntry("BLUE", 3);
	glutAddMenuEntry("BLACK", 4);

	// font sub-menu
	int fontSubMenuId = glutCreateMenu(fontMenuCallback);
	glutAddMenuEntry("TIMES NEW ROMAN", 1);
	glutAddMenuEntry("HELVETICA", 2);

	// create top level menu
	int topMenuId = glutCreateMenu(topMenuCallback);
	glutAddSubMenu("COLOR", colorSubMenuId);
	glutAddSubMenu("FONT", fontSubMenuId);
	glutAddMenuEntry("EXIT", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON); // attach event to top level menu

	glutDisplayFunc(myDisplayCallback);		// register a callback
	//glutMouseFunc(mouseCallback);
	glutKeyboardFunc(keyboardCallback);

	glutMainLoop();							// get into an infinite loop

	return 0;
}
