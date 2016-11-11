/*==================================================================================================
PROGRAMMERS: Jamie_____, Matthew Wilson and Garren Ijames
COURSE: CSC 525/625
MODIFIED BY: Jamie_____, Matthew Wilson and Garren Ijames
LAST MODIFIED DATE: 11/4/2016

CONTRIBUTIONS:
Matt - 33.3%
Garren - 33.3%
Jamie - 33.3%

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

#include <stdlib.h>
#include <GL/glut.h>				// include GLUT library
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

/*JJ ADDED FOR WRITING TO TEXTFILE*/
#include <fstream>			// ofstream
using namespace std;  //update later?
ofstream outputFile;     //write



using std::vector;
using std::string;
using std::cout;

// Variable Declarations
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
	string type;
	float color[3];
	void *font;
	string text;
};

// default style
float defaultColor[3] {0, 0, 0};
void* defaultFont = GLUT_BITMAP_HELVETICA_18;

// active colors

float activeColor[3];
void* activeFont;

// vector containing partitions
vector<Partition> partitions;

// Utility functions
void createOrModifyPartition(float color[3] = activeColor, void *font = activeFont){

	bool modifyPartition = partitions.size() > 0;
	modifyPartition = modifyPartition && partitions[partitions.size() - 1].text == "";
	modifyPartition = modifyPartition && partitions[partitions.size() - 1].type != "NEWLINE";

	if (modifyPartition)

	{
			partitions[partitions.size() - 1].color[0] = color[0];
			partitions[partitions.size() - 1].color[0] = color[1];
			partitions[partitions.size() - 1].color[0] = color[2];
			partitions[partitions.size() - 1].font = font;

	}
	else // create new partition
	{
		Partition partition;
		partition.type = "DEFAULT";
		partition.color[0] = color[0];
		partition.color[1] = color[1];
		partition.color[2] = color[2];
		partition.font = font;
		partitions.push_back(partition);
	}
}

void createNewLine(){
	// basically just a partition with all default styles and text = "\n"
	Partition newline;
	newline.type = "NEWLINE";
	partitions.push_back(newline);
}

// Setting up functions
void Init(){

	// set active color and font to default
	activeColor[0] = defaultColor[0];
	activeColor[1] = defaultColor[1];
	activeColor[2] = defaultColor[2];
	activeFont = defaultFont;

	// initialize partitions to a default font and color
	createOrModifyPartition();
	glClearColor(1, 1, 1, 0);			// specify a background color: white
	gluOrtho2D(-400, 400, -300, 300);  // specify a viewing area
}

