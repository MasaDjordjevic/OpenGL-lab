#pragma once
#include <gl/GL.h>
#include <gl\GLU.h>


class Renderer
{
protected:
	HGLRC m_hrc;	
	double angleX, angleY, angleZ;
public:
	Renderer();
	~Renderer();

	bool CreateGLContext(CDC* pDC);
	void PrepareScene(CDC* pDC);
	void DestroyScene(CDC* pDC);
	void Reshape(CDC* pDC, int w, int h);
	void DrawScene(CDC* pDC);

	void drawRainbowCube(double a, double b, double c);
	void DrawGrid(double dSize, int nSteps);
	void DrawAxes(double len);

	inline void ChangeViewAngleX(double diff) { angleX += diff; }
	inline void ChangeViewAngleY(double diff) { angleY += diff; }
	inline void ChangeViewAngleZ(double diff) { angleZ += diff; }

	void DrawCube(double a, double b, double c, double *color);
	void DrawWall(double size, double * color);
	void DrawWalls(double size);
	void DrawTable(double x, double y, double z, double width, double height, double depth, double offsetW, double offsetD, double topDepth, double bottomDepth, double legSize);
};

