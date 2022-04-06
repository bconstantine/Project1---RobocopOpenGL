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
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;



#define PARTSNUM 50

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
#define HOLDER3 10
#define HOLDER2 11
#define HOLDER1 12
#define RIGHT_LOWER_THIGH 13
#define RIGHT_FOOT 14
#define RIGHT_FOOT_TOES 15
#define RIGHT_UPPER_ARM 16
#define RIGHT_LOWER_ARM 17
#define RIGHT_PALM 18
#define RIGHT_THUMB_1 19
#define RIGHT_THUMB_2 20
#define RIGHT_POINTER_1 21
#define RIGHT_POINTER_2 22
#define RIGHT_POINTER_3 23
#define RIGHT_MIDDLE_1 24
#define RIGHT_MIDDLE_2 25
#define RIGHT_MIDDLE_3 26
#define RIGHT_INDEX_1 27
#define RIGHT_INDEX_2 28
#define RIGHT_INDEX_3 29
#define RIGHT_PINKY_1 30
#define RIGHT_PINKY_2 31
#define RIGHT_PINKY_3 32
#define LEFT_UPPER_ARM 33
#define LEFT_LOWER_ARM 34
#define LEFT_PALM 35
#define LEFT_THUMB_1 36
#define LEFT_THUMB_2 37
#define LEFT_POINTER_1 38
#define LEFT_POINTER_2 39
#define LEFT_POINTER_3 40
#define LEFT_MIDDLE_1 41
#define LEFT_MIDDLE_2 42
#define LEFT_MIDDLE_3 43
#define LEFT_INDEX_1 44
#define LEFT_INDEX_2 45
#define LEFT_INDEX_3 46
#define LEFT_PINKY_1 47
#define LEFT_PINKY_2 48
#define LEFT_PINKY_3 49

#define BODY 6
//not yet changed 
#define LEFTSHOUDER 1
#define ULEFTARM 2
#define DLEFTARM 3
#define LEFTHAND 4

void updateModels();
void myUpdateModel();

void init();

void ChangeSize(int w,int h);
void display();
void Keyboard(unsigned char key, int x, int y);
void Mouse(int button,int state,int x,int y);

void menuEvents(int option);
void ActionMenuEvents(int option);
void ModeMenuEvents(int option);
void ShaderMenuEvents(int option);

void idle(int dummy);

mat4 translate(float x,float y,float z);
mat4 scale(float x,float y,float z);
mat4 rotate(float angle,float x,float y,float z);

void Obj2Buffer();
void load2Buffer( string obj,int);

void updateObj(int);
void resetObj(int);

bool isFrame;

GLuint VAO;
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
#define WALK 1
#define IDLE 0
int mode;
int action;