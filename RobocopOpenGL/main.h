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
bool renderHead = true;
bool renderLeg = true;

//background usage
bool useBackground = true;
mat4 ModelBackground;
float backGroundShiftUp = 30.02f;
float robotShiftUp = 9.653 + 1.064;

//for imgui usage

const char* glsl_version = "#version 130";
bool show_demo_window = true;
bool show_another_window = false;
ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);

bool changePalm = false;
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

//for using with GLFW
GLFWwindow* initProgramGLFW(ImFont*);
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
void walk();
float walk();
void squat();
void jumpingJack();
void gangnamStyle();
void moonWalk();
void pushUp();
void sitUp();
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
GLuint VBOs[PARTSTOTAL+1];
GLuint uVBOs[PARTSTOTAL+1];
GLuint nVBOs[PARTSTOTAL+1];
GLuint program;
int pNo;

float rotateCentral = 180.f;


//for viewing
float position = 0.0;
float eyeAngley = 0.0;
float eyedistance = 20.0;
float FoV = 80; //in degree, between 30-90
float nearClip = 0.1;
float farClip = 200;


GLint MatricesIdx;
GLuint ModelID;
int vertices_size[PARTSTOTAL+1];
int uvs_size[PARTSTOTAL+1];
int normals_size[PARTSTOTAL+1];
int materialCount[PARTSTOTAL+1];

//for shadow and lighting
//void displayLightSource(mat4,mat4);
//GLuint lightVAO; //create a different VAO for lighting, so it won't get mix up with object VAO, for easier debugging
//bool renderLightBox = true;
//const float lightScale = 0.00002;
//GLuint lightBoxBuffer;
//GLuint lightBoxIndices;
//GLuint ModelMatrixID;
//GLuint ViewMatrixID;
//GLuint ProjectionMatrixID;
//const float lightModel[] = {
//	//x-y-z element
//	//0 = right up back
//	//1 = left up back
//	//2 = right up front
//	//3 = left up front
//	//4 = right bottom back
//	//5 = left bottom back
//	//6 = left bottom front
//	//7 = right bottom front
//	1,1,-1,
//	-1,1,-1,
//	1,1,1,
//	-1,1,1,
//	1,-1,-1,
//	-1,-1,-1,
//	-1,-1,1,
//	1,-1,1
//};
//static int recTriangleStripOrder[] = {
//	//https://stackoverflow.com/questions/28375338/cube-using-single-gl-triangle-strip
//	//obtained from here
//	3,2,6,7,4,2,0,3,1,6,5,4,1,0
//};
//float lightPosition[] = { 0,1,1 };
//GLuint renderLightProgram;
//GLuint depthMapFBO; //for frame buffer
//const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024; 
//unsigned int depthMap; //for textures

std::vector<std::string> mtls[PARTSTOTAL+1];//use material
std::vector<unsigned int> faces[PARTSTOTAL+1];//face count
map<string,vec3> KDs;//mtl-name&Kd
map<string,vec3> KSs;//mtl-name&Ks

mat4 Projection ;
mat4 View;
mat4 Model;
mat4 Models[PARTSNUM];

//for monitor
int widthStart = 400;
int heightStart = 0;

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