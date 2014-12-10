#include "Bullet.h"


Bullet::Bullet(vec3 pos, vec3 vel, GLfloat siz){
	position = pos;
	velocity = vel;
	size = siz;

	objectSize = vec3(size, size, size);
}

void Bullet::draw(){

	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	materials(&material);

	glTranslatef(position.x, position.y, position.z);
	glutSolidSphere(size, 10, 10);
	materials(&noMaterials);

	glPopAttrib();
	glPopMatrix();

	position.x += velocity.x * 0.1f;
	position.y += velocity.y * 0.1f;
	position.z += velocity.z * 0.1f;


}

void Bullet::update(){
	position.x += velocity.x * 0.01f;
	position.y += velocity.y * 0.01f;
	position.z += velocity.z * 0.01f;
}
