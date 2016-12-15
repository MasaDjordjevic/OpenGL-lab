#include "stdafx.h"
#include "Renderer.h"


Renderer::Renderer()
{
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
	
	/*double aspect = (double)w / (double)h;
	double H = 2.0;
	glOrtho(-H*aspect, H*aspect, -H, H, 0.5, 2.0);
	glMatrixMode(GL_MODELVIEW);*/

	//-----------------------------
	wglMakeCurrent(NULL, NULL);
}

void Renderer::DrawScene(CDC * pDC)
{

	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//------------------------
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glEnable(GL_DEPTH);
	glDepthFunc(GL_LEQUAL);
	glTranslated(0, -10, -50.0);

	//gluLookAt(100, 100, 100, 0, 0, 0, 0, 10, 0);
	this->drawCube(20.0, 20.0, 30.0);

	glFlush();
	//-----------------------------
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(NULL, NULL);
	
}

void Renderer::drawCube(double a, double b, double c) {
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
