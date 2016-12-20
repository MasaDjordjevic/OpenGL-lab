#include "stdafx.h"
#include "GLTexture.h"
#include "GLU.H"


CGLTexture::CGLTexture() {
}


CGLTexture::~CGLTexture() {
}

void CGLTexture::PrepareTexturing(bool isEnabledLighting) {
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, isEnabledLighting ? GL_MODULATE : GL_REPLACE);
}

void CGLTexture::loadFromFile(CString texFileName) {
	if (id) {
		this->release();
	}

	// Alokacija ID-a i kreiranje teksture
	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D, this->id);

	// Definisanje parametara teksture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Ucitavanje bitmape
	DImage *textureImage = new DImage();
	textureImage->Load(texFileName);

	// Koopiranje sadrzaja bitmape u teksturu
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureImage->Width(), textureImage->Height(), 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, textureImage->GetDIBBits());
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, textureImage->Width(), textureImage->Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, textureImage->GetDIBBits());
	delete textureImage;
}

void CGLTexture::release() {
	if (this->id) {
		glDeleteTextures(1, &this->id);
		id = 0;
	}
}

void CGLTexture::select() {
	if (this->id) {
		glBindTexture(GL_TEXTURE_2D, this->id);
	}
}
