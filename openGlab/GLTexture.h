#pragma once
#include "GL.H"
#include "DImage.h"

class CGLTexture {
private:
	unsigned int id;
public:
	CGLTexture();
	~CGLTexture();
	static void PrepareTexturing(bool isEnabledLighting);
	void loadFromFile(CString texFileName);
	void release();
	void select();
};

