#include "stdafx.h"
#include "Renderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"

#define rgb(r, g, b) r/255.0, g/255.0, b/255.0

Renderer::Renderer() {
	angleX = 35;
	angleY = -45;
	angleZ = 0;
	zoom = 30;

	angleLower = 25;
	angleUpper = -15;
	angleHead = 25;

	this->eyePosition[0] = 0.0;
	this->eyePosition[1] = 0.0;
}


Renderer::~Renderer() {
}

bool Renderer::CreateGLContext(CDC * pDC) {
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

void Renderer::PrepareScene(CDC * pDC) {
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//------------------------
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glutInitDisplayMode(GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	prepareLighting();
	prepareMaterials();
	//-----------------------------
	wglMakeCurrent(NULL, NULL);
}

void Renderer::DestroyScene(CDC * pDC) {
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//------------------------


	//-----------------------------
	wglMakeCurrent(NULL, NULL);
	if (m_hrc) {
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void Renderer::Reshape(CDC * pDC, int w, int h) {
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

void Renderer::DrawScene(CDC * pDC) {

	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(this->eyePosition[0], this->eyePosition[1], zoom, this->eyePosition[0], this->eyePosition[1], 1.0, 0.0, 1.0, 0.0);

	glRotatef(angleX, 1.0, 0.0, 0.0);
	glRotatef(angleY, 0.0, 1.0, 0.0);
	glRotatef(angleZ, 0.0, 0.0, 1.0);

	// Light stuff
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	float lightPosition[] = { 10.0, 30.0, 10.0, 1.0 };
	float spotDirection[] = { 0.0, -1.0, 0.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotDirection);

	this->DrawWalls(20);
	this->DrawTable(5, 0, 4, 5, 4, 3, 0.7, 0.7, 0.2, 1, 0.2);
	this->DrawLamp(-1, 0, 0.5, angleLower, angleUpper, angleHead);

	glFlush();
	//-----------------------------
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);

}

void Renderer::drawRainbowCube(double a, double b, double c) {
	glBegin(GL_QUADS);
	{
		// front
		glNormal3f(0.0, 0.0, 1.0);
		glColor3f(rgb(76, 175, 80));
		glVertex3d(-a / 2, 0, +c / 2);
		glVertex3d(-a / 2, b, +c / 2);
		glVertex3d(+a / 2, b, +c / 2);
		glVertex3d(+a / 2, 0, +c / 2);

		// right
		glNormal3f(1.0, 0.0, 0.0);
		glColor3f(rgb(156, 39, 176));
		glVertex3d(+a / 2, 0, +c / 2);
		glVertex3d(+a / 2, b, +c / 2);
		glVertex3d(+a / 2, b, -c / 2);
		glVertex3d(+a / 2, 0, -c / 2);

		// back
		glNormal3f(0.0, 0.0, -1.0);
		glColor3f(rgb(255, 193, 7));
		glVertex3d(+a / 2, 0, -c / 2);
		glVertex3d(+a / 2, b, -c / 2);
		glVertex3d(-a / 2, b, -c / 2);
		glVertex3d(-a / 2, 0, -c / 2);

		// left
		glNormal3f(-1.0, 0.0, 0.0);
		glColor3f(rgb(255, 235, 59));
		glVertex3d(-a / 2, 0, -c / 2);
		glVertex3d(-a / 2, b, -c / 2);
		glVertex3d(-a / 2, b, +c / 2);
		glVertex3d(-a / 2, 0, +c / 2);

		// bottom
		glNormal3f(0.0, -1.0, 0.0);
		glColor3f(rgb(233, 30, 99));
		glVertex3d(-a / 2, 0, -c / 2);
		glVertex3d(+a / 2, 0, -c / 2);
		glVertex3d(+a / 2, 0, +c / 2);
		glVertex3d(-a / 2, 0, +c / 2);

		// top
		glNormal3f(0.0, 1.0, 0.0);
		glColor3f(rgb(3, 169, 244));
		glVertex3d(-a / 2, b, -c / 2);
		glVertex3d(+a / 2, b, -c / 2);
		glVertex3d(+a / 2, b, +c / 2);
		glVertex3d(-a / 2, b, +c / 2);
	}
	glEnd();
}

void Renderer::DrawGrid(double dSize, int nSteps) {
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

void Renderer::DrawAxes(double len) {
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

void Renderer::DrawCube(double a, double b, double c, double *color) {
	glBegin(GL_QUADS);
	{
		glColor3d(rgb(color[0], color[1], color[2]));

		// front
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3d(-a / 2, -b / 2, +c / 2);
		glVertex3d(+a / 2, -b / 2, +c / 2);
		glVertex3d(+a / 2, +b / 2, +c / 2);
		glVertex3d(-a / 2, +b / 2, +c / 2);

		// right
		glNormal3f(1.0, 0.0, 0.0);
		glVertex3d(+a / 2, -b / 2, -c / 2);
		glVertex3d(+a / 2, +b / 2, -c / 2);
		glVertex3d(+a / 2, +b / 2, +c / 2);
		glVertex3d(+a / 2, -b / 2, +c / 2);

		// back
		glNormal3f(0.0, 0.0, -1.0);
		glVertex3d(-a / 2, -b / 2, -c / 2);
		glVertex3d(-a / 2, +b / 2, -c / 2);
		glVertex3d(+a / 2, +b / 2, -c / 2);
		glVertex3d(+a / 2, -b / 2, -c / 2);

		// left
		glNormal3f(-1.0, 0.0, 0.0);
		glVertex3d(-a / 2, -b / 2, -c / 2);
		glVertex3d(-a / 2, -b / 2, +c / 2);
		glVertex3d(-a / 2, +b / 2, +c / 2);
		glVertex3d(-a / 2, +b / 2, -c / 2);

		// bottom
		glNormal3f(0.0, -1.0, 0.0);
		glVertex3d(-a / 2, -b / 2, -c / 2);
		glVertex3d(+a / 2, -b / 2, -c / 2);
		glVertex3d(+a / 2, -b / 2, +c / 2);
		glVertex3d(-a / 2, -b / 2, +c / 2);

		// top
		glNormal3f(0.0, 1.0, 0.0);
		glVertex3d(-a / 2, +b / 2, -c / 2);
		glVertex3d(-a / 2, +b / 2, +c / 2);
		glVertex3d(+a / 2, +b / 2, +c / 2);
		glVertex3d(+a / 2, +b / 2, -c / 2);
	}
	glEnd();
}

void Renderer::DrawWall(double size, double* color) {
	glColor3dv(color);
	glBegin(GL_QUADS);
	{
		glNormal3d(0.0, 0.0, 1.0);
		glVertex3d(0.0, 0.0, 0.0);
		glVertex3d(size, 0.0, 0.0);
		glVertex3d(size, size, 0.0);
		glVertex3d(0.0, size, 0.0);
	}
	glEnd();
}

void Renderer::DrawWalls(double size) {
	double leftWallColor[3] = { 0.8, 0.8, 0.8 };
	double rightWallColor[3] = { 0.7, 0.7, 0.7 };
	double floorColor[3] = { 0.5, 0.5, 0.5 };

	this->wallMaterial.select();

	glFrontFace(GL_CW);
	this->DrawWall(size, rightWallColor);

	glFrontFace(GL_CCW);
	glPushMatrix();
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	this->DrawWall(size, leftWallColor);
	glPopMatrix();

	glPushMatrix();
	glRotatef(90.0, 1.0, 0.0, 0.0);
	this->DrawWall(size, floorColor);
	glPopMatrix();
}

void Renderer::DrawTable(double x, double y, double z, double width, double height, double depth, double offsetW, double offsetD, double topHeight, double bottomHeight, double legSize) {
	double top[3] = { rgb(69, 90, 100) };
	double bottom[3] = { rgb(255, 193, 7) };
	double leg[3] = { rgb(121, 85, 72) };

	this->woodMaterial.select();

	glTranslated(x, y + height - topHeight / 2, z);
	DrawCube(width, topHeight, depth, top);
	glPushMatrix();
	glTranslated(0, -topHeight / 2 - bottomHeight / 2, 0);
	DrawCube(width - offsetW, bottomHeight, depth - offsetD, bottom);
	glPopMatrix();

	double legHeight = height - topHeight;
	glPushMatrix();
	glTranslated(-(width - offsetW) / 2, -topHeight / 2 - legHeight / 2, -(depth - offsetD) / 2);
	DrawCube(legSize, legHeight, legSize, leg);

	glTranslated(width - offsetW, 0, 0);
	DrawCube(legSize, legHeight, legSize, leg);

	glTranslated(0, 0, depth - offsetD);
	DrawCube(legSize, legHeight, legSize, leg);

	glTranslated(-(width - offsetW), 0, 0);
	DrawCube(legSize, legHeight, legSize, leg);

	glPopMatrix();
}

void Renderer::DrawLamp(double x, double y, double z, double lowerAngle, double upperAngle, double headAngle) {
	double lampColor[3] = { rgb(121, 134, 203) };
	double headColor[3] = { rgb(33, 150, 243) };

	double barWidth = 0.1;
	double barHeight = 2;

	this->lampMaterial.select();

	glPushMatrix();
	glTranslated(x, y, z);
	glColor3dv(lampColor);

	double xPlane[4] = { 0, 1, 0, 0 };
	DrawHemisphere(xPlane, 0.5);

	//lower
	glRotatef(lowerAngle, 0.0, 0.0, 1.0);
	glTranslatef(0, barHeight / 2, 0);
	DrawCube(barWidth, barHeight, barWidth, lampColor);

	//joint
	glColor3dv(headColor);
	glTranslatef(0.0, barHeight / 2.0, 0.0);
	glutSolidSphere(0.15, 10, 10);

	//upper
	glColor3dv(lampColor);
	glRotatef(upperAngle - 1.5 * lowerAngle, 0, 0, 1);
	glTranslatef(0.0, barHeight / 2.0, 0.0);
	DrawCube(barWidth, barHeight, barWidth, lampColor);

	//head
	glRotatef(headAngle, 0, 1, 0);
	glTranslatef(0, barHeight / 2, 0);
	DrawLampHead();


	glPopMatrix();
}

void Renderer::DrawHemisphere(double* clipPlane, double radius) {
	glEnable(GL_CLIP_PLANE0);
	glClipPlane(GL_CLIP_PLANE0, clipPlane);
	glutSolidSphere(radius, 10, 10);
	glDisable(GL_CLIP_PLANE0);
}

void Renderer::DrawLampHead() {
	float headRadius = 0.5, headWidth = 0.8, headHeight = 0.3;
	double headColor[3] = { rgb(33, 150, 243) };
	glColor3dv(headColor);

	glPushMatrix();

	//small sphere
	glTranslatef(-headWidth / 2.0, 0.0, 0.0);
	double clip[4] = { -1.0, 0.0, 0.0, 0 };
	double radius = headHeight / 2 * sqrt(2);
	DrawHemisphere(clip, radius);

	// box
	glTranslatef(radius*0.8 + headHeight / 2, 0.0, 0.0);
	DrawCube(headWidth, headHeight, headHeight, headColor);

	// large sphere
	glTranslatef(headWidth * 0.8, 0.0, 0.0);
	double clipPlane[4] = { -1.0, 0.0, 0.0, headRadius / 2.0 };
	DrawHemisphere(clipPlane, headRadius);

	glPopMatrix();
}

void Renderer::prepareLighting() {
	float lightAmbient[] = { .1, .2, .2, 1.0 };
	float lightDiffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	float lightSpecular[] = { 1.0, 1.0, 1.0, 1.0 };

	// Color and light
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular);

	// Slabljenje
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, .5);

	// Usmeravanje izvora
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 7.0);

	// Aktiviranje
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
}

void Renderer::prepareMaterials() {
	this->wallMaterial.setAmbient(.3, .2, .1, 1.0);
	this->wallMaterial.setShininess(.03);

	this->woodMaterial.setAmbient(rgb(121, 85, 72), 1);
	this->woodMaterial.setDiffuse(rgb(121, 85, 72), 1);

	this->lampMaterial.setAmbient(rgb(211, 47, 47), 1);
	this->lampMaterial.setDiffuse(rgb(229, 57, 53), 1);
	this->lampMaterial.setSpecular(rgb(255, 235, 238), 1);

	this->bulbMaterial.setEmission(1.0, 1.0, 1.0, 1.0);
	this->bulbMaterial.setShininess(128);
	this->bulbMaterial.setAmbient(.7, .7, .7, 1.0);
}
