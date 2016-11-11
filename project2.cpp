/*==================================================================================================
PROGRAMMERS: Jamie Johnson, Matthew Wilson and Garren Ijames
COURSE: CSC 525/625
MODIFIED BY: Jamie Johnson, Matthew Wilson and Garren Ijames
LAST MODIFIED DATE: 11/10/2016
CONTRIBUTIONS:
	Matt - 33.3%
		Code cleanup
		Most documentation
		Cursor
			Cursor leads text
			Next line placement
			Mouse click specified start
		Text Drawing
			Mouse click specified start
		Debugging
	Garren - 33.3%
		Document Lines
		Text Drawing
			Character drawing
			Backspace
			Newlines
			Emoji
		Debugging
	Jamie - 33.3%
		Menus
		Info Window
		Emoji design
		Save to text file
		Debugging


DESCRIPTION: Project 2 - Text Editor

NOTES:
	This simple text editor allows a user to draw text to the window using the keyboard.
	Color and font options are available via menu, as is the option to show/hide document
	lines, place a smiley emoji, show the info window (if hidden), save the document to
	a text file (c:\temp\typed.txt) or exit the application.

	Bugs:
		Users can enter text beyond 30 lines. This text will start to fall outside of view area
			and can be saved to text file.
		Clicking a start position too far to the right may create a starting position beyond
			the desired position. "Too far to the right" is approx 550 pixels, which is larger
			the space filled by 60 spaces using monospace 9x15 font.
	

	Extra Features:
		Emoji Drawing - Using the menu a user can place smiley emojis
		Document lines - User can show/hide text guidelines


FILES: project2.cpp

IDE/COMPILER: Visual Studio 2013
INSTRUCTION FOR COMPILATION AND EXECUTION:
1.		Double click on projProject.sln		to OPEN the project
2.		Press Ctrl+F7						to COMPILE
3.		Press Ctrl+Shift+B					to BUILD (COMPILE+LINK)
4.		Press Ctrl+F5						to EXECUTE
==================================================================================================*/
#include <stdlib.h>
#include <GL/glut.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using std::vector;
using std::string;
using std::ofstream;
using std::cout;
using std::floor;

// Variable Declarations

// program constants
const int WINDOW_HEIGHT = 650;
const int WINDOW_WIDTH = 800;
const int WINWIDTHBY2 = WINDOW_WIDTH / 2;
const int WINHEIGHTBY2 = WINDOW_HEIGHT / 2;
const int LINE_HEIGHT = 20; 
const int MARGIN = 20;
const int MAX_LINES = 30;
const int CURSOR_SIZE = 10;
const int MAX_CHARS = 60;
const int CURSOR_HEIGHT = LINE_HEIGHT - 4;

// global access variables: typing related
int lastLinePosition    = 0;
int textLinePosition    = 0;
int currentLinePosition = 0;
int totalTextWidth      = 0;
int lineHeight;

// partition sections
struct Partition {
	string type;
	float color[3];
	void *font;
	string text;
};

// default style
float defaultColor[3] {0, 0, 0};
void* defaultFont = GLUT_BITMAP_9_BY_15;

// active colors
float activeColor[3];
void* activeFont;

// vector containing partitions
vector<Partition> partitions;

// window id's
int infoWindowId;
int mainWindowId;

// some state toggles
bool linesShowing = false;
bool startposition = false;

// emoji bitmap
GLubyte emoji[32] {
	0x0f, 0xf0, 0x38, 0x1c,
	0x60, 0x06, 0xc0, 0x03,
	0xc0, 0x03, 0x87, 0xe1,
	0x84, 0x21, 0x8e, 0x71,
	0x80, 0x01, 0x80, 0x01,
	0x86, 0x61, 0xc6, 0x63,
	0x60, 0x06, 0x30, 0x0c,
	0x18, 0x18, 0x0f, 0xf0
};

// Utility functions

