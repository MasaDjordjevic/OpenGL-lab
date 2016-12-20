#include "stdafx.h"
#include "GLMaterial.h"
#include "GL.h"


CGLMaterial::CGLMaterial() {
	this->vAmbient[0] = .2;
	this->vAmbient[1] = .2;
	this->vAmbient[2] = .2;
	this->vAmbient[3] = 1.0;

	this->vDiffuse[0] = .8;
	this->vDiffuse[1] = .8;
	this->vDiffuse[2] = .8;
	this->vDiffuse[3] = 1.0;

	this->vSpecular[0] = 1.0;
	this->vSpecular[1] = 1.0;
	this->vSpecular[2] = 1.0;
	this->vSpecular[3] = 1.0;

	this->vEmission[0] = 0.0;
	this->vEmission[1] = 0.0;
	this->vEmission[2] = 0.0;
	this->vEmission[3] = 1.0;

	this->fShineness = 64.0;
}


CGLMaterial::~CGLMaterial() {
}

void CGLMaterial::select() {
	const int GL_FRONT_OR_BACK_TYPE = GL_FRONT;
	glMaterialfv(GL_FRONT_OR_BACK_TYPE, GL_AMBIENT, this->vAmbient);
	glMaterialfv(GL_FRONT_OR_BACK_TYPE, GL_DIFFUSE, this->vDiffuse);
	glMaterialfv(GL_FRONT_OR_BACK_TYPE, GL_SPECULAR, this->vSpecular);
	glMaterialfv(GL_FRONT_OR_BACK_TYPE, GL_EMISSION, this->vEmission);
	glMaterialf(GL_FRONT_OR_BACK_TYPE, GL_SHININESS, this->fShineness);
}

void CGLMaterial::setAmbient(float r, float g, float b, float a) {
	this->vAmbient[0] = r;
	this->vAmbient[1] = g;
	this->vAmbient[2] = b;
	this->vAmbient[3] = a;
}

void CGLMaterial::setDiffuse(float r, float g, float b, float a) {
	this->vDiffuse[0] = r;
	this->vDiffuse[1] = g;
	this->vDiffuse[2] = b;
	this->vDiffuse[3] = a;
}

void CGLMaterial::setAmbientAndDiffuse(float r, float g, float b, float a) {
	this->setAmbient(r, g, b, a);
	this->setDiffuse(r, g, b, a);
}

void CGLMaterial::setSpecular(float r, float g, float b, float a) {
	this->vSpecular[0] = r;
	this->vSpecular[1] = g;
	this->vSpecular[2] = b;
	this->vSpecular[3] = a;
}

void CGLMaterial::setEmission(float r, float g, float b, float a) {
	this->vEmission[0] = r;
	this->vEmission[1] = g;
	this->vEmission[2] = b;
	this->vEmission[3] = a;
}

void CGLMaterial::setShininess(float s) {
	if (s < 0) {
		this->fShineness = 0.0;
	} else if (s > 128) {
		this->fShineness = 128.0;
	} else {
		this->fShineness = s;
	}
}