// Draw functions
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
	int runningTotal = 0;
	int drawLinePosition = 1;

	for (int i = 0; i < partitions.size(); i++)

	{
		int lineHeight = (WINDOW_HEIGHT / 2) - (drawLinePosition * LINE_HEIGHT) - MARGIN;

		if (partitions[i].type == "NEWLINE") {
			drawLinePosition += 1;
			lineHeight = (WINDOW_HEIGHT / 2) - (drawLinePosition * LINE_HEIGHT) - MARGIN;
			lastPartitionWidth = 0;
			runningTotal = 0;
		}

		glColor3fv(partitions[i].color);
		glRasterPos2i(lastPartitionWidth - (WINDOW_WIDTH / 2) + MARGIN, lineHeight);

		for (int j = 0; j < partitions[i].text.size(); j++)
		{
			runningTotal += 1;

			if (runningTotal % 30 == 0) {
				drawLinePosition += 1;
				lineHeight = (WINDOW_HEIGHT / 2) - (drawLinePosition * LINE_HEIGHT) - MARGIN;
				glRasterPos2i(-(WINDOW_WIDTH / 2) + MARGIN, lineHeight);
				lastPartitionWidth = 0;
			}
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

void drawDisplay(){
	glClear(GL_COLOR_BUFFER_BIT);	// draw the background
	drawLines();
	drawPartitions();
	drawCursor();
	glFlush(); // flush out the buffer contents

}

// Input callbacks
void mouseCallback(int button, int state, int x, int y){

	if (currentLinePosition >= 0 && currentLinePosition < 30)
		drawCursor();
	lastLinePosition = currentLinePosition;
	currentLinePosition = std::floor((y - MARGIN) / 18.0);

	if (currentLinePosition > textLinePosition)
		glRasterPos2i(MARGIN - (WINDOW_WIDTH / 2), (WINDOW_HEIGHT / 2) - currentLinePosition * LINE_HEIGHT - MARGIN);
}


void keyboardCallback(unsigned char key, int x, int y){
	if (text.size() == 0) {
		// initialize raster position
		glRasterPos2i(MARGIN - (WINDOW_WIDTH / 2), (WINDOW_HEIGHT / 2) - MARGIN - LINE_HEIGHT);
	}

	if (key == 8) // if backspace key
	{
		if (partitions[partitions.size() - 1].type == "NEWLINE") {
			partitions.pop_back();
			drawDisplay();
		}

		else {
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
			drawDisplay();
		}
	}

	else if (key == 13) // ENTER
	{
		createNewLine();
		createOrModifyPartition();
		drawDisplay();

	}
	else
	{	// add the key to the text of the current partition
		partitions[partitions.size() - 1].text += key;
		drawDisplay();
	}
}

// Menu callbacks

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

	activeColor[0] = red; activeColor[1] = green; activeColor[2] = blue;
	float color[3] {red, green, blue};
	void *font = partitions[partitions.size() - 1].font;
	createOrModifyPartition(color, font);

}

void fontMenuCallback(int entryId)
{
	void *font = GLUT_BITMAP_TIMES_ROMAN_24;

	switch(entryId)
	{
		case 1:
			font = GLUT_BITMAP_TIMES_ROMAN_24;
			break;

		case 2:
			font = GLUT_BITMAP_HELVETICA_18;
	}
	activeFont = font;
	createOrModifyPartition(partitions[partitions.size() - 1].color, font);

}

/*JJ ADDED FUNCTION BELOW*/
void saveTextFile() 
{
	/*Open to write to file.*/
	outputFile.open("C:\\TEMP\\typed.txt");

	if (outputFile.fail())
	{
		cout << "File was unable to be written." << endl;
		//system("pause");
		//return 0;
	}

	else
	{
		int runningTotal = 0;
		for (int i = 0; i < partitions.size(); i++)
		{
			if (partitions[i].type == "NEWLINE")
			{
				outputFile << "\n";
			}
			for (int j = 0; j < partitions[i].text.size(); j++)
			{
				if (++runningTotal % 30 == 0)
				{
					outputFile << "\n";
				}
				outputFile << partitions[i].text[j];
			}
		}
		/* Close the file */
		outputFile.close();
	}
}
/*JJ ADDED FUNCTION ABOVE*/


void topMenuCallback(int entryId)
{
	//***************************
	/*JJ UPDATED ADDING "SWITCH (ENTRYID)" STRUCTURE and "BREAK"*/
	// ORG CODE: if (entryId == 3) {
	//exit(0);
	// }
	switch (entryId)
	{
		/*JJ ADDED THE SAVE OPTION */
	case 3:
		saveTextFile();
		break;

		/*JJ ADDED THE INFO OPTION */
	case 4:
		glutSetWindow(1); // changing the current window to the "Info Window"
		glutShowWindow(); // displaying the Info Window
		break;
	
	/*EXIT*/
	case 5:
		exit(0);
		break;
	}
}

//*******************************************************************
//*******************************************************************
//  START
//*******************************************************************
//*******************************************************************
void infoInstructions()
{
	/*Display the message inside the Info Window*/
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.55, .20, 0.45);

	string myArray[18] = {
		"DISPLAY TEXT:", 
		"Place the cursor within the window",
		"and type the text of your choice. ",

		"CHANGE COLOR/FONT STYLE:", 
		"Right click the mouse button in the main",
		"window and make your selection. ",

		"CLOSE MAIN WINDOW:", 
		"Select the Close option from the drop down list.",

		"OPEN/RE-OPEN INFO WINDOW:", 
		"Right click the mouse button and select the Info option.",

		"CLOSE INFO WINDOW: ",
		"Right click the mouse button within the Info",
		"window and select the Close option.",
	};

	for (int i = 0; i < 18; i++)
	{
		glRasterPos2i(-375, 250 - 50 * i); // position 
		for (int j = 0; j < myArray[i].size(); j++)
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, myArray[i][j]);
		}
	}	
}

/*JJ infoDisplayCallback()*/
void infoDisplayCallback()
{
	glClear(GL_COLOR_BUFFER_BIT);	// draw the background

	infoInstructions(); // call the instructions function

	glFlush();
}

/*JJ Info Menu*/
void info_menu(int id)
{
	if (id == 1)
	{
		glutHideWindow(); //hide info window
	}
}

//*******************************************************************
//******************************************************************
//  END
//******************************************************************
//*******************************************************************

int main(int argc, char **argv){
	glutInit(&argc, argv);

//*******************************************************************
//**************************************************************
//  START
//**************************************************************
//*******************************************************************

	/*Info Window*/
	glutInitWindowSize(400, 600);			// specify a window size
	glutInitWindowPosition(925, 0);			// specify a window position
	int window1 = glutCreateWindow("Info");		// create a titled window
	//cout << "window1 " << window1 << endl;
	Init();									// setting up

	glutDisplayFunc(infoDisplayCallback);		// register a callback

	/*Info Menu*/
	glutCreateMenu(info_menu);
	glutAddMenuEntry("CLOSE", 1); // Close (hide) the Info Window

	/*Buttons Attached to Info Menu*/
	glutAttachMenu(GLUT_RIGHT_BUTTON); // right button attached to menu options	

//*******************************************************************
//*****************************************************************
//  END
//*****************************************************************
//*******************************************************************

	glutInitWindowSize(800, 600);				// specify a window size
	glutInitWindowPosition(100, 0);			// specify a window position

//////*JJ ASSIGNED WINDOW NAME*///////
	int window2= glutCreateWindow("JMG Text Editor");	// create a titled window
	
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

	/*JJ ADDED THE "HELP" AND "SAVE" OPTION BELOW AND 
	UPDATED THE MENU NUMBERS*/
	glutAddMenuEntry("SAVE", 3);
	glutAddMenuEntry("OPEN INFO", 4);
	glutAddMenuEntry("EXIT", 5);

	glutAttachMenu(GLUT_RIGHT_BUTTON); // attach event to top level menu

	glutDisplayFunc(drawDisplay);		// register a callback

	glutKeyboardFunc(keyboardCallback);

	glutMouseFunc(mouseCallback);
	
	glutMainLoop();							// get into an infinite loop

	return 0;

}