void saveTextFile()
{
	ofstream outputFile;
	outputFile.open("C:\\TEMP\\typed.txt");

	if (outputFile.fail())
	{
		cout << "File was unable to be written.\n";
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
			else if (partitions[i].type == "EMOJI")
			{
				outputFile << ":)";
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
		outputFile.close();
	}
}

void createOrModifyPartition(float color[3] = activeColor, void *font = activeFont){

	bool modifyPartition = partitions.size() > 0;
	modifyPartition = modifyPartition && partitions[partitions.size() - 1].text == "";
	modifyPartition = modifyPartition && partitions[partitions.size() - 1].type != "NEWLINE";
	modifyPartition = modifyPartition && partitions[partitions.size() -1].type != "EMOJI";

	if (modifyPartition)
	{
			partitions[partitions.size() - 1].color[0] = color[0];
			partitions[partitions.size() - 1].color[1] = color[1];
			partitions[partitions.size() - 1].color[2] = color[2];
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

void createEmoji(){

	// partition with type 'EMOJI'
	Partition emoji;
	emoji.type = "EMOJI";
	partitions.push_back(emoji);
}





// Draw functions

void drawCursor(){

	// set polygon mode to fill
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// draw new cursor
	glColor3f(1, 0, 0);

	float currentRastPos[4];
	glGetFloatv(GL_CURRENT_RASTER_POSITION, currentRastPos);

	int y0 = lineHeight;
	int x0 = currentRastPos[0] - WINWIDTHBY2;
	int x1 = x0;
	int x2 = x0 + CURSOR_SIZE;
	int x3 = x2;
	int y1 = y0 + CURSOR_HEIGHT;
	int y2 = y1;
	int y3 = y0;

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
	int runningTotal = -1;
	int drawLinePosition = 1;

	for (int i = 0; i < partitions.size(); i++)
	{
		lineHeight = WINHEIGHTBY2 - (drawLinePosition * LINE_HEIGHT) - MARGIN + 3;

		if (partitions[i].type == "NEWLINE") {
			drawLinePosition += 1;
			lineHeight = WINHEIGHTBY2 - (drawLinePosition * LINE_HEIGHT) - MARGIN + 3;
			lastPartitionWidth = 0;
			runningTotal = -1;
			glRasterPos2i(-WINWIDTHBY2 + MARGIN, lineHeight);
		}

		if (partitions[i].type == "EMOJI") {
				glBitmap(16, 16, 0, 0, 16, 0, emoji);
				runningTotal += 1;
				lastPartitionWidth += 20;
		}

		glColor3fv(partitions[i].color);
		glRasterPos2i(lastPartitionWidth - WINWIDTHBY2 + MARGIN, lineHeight);

		
		for (int j = 0; j < partitions[i].text.size(); j++)
		{
			if (++runningTotal == 60){
				drawLinePosition += 1;
				lineHeight = WINHEIGHTBY2 - (drawLinePosition * LINE_HEIGHT) - MARGIN + 3;
				glRasterPos2i(-WINWIDTHBY2 + MARGIN, lineHeight);
				lastPartitionWidth = 0;
				runningTotal = 0;
			}

			glutBitmapCharacter(partitions[i].font, partitions[i].text[j]);
			lastPartitionWidth += glutBitmapWidth(partitions[i].font, partitions[i].text[j]);
		}
		if (runningTotal == 59 ){
			drawLinePosition += 1;
			lineHeight = WINHEIGHTBY2 - (drawLinePosition * LINE_HEIGHT) - MARGIN + 3;
			glRasterPos2i(-WINWIDTHBY2 + MARGIN, lineHeight);
		}
	}
}

void drawLines(){

	// draw margin lines and others just to see layout of page
	glPointSize(1);

	glBegin(GL_LINES);
		glColor3f(0, 0, 0);
		glVertex2i(-WINWIDTHBY2, (WINHEIGHTBY2) - MARGIN); // margin line
		glVertex2i(WINWIDTHBY2, (WINHEIGHTBY2) - MARGIN);
		glVertex2i((-WINWIDTHBY2) + MARGIN -1, WINHEIGHTBY2);
		glVertex2i((-WINWIDTHBY2) + MARGIN -1, -WINHEIGHTBY2);
		glColor3f(0.8, 0.8, 0.8);
		for(int i = 1; i < MAX_LINES + 1; i++) {
			glVertex2i(-WINWIDTHBY2, ((WINHEIGHTBY2) - MARGIN) - i*LINE_HEIGHT);
			glVertex2i(WINWIDTHBY2, ((WINHEIGHTBY2) - MARGIN) - i*LINE_HEIGHT);
		}
	glEnd();
}

void drawDisplay(){

	glClear(GL_COLOR_BUFFER_BIT);	// draw the background
	if (linesShowing) {
		drawLines();
	}
	drawPartitions();
	drawCursor();
	glFlush(); // flush out the buffer contents
}


// Input callbacks

void keyboardCallback(unsigned char key, int x, int y){
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
	{
		// add the key to the text of the current partition
		partitions.back().text += key;
		drawDisplay();
	}
}

void mouseCallback(int button, int state, int x, int y){
	if (state == GLUT_DOWN && !startposition){
		if (currentLinePosition >= 0 && currentLinePosition < 30)
			drawCursor();

		lastLinePosition = currentLinePosition;
		currentLinePosition = floor((y - MARGIN) / 18.0);

		for (int i = 0; i < currentLinePosition; i++){
			createNewLine();
			createOrModifyPartition();
			drawDisplay();
		}

		if (currentLinePosition > textLinePosition)
			glRasterPos2i(MARGIN - (WINWIDTHBY2), (WINHEIGHTBY2) - currentLinePosition * LINE_HEIGHT - MARGIN);
		int ClickXDist = floor((x - MARGIN) / 9);

		for (int i = 0; i < ClickXDist; i++){
			keyboardCallback(32, 0, 0);
		}
		startposition = true;
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
	void *font = GLUT_BITMAP_TIMES_ROMAN_10;

	switch(entryId)
	{
		case 1:
			font = GLUT_BITMAP_TIMES_ROMAN_10;
			break;
		case 2:
			font = GLUT_BITMAP_HELVETICA_18;
			break;
		default:
			font = GLUT_BITMAP_9_BY_15;
			break;
	}
	activeFont = font;
	createOrModifyPartition(partitions[partitions.size() - 1].color, font);
}

void topMenuCallback(int entryId)
{
	switch (entryId)
	{
	case 8:
		createEmoji();
		createOrModifyPartition();
		drawDisplay();
		break;
	case 7:
		linesShowing = !linesShowing;
		drawDisplay();
		break;
	case 3:
		saveTextFile();
		break;
	case 4:
		glutSetWindow(infoWindowId); // changing the current window to the "Info Window"
		glutShowWindow(); // displaying the Info Window
		break;
	case -99:
		exit(0);
		break;
	}
}

void infoInstructions()
{
	/*Display the message inside the Info Window*/
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.55, .20, 0.45);

	string myArray[19] = {
		"DISPLAY TEXT:",
		"Place the cursor within the window",
		"and type the text of your choice. ",
		"Each line may hold 60 characters with a max of 30 lines.",

		"OPEN MENU:",
		"To open the menu for a window, simply right-click to open",
		"the menu associated with that window.",

		"CHANGE COLOR/FONT STYLE:",
		"Open the right-menu,",

		"SAVE TEXT:",
		"Select Save from the right-click menu",
		"Text is saved to c:\\temp\\typed.txt.",

		"CLOSE MAIN WINDOW:",
		"Select the Close option from the right-click menu.",

		"OPEN/RE-OPEN INFO WINDOW:",
		"Select the Info option from the right-click menu.",

		"CLOSE INFO WINDOW: ",
		"Select the Close option from the right-click menu",
		"within the Info window."
	};

	for (int i = 0; i < 19; i++)
	{
		glRasterPos2i(-300, 250 - 25 * i); // position
		for (int j = 0; j < myArray[i].size(); j++)
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, myArray[i][j]);
		}
	}
}

void infoDisplayCallback()
{
	glClear(GL_COLOR_BUFFER_BIT);	// draw the background

	infoInstructions(); // call the instructions function

	glFlush();
}

void info_menu(int id)
{
	if (id == 1)
	{
		glutHideWindow(); //hide info window
	}
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
	gluOrtho2D(-WINWIDTHBY2, WINWIDTHBY2, -WINHEIGHTBY2, WINHEIGHTBY2);  // specify a viewing area
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

int main(int argc, char **argv){

	glutInit(&argc, argv);

	// info window
	glutInitWindowSize(400, 700);
	glutInitWindowPosition(925, 0);
	infoWindowId = glutCreateWindow("Info");

	Init();

	glutCreateMenu(info_menu);
	glutAddMenuEntry("CLOSE", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutDisplayFunc(infoDisplayCallback);


	// main window
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(100, 0);
	mainWindowId = glutCreateWindow("JMG Text Editor");

	Init();

	// color sub-menu
	int colorSubMenuId = glutCreateMenu(colorMenuCallback);
	glutAddMenuEntry("RED", 1);
	glutAddMenuEntry("GREEN", 2);
	glutAddMenuEntry("BLUE", 3);
	glutAddMenuEntry("BLACK", 4);

	// font sub-menu
	int fontSubMenuId = glutCreateMenu(fontMenuCallback);
	glutAddMenuEntry("TIMES NEW ROMAN (10pt)", 1);
	glutAddMenuEntry("HELVETICA (18pt)", 2);
	glutAddMenuEntry("Monospace 9x15", -11);

	// create top level menu
	int topMenuId = glutCreateMenu(topMenuCallback);
	glutAddSubMenu("COLOR", colorSubMenuId);
	glutAddSubMenu("FONT", fontSubMenuId);
	glutAddMenuEntry("INSERT SMILE", 8);
	glutAddMenuEntry("TOGGLE LINES", 7);
	glutAddMenuEntry("SAVE", 3);
	glutAddMenuEntry("OPEN INFO", 4);
	glutAddMenuEntry("EXIT", -99);
	glutAttachMenu(GLUT_RIGHT_BUTTON); // attach event to top level menu

	glutDisplayFunc(drawDisplay);		// register a callback
	glutKeyboardFunc(keyboardCallback);
	glutMouseFunc(mouseCallback);

	glutMainLoop();							// get into an infinite loop

	return 0;
}
