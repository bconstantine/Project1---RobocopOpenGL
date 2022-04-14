#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;
//#include <cmath>

//for imgui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "vgl.h"
#include "LoadShaders.h"
#include "objloader.hpp"
#include "imgui/imgui.h"
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include <thread>

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

#define WALKSPEED 1;

#define M_PI 3.14;

#define TARGET_FPS 60;

//for debugging
bool renderBodyTop = true;
bool renderArm = true;
bool renderHead = false;
bool renderLeg = true;

//background usage
bool useBackground = false;
mat4 ModelBackground;

//for imgui usage

const char* glsl_version = "#version 130";
bool show_demo_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

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

void ChangeSize(int w,int h);
void Mouse(int button,int state,int x,int y);
void myTimerFunc(int);
void resetModel();

//for using with GLFW
GLFWwindow* initProgramGLFW();
void displayGLFW(GLFWwindow* window); //displayGLFW()
void KeyboardGLFW(GLFWwindow*, int key, int scancode, int action, int mods);

//for GLFW
void error_callback(int error, const char* description) {
	fprintf(stderr, "Error: %s\n", description);
}

void Obj2Buffer();
void load2Buffer( string obj,int);

//animation purposes
float walk();
void squat();
void jumpingJack();
void gangnamStyle();
void moonWalk();
void pushUp();
void sitUp();

//additional tools function
float clip(float &var, float min, float max);

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
GLuint VBOs[PARTSNUM+1];
GLuint uVBOs[PARTSNUM+1];
GLuint nVBOs[PARTSNUM+1];
GLuint program;
int pNo;

float rotateCentral = 180.f;

float angles[PARTSNUM];
float position = 0.0;
float angleMain= 0.0;
float eyeAngley = 0.0;
float eyedistance = 20.0;
float size = 1;
GLfloat movex,movey;
GLint MatricesIdx;
GLuint ModelID;

int vertices_size[PARTSNUM+1];
int uvs_size[PARTSNUM+1];
int normals_size[PARTSNUM+1];
int materialCount[PARTSNUM+1];

std::vector<std::string> mtls[PARTSNUM+1];//use material
std::vector<unsigned int> faces[PARTSNUM+1];//face count
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

float clampValMin(float x, float clampToMin);
float clampValMax(float x, float clampToMax);


float startTime;
///////////
// For walk
bool r_isFr = false;

bool r_isUp			= true; // upper leg
bool r_isUp2		= false; // lower leg
bool r_isUp3		= false; // foot
bool r_isUp4		= true; // foot toes
bool r_is_return	= false;

bool l_isUp = false; // upper leg
bool l_isUp2 = false; // lower leg
bool l_isUp3 = false; // foot
bool l_isUp4 = true; // foot toes
bool l_is_return = false;

///////////
// For Jumping Jack
bool isOpen = false;
bool squat1 = false;
bool squat2 = false;
bool pause	= true;