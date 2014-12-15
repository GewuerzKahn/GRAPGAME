#pragma once

#include "Globals.h"
#include "OwnObject.h"

using namespace std;
using namespace glm;

class Bullet : public OwnObject
{
public:
	vec3 velocity;
	GLfloat size;

	Bullet(vec3 pos, vec3 vel, GLfloat size);
	void draw();
	void update();
};
