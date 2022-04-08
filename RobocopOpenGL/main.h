#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;
//#include <cmath>

#include "vgl.h"
#include "LoadShaders.h"
#include "objloader.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glut.h"
#include "imgui/imgui_impl_opengl2.h"
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#define deg2rad(x) ((x)*((3.1415926f)/(180.0f)))
#define rad2deg(x) ((180.0f) / ((x)*(3.1415926f)))

#define EXTRAPARTS 2
#define PARTSNUM 19
#define PARTSTOTAL 19+2

#define HEAD 0
#define NECK 1
#define UPPER_BODY 2
#define ABS 3
#define CROTCH 4
#define LEFT_UPPER_THIGH 5
#define LEFT_LOWER_THIGH 6
#define LEFT_FOOT 7
#define LEFT_FOOT_TOES 8
#define RIGHT_UPPER_THIGH 9
#define RIGHT_LOWER_THIGH 10
#define RIGHT_FOOT 11
#define RIGHT_FOOT_TOES 12
#define RIGHT_UPPER_ARM 13
#define RIGHT_LOWER_ARM 14
#define RIGHT_PALM 15
#define LEFT_UPPER_ARM 16
#define LEFT_LOWER_ARM 17
#define LEFT_PALM 18

enum PALMMODE {
	OPEN, 
	CLENCH
};

enum ANIMATEMODE {
	IDLE,
	WALK, 
	SQUAT, 
	JUMPINGJACK,
	GANGNAMSTYLE,
	MOONWALK,
	PUSHUP,
	SITUP
};

void myUpdateModel();

void init();

void ChangeSize(int w,int h);
void display();
void Keyboard(unsigned char key, int x, int y);
void Mouse(int button,int state,int x,int y);
void myTimerFunc(int);

void menuEvents(int option);
void ActionMenuEvents(int option);
void ModeMenuEvents(int option);
void ShaderMenuEvents(int option);

mat4 translate(float x,float y,float z);
mat4 scale(float x,float y,float z);
mat4 rotate(float angle,float x,float y,float z);

void Obj2Buffer();
void load2Buffer( string obj,int);

//animation purposes
void walk();
void squat();
void jumpingJack();
void gangnamStyle();
void moonWalk();
void pushUp();
void sitUp();

bool isFrame;
PALMMODE palmMode;
ANIMATEMODE animateMode;

GLuint VAO;
GLuint EBO; 
GLuint VBO;
GLuint uVBO;
GLuint nVBO;
GLuint mVBO;
GLuint UBO;
GLuint VBOs[PARTSNUM];
GLuint uVBOs[PARTSNUM];
GLuint nVBOs[PARTSNUM];
GLuint program;
int pNo;

float rotateCentral = 180.f;

float angles[PARTSNUM];
float position = 0.0;
float angleMain = 0.0;
float eyeAngley = 0.0;
float eyedistance = 20.0;
float size = 1;
GLfloat movex,movey;
GLint MatricesIdx;
GLuint ModelID;

int vertices_size[PARTSNUM];
int uvs_size[PARTSNUM];
int normals_size[PARTSNUM];
int materialCount[PARTSNUM];

std::vector<std::string> mtls[PARTSNUM];//use material
std::vector<unsigned int> faces[PARTSNUM];//face count
map<string,vec3> KDs;//mtl-name&Kd
map<string,vec3> KSs;//mtl-name&Ks

mat4 Projection ;
mat4 View;
mat4 Model;
mat4 Models[PARTSNUM];

#define leftHand 0
#define rightHand 1
#define leftFoot 2
#define rightFoot 3
int mode;
int action;