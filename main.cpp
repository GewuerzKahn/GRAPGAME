

#include "Globals.h"
#include <stdlib.h>
#include "math.h"

#include "cSkybox.h"
#include "cTexture.h"
#include "OwnCube.h"
#include "Camera.h"
#include "LightSource.h"
#include "enums.h"
#include "Model.h"
#include "Terrain.h"
#include "Bullet.h"
#include "Score.h"

#define ANGLE 0.2
#define SCALE 0.2
#define RAND 0.01

enum MENU_TYPE {
    M_TASKS,
    M_SHAPE_LINE,
    M_SHAPE_FILL,
    M_SHAPE_STIPPLE,
    M_SD_DIAGONALS,
    M_LINEWIDTH_1,
    M_LINEWIDTH_2,
    M_WIREFRAME,
    M_BEAMCUBE,
    M_DEFAULT_POSITION,
    M_MOVE_LIGHT,
    M_SHOWMENU
};

int mainWindow, subWindow1, subWindow2, subWindow3;

float intAngle = 0.5;

float width, heigth;

static GLuint PreviousClock;

Camera *camera, *camera2;


OwnCube *cube1, *cube2;
LightSource *lightSource;
cSkybox *skybox;

Model *mObject;
Model *mObject2;

Terrain *terrain;

Bullet *bullet;

vector<Bullet*> bullets;
vector<int> bulletsToRemove;
vector<OwnObject*> targets;

char time[50];
char fps[50];
char score[50];


//TEXTURE ID
int textureFrontId;
int textureBackId;
int textureSkyboxId;

bool toogleTasks = false;
bool fpsMode = true;
bool mouseLeftDown = false;
bool mouseRightDown = false;
bool keyPressed[256];
bool warped = false;
bool wireframeMode = false;

const GLfloat cameraSpeed = 0.15f;
const GLfloat cameraRotationSpeed = 0.02;


GLint gFramesPerSecond = 0;
GLint lastMousePositionX = 0;
GLint lastMousePositionY = 0;


MENU_TYPE show = M_SHOWMENU;

//Position of main camera
GLfloat positionX, positionY, positionZ;
GLfloat pp, yy;

void menu(int);
void drawAxis(GLfloat metric);
void specialKeyboard(int key, int kx, int ky);
void fpsKeyboard();
void drawFrame();
bool checkCollision(OwnObject *a, OwnObject *b);

void initObjects(){
	camera = new Camera();
	camera2 = new Camera();

	mObject = new Model();

	mObject->loadTxtFile("monkey.txt");
	mObject->setMaterial(EnumMaterial::EMERALD);

	mObject2 = new Model();
	mObject2->loadObjFile("untitled2.obj");
	mObject2->setMaterial(EnumMaterial::BRASS);

	terrain = new Terrain();
	terrain->loadTerrain("heightmap.bmp", 20);

	cube1 = new OwnCube();
	cube2 = new OwnCube();

	skybox = new cSkybox();

	skybox->Load(200.0);

	lightSource = new LightSource(2, 2, -3);


	cube1->setPosition(20, -5, 20);
	cube2->setPosition(20, -5, 23);

	bullet = new Bullet(vec3(20, 0, 20), vec3(0.1, 0, 0), 2);

	targets.push_back(cube1);
	targets.push_back(cube2);

}

