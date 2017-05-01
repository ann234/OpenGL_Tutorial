#include <Windows.h>
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/glut.h>
#include <string.h>
#define _USE_MATH_DEFINES // for C++
#include <cmath>
#include <iostream>

#pragma comment(lib, "glew32.lib")

using namespace std;

static bool mouseLeftDown;

void timer(int value);

void RenderScene();
void Init();
void SetupRC();
void ChangeSize(int w, int h);
void timer(int value);
void keyboard(unsigned char key, int x, int y);
void specialKeyboard(int key, int x, int y);
void mouseButton(int button, int state, int x, int y);
void mouseMotion(int x, int y);
void menu(int item);

float theta[3];
float camPos[3];
float currentMatrix[16];

void getRotationMat(float x, float y, float z, float *ret);
void translate(float x, float y, float z, float *ret);

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Simple");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutTimerFunc(1000 / 30, timer, 1);

	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeyboard);

	Init();
	SetupRC();

	glutMainLoop();
}

void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//////////Camera Walking

	//glRotatef(theta[0], 1, 0, 0);
	//glRotatef(theta[1], 0, 1, 0);
	//glRotatef(theta[2], 0, 0, 1);
	//glTranslatef(camPos[0], camPos[1], camPos[2]);

	float mat[16];
	for (int i = 0; i < 16; i++)
		mat[i] = 0;
	mat[0] = mat[5] = mat[10] = mat[15] = 1;
	getRotationMat(theta[0], -theta[1], -theta[2], mat);
	glMultMatrixf(mat);

	translate(camPos[0], camPos[1], camPos[2], mat);
	glMultMatrixf(mat);

	//////////

	//gluLookAt(camPos[0], camPos[1], camPos[2], 0, 0, -1, 0, 1, 0);

	glPushMatrix();
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glColor3f(0, 0, 0);
	glTranslatef(0, 0, -2);

	glutSolidTeapot(1.0);
	glPopMatrix();

	//Plane
	glBegin(GL_QUADS);
	glColor3f(0.8, 0.8, 0.8);
	glVertex3f(-5, -2, -5);
	glVertex3f(-5, -2, 5);
	glVertex3f(5, -2, 5);
	glVertex3f(5, -2, -5);
	glEnd();


	//GLfloat vertices[8][3] = { { -1, -1, 1 },{ -1, 1, 1 },
	//{ 1, 1, 1 },{ 1, -1, 1 },{ -1, -1, -1 },
	//{ -1, 1, -1 },{ 1, 1, -1 },{ 1, -1, -1 } };

	//GLfloat colors[8][3] = { { 0, 0, 1 },{ 0, 1, 1 },
	//{ 1, 1, 1 },{ 1, 0, 1 },{ 0, 0, 0 },
	//{ 0, 1, 0 },{ 1, 1, 0 },{ 1, 0, 0 } };

	//glBegin(GL_QUADS);
	//// quad #1
	//glColor3fv(colors[0]);	glVertex3fv(vertices[0]);
	//glColor3fv(colors[3]);	glVertex3fv(vertices[3]);
	//glColor3fv(colors[2]);	glVertex3fv(vertices[2]);
	//glColor3fv(colors[1]);	glVertex3fv(vertices[1]);
	//// quad #2
	//glColor3fv(colors[2]);	glVertex3fv(vertices[2]);
	//glColor3fv(colors[3]);	glVertex3fv(vertices[3]);
	//glColor3fv(colors[7]);	glVertex3fv(vertices[7]);
	//glColor3fv(colors[6]);	glVertex3fv(vertices[6]);
	//// quad #3
	//glColor3fv(colors[3]);	glVertex3fv(vertices[3]);
	//glColor3fv(colors[0]);	glVertex3fv(vertices[0]);
	//glColor3fv(colors[4]);	glVertex3fv(vertices[4]);
	//glColor3fv(colors[7]);	glVertex3fv(vertices[7]);
	//// quad #4
	//glColor3fv(colors[1]);	glVertex3fv(vertices[1]);
	//glColor3fv(colors[2]);	glVertex3fv(vertices[2]);
	//glColor3fv(colors[6]);	glVertex3fv(vertices[6]);
	//glColor3fv(colors[5]);	glVertex3fv(vertices[5]);
	//// quad #5
	//glColor3fv(colors[4]);	glVertex3fv(vertices[4]);
	//glColor3fv(colors[5]);	glVertex3fv(vertices[5]);
	//glColor3fv(colors[6]);	glVertex3fv(vertices[6]);
	//glColor3fv(colors[7]);	glVertex3fv(vertices[7]);
	//// quad #6
	//glColor3fv(colors[5]);	glVertex3fv(vertices[5]);
	//glColor3fv(colors[4]);	glVertex3fv(vertices[4]);
	//glColor3fv(colors[0]);	glVertex3fv(vertices[0]);
	//glColor3fv(colors[1]);	glVertex3fv(vertices[1]);
	//glEnd();

	glutSwapBuffers();
}

