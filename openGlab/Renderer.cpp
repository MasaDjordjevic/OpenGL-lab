#include "stdafx.h"
#include "Renderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"

#define rgb(r, g, b) r/255.0, g/255.0, b/255.0

Renderer::Renderer() {
	this->angleX = 25;
	this->angleY = -45;
	this->angleZ = 0;
	this->zoom = 20;

	this->angleLower = 45;
	this->angleUpper = -25;
	this->angleHead = 15;

	this->eyePosition[0] = 0.0;
	this->eyePosition[1] = .5;
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

	CGLTexture::PrepareTexturing(true);
	this->carpetTexture.loadFromFile(L"ASHSEN512.bmp");
	this->wallTexture.loadFromFile(L"WALL512.bmp");
	this->woodTexture.loadFromFile(L"PAT39.bmp");

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

	// Sijalica gore
	float lightPosition[] = { 10.0, 30.0, 10.0, 1.0 };
	float spotDirection[] = { 0, -1.0, 0 };
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

void Renderer::DrawCube(double a, double b, double c, int numberOfTiles, CGLTexture* texture) {
	glBegin(GL_QUADS);
	{
		// front
		glNormal3d(0.0, 0.0, 1.0);
		glVertex3d(-a / 2, -b / 2, +c / 2);
		glVertex3d(+a / 2, -b / 2, +c / 2);
		glVertex3d(+a / 2, +b / 2, +c / 2);
		glVertex3d(-a / 2, +b / 2, +c / 2);

		// right
		glNormal3d(1.0, 0.0, 0.0);
		glVertex3d(+a / 2, -b / 2, -c / 2);
		glVertex3d(+a / 2, +b / 2, -c / 2);
		glVertex3d(+a / 2, +b / 2, +c / 2);
		glVertex3d(+a / 2, -b / 2, +c / 2);

		// back
		glNormal3d(0.0, 0.0, -1.0);
		glVertex3d(-a / 2, -b / 2, -c / 2);
		glVertex3d(-a / 2, +b / 2, -c / 2);
		glVertex3d(+a / 2, +b / 2, -c / 2);
		glVertex3d(+a / 2, -b / 2, -c / 2);

		// left
		glNormal3d(-1.0, 0.0, 0.0);
		glVertex3d(-a / 2, -b / 2, -c / 2);
		glVertex3d(-a / 2, -b / 2, +c / 2);
		glVertex3d(-a / 2, +b / 2, +c / 2);
		glVertex3d(-a / 2, +b / 2, -c / 2);

		// bottom
		glNormal3d(0.0, -1.0, 0.0);
		glVertex3d(-a / 2, -b / 2, -c / 2);
		glVertex3d(+a / 2, -b / 2, -c / 2);
		glVertex3d(+a / 2, -b / 2, +c / 2);
		glVertex3d(-a / 2, -b / 2, +c / 2);

		// top
		if (numberOfTiles == 0) {
			glNormal3d(0.0, 1.0, 0.0);
			glVertex3d(-a / 2, +b / 2, -c / 2);
			glVertex3d(-a / 2, +b / 2, +c / 2);
			glVertex3d(+a / 2, +b / 2, +c / 2);
			glVertex3d(+a / 2, +b / 2, -c / 2);
		} else {
			double xStep = a / numberOfTiles;
			double zStep = c / numberOfTiles;
			for (double x = -a / 2; x < a / 2; x += xStep) {
				for (double z = -c / 2; z < c / 2; z += zStep) {
					glNormal3d(0.0, 1.0, 0.0);
					if (texture) glTexCoord2d(0, 0);
					glVertex3d(x, b / 2, z);
					if (texture) glTexCoord2d(1, 0);
					glVertex3d(x + xStep, b / 2, z);
					if (texture) glTexCoord2d(1, 1);
					glVertex3d(x + xStep, b / 2, z + zStep);
					if (texture) glTexCoord2d(0, 1);
					glVertex3d(x, b / 2, z + zStep);
				}
			}
		}
	}
	glEnd();
}

void Renderer::DrawWall(double size, int numberOfTiles, CGLTexture* texture) {
	if (numberOfTiles == 0) {
		glBegin(GL_QUADS);
		{
			glNormal3d(0.0, 0.0, 1.0);
			if (texture) glTexCoord2d(0, 0);
			glVertex2d(0.0, 0.0);
			if (texture) glTexCoord2d(1, 0);
			glVertex2d(size, 0.0);
			if (texture) glTexCoord2d(1, 1);
			glVertex2d(size, size);
			if (texture) glTexCoord2d(0, 1);
			glVertex2d(0.0, size);
		}
		glEnd();
	} else {
		glBegin(GL_QUADS);
		{
			double step = size / numberOfTiles;
			for (double x = 0; x < size; x += step) {
				for (double y = 0; y < size; y += step) {
					glNormal3d(0.0, 0.0, 1.0);
					if (texture) glTexCoord2d(0, 0);
					glVertex2d(x, y);
					if (texture) glTexCoord2d(1, 0);
					glVertex2d(x + step, y);
					if (texture) glTexCoord2d(1, 1);
					glVertex2d(x + step, y + step);
					if (texture) glTexCoord2d(0, 1);
					glVertex2d(x, y + step);
				}
			}
		}
		glEnd();
	}
}

void Renderer::DrawWalls(double size) {
	glEnable(GL_TEXTURE_2D);

	// zid napred
	this->wallTexture.select();
	this->wallMaterial.select();
	this->DrawWall(size, 200);

	glFrontFace(GL_CW);

	// zid levo
	glPushMatrix();
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	this->DrawWall(size, 200);
	glPopMatrix();

	// zid dole (poznat kao patos)
	glPushMatrix();
	this->carpetTexture.select();
	glRotatef(90.0, 1.0, 0.0, 0.0);
	this->DrawWall(size, 200);
	glPopMatrix();
	
	glFrontFace(GL_CCW);
	glDisable(GL_TEXTURE_2D);
}

void Renderer::DrawTable(double x, double y, double z, double width, double height, double depth, double offsetW, double offsetD, double topHeight, double bottomHeight, double legSize) {

	// top
	glTranslated(x, y + height - topHeight / 2, z);
	this->woodMaterial.select();
	glEnable(GL_TEXTURE_2D);
	this->woodTexture.select();
	DrawCube(width, topHeight, depth, 200);
	glDisable(GL_TEXTURE_2D);

	// bottom
	glPushMatrix();
	glTranslated(0, -topHeight / 2 - bottomHeight / 2, 0);
	DrawCube(width - offsetW, bottomHeight, depth - offsetD);
	glPopMatrix();

	// legs
	double legHeight = height - topHeight;
	glPushMatrix();
	glTranslated(-(width - offsetW) / 2, -topHeight / 2 - legHeight / 2, -(depth - offsetD) / 2);
	DrawCube(legSize, legHeight, legSize);
	glTranslated(width - offsetW, 0, 0);
	DrawCube(legSize, legHeight, legSize);
	glTranslated(0, 0, depth - offsetD);
	DrawCube(legSize, legHeight, legSize);
	glTranslated(-(width - offsetW), 0, 0);
	DrawCube(legSize, legHeight, legSize);

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
	DrawCube(barWidth, barHeight, barWidth);

	//joint
	glColor3dv(headColor);
	glTranslatef(0.0, barHeight / 2.0, 0.0);
	glutSolidSphere(0.15, 10, 10);

	//upper
	glColor3dv(lampColor);
	glRotatef(upperAngle - 1.5 * lowerAngle, 0, 0, 1);
	glTranslatef(0.0, barHeight / 2.0, 0.0);
	DrawCube(barWidth, barHeight, barWidth);

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

	glPushMatrix();

	//small sphere
	glTranslatef(-headWidth / 2.0, 0.0, 0.0);
	double clip[4] = { -1.0, 0.0, 0.0, 0 };
	double radius = headHeight / 2 * sqrt(2);
	DrawHemisphere(clip, radius);

	// box
	glTranslatef(radius*0.8 + headHeight / 2, 0.0, 0.0);
	DrawCube(headWidth, headHeight, headHeight);

	// large sphere
	glTranslatef(headWidth * 0.8, 0.0, 0.0);
	double clipPlane[4] = { -1.0, 0.0, 0.0, headRadius / 2.0 };
	DrawHemisphere(clipPlane, headRadius);

	this->bulbMaterial.select();
	glutSolidSphere(.2, 20, 20);

	// pozicija sijalice
	float bulbLightPosition[] = { 0.0, 0.0, 0.0 };
	float bulbLightDirection[] = { 1.0, 0.0, 0.0 };
	glLightfv(GL_LIGHT2, GL_POSITION, bulbLightPosition);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, bulbLightDirection);

	glPopMatrix();
}