void init(void) {

    glShadeModel(GL_SMOOTH);

	cTexture *texture = new cTexture();
	texture->Load("Textures/lava.png", GL_RGBA);
	textureFrontId = texture->GetID();
	cube1->setTextId(textureFrontId, textureFrontId);


	cTexture *texture2 = new cTexture();
	texture2->Load("Textures/skybox.png", GL_RGBA);
	textureSkyboxId = texture2->GetID();
	skybox->Load(200.0);



	cTexture *texture3 = new cTexture();
	texture3->Load("Textures/grass.png", GL_RGBA);
	textureBackId = texture3->GetID();
	cube2->setTextId(textureBackId, textureBackId);
    //glShadeModel(GL_FLAT);
    //glMatrixMode(GL_MODELVIEW);

	terrain->loadTerrain("heightmap.bmp", 20);
	terrain->setTextId(textureBackId, textureFrontId);

}
void renderBitmapString(float x,float y,float z,void *font,char *string) {
	char *c;
	glRasterPos3f(x, y, z);
	glColor3f(1.0, 1.0, 0.0);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void FPS(void) {
    static GLint Frames = 0; // frames averaged over 1000mS
    static GLuint Clock; // [milliSeconds]
    static GLuint NextClock = 0; // [milliSeconds]

    ++Frames;
    Clock = glutGet(GLUT_ELAPSED_TIME); //has limited resolution, so average over 1000mS
    if (Clock < NextClock) return;

    gFramesPerSecond = Frames / 1; // store the averaged number of frames per second

    PreviousClock = Clock;
    NextClock = Clock + 1000; // 1000mS=1S in the future
    Frames = 0;
}

void timer(int value) {
    const int desiredFPS = 120;
    glutTimerFunc(1000 / desiredFPS, timer, ++value);
    FPS(); //only call once per frame loop to measure FPS 

    fpsKeyboard();

    glutPostRedisplay();
}

float myrand(float R)
{
	return (2 * R * rand()) / RAND_MAX - R;
}

void tree(float x1, float y1, float length1, float angle1, int depth)
{
	if (depth > 0)
	{
		// Draw line segment
		float x2 = x1 + length1 * cos(angle1);
		float y2 = y1 + length1 * sin(angle1);
		glVertex2f(x1, y1);
		glVertex2f(x2, y2);

		// Make two recursive calls
		float length2 = length1 * (2 + myrand(RAND));
		float angle2 = angle1 + ANGLE + myrand(RAND);
		tree(x2, y2, length2, angle2, depth - 1);
		length2 = length1 * (2 + myrand(RAND));
		angle2 = angle1 - ANGLE + myrand(RAND);
		tree(x2, y2, length2, angle2, depth - 1);
	}


	//DRAW
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	tree(0, 0, 1, 1.5, 10);
	glEnd();
}




void renderScene() {
	glutSetWindow(mainWindow);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutSwapBuffers();
}

void render2(void);
void renderScene2(void);

void render1(void) {

	glutSetWindow(subWindow1);

	
	if (wireframeMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glDisable(GL_NORMALIZE);
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glDisable(GL_DEPTH_TEST);


	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glEnable(GL_NORMALIZE);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_DEPTH_TEST);

	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat x, y, z;
	camera2->getPosition(x, y, z);
	//cout << "x2:" << x << " y:" << y << " z:" << z << "\n";
	camera2->setPosition(positionX , 20, positionZ);
	camera2->setPitch(-1.55F);
	camera2->setYaw(yy);
	camera2->refresh();

	renderScene2();
	glDisable(GL_LIGHTING);
	// Draw red cone at the location of the main camera
	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(positionX, positionY, positionZ);
	glRotatef(90 - (yy)*180.0 / 3.14, 0.0, 1.0, 0.0);
	glutSolidCone(0.2, 0.8f, 4, 4);
	glPopMatrix();

	glutSwapBuffers();

}

void render2(void) {
	glutSetWindow(subWindow2);


	if (wireframeMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glDisable(GL_NORMALIZE);
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glDisable(GL_DEPTH_TEST);


	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glEnable(GL_NORMALIZE);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_DEPTH_TEST);

	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera->getPosition(positionX, positionY, positionZ);
	cout << "x:" << positionX << " y:" << positionY << " z:" << positionZ << "\n";

	camera->getPitch(pp);
	camera->getYaw(yy);
	//cout << "Pitch: " << pp << " Yaw: " << yy;
	GLfloat height = terrain->getHeight(positionX, positionZ);
	Vec3f normal = terrain->getNormal(positionX, positionZ);
	//cout << positionY << "  " << height << "\n";
	//cout << normal[0] << " " << normal[1] << " " << normal[2] << "\n";
	positionY = height + 2;
	camera->setPosition(positionX, positionY, positionZ);
	camera->refresh();

	renderScene2();


	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, width, heigth, 0.0, -1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);

	//<2D Stuff
	glLineWidth(1.1F);
	glClear(GL_DEPTH_BUFFER_BIT);
	glTranslatef((width / 2 - 5), (heigth / 2 - 5), 0);
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0);
	glVertex2f(0.0, 5.0);
	glVertex2f(0.0, 20.0);
	glVertex2f(0.0, -5.0);
	glVertex2f(0.0, -20.0);
	glVertex2f(5.0, 0.0);
	glVertex2f(20.0, 0.0);
	glVertex2f(-5.0, 0.0);
	glVertex2f(-20.0, 0.0);
	glEnd();

	//2D Stuff>

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();



	glutSwapBuffers();

}