void Init(void)
{
	//theta = 0.0f;
	glutTimerFunc(1000 / 30, timer, 1);

	float mat[16];
}

void SetupRC()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void ChangeSize(int w, int h)
{
	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60, (GLfloat)w / (GLfloat)h, 1.0, 1000.0);
}

void timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(1000 / 30, timer, 1);
}

void multMat(float *Lmat, float *Rmat, float *ret)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			float r = 0;
			for (int k = 0; k < 4; k++)
			{
				r += Lmat[i + (k * 4)] * Rmat[k + (j * 4)];
			}
			ret[i + (j * 4)] = r;
		}
	}
}

void getRotationMat(float x, float y, float z, float *ret)
{
	float identity[16];
	float matX[16], matY[16], matZ[16];

	for (int i = 0; i < 16; i++)
	{
		identity[i] = 0;
		matX[i] = matY[i] = matZ[i] = 0;
	}
	x = x / 180 * M_PI;
	y = y / 180 * M_PI;
	z = z / 180 * M_PI;

	identity[0] = identity[5] = identity[10] = identity[15] = 1;
	matX[0] = 1;	matX[5] = cosf(x);	matX[6] = sinf(x);	matX[9] = -sinf(x);	matX[10] = cosf(x);	matX[15] = 1;
	matY[0] = cosf(y);	matY[2] = sinf(y);	matY[5] = 1;	matY[8] = -sinf(y);	matY[10] = cosf(y);	matY[15] = 1;
	matZ[0] = cosf(z);	matZ[1] = sinf(z);	matZ[4] = -sinf(z);	matZ[5] = cos(z);	matZ[10] = 1;	matZ[15] = 1;

	multMat(matX, identity, ret);
	for (int i = 0; i < 16; i++)
	{
		identity[i] = 0;
	}
	identity[0] = identity[5] = identity[10] = identity[15] = 1;
	 
	multMat(matY, ret, identity);
	for (int i = 0; i < 16; i++)
	{
		ret[i] = identity[i];
	}
	for (int i = 0; i < 16; i++)
	{
		identity[i] = 0;
	}
	identity[0] = identity[5] = identity[10] = identity[15] = 1;

	multMat(matZ, ret, identity);
	for (int i = 0; i < 16; i++)
	{
		ret[i] = identity[i];
	}
	for (int i = 0; i < 16; i++)
	{
		identity[i] = 0;
	}
	identity[0] = identity[5] = identity[10] = identity[15] = 1;
}

void translate(float x, float y, float z, float *ret)
{
	float beforeMat[16];
	float rotMat[16];
	float trMat[16];
	float rotANDtr[16];

	//make identity matrix
	for (int i = 0; i < 16; i++)
	{
		beforeMat[i] = ret[i];
		rotMat[i] = 0; trMat[i] = 0; rotANDtr[i] = 0;
	}
	trMat[0] = 1;	trMat[5] = 1;	trMat[10] = 1;
	trMat[12] = x;	trMat[13] = y;	trMat[14] = z;	trMat[15] = 1;

	//multMat(trMat, beforeMat, ret);
	getRotationMat(theta[0], theta[1], theta[2], rotMat);
	multMat(trMat, rotMat, rotANDtr);

	for (int i = 0; i < 16; i++)
	{
		cout << "mat[" << i << "] : " << rotANDtr[i] << endl;
	}
	
	multMat(rotANDtr, beforeMat, ret);
}

void calculateMatrix(float *m)
{

}

void specialKeyboard(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_LEFT:
		theta[1] -= 10.f;	break;
	case GLUT_KEY_RIGHT:
		theta[1] += 10.f;	break;
	case GLUT_KEY_UP:
		theta[0] -= 10.f;	break;
	case GLUT_KEY_DOWN:
		theta[0] += 10.f;	break;
	default:
		break;
	}
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'w' | 'W':
		camPos[2] += 0.1f;	break;
	case 's' | 'S':
		camPos[2] -= 0.1f;	break;
	case 'a' | 'A':
		camPos[0] += 0.1f;	break;
	case 'd' | 'D':
		camPos[0] -= 0.1f;	break;
	case 'q' | 'Q':
		camPos[1] += 0.1f;	break;
	case 'e' | 'E':
		camPos[1] -= 0.1f;	break;
	case VK_ESCAPE:
		exit(0); break;
	defalut:
		break;
	}

	cout << theta[0] << endl;
}

void mouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mouseLeftDown = true;
		}
		else if (state == GLUT_UP)
		{
			if (mouseLeftDown)
				mouseLeftDown = false;
		}
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
		}
		else if (state == GLUT_UP)
		{
		}
	}

	glutPostRedisplay();
}

void mouseMotion(int x, int y)
{
}

void menu(int item)
{
	keyboard((unsigned char)item, 0, 0);
}