void Renderer::prepareLighting() {
	// Global lighting
	GLfloat lightModelAmbient[] = { .2, .2, .2, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightModelAmbient);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);


	// Nevidljiva sijalica
	float lightAmbient[] = { .2, .2, .2, 1.0 };
	float lightDiffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	float lightSpecular[] = { 1.0, 1.0, 1.0, 1.0 };

	// Color and light
	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular);

	// Slabljenje
	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, .5);

	// Usmeravanje izvora
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 7.0);

	
	// Sijalica u lampi
	float bulbLightAmbient[] = { rgb(255,249,196), 1.0 };
	float bulbLightDiffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	float bulbLightSpecular[] = { 1.0, 1.0, 1.0, 1.0 };

	glLightfv(GL_LIGHT2, GL_AMBIENT, bulbLightAmbient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, bulbLightDiffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, bulbLightSpecular);

	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 45.0);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 2.0);
	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, .25);

	// Aktiviranje
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHTING);
}

void Renderer::prepareMaterials() {
	this->wallMaterial.setAmbientAndDiffuse(rgb(207, 216, 220), 1.0);

	this->woodMaterial.setAmbient(rgb(244, 81, 30), 1.0);
	this->woodMaterial.setDiffuse(rgb(121, 85, 72), 1.0);

	this->lampMaterial.setAmbient(rgb(211, 47, 47), 1.0);
	this->lampMaterial.setDiffuse(rgb(229, 57, 53), 1.0);
	this->lampMaterial.setSpecular(rgb(255, 235, 238), 1.0);

	this->bulbMaterial.setEmission(rgb(255, 245, 157), 1.0);
	this->bulbMaterial.setShininess(128);
	this->bulbMaterial.setAmbient(.7, .7, .7, 1.0);
}