void render3(){

	glutSetWindow(subWindow3);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);


	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 400, 50, 0);
	glMatrixMode(GL_MODELVIEW);

	sprintf(fps, "FPS: %i", gFramesPerSecond);
	sprintf(time, "Time: %is", PreviousClock / 1000);
	sprintf(score, "Score: %i", Score::getScore());

	renderBitmapString(10, 32, 0, GLUT_BITMAP_TIMES_ROMAN_24, fps);
	renderBitmapString(120, 32, 0, GLUT_BITMAP_TIMES_ROMAN_24, time);
	renderBitmapString(250, 32, 0, GLUT_BITMAP_TIMES_ROMAN_24, score);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glutSwapBuffers();

}

void renderSceneAll() {
	render1();
	render2();
	render3();
}


void renderScene2(void) {

	lightSource->draw();

	glPushMatrix();
	//glTranslatef(0.0, 0.0, -50.0);
	//glTranslatef(-50.0, 0.0, 0.0);
	terrain->draw();
	glPopMatrix();


	skybox->Draw(textureSkyboxId, positionX, 0, positionZ);
	

	//Draw monkey
	glPushMatrix();
	glTranslatef(20, -3, 20);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glRotatef(180.0, 1.0, 0.0, 0.0);
	glRotatef(180.0, 0.0, 0.0, 1.0);

	mObject->draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(20, -3, 23);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glRotatef(180.0, 1.0, 0.0, 0.0);
	glRotatef(180.0, 0.0, 0.0, 1.0);
	glRotatef(-intAngle, 0.0, 0.0, 1.0);

	mObject->draw();
	glPopMatrix();

	//Draw letter
	glPushMatrix();
	glTranslatef(22, -1, 22);

	glPushMatrix();
	glRotatef(-intAngle, 0, 1, 0);
	glTranslatef(2, -1, 2);
	glRotatef(intAngle++, 0, 1, 0);
	glRotatef(90.0, 1.0, 0.0, 0.0);

	
	mObject2->draw();
	glPopMatrix();
	glPopMatrix();

	//Draw letter
	glPushMatrix();
	glTranslatef(15, 0, 27);

	glPushMatrix();
	glRotatef(-intAngle, 0, 1, 0);
	glTranslatef(2, -1, 2);
	glRotatef(intAngle++, 1, 1, 0);




	mObject2->draw();
	glPopMatrix();
	glPopMatrix();

	for (int x = 0; x < bullets.size(); x++){
		
		if (!bullets[x]->isDead()){
			bullets[x]->draw();

			for (int x = 0; x < targets.size(); x++){
				if (checkCollision(bullets[x], targets[x])){
					//Score::addScore(1);
					//cout << "true" << "\n";
				}
			}
		
		}
	}

	for (int x = 0; x < targets.size(); x++){
		if (!targets[x]->isDead()){
			targets[x]->draw();
			
		}
	}


	drawAxis(1.0);
}


