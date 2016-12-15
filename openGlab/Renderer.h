#pragma once
#include <gl/GL.h>
#include <gl\GLU.h>

class Renderer
{
protected:
	HGLRC m_hrc;	
public:
	Renderer();
	~Renderer();

	bool CreateGLContext(CDC* pDC);
	void PrepareScene(CDC* pDC);
	void DestroyScene(CDC* pDC);
	void Reshape(CDC* pDC, int w, int h);
	void DrawScene(CDC* pDC);
	void drawCube(double a, double b, double c);
};

