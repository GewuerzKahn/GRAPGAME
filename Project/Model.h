#pragma once

#include "Globals.h"


using namespace std;
using namespace glm;

class Model
{
	vector<vec3> vertices;
	vector<vec3> triangles;
	vector<vec3> triangleFaces;
	vector<vec3> normals;
	vector<vec3> colors;
	vector<vec3> colorsPicking;
	vec3 color;
	string fileName;

	//Arrays for objLoader
	vector<vec2> vTextures;

	void fillUpVertices( ifstream &s, int number);
	void fillUpTriangles( ifstream &s, int number);
	void fillUpColors( ifstream &s, int number);
	void fillUpNormals();
	void printVectorFirstLast(vector<vec3> &v, int number);
	void addColorToColors(vec3 color, int id);
	bool mIsInitialised;
public:
	Model();
	~Model();

	bool loadObjFile(const std::string &filename);
	bool loadTxtFile(const std::string &filename);
	bool isInitialised() { return mIsInitialised; }
	void setColorVector(vec3 color);
	void changeTriangleColor(vec3 color, int position);
	vector<vec3> getColorVector();
	void setColorVector(vector<vec3> colors);

	void draw();
	void drawPicking();
	void setCameraVector(vec3 zoom);
	int numberOfTriangle();
	void undo();

	void saveTxtFile();

};