void backup(void) {

	glutSetWindow(subWindow2);
	if (wireframeMode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glDisable(GL_NORMALIZE);
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glDisable(GL_DEPTH_TEST);


	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glEnable(GL_NORMALIZE);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_DEPTH_TEST);

	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	lightSource->draw();



	GLfloat positionX, positionY, positionZ;
	camera->getPosition(positionX, positionY, positionZ);
	cout << "x:" << positionX << " y:" << positionY << " z:" << positionZ << "\n";
	GLfloat height = terrain->getHeight(positionX, positionZ);
	cout << positionY << "  " << height << "\n";
	positionY = height + 2;
	camera->setPosition(positionX, positionY, positionZ);
	camera->refresh();

	glPushMatrix();
	//glTranslatef(0.0, 0.0, -50.0);
	//glTranslatef(-50.0, 0.0, 0.0);
	terrain->draw();
	glPopMatrix();


	skybox->Draw(textureSkyboxId, positionX, 0, positionZ);
	cube1->draw();
	cube2->draw();

	//Draw monkey
	glPushMatrix();
	glTranslatef(6, 0, 0);
	glRotatef(90.0, 0.0, 0.0, 1.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glRotatef(180.0, 1.0, 0.0, 0.0);
	glRotatef(180.0, 0.0, 0.0, 1.0);

	mObject->draw();
	glPopMatrix();



	drawAxis(1.0);



	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, width, heigth, 0.0, -1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);

	//<2D Stuff
	glLineWidth(1.1F);
	glClear(GL_DEPTH_BUFFER_BIT);
	glTranslatef((width / 2 - 5), (heigth / 2 - 5), 0);
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0);
	glVertex2f(0.0, 5.0);
	glVertex2f(0.0, 20.0);
	glVertex2f(0.0, -5.0);
	glVertex2f(0.0, -20.0);
	glVertex2f(5.0, 0.0);
	glVertex2f(20.0, 0.0);
	glVertex2f(-5.0, 0.0);
	glVertex2f(-20.0, 0.0);
	glEnd();

	//2D Stuff>

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();


	glutSwapBuffers();

}
void reshape(int w, int h) {

    width = w;
    heigth = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    gluPerspective(65.0, GLfloat(w) / GLfloat(h), 0.1, 200.0);
    glMatrixMode(GL_MODELVIEW);



}

void myReshape(GLsizei w, GLsizei h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h) {
        gluOrtho2D(-2.0, 2.0, -2.0 * (GLfloat) h / (GLfloat) w, 2.0 * (GLfloat) h / (GLfloat) w);
    } else {
        gluOrtho2D(-2.0 * (GLfloat) w / (GLfloat) h, 2.0 * (GLfloat) w / (GLfloat) h, -2.0, 2.0);
    }
    glMatrixMode(GL_MODELVIEW);
}

void mouse(int button, int state, int x, int y) {
    switch (button) {
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN) {
                mouseLeftDown = true;

				GLfloat dX, dY, dZ;
				camera->getDirection(dX, dY, dZ);

				Bullet *b = new Bullet(vec3(positionX, positionY, positionZ), vec3(dX, dY, dZ), 0.1);
				b->setMaterial(EnumMaterial::RED_PLASTIK);

				bullets.push_back(b);

            } else {
                mouseLeftDown = false;
            }
            break;
        case GLUT_RIGHT_BUTTON:
            if (state == GLUT_DOWN) {
                mouseRightDown = true;

            } else {
                mouseRightDown = false;
            }
            break;
        default:
            break;
    }

    lastMousePositionX = x;
    lastMousePositionY = y;
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {

        case 49:
			GLfloat dX, dY, dZ;
			camera->getDirection(dX, dY, dZ);

			//bullet = new Bullet(vec3(positionX, positionY, positionZ), vec3(dX, dY, dZ), 0.1);

			bullets.push_back(new Bullet(vec3(positionX, positionY, positionZ), vec3(dX, dY, dZ), 0.1));
            break;

        case 50:
            break;

        case 51:
            break;

        case 52:
            break;

        case 53:
            break;

        case 54:
            break;

        case 55:
            camera->setPosition(5, 5, 5);
            camera->setPitch(0);
            camera->setYaw(0);
            break;

        case 56:
            camera->setPosition(3, 3, 8);
            camera->setPitch(-0.58);
            camera->setYaw(0.95);
            break;

        case 27:
            exit(0);
            break;

        case 32:
            if (wireframeMode) {
                wireframeMode = false;
            } else {
                wireframeMode = true;
            }
            break;




    }
    keyPressed[key] = true;

}

