#include "stdafx.h"
#include "Renderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"

Renderer::Renderer()
{
	angleX = 0.0;
	angleY = 0;
	angleZ = 0.0;

}


Renderer::~Renderer()
{
}

bool Renderer::CreateGLContext(CDC * pDC)
{
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PPIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24; //32
	pfd.cDepthBits = 24; //32
	pfd.iLayerType = PFD_MAIN_PLANE;

	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);
	if (nPixelFormat == 0)
		return false;

	int bResult = SetPixelFormat(pDC->m_hDC, nPixelFormat, &pfd);
	if (!bResult)
		return false;

	m_hrc = wglCreateContext(pDC->m_hDC);
	if (!m_hrc)
		return false;

	return true;
}

void Renderer::PrepareScene(CDC * pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//------------------------
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glutInitDisplayMode(GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);

	//-----------------------------
	wglMakeCurrent(NULL, NULL);
}

void Renderer::DestroyScene(CDC * pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//------------------------
	

	//-----------------------------
	wglMakeCurrent(NULL, NULL);
	if (m_hrc)
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void Renderer::Reshape(CDC * pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//------------------------
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, (double)w / (double)h, 1, 100);
	glMatrixMode(GL_MODELVIEW);	
	
	//-----------------------------
	wglMakeCurrent(NULL, NULL);
}

void Renderer::DrawScene(CDC * pDC)
{

	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(20.0, 15.0, 20.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glRotatef(angleX, 1.0, 0.0, 0.0);
	glRotatef(angleY, 0.0, 1.0, 0.0);
	glRotatef(angleZ, 0.0, 0.0, 1.0);

	//this->DrawAxes(10);	
	//this->drawRainbowCube(10.0, 10.0, 20.0);
	//this->DrawGrid(50, 5);

	this->DrawWalls(10);
	this->DrawTable(4, 0, 3, 5, 4, 3, 0.7, 0.7, 0.2, 1, 0.2);

	glFlush();
	//-----------------------------
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
	
}



void Renderer::drawRainbowCube(double a, double b, double c) {
	glBegin(GL_QUAD_STRIP);
	{
		glColor3f(1, .5, .5);
		glVertex3d(-a / 2, 0, +c / 2);
		glVertex3d(-a / 2, b, +c / 2);

		glColor3f(.5, 1, .5);
		glVertex3d(+a / 2, 0, +c / 2);
		glVertex3d(+a / 2, b, +c / 2);

		glColor3f(.5, .5, 1);
		glVertex3d(+a / 2, 0, -c / 2);
		glVertex3d(+a / 2, b, -c / 2);

		glColor3f(.5, .5, .5);
		glVertex3d(-a / 2, 0, -c / 2);
		glVertex3d(-a / 2, b, -c / 2);

		glColor3f(1, .5, .5);
		glVertex3d(-a / 2, 0, +c / 2);
		glVertex3d(-a / 2, b, +c / 2);
	}
	glEnd();

	glBegin(GL_QUADS);
	{
		glColor3f(.5, .5, .5);

		glVertex3d(-a / 2, 0, -c / 2);
		glVertex3d(+a / 2, 0, -c / 2);
		glVertex3d(+a / 2, 0, +c / 2);
		glVertex3d(-a / 2, 0, +c / 2);

		glVertex3d(-a / 2, b, -c / 2);
		glVertex3d(+a / 2, b, -c / 2);
		glVertex3d(+a / 2, b, +c / 2);
		glVertex3d(-a / 2, b, +c / 2);
	}
	glEnd();
}


void Renderer::DrawGrid(double dSize, int nSteps)
{
	double stepSize = dSize / (double)nSteps;
	double halfSize = dSize / 2.0;

	glLineWidth(1.0);
	glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 0.0);
	for (int i = 0; i <= nSteps; i++) {
		// horizontalne linije
		glVertex3f(-halfSize, 0.0, -halfSize + i*stepSize);
		glVertex3f(halfSize, 0.0, -halfSize + i*stepSize);

		// vertikalne linije
		glVertex3f(-halfSize + i * stepSize, 0.0, -halfSize);
		glVertex3f(-halfSize + i * stepSize, 0.0, halfSize);
	}
	glEnd();
}

