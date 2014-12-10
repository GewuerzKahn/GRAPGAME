#pragma once


#include "Globals.h"
#include "cTexture.h"
#include "imageloader.h"

class Terrain {
private:
	int w; //Width
	int l; //Length
	float** hs; //Heights
	Vec3f** normals;
	bool computedNormals; //Whether normals is up-to-date
	float _angle = 60.0f;
	Terrain* _terrain;
	int id_Terrain;

	GLuint textureId1;
	GLuint textureId2;

public:
	Terrain();
	~Terrain();
	int width();
	int length();
	void setHeight(int x, int z, float y);
	float getHeight(int x, int z);
	void computeNormals();
	Vec3f getNormal(int x, int z);
	void draw();
	void loadTerrain(const char* filename, float height);
	void setTextId(GLuint txt, GLuint txt2);

};