void keyboardUp(unsigned char key, int x, int y) {
    keyPressed[key] = false;
}

void specialKeyboard(unsigned char key, int x, int y) {
}

void fpsKeyboard() {

    if (fpsMode) {
        if (keyPressed['w'] ) {
            camera->move(cameraSpeed);
		}
		else if (keyPressed['W']) {
			camera->move(cameraSpeed + 0.5);
		}
		else if (keyPressed['s'] || keyPressed['S']) {
            camera->move(-cameraSpeed);
        }
        if (keyPressed['a'] || keyPressed['A']) {
            camera->strafe(cameraSpeed);
        } else if (keyPressed['d'] || keyPressed['D']) {
            camera->strafe(-cameraSpeed);
        }
        if (keyPressed['r'] || keyPressed['R']) {
            camera->fly(cameraSpeed);
        } else if (keyPressed['f'] || keyPressed['F']) {
            camera->fly(-cameraSpeed);
        }

        if (keyPressed['m'] || keyPressed['M']) {
            GLfloat cameraPitchAngle;
            camera->getPitch(cameraPitchAngle);
            camera->setPitch(cameraPitchAngle - 0.03f);
        } else if (keyPressed['k'] || keyPressed['K']) {
            GLfloat cameraPitchAngle;
            camera->getPitch(cameraPitchAngle);
            camera->setPitch(cameraPitchAngle + 0.03f);
        }
        if (keyPressed['j'] || keyPressed['J']) {
            GLfloat cameraYawAngle;
            camera->getYaw(cameraYawAngle);
            camera->setYaw(cameraYawAngle - 0.03f);
        } else if (keyPressed['l'] || keyPressed['L']) {
            GLfloat cameraYawAngle;
            camera->getYaw(cameraYawAngle);
            camera->setYaw(cameraYawAngle + 0.03f);
        }
    }
}

void mouseMotion(int x, int y) {
        glutSetCursor(GLUT_CURSOR_NONE);

    if (fpsMode) {

        GLint dx = lastMousePositionX - x;
        GLint dy = lastMousePositionY - y;

        lastMousePositionX = x;
        lastMousePositionY = y;

        if (mouseLeftDown) {
            GLfloat angleX, angleY, angleZ;
            cube1->getAngle(angleX, angleY, angleZ);

            angleX -= dx * 0.6f;
            angleY -= dy * 0.6f;

            cube1->setAngle(angleX, angleY, 0.0);

        }

              camera->rotateYaw(-cameraRotationSpeed * dx);
              camera->rotatePitch(cameraRotationSpeed * dy);
    }
	if (x > width || y > heigth || x < 50 || y < 50){
		glutWarpPointer(width / 2, heigth / 2);
	}
}