void Renderer::DrawAxes(double len)
{
	glLineWidth(3.0);
	glBegin(GL_LINES);

	glColor3d(0.0, 0.0, 0.0);

	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(len, 0.0, 0.0);

	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(0.0, len, 0.0);

	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(0.0, 0.0, len);

	glEnd();
}

void Renderer::DrawCube(double a, double b, double c, double *color)
{
	float vert[24];
	float col[24];
	byte ind[24];

	vert[0] = -a / 2.0; vert[1] = -b / 2.0; vert[2] = c / 2.0;
	vert[3] = a / 2.0; vert[4] = -b / 2.0; vert[5] = c / 2.0;
	vert[6] = a / 2.0; vert[7] = b / 2.0; vert[8] = c / 2.0;
	vert[9] = -a / 2.0; vert[10] = b / 2.0; vert[11] = c / 2.0;
	vert[12] = -a / 2.0; vert[13] = -b / 2.0; vert[14] = -c / 2.0;
	vert[15] = a / 2.0; vert[16] = -b / 2.0; vert[17] = -c / 2.0;
	vert[18] = a / 2.0; vert[19] = b / 2.0; vert[20] = -c / 2.0;
	vert[21] = -a / 2.0; vert[22] = b / 2.0; vert[23] = -c / 2.0;

	for (int i = 0; i < 24; i++) {
		col[i] = color[i % 3];
	}

	ind[0] = 0; ind[1] = 1; ind[2] = 2; ind[3] = 3;
	ind[4] = 1; ind[5] = 5; ind[6] = 6; ind[7] = 2;
	ind[8] = 7; ind[9] = 6; ind[10] = 5; ind[11] = 4;
	ind[12] = 0; ind[13] = 3; ind[14] = 7; ind[15] = 4;
	ind[16] = 7; ind[17] = 3; ind[18] = 2; ind[19] = 6;
	ind[20] = 0; ind[21] = 4; ind[22] = 5; ind[23] = 1;

	glVertexPointer(3, GL_FLOAT, 0, vert);
	glColorPointer(3, GL_FLOAT, 0, col);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, ind);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void Renderer::DrawWall(double size, double* color)
{
	glColor3dv(color);
	glBegin(GL_QUADS);
	{
		glVertex2f(0 , 0 );
		glVertex2f(0, size );
		glVertex2f(size , size );
		glVertex2f(size , 0 );
	}
	glEnd();
}

void Renderer::DrawWalls(double size)
{
	double leftWallColor[3] = { 0.8, 0.8, 0.8 };
	double rightWallColor[3] = { 0.7, 0.7, 0.7 };
	double floorColor[3] = { 0.5, 0.5, 0.5 };
	
	this->DrawWall(size, rightWallColor);

	glPushMatrix();
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	this->DrawWall(size, leftWallColor);
	glPopMatrix();

	// ja ne umem da posmatram ove uglove izgleda??!!
	glPushMatrix();
	glRotatef(90.0, 1.0, 0.0, 0.0);
	this->DrawWall(size, floorColor);
	glPopMatrix();
}

void Renderer::DrawTable(double x, double y, double z, double width, double height, double depth, double offsetW, double offsetD, double topHeight, double bottomHeight, double legSize)
{
	double top[3] = { 0.6, 0.4, 0.4 };
	double bottom[3] = { 0.4, 0.1, 0.1 };
	double leg[3] = { 0.3, 0.6, 0.3 };

	
	glTranslated(x, y + height - topHeight/2, z);
	DrawCube(width, topHeight, depth, top);
	glPushMatrix();
	glTranslated(0, -topHeight/2 -bottomHeight/2, 0);
	DrawCube(width - offsetW, bottomHeight, depth - offsetD ,bottom);
	glPopMatrix();

	double legHeight = height - topHeight;
	glPushMatrix();
	glTranslated(-(width-offsetW)/2, -topHeight / 2 -legHeight/2, - (depth-offsetD)/2);
	DrawCube(legSize, legHeight, legSize, leg);
	
	glTranslated(width - offsetW, 0, 0);
	DrawCube(legSize, legHeight, legSize, leg);

	glTranslated(0,0, depth - offsetD);
	DrawCube(legSize, legHeight, legSize, leg);

	glTranslated(-(width - offsetW), 0, 0);
	DrawCube(legSize, legHeight, legSize, leg);
	

	glPopMatrix();
}