
#include <time.h>
#include <random>
#include <iostream> 
#include "glut.h"
#include <ctime>
#include <string>
#include <iomanip>

char ansver = 'y';
int N = 40, M = 40;											// size fild
int scale = 12;												// distance and scale

int w = scale * N;											// widht field
int h = scale * M;											// height field
int counter = 0;											// counter score

float mass_color[] = {0.1, 0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1 };			//variant colors;


int dir, num = 4;											// direction and size.
struct { int x; int y; int z; } s[100];						// size snake

class Barier {												// rectangle barier
public:
	int left_down_position_x = scale;
	int left_down_position_y = scale;
	int right_down_position_x = N * scale;
	int right_down_position_y = scale;
	int right_hight_position_x = N * scale;
	int right_hight_position_y = M * scale - 21;
	int left_hight_position_x = scale;
	int left_hight_position_y = M * scale - 21;

	void DrawBarier() {										// draw rectangle barier
		glBegin(GL_LINE_LOOP);

		glColor3f(1, 0, 0);
		glVertex3f(left_down_position_x, left_down_position_y, 0);
		glVertex3f(right_down_position_x, right_down_position_y, 0);
		glVertex3f(right_hight_position_x, right_hight_position_y, 0);
		glVertex3f(left_hight_position_x, left_hight_position_y, 0);
		glEnd();
	}
}barier;

class Target												// targets
{
public:
	int x, y, z, temp_x, temp_y;							// koor-d targets

	void New()												// create new target
	{
		temp_x = (barier.left_down_position_x+10 + rand() % barier.right_down_position_x-scale-20) / scale;		// x position
		temp_y = (barier.left_down_position_y+10+rand() % barier.right_hight_position_y - scale-20) / scale;	// y position
		z = 0;																									// z position

		for (int k = 0; k < num; k++) {
			if ((temp_x==s[k].x && temp_y==s[k].y)																// check for spawn target not on snake;
				|| (temp_x>= barier.right_down_position_x - 10 - scale											// check spawn target in barier
				|| temp_x<= barier.left_down_position_x)
				|| (temp_y >= barier.right_hight_position_y - 10 - scale
				|| temp_y <= barier.left_down_position_y))
			m.New();
		}
		x = temp_x;
		y = temp_y;
	}

	void DrawTarget()
	{
		srand(time(NULL));									// off psevdorandom													
		int p = rand() % 10;
		float r = mass_color[rand() % 10];					// random red
		float g = mass_color[rand() % 10];					// random green
		float b = mass_color[rand() % 10];					// random blue

		glColor3d(r,g,b);									// random color target
		glRectf(x * scale, y * scale, (x + 1) * scale, (y+1) * scale); 
	}
} m; 

void Draw()													//Debug
{
	glColor3f(1, 0, 0);										// red color
	glBegin(GL_LINES); 
	for (int i = 0; i < w; i += scale)						// draw horizontal line
	{
		glVertex3f(i, 0, 0); glVertex3f(i, h, 0);			
	}
	for (int j = 0; j < h; j += scale)						// draw vertical line
	{
		glVertex3f(0, j, 0); glVertex3f(w, j, 0); 
	} 
	glEnd(); 
}

void Movement()												// all rendering 
{


	for (int i = num; i > 0; --i)							// move snake
	{
		s[i].x = s[i - 1].x;								// ’ [i] bloc = i - 1
		s[i].y = s[i - 1].y;								// ....
	}

	//direction
	if (dir == 0) s[0].y += 1;								// to up
	if (dir == 1) s[0].x -= 1;								// to left
	if (dir == 2) s[0].x += 1;								// to right
	if (dir == 3) s[0].y -= 1;								// to down

	for (int i = 0; i < 10; i++)							// size snake grow up
	{
		if ((s[0].x == m.x) && (s[0].y == m.y))				// grow up if koor-d head = koor-d target
		{
			glColor3f(0, 0.5, 0.1);
			num++;											// size snake ++
			counter++;
			
			m.New();										// new target
		}
	}

	// check the borders
	if (s[0].x > barier.right_down_position_x/scale-1) {	// right border
		std::cout << std::endl << std::endl << std::setw(56) << "GAME OVER. YOUR SCORE: " << counter;
		//std::cout << "Retry game? (y/n)" << std::endl;
		//std::cin >> ansver;
		//if (ansver == 'y') goto start;
		exit(0);
	}
	if (s[0].y > barier.left_hight_position_y/scale-1) {	// up border
		std::cout << std::endl << std::endl << std::setw(56) << "GAME OVER. YOUR SCORE: " << counter;
		exit(0);
	}
	if (s[0].x < barier.left_down_position_x/scale) {		// left border
		std::cout << std::endl << std::endl << std::setw(56) << "GAME OVER. YOUR SCORE: " << counter;
		exit(0);
	}
	if (s[0].y < barier.left_down_position_y/scale) {		// down border
		std::cout << std::endl << std::endl << std::setw(56) << "GAME OVER. YOUR SCORE: " << counter;
		exit(0);
	}

	for (int i = 1; i < num; i++)							// cutting snake if accidant
		if (s[0].x == s[i].x && s[0].y == s[i].y) {			// check
			num = i;										// cutting tail snake
			counter = i;									// counter = size snake
		}
}

