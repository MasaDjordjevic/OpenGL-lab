#pragma once
class CGLMaterial {
protected:
	float vAmbient[4];
	float vDiffuse[4];
	float vSpecular[4];
	float vEmission[4];
	float fShineness;
public:
	CGLMaterial();
	~CGLMaterial();

	void select();
	void setAmbient(float r, float g, float b, float a);
	void setDiffuse(float r, float g, float b, float a);
	void setSpecular(float r, float g, float b, float a);
	void setEmission(float r, float g, float b, float a);
	void setShininess(float s);
};