void createMenu() {
    glutCreateMenu(menu);

    glutAddMenuEntry("Switch between tasks", M_TASKS);

    glutAddMenuEntry("Shape line", M_SHAPE_LINE);
    glutAddMenuEntry("Shape fill", M_SHAPE_FILL);
    glutAddMenuEntry("Shape stipple", M_SHAPE_STIPPLE);
    glutAddMenuEntry("Show/Hide diagonals", M_SD_DIAGONALS);
    glutAddMenuEntry("Toggle wirefrane", M_WIREFRAME);
    glutAddMenuEntry("Beam cube", M_BEAMCUBE);
    glutAddMenuEntry("Set default position", M_DEFAULT_POSITION);
    glutAddMenuEntry("De-/Active moving lightsource", M_MOVE_LIGHT);
    glutAddMenuEntry("Line width 1", M_LINEWIDTH_1);
    glutAddMenuEntry("Line width 2", M_LINEWIDTH_2);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1600, 900);
    glutInitWindowPosition(100, 100);
  
	initObjects();

	mainWindow = glutCreateWindow("Banane");
	init();
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderSceneAll);

	subWindow1 = glutCreateSubWindow(mainWindow, 1200, 0, 400, 400);
	init();
	glutDisplayFunc(render1);
	glutReshapeFunc(reshape);

	subWindow3 = glutCreateSubWindow(mainWindow, 1200, 400, 400, 50);
	//init();
	glutDisplayFunc(render3);
	glutReshapeFunc(reshape);

	subWindow2 = glutCreateSubWindow(mainWindow, 0, 0, 1600, 900);
	init();
	createMenu();
	glutDisplayFunc(render2);
	glutReshapeFunc(reshape);


    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutPassiveMotionFunc(mouseMotion);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    //    glutSpecialFunc(specialKeyboard);

    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}

void menu(int item) {
    switch (item) {
        case M_SHOWMENU:
        {
            show = (MENU_TYPE) item;
        }
            break;

        case M_TASKS:
            if (toogleTasks) {
                toogleTasks = false;
            } else {
                toogleTasks = true;
            }
            break;


        case M_SHAPE_LINE:
            break;

        case M_SHAPE_FILL:
            break;

        case M_SHAPE_STIPPLE:
            break;

        case M_SD_DIAGONALS:


        case M_LINEWIDTH_1:
            break;

        case M_LINEWIDTH_2:
            break;

        case M_WIREFRAME:
            if (wireframeMode) {
                wireframeMode = false;
            } else {
                wireframeMode = true;
            }
            break;

        case M_BEAMCUBE:
            cube2->setScale(2, 1, 1);
            cube2->setPosition(1, 0, -5);
            break;

        case M_DEFAULT_POSITION:
            camera->setPosition(-3, 3, -8);
            camera->setPitch(-0.58);
            camera->setYaw(0.95);
            break;

        case M_MOVE_LIGHT:
            lightSource->toggleMovingLight();
            break;

        default:
        {
            /* Nothing */        }
            break;
    }

    return;
}

void drawAxis(GLfloat metric) {

    glPushMatrix();

    glEnable(GL_LINE_WIDTH);
    glLineWidth(5.0); // 1.0 is default
    glBegin(GL_LINES);
    glColor3f(0, 0, 1);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(metric, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, metric, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, metric);
    glEnd();
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(3, 0xcccc);
    glBegin(GL_LINES);
    glColor3f(0, 0, 1);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(-metric, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, -metric, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, -metric);
    glEnd();
    glDisable(GL_LINE_STIPPLE);

    glPopMatrix();
}

bool checkCollision(OwnObject *a, OwnObject *b){
	{
		bool collision = false;
		GLfloat aX, aY, aZ;
		GLfloat bX, bY, bZ;

		a->getPosition(aX, aY, aZ);
		b->getPosition(bX, bY, bZ);

		//check the X axis
		
		if (abs(aX - bX) < a->getSize().x + b->getSize().x)
		{
			//check the Y axis
			if (abs(aY - bY) < a->getSize().y + b->getSize().y)
			{
				//check the Z axis
				if (abs(aZ - bZ) < a->getSize().z + b->getSize().z)
				{
					collision = true;
				}
			}
		}
		
		return collision;
	}
}