void Snake() 
{
	for (int i = 0; i < num; i++)
	{
		glRectf(s[i].x * scale, s[i].y * scale, (s[i].x + 1) * scale, (s[i].y +1) * scale); 
	}
}

void Key(int key, int a, int b)								// read keybord
{
	switch (key)											// switch key
	{
	case GLUT_KEY_UP:
	{
		if (dir == 3)	dir = 3;							// check swap direction
		else dir = 0;										// move to up
		break;					
	}
	case GLUT_KEY_RIGHT:
	{
		if (dir == 1)	dir = 1;							// check swap direction
		else dir = 2;										// move to right
		break;					
	}
	case GLUT_KEY_LEFT:
	{
		if (dir == 2)   dir = 2;							// check swap direction
		else dir = 1;										// move to left
		break;					
	}
	case GLUT_KEY_DOWN:
	{
		if (dir == 0)	dir = 0;							// check swap direction						
		else dir = 3;										// move to down	
		break;					
	}
	case GLUT_KEY_END:   m.New(); break;					// spawn new target (for debug)
	}
}

//function transfer int in char
void Textout( const char* str, int x, int y, GLfloat red, GLfloat green, GLfloat blue)
{
	glColor3f(red, green, blue);
	glRasterPos2f(x, y);
	int i = 0;
	while (str[i] != '\0')
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, str[i]);
		i++;
		glRasterPos2f(x + i * 0.2, y);

	}
}

void Display()												// general rendering function
{
	glClear(GL_COLOR_BUFFER_BIT);							// clear window

	//character-by-character output of points				(посимвольный вывод счета)

	Textout("S",  barier.left_hight_position_x + 10, barier.left_hight_position_y + 10,  0.8, 0.4, 0);
	Textout("C",  barier.left_hight_position_x + 20, barier.left_hight_position_y + 10,  0.8, 0.4, 0);
	Textout("O",  barier.left_hight_position_x + 30, barier.left_hight_position_y + 10,  0.8, 0.4, 0);
	Textout("R",  barier.left_hight_position_x + 40, barier.left_hight_position_y + 10,  0.8, 0.4, 0);
	Textout("E",  barier.left_hight_position_x + 50, barier.left_hight_position_y + 10,  0.8, 0.4, 0);
	Textout(":",  barier.left_hight_position_x + 60, barier.left_hight_position_y + 10,  0.8, 0.4, 0);


	int fs = counter / 10;									// first symbol score
	int ls = counter % 10;									// last symbol score
	char str[100];
	char strl[100];
	sprintf_s(str, "%d", fs);
	sprintf_s(strl, "%d", ls);
	
	if(counter<10){											// cout last symbol score
		Textout(strl, barier.left_hight_position_x + 90, barier.left_hight_position_y + 10, 0.8, 0.4, 0);
	}
	else {													// cout first and last symbol score

		Textout(str, barier.left_hight_position_x + 90, barier.left_hight_position_y + 10, 0.8, 0.4, 0);
		Textout(strl, barier.left_hight_position_x + 105, barier.left_hight_position_y + 10, 0.8, 0.4, 0);
	}


	//Draw();												// Debug function			(uncomment for debugging)
	Snake();												// rendering snake

	m.DrawTarget();											// spawn target
	barier.DrawBarier();									// rendering barier
	
	glFlush();												// rendering
	glutSwapBuffers();										// redraw
}

void timer(int = 0)											// Timer
{
	Display();  
	Movement();
	glutTimerFunc(100, timer, 0); 
}

int main(int argc, char** argv) 
{
	std::cout <<std::endl<<std::endl<<std::setw(60)<< "Game 'SNAKE' by Artemka =)))0)"; 
	
start:

		srand(time(0));
		for (int i = 0; i < 10; i++)							// first draw target
			m.New();

		s[0].x = 10;											// X start position snake
		s[0].y = 10;											// Y start position snake
		s[0].z = 0;


		//OpenGL (glut) standart function
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
		glutInitWindowSize(w, h);								//widht and hight window
		glutCreateWindow("Snake");								// name window
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0, 520, 0, 520);
		glutDisplayFunc(Display);								//  "Renderscene"
		glutSpecialFunc(Key);
		glutTimerFunc(50, timer, 0);
		glutMainLoop();
			
	return 0;

	//добавить game over при столкновении со стенками
	//разобратьс€ с 7границами
}