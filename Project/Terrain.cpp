#include "Terrain.h"

Terrain::Terrain() {

}

Terrain::~Terrain() {
	for (int i = 0; i < l; i++) {
		delete[] hs[i];
	}
	delete[] hs;

	for (int i = 0; i < l; i++) {
		delete[] normals[i];
	}
	delete[] normals;
}

int Terrain::width() {
	return w;
}

int Terrain::length() {
	return l;
}

//Sets the height at (x, z) to y
void Terrain::setHeight(int x, int z, float y) {
	hs[z][x] = y;
	computedNormals = false;
}

//Returns the height at (x, z)
float Terrain::getHeight(int x, int z) {
	return hs[z][x];
}

//Computes the normals, if they haven't been computed yet
void Terrain::computeNormals() {
	if (computedNormals) {
		return;
	}

	//Compute the rough version of the normals
	Vec3f** normals2 = new Vec3f*[l];
	for (int i = 0; i < l; i++) {
		normals2[i] = new Vec3f[w];
	}

	for (int z = 0; z < l; z++) {
		for (int x = 0; x < w; x++) {
			Vec3f sum(0.0f, 0.0f, 0.0f);

			Vec3f out;
			if (z > 0) {
				out = Vec3f(0.0f, hs[z - 1][x] - hs[z][x], -1.0f);
			}
			Vec3f in;
			if (z < l - 1) {
				in = Vec3f(0.0f, hs[z + 1][x] - hs[z][x], 1.0f);
			}
			Vec3f left;
			if (x > 0) {
				left = Vec3f(-1.0f, hs[z][x - 1] - hs[z][x], 0.0f);
			}
			Vec3f right;
			if (x < w - 1) {
				right = Vec3f(1.0f, hs[z][x + 1] - hs[z][x], 0.0f);
			}

			if (x > 0 && z > 0) {
				sum += out.cross(left).normalize();
			}
			if (x > 0 && z < l - 1) {
				sum += left.cross(in).normalize();
			}
			if (x < w - 1 && z < l - 1) {
				sum += in.cross(right).normalize();
			}
			if (x < w - 1 && z > 0) {
				sum += right.cross(out).normalize();
			}

			normals2[z][x] = sum;
		}
	}

	//Smooth out the normals
	const float FALLOUT_RATIO = 0.5f;
	for (int z = 0; z < l; z++) {
		for (int x = 0; x < w; x++) {
			Vec3f sum = normals2[z][x];

			if (x > 0) {
				sum += normals2[z][x - 1] * FALLOUT_RATIO;
			}
			if (x < w - 1) {
				sum += normals2[z][x + 1] * FALLOUT_RATIO;
			}
			if (z > 0) {
				sum += normals2[z - 1][x] * FALLOUT_RATIO;
			}
			if (z < l - 1) {
				sum += normals2[z + 1][x] * FALLOUT_RATIO;
			}

			if (sum.magnitude() == 0) {
				sum = Vec3f(0.0f, 1.0f, 0.0f);
			}
			normals[z][x] = sum;
		}
	}

	for (int i = 0; i < l; i++) {
		delete[] normals2[i];
	}
	delete[] normals2;

	computedNormals = true;
}

//Returns the normal at (x, z)
Vec3f Terrain::getNormal(int x, int z) {
	if (!computedNormals) {
		computeNormals();
	}
	return normals[z][x];
}

void Terrain::loadTerrain(const char* filename, float height) {
	Image* image = loadBMP(filename);

	cout << "Image height: "<< image->height << "\n";

	w = image->width;
	l = image->height;

	hs = new float*[l];
	for (int i = 0; i < l; i++) {
		hs[i] = new float[w];
	}

	normals = new Vec3f*[l];
	for (int i = 0; i < l; i++) {
		normals[i] = new Vec3f[w];
	}

	computedNormals = false;


	for (int y = 0; y < image->height; y++) {
		for (int x = 0; x < image->width; x++) {
			unsigned char color =
				(unsigned char)image->pixels[3 * (y * image->width + x)];
			float h = height * ((color / 255.0f) - 0.5f);
			setHeight(x, y, h);
		}
	}
	delete image;


	id_Terrain = glGenLists(1);
	glNewList(id_Terrain, GL_COMPILE);
	cout << "List Terrain ID: " << id_Terrain << "\n";
	
	bool horizontal = false;
	glEnable(GL_TEXTURE_2D);

	for (int z = 0; z < length() - 1; z++) {

		if (horizontal){
			glBindTexture(GL_TEXTURE_2D, textureId1);
		}
		else{
			glBindTexture(GL_TEXTURE_2D, textureId2);
		}

		glBegin(GL_TRIANGLE_STRIP);
		for (int x = 0; x < width(); x++) {

			glTexCoord2f((float)x / length() * 64, (float)z / length() * 64);
			Vec3f normal = getNormal(x, z);
			glNormal3f(normal[0], normal[1], normal[2]);
			glVertex3f(x, getHeight(x, z), z);
			
		//	cout << "normal 0: " << normal[0] << "\n";
			if (normal[0] > 3 || normal[0] < -3){
				horizontal = false;
			}
			else{
				horizontal = true;
			}

			glTexCoord2f((float)x / length() * 64, (float)(z + 1) / length() * 64);
			normal = getNormal(x, z + 1);
			glNormal3f(normal[0], normal[1], normal[2]);
			glVertex3f(x, getHeight(x, z + 1), z + 1);
		//	cout << "normal2 0: " << normal[0] << "\n";

		}
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);

	glEndList();

}

void Terrain::draw() {
	
//glColor3f(0.3f, 0.9f, 0.0f);
	glEnable(GL_TEXTURE_2D);

	glCallList(id_Terrain);

	glDisable(GL_TEXTURE_2D);

}

void Terrain::setTextId(GLuint txt, GLuint txt2) {
	textureId1 = txt;
	textureId2 = txt2;
}