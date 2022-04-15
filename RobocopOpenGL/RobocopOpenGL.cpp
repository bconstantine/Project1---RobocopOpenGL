#include "main.h"

vec3 camera = vec3(0, 0, 20);
string partsList[PARTSNUM] = { "head",
"neck",
"upper_body",
"abs",
"crotch",
"left_upper_thigh",
"left_lower_thigh",
"left_foot",
"left_foot_toes",
"right_upper_thigh",
"right_lower_thigh",
"right_foot",
"right_foot_toes",
"right_upper_arm",
"right_lower_arm",
"right_palm",
"left_upper_arm",
"left_lower_arm",
"left_palm"
};

string extraBodyPart[EXTRAPARTS] = {
	"right_palm_clench",
	"left_palm_clench"
};

float initialOffset[][3] = {
	{0, 6.094, 0.173}, //head
	{0, 4.581, 0.673}, //neck
	{0,2.3932, 0}, //upper_body
	{0,1.245, 0.085}, //abs
	{0,0,0}, //crotch
	{-1.288, -0.735, 0.172}, //left_upper_thigh 
	{-1.358, -5.544, -0.313}, //left_lower_thigh 
	{-1.432, -9.653, -0.182}, //left_foot 
	{-1.543, -10.489, -1.762}, //left_foot_toes 
	{1.288, -0.735, 0.172}, //right_upper_thigh
	{1.358, -5.544, -0.313}, //right_lower_thigh //10 index
	{1.432, -9.653, -0.182}, //right_foot
	{1.543, -10.489, -1.762}, //right_foot_toes
	{2.963,3.59,0.491 }, //right_upper_arm 
	{3.023,0.991,0.4}, //right_lower_arm
	{3.249,-2.01,0.342}, //right_palm // 15
	{-2.963,3.59,0.491}, //left_upper_arm
	{-3.023,0.991,0.4}, //left_lower_arm
	{-3.249,-2.01,0.342}, //left_parm // 18
};

float translatePart[PARTSNUM][3];
float rotatePart[PARTSNUM][3];

int main(int argc, char** argv) {
	//init glfw
	ImFont* font1 = NULL;
	GLFWwindow* window = initProgramGLFW(font1);
  
	double lasttime = glfwGetTime();
	float returnDegree = 0;
	while (!glfwWindowShouldClose(window)) {
		//Keep running, put the code here
		//correct eyeAngleY
		if (eyeAngley < 0) {
			eyeAngley = 360 + eyeAngley;
		}
		else if (eyeAngley > 360) {
			eyeAngley = eyeAngley - 360;
		}


		 // Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//push font
		ImGui::PushFont(font1);

		if (show_demo_window) {
			ImGui::ShowDemoWindow(&show_demo_window);
		}

		{
			ImGui::SetNextWindowSizeConstraints(ImVec2(400,1080), ImVec2(400,1080)); //width x height fixed
			ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGui::Begin("Miscellaneous Toolbox");                          // Create a window called "Hello, world!" and append into it.
			ImGui::Checkbox("Demo Window", &show_demo_window);
			//ImGui::Checkbox("Show Light Box", &renderLightBox);
			if (lightingFollowRobocop) {
				lightPosition[0] = initialOffset[CROTCH][0];
				lightPosition[1] = initialOffset[CROTCH][1] + robotShiftUp;
				lightPosition[2] = initialOffset[CROTCH][2];
				discoticLighting = false;
			}
			if (discoticLighting) {
				lightingFollowRobocop = false;
			}
			if (ImGui::CollapsingHeader("Lighting related")) {
				ImGui::Checkbox("Show Lighting box", &renderLightBox);
				ImGui::SliderFloat3("Light position", lightPosition, -1000, 1000);
				ImGui::Checkbox("Make Lighting follow Robocop", &lightingFollowRobocop);
				ImGui::Checkbox("Discotic Lighting", &discoticLighting);
				if (ImGui::Button("Reset Lighting")) {
					lightingFollowRobocop = false;
					discoticLighting = false;
					lightPosition[0] = 0;
					lightPosition[1] = 0;
					lightPosition[2] = 0;
					ambientColor[0] = 0.1;
					ambientColor[1] = 0.1;
					ambientColor[2] = 0.1;
					diffuseColor[0] = 0.8;
					diffuseColor[1] = 0.8;
					diffuseColor[2] = 0.8;
					specularColor[0] = 1;
					specularColor[1] = 1;
					specularColor[2] = 1;
					Shininess = 32;
				}
				if (ImGui::TreeNode("Ambient Color")) {
					ImGui::SetColorEditOptions(ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel);
					ImGui::ColorPicker4("", (float*)&ambientColor); // Edit 3 floats representing a color
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Diffuse Color")) {
					ImGui::SetColorEditOptions(ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel);
					ImGui::ColorPicker4("", (float*)&diffuseColor); // Edit 3 floats representing a color
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Specular Color")) {
					ImGui::SetColorEditOptions(ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel);
					ImGui::ColorPicker4("", (float*)&specularColor); // Edit 3 floats representing a color
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Shininess")) {
					ImGui::SliderFloat("Shininess", &Shininess, 2.0, 200);
					ImGui::TreePop();
				}
			}
			if (ImGui::CollapsingHeader("Projection Related")) {
				ImGui::SliderFloat("Change FoV: ", &FoV, 30.0f, 90.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
				ImGui::SliderFloat("Change near clip: ", &nearClip, 0.1f, 10.f);
				ImGui::SliderFloat("Change far clip: ", &farClip, 50.f, 200.f);
			}
			if (ImGui::CollapsingHeader("View Related: ")) {
				ImGui::SliderFloat("Change zoom: ", &eyedistance, 10.0f, 40.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
				ImGui::SliderFloat("Change eye rotation: ", &eyeAngley, 0.f, 360.f);
			}
			if (ImGui::CollapsingHeader("Background color: ")) {
				ImGui::SetColorEditOptions(ImGuiColorEditFlags_Float | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_PickerHueWheel);
				ImGui::ColorPicker3("", (float*)&clear_color); // Edit 3 floats representing a color
			}

			if (ImGui::CollapsingHeader("Current Modes: ")) {
				static int currentMode;
				static string animateInfo;
				if (animateMode == IDLE) {
					currentMode = 0;
				}
				else if (animateMode == WALK) {
					currentMode = 1;
				}
				else if (animateMode == SQUAT) {
					currentMode = 2;
				}
				else if (animateMode == JUMPINGJACK) {
					currentMode = 3;
				}
				else if (animateMode == GANGNAMSTYLE) {
					currentMode = 4;
				}
				else if (animateMode == MOONWALK) {
					currentMode = 5;
				}
				else if (animateMode == PUSHUP) {
					currentMode = 6;
				}
				else if (animateMode == SITUP) {
					currentMode = 7;
				}
				ImGui::RadioButton("Idle", &currentMode, 0); ImGui::SameLine();
				ImGui::RadioButton("Walk", &currentMode, 1); ImGui::SameLine();
				ImGui::RadioButton("Squat", &currentMode, 2); ImGui::SameLine();
				ImGui::RadioButton("Jumping Jack", &currentMode, 3); 
				ImGui::RadioButton("Gangnam Style", &currentMode, 4); ImGui::SameLine();
				ImGui::RadioButton("MoonWalk", &currentMode, 5); ImGui::SameLine();
				ImGui::RadioButton("Pushup", &currentMode, 6); ImGui::SameLine();
				ImGui::RadioButton("Situp", &currentMode, 7);

				if (currentMode == 0) {
					animateMode = IDLE;
					animateInfo = "IDLE";
				}
				else if (currentMode == 1) {
					animateMode = WALK;
					animateInfo = "WALK";
				}
				else if (currentMode == 2) {
					animateMode = SQUAT;
					animateInfo = "SQUAT";
				}
				else if (currentMode == 3) {
					animateMode = JUMPINGJACK;
					animateInfo = "JUMPING JACK";
				}
				else if (currentMode == 4) {
					animateMode = GANGNAMSTYLE;
					animateInfo = "GANGNAM STYLE";
				}
				else if (currentMode == 5) {
					animateMode = MOONWALK;
					animateInfo = "MOON WALK";
				}
				else if (currentMode == 6) {
					animateMode = PUSHUP;
					animateInfo = "PUSH UP";
				}
				else if (currentMode == 7) {
					animateMode = SITUP;
					animateInfo = "SIT UP";
				}
				ImGui::Text("Current Mode = %s", animateInfo.c_str());
			}
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::Text("Left upper thigh: %.3f\n",rotatePart[LEFT_UPPER_THIGH][0]);
			ImGui::Text("Right upper thigh: %.3f\n", rotatePart[RIGHT_UPPER_THIGH][0]);
			ImGui::Text("Left upper thigh: %.3f\n", rotatePart[LEFT_UPPER_ARM][0]);
			ImGui::Text("Right upper thigh: %.3f\n", rotatePart[RIGHT_UPPER_ARM][0]);
			ImGui::End();
		}
		//popping font
		ImGui::PopFont();
		// Rendering

		/*glfwSwapBuffers(window);

		float ratio;
		int width, height;
		mat4 m, p, mvp;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);*/
		
		//check based on animation
		if (animateMode == WALK) {
			for (int i = 0; i < 3; i++) {
				walk();
			}
		}
		else if (animateMode == JUMPINGJACK) {
			jumpingJack();
		}

		displayGLFW(window);
	}


	//stop imgui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	//stop glfw
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void resetModel() {
	for (int i = 0; i < PARTSNUM; i++) {
		translatePart[i][0] = 0.f;
		translatePart[i][1] = 0.f;
		translatePart[i][2] = 0.f;
		rotatePart[i][0] = 0.f;
		rotatePart[i][1] = 0.f;
		rotatePart[i][2] = 0.f;
	}
}

GLFWwindow* initProgramGLFW(ImFont* font1) {
	resetModel();
	ModelBackground = mat4(1.0);
	ModelBackground = translate(ModelBackground, vec3(0, backGroundShiftUp, 0));

	//work backwards to correct the offset
	initialOffset[LEFT_FOOT_TOES][0] -= initialOffset[LEFT_FOOT][0];
	initialOffset[LEFT_FOOT_TOES][1] -= initialOffset[LEFT_FOOT][1];
	initialOffset[LEFT_FOOT_TOES][2] -= initialOffset[LEFT_FOOT][2];

	initialOffset[LEFT_FOOT][0] -= initialOffset[LEFT_LOWER_THIGH][0];
	initialOffset[LEFT_FOOT][1] -= initialOffset[LEFT_LOWER_THIGH][1];
	initialOffset[LEFT_FOOT][2] -= initialOffset[LEFT_LOWER_THIGH][2];

	initialOffset[LEFT_LOWER_THIGH][0] -= initialOffset[LEFT_UPPER_THIGH][0];
	initialOffset[LEFT_LOWER_THIGH][1] -= initialOffset[LEFT_UPPER_THIGH][1];
	initialOffset[LEFT_LOWER_THIGH][2] -= initialOffset[LEFT_UPPER_THIGH][2];

	initialOffset[RIGHT_FOOT_TOES][0] -= initialOffset[RIGHT_FOOT][0];
	initialOffset[RIGHT_FOOT_TOES][1] -= initialOffset[RIGHT_FOOT][1];
	initialOffset[RIGHT_FOOT_TOES][2] -= initialOffset[RIGHT_FOOT][2];

	initialOffset[RIGHT_FOOT][0] -= initialOffset[RIGHT_LOWER_THIGH][0];
	initialOffset[RIGHT_FOOT][1] -= initialOffset[RIGHT_LOWER_THIGH][1];
	initialOffset[RIGHT_FOOT][2] -= initialOffset[RIGHT_LOWER_THIGH][2];

	initialOffset[RIGHT_LOWER_THIGH][0] -= initialOffset[RIGHT_UPPER_THIGH][0];
	initialOffset[RIGHT_LOWER_THIGH][1] -= initialOffset[RIGHT_UPPER_THIGH][1];
	initialOffset[RIGHT_LOWER_THIGH][2] -= initialOffset[RIGHT_UPPER_THIGH][2];

	initialOffset[LEFT_PALM][0] -= initialOffset[LEFT_LOWER_ARM][0];
	initialOffset[LEFT_PALM][1] -= initialOffset[LEFT_LOWER_ARM][1];
	initialOffset[LEFT_PALM][2] -= initialOffset[LEFT_LOWER_ARM][2];

	initialOffset[LEFT_LOWER_ARM][0] -= initialOffset[LEFT_UPPER_ARM][0];
	initialOffset[LEFT_LOWER_ARM][1] -= initialOffset[LEFT_UPPER_ARM][1];
	initialOffset[LEFT_LOWER_ARM][2] -= initialOffset[LEFT_UPPER_ARM][2];

	initialOffset[LEFT_UPPER_ARM][0] -= initialOffset[UPPER_BODY][0];
	initialOffset[LEFT_UPPER_ARM][1] -= initialOffset[UPPER_BODY][1];
	initialOffset[LEFT_UPPER_ARM][2] -= initialOffset[UPPER_BODY][2];

	initialOffset[RIGHT_PALM][0] -= initialOffset[RIGHT_LOWER_ARM][0];
	initialOffset[RIGHT_PALM][1] -= initialOffset[RIGHT_LOWER_ARM][1];
	initialOffset[RIGHT_PALM][2] -= initialOffset[RIGHT_LOWER_ARM][2];

	initialOffset[RIGHT_LOWER_ARM][0] -= initialOffset[RIGHT_UPPER_ARM][0];
	initialOffset[RIGHT_LOWER_ARM][1] -= initialOffset[RIGHT_UPPER_ARM][1];
	initialOffset[RIGHT_LOWER_ARM][2] -= initialOffset[RIGHT_UPPER_ARM][2];

	initialOffset[RIGHT_UPPER_ARM][0] -= initialOffset[UPPER_BODY][0];
	initialOffset[RIGHT_UPPER_ARM][1] -= initialOffset[UPPER_BODY][1];
	initialOffset[RIGHT_UPPER_ARM][2] -= initialOffset[UPPER_BODY][2];

	initialOffset[HEAD][0] -= initialOffset[NECK][0];
	initialOffset[HEAD][1] -= initialOffset[NECK][1];
	initialOffset[HEAD][2] -= initialOffset[NECK][2];

	initialOffset[NECK][0] -= initialOffset[UPPER_BODY][0];
	initialOffset[NECK][1] -= initialOffset[UPPER_BODY][1];
	initialOffset[NECK][2] -= initialOffset[UPPER_BODY][2];

	initialOffset[UPPER_BODY][0] -= initialOffset[ABS][0];
	initialOffset[UPPER_BODY][1] -= initialOffset[ABS][1];
	initialOffset[UPPER_BODY][2] -= initialOffset[ABS][2];

	
	for (int i = 0; i < PARTSNUM; i++) {
		initialOffset[i][0] *= -1;
		initialOffset[i][2] *= -1;
	}

	if (!glfwInit()) {
		cout << "Fail initialization of GLFW\n";
		exit(EXIT_FAILURE);
	}
	glfwSetErrorCallback(error_callback);

	//create window and context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_REFRESH_RATE, 60);

	//fullscreen
	//GLFWwindow* window = glfwCreateWindow(1920, 1080, "Project1 - Robocop", glfwGetPrimaryMonitor(), NULL);
	
	//windowed full screen
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Project1 - Robocop", NULL, NULL);

	if (!window) {
		//window creation failed, check whether version apply with the machine
		cout << "window creation failed\n";
		exit(EXIT_FAILURE);
	}
	r_isUp = true; // upper leg
	r_isUp2 = false; // lower leg

	isOpen = false;
	squat1 = false;
	squat2 = false;

	glfwSetKeyCallback(window, KeyboardGLFW);
	glfwMakeContextCurrent(window);

	//init glew
	glewExperimental = TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		cout << "glew init failed: " << glewGetErrorString(err) << endl;
		exit(EXIT_FAILURE);
	}



	//multisample for polygons smooth
	//glfw by default already use double buffering, depth buffer, and also (RGB?)
	glEnable(GL_MULTISAMPLE);
	//glDisable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LEQUAL);
	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glDisable(GL_CULL_FACE);

	glfwSwapInterval(1); // the time wait for every change

	isFrame = false;
	pNo = 0;
	animateMode = IDLE;
	palmMode = OPEN;

	//init imgui
	 // Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	//set fonts
	font1 = io.Fonts->AddFontFromFileTTF("fonts/Roboto-Medium.ttf", 16.0f);
	if (font1 == NULL) {
		cout << "load unsuccessful\n";
	}
	else {
		cout << "load successful\n";
	}
	IM_ASSERT(font1 != NULL);

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform / Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	//VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//EBO
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	//for lighting
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glGenBuffers(1, &lightBoxBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, lightBoxBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lightModel), lightModel, GL_STATIC_DRAW);
	glGenBuffers(1, &lightBoxIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightBoxIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(recTriangleStripOrder), recTriangleStripOrder, GL_STATIC_DRAW);

	//for shadow
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap); //2d texture similar to image
	//depth map we only care to its depth, sp set format ot GL_DEPTH_COMPONENT, each texture we give it a width and height of 1024
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//bind the frame buffer to the generated depth frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	//we only need depth information when rendering shadow, color will not affect, so no need to put color
	//However, framebuffer object however needs color buffer, so we tell opengl we won't need color data by telling
	//DrawBuffer and ReadBuffer to GL_NONE
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0); //unbind frame buffer

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "DSPhong_Material.vp" },//vertex shader
		{ GL_FRAGMENT_SHADER, "DSPhong_Material.fp" },//fragment shader
		{ GL_NONE, NULL } };
	program = LoadShaders(shaders);//Ū��shader

	ShaderInfo lightshaders[] = {
		{ GL_VERTEX_SHADER, "light_box.vp" },//vertex shader
		{ GL_FRAGMENT_SHADER, "light_box.fp" },//fragment shader
		{ GL_NONE, NULL }
	};
	renderLightProgram = LoadShaders(lightshaders);

	glUseProgram(renderLightProgram);

	ProjectionMatrixID = glGetUniformLocation(renderLightProgram, "Projection");
	ViewMatrixID = glGetUniformLocation(renderLightProgram, "View");
	ModelMatrixID = glGetUniformLocation(renderLightProgram, "ModelMatrix");

	glUseProgram(program);

	MatricesIdx = glGetUniformBlockIndex(program, "MatVP");
	ModelID = glGetUniformLocation(program, "Model");
	lightPosID = glGetUniformLocation(program, "vLightPosition");
	M_KaID = glGetUniformLocation(program, "Material.Ka");
	M_KdID = glGetUniformLocation(program, "Material.Kd");
	M_KsID = glGetUniformLocation(program, "Material.Ks");
	ambientID = glGetUniformLocation(program, "ambientColor");
	diffuseID = glGetUniformLocation(program, "diffuseColor");
	specularID = glGetUniformLocation(program, "specularColor");
	ShininessID = glGetUniformLocation(program, "Shininess");
	viewPosID = glGetUniformLocation(program, "vViewPos");
	

	Projection = glm::perspective(80.0f, (float)(1920) / (1080), 0.1f, 100.f);
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

	// Camera matrix
	viewPos[0] = translatePart[CROTCH][0];
	viewPos[1] = translatePart[CROTCH][1]+robotShiftUp+2;
	viewPos[2] = translatePart[CROTCH][2]+eyedistance;
	View = glm::lookAt(
		glm::vec3(viewPos[0],viewPos[1], viewPos[2]), // Camera is at (0,0,20), in World Space), // Camera is at (0,10,25), in World Space
		glm::vec3(translatePart[CROTCH][0], translatePart[CROTCH][1] + robotShiftUp, translatePart[CROTCH][2]), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,1,0 to look upside-down)
	);

	Obj2Buffer();

	//UBO
	glGenBuffers(1, &UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4) * 2, NULL, GL_DYNAMIC_DRAW);
	//get uniform struct size
	int UBOsize = 0;
	glGetActiveUniformBlockiv(program, MatricesIdx, GL_UNIFORM_BLOCK_DATA_SIZE, &UBOsize);
	//bind UBO to its idx
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, UBOsize);
	glUniformBlockBinding(program, MatricesIdx, 0);

	/*
	printf("Vendor: %s\n", glGetString (GL_VENDOR));
	printf("Renderer: %s\n", glGetString (GL_RENDERER));
	printf("Version: %s\n", glGetString (GL_VERSION));
	printf("GLSL: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));
	*/
	
	return window;
}

#define DOR(angle) (angle*3.1415/180);

void displayLightSource(mat4 view, mat4 proj) {
	glBindBuffer(GL_ARRAY_BUFFER, lightBoxBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightBoxIndices);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	mat4 model = mat4(1.0);
	model = translate(model, vec3(lightPosition[0], lightPosition[1], lightPosition[2]));
	model = scale(model, vec3(lightScale, lightScale, lightScale));
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &model[0][0]);
	mat4 viewMatrix = view;
	mat4 projMatrix = proj;
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(ProjectionMatrixID, 1, GL_FALSE, &projMatrix[0][0]);
	glDrawElementsBaseVertex(GL_TRIANGLE_STRIP, sizeof(recTriangleStripOrder) / sizeof(int), GL_UNSIGNED_INT, NULL,0);
}

void displayOnly(GLFWwindow* window) {
	
}

void displayGLFW(GLFWwindow* window) {
	//glClearColor(0.7, 0.7, 0.7, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
	//glClear(GL_COLOR_BUFFER_BIT);
	
	//usual rendering
	glViewport(widthStart, heightStart, 1920, 1080);

	//only update View when angle change, to save computation
	if (viewChange) {
		float eyeAngleRad = DOR(eyeAngley);
		viewPos[0] = translatePart[CROTCH][0] + eyedistance * sin(eyeAngleRad);
		viewPos[1] = translatePart[CROTCH][1] + robotShiftUp + 2;
		viewPos[2] = translatePart[CROTCH][2] + eyedistance * cos(eyeAngleRad);
		View = lookAt(
			vec3(viewPos[0],viewPos[1],viewPos[2] ), // Camera is at (0,0,20), in World Space
			vec3(translatePart[CROTCH][0], translatePart[CROTCH][1] + robotShiftUp, translatePart[CROTCH][2]), // and looks at the origin
			vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
		viewChange = false;
	}

	Projection = perspective(FoV, (float)(1920 - widthStart) / (1080 - heightStart), nearClip, farClip);

	glBindVertexArray(lightVAO);
	glUseProgram(renderLightProgram);
	if (discoticLighting) {
		mat4 lightPosMV(1.0);
		float currentTime = getTime();
		lightPosMV = translate(lightPosMV, vec3(sinf(5.1f * currentTime) * 2.5f, cosf(5.7f * currentTime) * 2.5f, sinf(5.3f * currentTime) * cosf(5.5f * currentTime) * 2.5f));
		lightPosMV = rotate(lightPosMV, currentTime * 45.0f, vec3(0.0f, 1.0f, 0.0f));
		lightPosMV = rotate(lightPosMV, currentTime * 81.0f, vec3(1.0f, 0.0f, 0.0f));
		vec4 robocopPos(translatePart[CROTCH][0], translatePart[CROTCH][1] + robotShiftUp, translatePart[CROTCH][2], 1.0f);
		robocopPos = lightPosMV * robocopPos;
		lightPosition[0] = robocopPos[0];
		lightPosition[1] = robocopPos[1];
		lightPosition[2] = robocopPos[2];
	}
	if (renderLightBox) {
		displayLightSource(View, Projection);
	}

	glBindVertexArray(VAO);
	glUseProgram(program);//uniform�ѼƼƭȫe������use shader
	myUpdateModel();

	//update data to UBO for MVP
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), &View);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), &Projection);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	GLuint offset[3] = { 0,0,0 };//offset for vertices , uvs , normals
	glUniform4fv(ambientID, 1, &ambientColor[0]);
	glUniform4fv(diffuseID, 1, &diffuseColor[0]);
	glUniform4fv(specularID, 1, &specularColor[0]);
	glUniform1fv(ShininessID, 1, &Shininess);
	glUniform3fv(viewPosID, 1, &viewPos[0]);
	for (int i = 0; i < PARTSNUM; i++) {
		glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Models[i][0][0]);
		glUniform3fv(lightPosID, 1, &lightPosition[0]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);


		//handle extra part substitution
		int currentIndex = i; //for later mtls accessing
		if (palmMode == CLENCH && (i == 15||i==18)) {
			GLuint tempOffset[3] = { offset[0], offset[1], offset[2] };
			
			if (i == 15) {
				for (int j = 15; j < PARTSNUM; j++) {
					tempOffset[0] += vertices_size[j] * sizeof(vec3);
					tempOffset[1] += uvs_size[j] * sizeof(vec2);
					tempOffset[2] += normals_size[j] * sizeof(vec3);
				}
				currentIndex = PARTSNUM;
			}
			else {
				for (int j = 18; j < PARTSNUM+1; j++) {
					tempOffset[0] += vertices_size[j] * sizeof(vec3);
					tempOffset[1] += uvs_size[j] * sizeof(vec2);
					tempOffset[2] += normals_size[j] * sizeof(vec3);
				}
				currentIndex = PARTSNUM+1;
			}


			glVertexAttribPointer(0,				//location
				3,				//vec3
				GL_FLOAT,			//type
				GL_FALSE,			//not normalized
				0,				//strip
				(void*)tempOffset[0]);//buffer offset
	//(location,vec3,type,�T�w�I,�s���I�������q,buffer point)
			offset[0] += vertices_size[i] * sizeof(vec3);

			// 2nd attribute buffer : UVs
			glEnableVertexAttribArray(1);//location 1 :vec2 UV
			glBindBuffer(GL_ARRAY_BUFFER, uVBO);
			glVertexAttribPointer(1,
				2,
				GL_FLOAT,
				GL_FALSE,
				0,
				(void*)tempOffset[1]);
			//(location,vec2,type,�T�w�I,�s���I�������q,point)
			offset[1] += uvs_size[i] * sizeof(vec2);

			// 3rd attribute buffer : normals
			glEnableVertexAttribArray(2);//location 2 :vec3 Normal
			glBindBuffer(GL_ARRAY_BUFFER, nVBO);
			glVertexAttribPointer(2,
				3,
				GL_FLOAT,
				GL_FALSE,
				0,
				(void*)tempOffset[2]);
			//(location,vec3,type,�T�w�I,�s���I�������q,point)
			offset[2] += normals_size[i] * sizeof(vec3);
		}
		else {

			glVertexAttribPointer(0,				//location
				3,				//vec3
				GL_FLOAT,			//type
				GL_FALSE,			//not normalized
				0,				//strip
				(void*)offset[0]);//buffer offset
	//(location,vec3,type,�T�w�I,�s���I�������q,buffer point)
			offset[0] += vertices_size[i] * sizeof(vec3);

			// 2nd attribute buffer : UVs
			glEnableVertexAttribArray(1);//location 1 :vec2 UV
			glBindBuffer(GL_ARRAY_BUFFER, uVBO);
			glVertexAttribPointer(1,
				2,
				GL_FLOAT,
				GL_FALSE,
				0,
				(void*)offset[1]);
			//(location,vec2,type,�T�w�I,�s���I�������q,point)
			offset[1] += uvs_size[i] * sizeof(vec2);

			// 3rd attribute buffer : normals
			glEnableVertexAttribArray(2);//location 2 :vec3 Normal
			glBindBuffer(GL_ARRAY_BUFFER, nVBO);
			glVertexAttribPointer(2,
				3,
				GL_FLOAT,
				GL_FALSE,
				0,
				(void*)offset[2]);
			//(location,vec3,type,�T�w�I,�s���I�������q,point)
			offset[2] += normals_size[i] * sizeof(vec3);
		}

		int vertexIDoffset = 0;//glVertexID's offset 
		string mtlname;//material name
		vec3 Ks = vec3(1, 1, 1);//because .mtl excluding specular , so give it here.

		for (int j = 0; j < mtls[currentIndex].size(); j++) {//
			mtlname = mtls[currentIndex][j];
			//find the material diffuse color in map:KDs by material name.
			glUniform3fv(M_KaID, 1, &KAs[mtlname][0]);
			glUniform3fv(M_KdID, 1, &KDs[mtlname][0]);
			glUniform3fv(M_KsID, 1, &KSs[mtlname][0]);
			//          (primitive   , glVertexID base , vertex count    )
			glDrawArrays(GL_TRIANGLES, vertexIDoffset, faces[currentIndex][j + 1] * 3);
			//we draw triangles by giving the glVertexID base and vertex count is face count*3
			vertexIDoffset += faces[currentIndex][j + 1] * 3;//glVertexID's base offset is face count*3
		}//end for loop for draw one part of the robot	

	}//end for loop for updating and drawing model

	//offset added by extra parts offset
	for (int j = PARTSNUM; j < PARTSTOTAL; j++) {
		offset[0] += vertices_size[j] * sizeof(vec3);
		offset[1] += uvs_size[j] * sizeof(vec2);
		offset[2] += normals_size[j] * sizeof(vec3);
	}

	if (useBackground) {
		glUniformMatrix4fv(ModelID, 1, GL_FALSE, &ModelBackground[0][0]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,				//location
			3,				//vec3
			GL_FLOAT,			//type
			GL_FALSE,			//not normalized
			0,				//strip
			(void*)offset[0]);//buffer offset
//(location,vec3,type,�T�w�I,�s���I�������q,buffer point)
		offset[0] += vertices_size[PARTSTOTAL] * sizeof(vec3);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);//location 1 :vec2 UV
		glBindBuffer(GL_ARRAY_BUFFER, uVBO);
		glVertexAttribPointer(1,
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)offset[1]);
		//(location,vec2,type,�T�w�I,�s���I�������q,point)
		offset[1] += uvs_size[PARTSTOTAL] * sizeof(vec2);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);//location 2 :vec3 Normal
		glBindBuffer(GL_ARRAY_BUFFER, nVBO);
		glVertexAttribPointer(2,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)offset[2]);
		//(location,vec3,type,�T�w�I,�s���I�������q,point)
		offset[2] += normals_size[PARTSTOTAL] * sizeof(vec3);

		int vertexIDoffset = 0;//glVertexID's offset 
		string mtlname;//material name
		vec3 Ks = vec3(1, 1, 1);//because .mtl excluding specular , so give it here.
		for (int j = 0; j < mtls[PARTSNUM].size(); j++) {//
			mtlname = mtls[PARTSTOTAL][j];
			//find the material diffuse color in map:KDs by material name.
			glUniform3fv(M_KdID, 1, &KDs[mtlname][0]);
			glUniform3fv(M_KsID, 1, &Ks[0]);
			//          (primitive   , glVertexID base , vertex count    )
			glDrawArrays(GL_TRIANGLES, vertexIDoffset, faces[PARTSTOTAL][j + 1] * 3);
			//we draw triangles by giving the glVertexID base and vertex count is face count*3
			vertexIDoffset += faces[PARTSTOTAL][j + 1] * 3;//glVertexID's base offset is face count*3
		}//end for loop for draw one part of the robot
	}

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(window);//�մ��e�x�M��xbuffer ,���Obuffer�e���M�e�xbuffer�洫�ϧڭ̬ݨ���
	glfwPollEvents();
}

void Obj2Buffer() {
	std::vector<vec3> Kds;
	std::vector<vec3> Kas;
	std::vector<vec3> Kss;
	std::vector<std::string> Materials;//mtl-name
	std::string texture;
	for (int i = 0; i < PARTSNUM; i++) {
		//for debug purposes

		loadMTL(("../Assets/Obj/" + partsList[i] + ".mtl").c_str(), Kds, Kas, Kss, Materials, texture);

	}
	for (int i = 0; i < EXTRAPARTS; i++) {
		loadMTL(("../Assets/Obj/" + partsList[i] + ".mtl").c_str(), Kds, Kas, Kss, Materials, texture);
	}
	//printf("%d\n",texture);
	if (useBackground) {
		loadMTL("../Assets/Obj/Sci_Fi_Corridor.mtl", Kds, Kas, Kss, Materials, texture);
	}/*
	cout << "Kds size: " << Kds.size() << endl;
	cout << "Kas size: " << Kas.size() << endl;
	cout << "Kss size: " << Kss.size() << endl;*/
	for (int i = 0; i < Materials.size(); i++) {
		string mtlname = Materials[i];
		//  name            vec3
		KDs[mtlname] = Kds[i];
		KAs[mtlname] = Kas[i];
		KSs[mtlname] = Kss[i];
	}


	for (int i = 0; i < PARTSNUM; i++) {
		if (!renderBodyTop && (i >= 0 && i <= 3 || i >= 13)) {
			continue;
		}

		if (!renderHead && (i >= 0 && i <= 1)) {
			continue;
		}

		if (!renderArm && (i >= 13)) {
			continue;
		}

		if (!renderLeg && (i >= 5 && i <= 12)) {
			continue;
		}
		load2Buffer("../Assets/Obj/"+partsList[i]+".obj", i);
	}
	for (int i = 0; i < EXTRAPARTS; i++) {
		if (!renderBodyTop || !renderArm) {
			continue;
		}
		load2Buffer("../Assets/Obj/" + extraBodyPart[i] + ".obj", i + PARTSNUM);
	}
	if (useBackground) {
		load2Buffer("../Assets/Obj/Sci_Fi_Corridor.obj", PARTSTOTAL);
	}

	GLuint totalSize[3] = { 0,0,0 };
	GLuint offset[3] = { 0,0,0 };
	for (int i = 0; i < PARTSTOTAL; i++) {
		totalSize[0] += vertices_size[i] * sizeof(vec3);
		totalSize[1] += uvs_size[i] * sizeof(vec2);
		totalSize[2] += normals_size[i] * sizeof(vec3);
	}
	//cout << "s1\n";
	if (useBackground) {
		totalSize[0] += vertices_size[PARTSTOTAL] * sizeof(vec3);
		totalSize[1] += uvs_size[PARTSTOTAL] * sizeof(vec2);
		totalSize[2] += normals_size[PARTSTOTAL] * sizeof(vec3);
	}
	//cout << "s2\n";
	//generate vbo
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &uVBO);
	glGenBuffers(1, &nVBO);
	//bind vbo ,�Ĥ@��bind�]�P���� create vbo 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//VBO��target�OGL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, totalSize[0], NULL, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uVBO);//VBO��target�OGL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, totalSize[1], NULL, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, nVBO);//VBO��target�OGL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, totalSize[2], NULL, GL_STATIC_DRAW);
	//cout << "s3\n";
	int rep = PARTSTOTAL; 
	//cout << "s4\n";
	for (int i = 0; i < rep; i++) {
		glBindBuffer(GL_COPY_WRITE_BUFFER, VBO);
		glBindBuffer(GL_COPY_READ_BUFFER, VBOs[i]);
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, offset[0], vertices_size[i] * sizeof(vec3));
		offset[0] += vertices_size[i] * sizeof(vec3);
		glInvalidateBufferData(VBOs[i]);//free vbo
		glBindBuffer(GL_COPY_WRITE_BUFFER, 0);

		glBindBuffer(GL_COPY_WRITE_BUFFER, uVBO);
		glBindBuffer(GL_COPY_READ_BUFFER, uVBOs[i]);
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, offset[1], uvs_size[i] * sizeof(vec2));
		offset[1] += uvs_size[i] * sizeof(vec2);
		glInvalidateBufferData(uVBOs[i]);//free vbo
		glBindBuffer(GL_COPY_WRITE_BUFFER, 0);

		glBindBuffer(GL_COPY_WRITE_BUFFER, nVBO);
		glBindBuffer(GL_COPY_READ_BUFFER, nVBOs[i]);
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, offset[2], normals_size[i] * sizeof(vec3));
		offset[2] += normals_size[i] * sizeof(vec3);
		glInvalidateBufferData(uVBOs[i]);//free vbo
		glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
	}
	if (useBackground) {
		glBindBuffer(GL_COPY_WRITE_BUFFER, VBO);
		glBindBuffer(GL_COPY_READ_BUFFER, VBOs[PARTSTOTAL]);
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, offset[0], vertices_size[PARTSTOTAL] * sizeof(vec3));
		offset[0] += vertices_size[PARTSTOTAL] * sizeof(vec3);
		glInvalidateBufferData(VBOs[PARTSTOTAL]);//free vbo
		glBindBuffer(GL_COPY_WRITE_BUFFER, 0);

		glBindBuffer(GL_COPY_WRITE_BUFFER, uVBO);
		glBindBuffer(GL_COPY_READ_BUFFER, uVBOs[PARTSTOTAL]);
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, offset[1], uvs_size[PARTSTOTAL] * sizeof(vec2));
		offset[1] += uvs_size[PARTSTOTAL] * sizeof(vec2);
		glInvalidateBufferData(uVBOs[PARTSTOTAL]);//free vbo
		glBindBuffer(GL_COPY_WRITE_BUFFER, 0);

		glBindBuffer(GL_COPY_WRITE_BUFFER, nVBO);
		glBindBuffer(GL_COPY_READ_BUFFER, nVBOs[PARTSTOTAL]);
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, offset[2], normals_size[PARTSTOTAL] * sizeof(vec3));
		offset[2] += normals_size[PARTSTOTAL] * sizeof(vec3);
		glInvalidateBufferData(uVBOs[PARTSTOTAL]);//free vbo
		glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
	}

	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
	//cout << "s5\n";
}


void myUpdateModel() {
	//reset state, translate to initial place
	for (int i = 0; i < PARTSNUM; i++) {
		Models[i] = mat4(1.0f);
	}

	//Body
	Models[CROTCH] = translate(Models[CROTCH], vec3(0, robotShiftUp, 0));
	Models[CROTCH] = translate(Models[CROTCH], vec3(translatePart[CROTCH][0], translatePart[CROTCH][1], translatePart[CROTCH][2]));
	Models[CROTCH] = translate(Models[CROTCH], vec3(initialOffset[CROTCH][0], initialOffset[CROTCH][1], initialOffset[CROTCH][2]));
	Models[CROTCH] = rotate(Models[CROTCH], rotatePart[CROTCH][2], vec3(0, 0, 1));
	Models[CROTCH] = rotate(Models[CROTCH], rotatePart[CROTCH][1], vec3(0, 1, 0));
	Models[CROTCH] = rotate(Models[CROTCH], rotatePart[CROTCH][0], vec3(1, 0, 0));
		//Abs
		Models[ABS] = translate(Models[CROTCH], vec3(translatePart[ABS][0], translatePart[ABS][1], translatePart[ABS][2]));
		Models[ABS] = translate(Models[ABS], vec3(initialOffset[ABS][0], initialOffset[ABS][1], initialOffset[ABS][2]));
		Models[ABS] = rotate(Models[ABS], rotatePart[ABS][2], vec3(0, 0, 1));
		Models[ABS] = rotate(Models[ABS], rotatePart[ABS][1], vec3(0, 1, 0));
		Models[ABS] = rotate(Models[ABS], rotatePart[ABS][0], vec3(1, 0, 0));
			//Upper Body
			Models[UPPER_BODY] = translate(Models[ABS], vec3(translatePart[UPPER_BODY][0], translatePart[UPPER_BODY][1], translatePart[UPPER_BODY][2]));
			Models[UPPER_BODY] = translate(Models[UPPER_BODY], vec3(initialOffset[UPPER_BODY][0], initialOffset[UPPER_BODY][1], initialOffset[UPPER_BODY][2]));
			Models[UPPER_BODY] = rotate(Models[UPPER_BODY], rotatePart[UPPER_BODY][2], vec3(0, 0, 1));
			Models[UPPER_BODY] = rotate(Models[UPPER_BODY], rotatePart[UPPER_BODY][1], vec3(0, 1, 0));
			Models[UPPER_BODY] = rotate(Models[UPPER_BODY], rotatePart[UPPER_BODY][0], vec3(1, 0, 0));
				//Neck
				Models[NECK] = translate(Models[UPPER_BODY], vec3(translatePart[NECK][0], translatePart[NECK][1], translatePart[NECK][2]));
				Models[NECK] = translate(Models[NECK], vec3(initialOffset[NECK][0], initialOffset[NECK][1], initialOffset[NECK][2]));
				Models[NECK] = rotate(Models[NECK], rotatePart[NECK][2], vec3(0, 0, 1));
				Models[NECK] = rotate(Models[NECK], rotatePart[NECK][1], vec3(0, 1, 0));
				Models[NECK] = rotate(Models[NECK], rotatePart[NECK][0], vec3(1, 0, 0));
					//Head
					Models[HEAD] = translate(Models[NECK], vec3(translatePart[HEAD][0], translatePart[HEAD][1], translatePart[HEAD][2]));
					Models[HEAD] = translate(Models[HEAD], vec3(initialOffset[HEAD][0], initialOffset[HEAD][1], initialOffset[HEAD][2]));
					Models[HEAD] = rotate(Models[HEAD], rotatePart[HEAD][2], vec3(0, 0, 1));
					Models[HEAD] = rotate(Models[HEAD], rotatePart[HEAD][1], vec3(0, 1, 0));
					Models[HEAD] = rotate(Models[HEAD], rotatePart[HEAD][0], vec3(1, 0, 0));

				//Right_Upper_arm
				Models[RIGHT_UPPER_ARM] = translate(Models[UPPER_BODY], vec3(translatePart[RIGHT_UPPER_ARM][0], translatePart[RIGHT_UPPER_ARM][1], translatePart[RIGHT_UPPER_ARM][2]));
				Models[RIGHT_UPPER_ARM] = translate(Models[RIGHT_UPPER_ARM], vec3(initialOffset[RIGHT_UPPER_ARM][0], initialOffset[RIGHT_UPPER_ARM][1], initialOffset[RIGHT_UPPER_ARM][2]));
				Models[RIGHT_UPPER_ARM] = rotate(Models[RIGHT_UPPER_ARM], rotatePart[RIGHT_UPPER_ARM][2], vec3(0, 0, 1));
				Models[RIGHT_UPPER_ARM] = rotate(Models[RIGHT_UPPER_ARM], rotatePart[RIGHT_UPPER_ARM][1], vec3(0, 1, 0));
				Models[RIGHT_UPPER_ARM] = rotate(Models[RIGHT_UPPER_ARM], rotatePart[RIGHT_UPPER_ARM][0], vec3(1, 0, 0));
					//Right_Lower_arm
					Models[RIGHT_LOWER_ARM] = translate(Models[RIGHT_UPPER_ARM], vec3(translatePart[RIGHT_LOWER_ARM][0], translatePart[RIGHT_LOWER_ARM][1], translatePart[RIGHT_LOWER_ARM][2]));
					Models[RIGHT_LOWER_ARM] = translate(Models[RIGHT_LOWER_ARM], vec3(initialOffset[RIGHT_LOWER_ARM][0], initialOffset[RIGHT_LOWER_ARM][1], initialOffset[RIGHT_LOWER_ARM][2]));
					Models[RIGHT_LOWER_ARM] = rotate(Models[RIGHT_LOWER_ARM], rotatePart[RIGHT_LOWER_ARM][2], vec3(0, 0, 1));
					Models[RIGHT_LOWER_ARM] = rotate(Models[RIGHT_LOWER_ARM], rotatePart[RIGHT_LOWER_ARM][1], vec3(0, 1, 0));
					Models[RIGHT_LOWER_ARM] = rotate(Models[RIGHT_LOWER_ARM], rotatePart[RIGHT_LOWER_ARM][0], vec3(1, 0, 0));
						//Right_Palm
						Models[RIGHT_PALM] = translate(Models[RIGHT_LOWER_ARM], vec3(translatePart[RIGHT_PALM][0], translatePart[RIGHT_PALM][1], translatePart[RIGHT_PALM][2]));
						Models[RIGHT_PALM] = translate(Models[RIGHT_PALM], vec3(initialOffset[RIGHT_PALM][0], initialOffset[RIGHT_PALM][1], initialOffset[RIGHT_PALM][2]));
						Models[RIGHT_PALM] = rotate(Models[RIGHT_PALM], rotatePart[RIGHT_PALM][2], vec3(0, 0, 1));
						Models[RIGHT_PALM] = rotate(Models[RIGHT_PALM], rotatePart[RIGHT_PALM][1], vec3(0, 1, 0));
						Models[RIGHT_PALM] = rotate(Models[RIGHT_PALM], rotatePart[RIGHT_PALM][0], vec3(1, 0, 0));
				//LEFT_Upper_arm
				Models[LEFT_UPPER_ARM] = translate(Models[UPPER_BODY], vec3(translatePart[LEFT_UPPER_ARM][0], translatePart[LEFT_UPPER_ARM][1], translatePart[LEFT_UPPER_ARM][2]));
				Models[LEFT_UPPER_ARM] = translate(Models[LEFT_UPPER_ARM], vec3(initialOffset[LEFT_UPPER_ARM][0], initialOffset[LEFT_UPPER_ARM][1], initialOffset[LEFT_UPPER_ARM][2]));
				Models[LEFT_UPPER_ARM] = rotate(Models[LEFT_UPPER_ARM], rotatePart[LEFT_UPPER_ARM][2], vec3(0, 0, 1));
				Models[LEFT_UPPER_ARM] = rotate(Models[LEFT_UPPER_ARM], rotatePart[LEFT_UPPER_ARM][1], vec3(0, 1, 0));
				Models[LEFT_UPPER_ARM] = rotate(Models[LEFT_UPPER_ARM], rotatePart[LEFT_UPPER_ARM][0], vec3(1, 0, 0));
					//LEFT_Lower_arm
					Models[LEFT_LOWER_ARM] = translate(Models[LEFT_UPPER_ARM], vec3(translatePart[LEFT_LOWER_ARM][0], translatePart[LEFT_LOWER_ARM][1], translatePart[LEFT_LOWER_ARM][2]));
					Models[LEFT_LOWER_ARM] = translate(Models[LEFT_LOWER_ARM], vec3(initialOffset[LEFT_LOWER_ARM][0], initialOffset[LEFT_LOWER_ARM][1], initialOffset[LEFT_LOWER_ARM][2]));
					Models[LEFT_LOWER_ARM] = rotate(Models[LEFT_LOWER_ARM], rotatePart[LEFT_LOWER_ARM][2], vec3(0, 0, 1));
					Models[LEFT_LOWER_ARM] = rotate(Models[LEFT_LOWER_ARM], rotatePart[LEFT_LOWER_ARM][1], vec3(0, 1, 0));
					Models[LEFT_LOWER_ARM] = rotate(Models[LEFT_LOWER_ARM], rotatePart[LEFT_LOWER_ARM][0], vec3(1, 0, 0));
						//LEFT_Palm
						Models[LEFT_PALM] = translate(Models[LEFT_LOWER_ARM], vec3(translatePart[LEFT_PALM][0], translatePart[LEFT_PALM][1], translatePart[LEFT_PALM][2]));
						Models[LEFT_PALM] = translate(Models[LEFT_PALM], vec3(initialOffset[LEFT_PALM][0], initialOffset[LEFT_PALM][1], initialOffset[LEFT_PALM][2]));
						Models[LEFT_PALM] = rotate(Models[LEFT_PALM], rotatePart[LEFT_PALM][2], vec3(0, 0, 1));
						Models[LEFT_PALM] = rotate(Models[LEFT_PALM], rotatePart[LEFT_PALM][1], vec3(0, 1, 0));
						Models[LEFT_PALM] = rotate(Models[LEFT_PALM], rotatePart[LEFT_PALM][0], vec3(1, 0, 0));

		//Left_Upper_thigh
		Models[LEFT_UPPER_THIGH] = translate(Models[CROTCH], vec3(translatePart[LEFT_UPPER_THIGH][0], translatePart[LEFT_UPPER_THIGH][1], translatePart[LEFT_UPPER_THIGH][2]));
		Models[LEFT_UPPER_THIGH] = translate(Models[LEFT_UPPER_THIGH], vec3(initialOffset[LEFT_UPPER_THIGH][0], initialOffset[LEFT_UPPER_THIGH][1], initialOffset[LEFT_UPPER_THIGH][2]));
		Models[LEFT_UPPER_THIGH] = rotate(Models[LEFT_UPPER_THIGH], rotatePart[LEFT_UPPER_THIGH][2], vec3(0, 0, 1));
		Models[LEFT_UPPER_THIGH] = rotate(Models[LEFT_UPPER_THIGH], rotatePart[LEFT_UPPER_THIGH][1], vec3(0, 1, 0));
		Models[LEFT_UPPER_THIGH] = rotate(Models[LEFT_UPPER_THIGH], rotatePart[LEFT_UPPER_THIGH][0], vec3(1, 0, 0));
			//LEFT_lower_thigh
			Models[LEFT_LOWER_THIGH] = translate(Models[LEFT_UPPER_THIGH], vec3(translatePart[LEFT_LOWER_THIGH][0], translatePart[LEFT_LOWER_THIGH][1], translatePart[LEFT_LOWER_THIGH][2]));
			Models[LEFT_LOWER_THIGH] = translate(Models[LEFT_LOWER_THIGH], vec3(initialOffset[LEFT_LOWER_THIGH][0], initialOffset[LEFT_LOWER_THIGH][1], initialOffset[LEFT_LOWER_THIGH][2]));
			Models[LEFT_LOWER_THIGH] = rotate(Models[LEFT_LOWER_THIGH], rotatePart[LEFT_LOWER_THIGH][2], vec3(0, 0, 1));
			Models[LEFT_LOWER_THIGH] = rotate(Models[LEFT_LOWER_THIGH], rotatePart[LEFT_LOWER_THIGH][1], vec3(0, 1, 0));
			Models[LEFT_LOWER_THIGH] = rotate(Models[LEFT_LOWER_THIGH], rotatePart[LEFT_LOWER_THIGH][0], vec3(1, 0, 0));
				//LEFT_foot
				Models[LEFT_FOOT] = translate(Models[LEFT_LOWER_THIGH], vec3(translatePart[LEFT_FOOT][0], translatePart[LEFT_FOOT][1], translatePart[LEFT_FOOT][2]));
				Models[LEFT_FOOT] = translate(Models[LEFT_FOOT], vec3(initialOffset[LEFT_FOOT][0], initialOffset[LEFT_FOOT][1], initialOffset[LEFT_FOOT][2]));
				Models[LEFT_FOOT] = rotate(Models[LEFT_FOOT], rotatePart[LEFT_FOOT][2], vec3(0, 0, 1));
				Models[LEFT_FOOT] = rotate(Models[LEFT_FOOT], rotatePart[LEFT_FOOT][1], vec3(0, 1, 0));
				Models[LEFT_FOOT] = rotate(Models[LEFT_FOOT], rotatePart[LEFT_FOOT][0], vec3(1, 0, 0));
					//LEFT_foot_toes
					Models[LEFT_FOOT_TOES] = translate(Models[LEFT_FOOT], vec3(translatePart[LEFT_FOOT_TOES][0], translatePart[LEFT_FOOT_TOES][1], translatePart[LEFT_FOOT_TOES][2]));
					Models[LEFT_FOOT_TOES] = translate(Models[LEFT_FOOT_TOES], vec3(initialOffset[LEFT_FOOT_TOES][0], initialOffset[LEFT_FOOT_TOES][1], initialOffset[LEFT_FOOT_TOES][2]));
					Models[LEFT_FOOT_TOES] = rotate(Models[LEFT_FOOT_TOES], rotatePart[LEFT_FOOT_TOES][2], vec3(0, 0, 1));
					Models[LEFT_FOOT_TOES] = rotate(Models[LEFT_FOOT_TOES], rotatePart[LEFT_FOOT_TOES][1], vec3(0, 1, 0));
					Models[LEFT_FOOT_TOES] = rotate(Models[LEFT_FOOT_TOES], rotatePart[LEFT_FOOT_TOES][0], vec3(1, 0, 0));
		//right_upper_thigh
		Models[RIGHT_UPPER_THIGH] = translate(Models[CROTCH], vec3(translatePart[RIGHT_UPPER_THIGH][0], translatePart[RIGHT_UPPER_THIGH][1], translatePart[RIGHT_UPPER_THIGH][2]));
		Models[RIGHT_UPPER_THIGH] = translate(Models[RIGHT_UPPER_THIGH], vec3(initialOffset[RIGHT_UPPER_THIGH][0], initialOffset[RIGHT_UPPER_THIGH][1], initialOffset[RIGHT_UPPER_THIGH][2]));
		Models[RIGHT_UPPER_THIGH] = rotate(Models[RIGHT_UPPER_THIGH], rotatePart[RIGHT_UPPER_THIGH][2], vec3(0, 0, 1));
		Models[RIGHT_UPPER_THIGH] = rotate(Models[RIGHT_UPPER_THIGH], rotatePart[RIGHT_UPPER_THIGH][1], vec3(0, 1, 0));
		Models[RIGHT_UPPER_THIGH] = rotate(Models[RIGHT_UPPER_THIGH], rotatePart[RIGHT_UPPER_THIGH][0], vec3(1, 0, 0));
			//Right_lower_thigh
			Models[RIGHT_LOWER_THIGH] = translate(Models[RIGHT_UPPER_THIGH], vec3(translatePart[RIGHT_LOWER_THIGH][0], translatePart[RIGHT_LOWER_THIGH][1], translatePart[RIGHT_LOWER_THIGH][2]));
			Models[RIGHT_LOWER_THIGH] = translate(Models[RIGHT_LOWER_THIGH], vec3(initialOffset[RIGHT_LOWER_THIGH][0], initialOffset[RIGHT_LOWER_THIGH][1], initialOffset[RIGHT_LOWER_THIGH][2]));
			Models[RIGHT_LOWER_THIGH] = rotate(Models[RIGHT_LOWER_THIGH], rotatePart[RIGHT_LOWER_THIGH][2], vec3(0, 0, 1));
			Models[RIGHT_LOWER_THIGH] = rotate(Models[RIGHT_LOWER_THIGH], rotatePart[RIGHT_LOWER_THIGH][1], vec3(0, 1, 0));
			Models[RIGHT_LOWER_THIGH] = rotate(Models[RIGHT_LOWER_THIGH], rotatePart[RIGHT_LOWER_THIGH][0], vec3(1, 0, 0));
				//Right_foot
				Models[RIGHT_FOOT] = translate(Models[RIGHT_LOWER_THIGH], vec3(translatePart[RIGHT_FOOT][0], translatePart[RIGHT_FOOT][1], translatePart[RIGHT_FOOT][2]));
				Models[RIGHT_FOOT] = translate(Models[RIGHT_FOOT], vec3(initialOffset[RIGHT_FOOT][0], initialOffset[RIGHT_FOOT][1], initialOffset[RIGHT_FOOT][2]));
				Models[RIGHT_FOOT] = rotate(Models[RIGHT_FOOT], rotatePart[RIGHT_FOOT][2], vec3(0, 0, 1));
				Models[RIGHT_FOOT] = rotate(Models[RIGHT_FOOT], rotatePart[RIGHT_FOOT][1], vec3(0, 1, 0));
				Models[RIGHT_FOOT] = rotate(Models[RIGHT_FOOT], rotatePart[RIGHT_FOOT][0], vec3(1, 0, 0));
					//Right_foot_toes
					Models[RIGHT_FOOT_TOES] = translate(Models[RIGHT_FOOT], vec3(translatePart[RIGHT_FOOT_TOES][0], translatePart[RIGHT_FOOT_TOES][1], translatePart[RIGHT_FOOT_TOES][2]));
					Models[RIGHT_FOOT_TOES] = translate(Models[RIGHT_FOOT_TOES], vec3(initialOffset[RIGHT_FOOT_TOES][0], initialOffset[RIGHT_FOOT_TOES][1], initialOffset[RIGHT_FOOT_TOES][2]));
					Models[RIGHT_FOOT_TOES] = rotate(Models[RIGHT_FOOT_TOES], rotatePart[RIGHT_FOOT_TOES][2], vec3(0, 0, 1));
					Models[RIGHT_FOOT_TOES] = rotate(Models[RIGHT_FOOT_TOES], rotatePart[RIGHT_FOOT_TOES][1], vec3(0, 1, 0));
					Models[RIGHT_FOOT_TOES] = rotate(Models[RIGHT_FOOT_TOES], rotatePart[RIGHT_FOOT_TOES][0], vec3(1, 0, 0));

	//placing to initial codes
	for (int i = 0; i < PARTSNUM; i++) {
		Models[i] = rotate(Models[i], rotateCentral, vec3(0, 1, 0));
	}
}

void load2Buffer(string obj, int i) {
	std::vector<vec3> vertices;
	std::vector<vec2> uvs;
	std::vector<vec3> normals; // Won't be used at the moment.
	std::vector<unsigned int> materialIndices;

	const char* c = obj.c_str();
	bool res = loadOBJ(c, vertices, uvs, normals, faces[i], mtls[i]);
	cout << "load finished!\n";
	if (!res) printf("load failed\n");

	//glUseProgram(program);

	glGenBuffers(1, &VBOs[i]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), &vertices[0], GL_STATIC_DRAW);
	vertices_size[i] = vertices.size();

	//(buffer type,data�_�l��m,data size,data first ptr)
	//vertices_size[i] = glm_model->numtriangles;

	//printf("vertices:%d\n",vertices_size[);

	glGenBuffers(1, &uVBOs[i]);
	glBindBuffer(GL_ARRAY_BUFFER, uVBOs[i]);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vec2), &uvs[0], GL_STATIC_DRAW);
	uvs_size[i] = uvs.size();

	glGenBuffers(1, &nVBOs[i]);
	glBindBuffer(GL_ARRAY_BUFFER, nVBOs[i]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals[0], GL_STATIC_DRAW);
	normals_size[i] = normals.size();
}
//Keyboard for GLFW
void KeyboardGLFW(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//related to animation
	bool notAnimationKey = (key != GLFW_KEY_W) && (key != GLFW_KEY_A) && (key != GLFW_KEY_S) && (key != GLFW_KEY_D) && (key != GLFW_KEY_SPACE);
	if (notAnimationKey) {
		animateMode = IDLE;
	}
	else if (key == GLFW_KEY_W) {
		animateMode = WALK;
		viewChange = true;
		rotatePart[CROTCH][0] = 0;
		rotatePart[CROTCH][1] = 180;
		rotatePart[CROTCH][2] = 0;

		//translatePart[CROTCH][0] += 0;
		//translatePart[CROTCH][1] += 0;
		translatePart[CROTCH][2] -= WALKSPEED;
	}
	else if (key == GLFW_KEY_A) {
		animateMode = WALK;
		viewChange = true;
		rotatePart[CROTCH][0] = 0;
		rotatePart[CROTCH][1] = -90;
		rotatePart[CROTCH][2] = 0;

		translatePart[CROTCH][0] -= WALKSPEED;
		//translatePart[CROTCH][1] += 0;
		//translatePart[CROTCH][2] += 0;
	}
	else if (key == GLFW_KEY_S) {
		animateMode = WALK;
		viewChange = true;
		rotatePart[CROTCH][0] = 0;
		rotatePart[CROTCH][1] = 0;
		rotatePart[CROTCH][2] = 0;

		//translatePart[CROTCH][0] += 0;
		//translatePart[CROTCH][1] += 0;
		translatePart[CROTCH][2] += WALKSPEED;
	}
	else if (key == GLFW_KEY_D) {
		animateMode = WALK;
		viewChange = true;
		rotatePart[CROTCH][0] = 0;
		rotatePart[CROTCH][1] = 90;
		rotatePart[CROTCH][2] = 0;

		translatePart[CROTCH][0] += WALKSPEED;
		//translatePart[CROTCH][1] += 0;
		//translatePart[CROTCH][2] += 0;
	}
	else if (key == GLFW_KEY_SPACE) {
		if (animateMode == JUMPINGJACK) {
			animateMode = IDLE;
			resetModel();
		}
		else {
			animateMode = JUMPINGJACK;
		}
	}
	//related to viewing
	switch (key) {
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		break;
	case GLFW_KEY_G:
		animateMode = IDLE;
		rotatePart[LEFT_UPPER_ARM][0] += 4;
		break;
	case GLFW_KEY_H:
		animateMode = IDLE;
		rotatePart[LEFT_UPPER_ARM][0] -= 4;
		break;
	case GLFW_KEY_J:
		animateMode = IDLE;
		rotatePart[LEFT_LOWER_ARM][0] += 4;
		break;
	case GLFW_KEY_K:
		animateMode = IDLE;
		rotatePart[LEFT_LOWER_ARM][0] -= 4;
		break;
	case GLFW_KEY_UP:
		viewChange = true;
		eyedistance -= 0.2;
		clip(eyedistance, 5, 40);
		break;
	case GLFW_KEY_DOWN:
		viewChange = true;
		eyedistance += 0.2;
		clip(eyedistance, 5, 40);
		break;
	case GLFW_KEY_LEFT:
		viewChange = true;
		eyeAngley -= 10;
		break;
	case GLFW_KEY_RIGHT:
		eyeAngley += 10;
		viewChange = true;
		break;
	case GLFW_KEY_0:
		palmMode = CLENCH;
		break;
	case GLFW_KEY_1:
		palmMode = OPEN;
		break;
	default:
		animateMode = IDLE;
	}
}

float clip(float &var, float min, float max) {
	if (var < min) {
		var = min;
	}
	else if (var > max) {
		var = max;
	}
	return var;
}

float getTime()
{
	return glfwGetTime();
}

float clampValMin(float x, float clampToMin)
{
	if (x <= clampToMin)
	{
		return clampToMin;
	}
	else
	{
		return x;
	}
}

float clampValMaxMin(float x, float clampToMax, float clampToMin)
{
	if (x >= clampToMax)
	{
		return clampToMax;
	}
	else if (x <= clampToMin)
	{
		return clampToMin;
	}
	else
	{
		return x;
	}
}

float clampValMax(float x, float clampToMax)
{
	if (x >= clampToMax)
	{
		return clampToMax;
	}
	else
	{
		return x;
	}
}

float sf = 1.0f;
float walk()
{
	float currentTime = glfwGetTime();
	cout << rotatePart[RIGHT_UPPER_ARM][0] << endl;

	// RIGHTLEG
	if (r_isUp)
	{
		// leg go up
		if (rotatePart[RIGHT_UPPER_THIGH][0] > -30.0f)
		{
			rotatePart[RIGHT_UPPER_THIGH][0] -= 0.7f;
		}

		if (!r_isUp2 && !r_is_return)
		{
			rotatePart[RIGHT_LOWER_THIGH][0] += 1.6f;
			if (rotatePart[RIGHT_LOWER_THIGH][0] >= 35.0f)
			{
				r_isUp2 = true;
			}
		}
		else if (!r_isUp2 && r_is_return)
		{
			if (rotatePart[RIGHT_UPPER_THIGH][0] <= 999.0f)
			{
				rotatePart[RIGHT_LOWER_THIGH][0] += 1.0f;
				if (rotatePart[RIGHT_LOWER_THIGH][0] >= 60.0f)
				{
					r_isUp2 = true;
				}
			}
		}
		else
		{
			rotatePart[RIGHT_LOWER_THIGH][0] -= 1.0f;
			if (rotatePart[RIGHT_LOWER_THIGH][0] <= 0.0f)
			{
				rotatePart[RIGHT_LOWER_THIGH][0] = 0.0f;
				// do nothing
			}
		}

		if (!r_isUp4 && r_is_return)
		{
			rotatePart[RIGHT_FOOT_TOES][0] += 0.5f;
			if (rotatePart[RIGHT_FOOT_TOES][0] >= 0.0f)
			{
				rotatePart[RIGHT_FOOT_TOES][0] = 0.0f;
				r_isUp4 = false;
			}
		}


		if (rotatePart[RIGHT_UPPER_THIGH][0] <= -30.0f && rotatePart[RIGHT_LOWER_THIGH][0] == 0.0f)
		{
			r_isUp = false;
			r_isUp2 = false;
			r_isUp3 = false;
			r_isUp4 = true;
			// system("pause");
		}
	}
	else
	{
		// leg go down
		rotatePart[RIGHT_UPPER_THIGH][0] += 0.7f;
		if (!r_isUp2 && rotatePart[RIGHT_UPPER_THIGH][0] >= 0.0f)
		{
			rotatePart[RIGHT_LOWER_THIGH][0] += 0.6f;

			if (rotatePart[RIGHT_LOWER_THIGH][0] >= 30.0f)
			{
				rotatePart[RIGHT_LOWER_THIGH][0] = 30.0f;
				r_isUp2 = true;
			}
		}
		else if (!r_isUp2 && rotatePart[RIGHT_UPPER_THIGH][0] >= -10.0f)
		{
			rotatePart[RIGHT_LOWER_THIGH][0] += 0.3f;
			// cout << rotatePart[RIGHT_LOWER_THIGH][0] << endl;
			// system("pause");
		} 

		if (!r_isUp3)
		{
			rotatePart[RIGHT_FOOT][0] += 0.8f;

			if (rotatePart[RIGHT_FOOT][0] >= 10.0f)
			{
				rotatePart[RIGHT_FOOT][0] = 10.0f;
				r_isUp3 = true;
			}
		}
		else
		{
			rotatePart[RIGHT_FOOT][0] -= 0.8f;
			if (rotatePart[RIGHT_FOOT][0] <= 0.0f)
			{
				rotatePart[RIGHT_FOOT][0] = 0.0f;
				r_isUp3 = true;
			}
		}

		if (r_isUp4 && rotatePart[RIGHT_FOOT][0] <= 0.0f)
		{
			rotatePart[RIGHT_FOOT_TOES][0] -= 0.5f;
			if (rotatePart[RIGHT_FOOT_TOES][0] <= -20.0f)
			{
				rotatePart[RIGHT_FOOT_TOES][0] = -20.0f;
				r_isUp4 = false;
			}
		}
		


		if (rotatePart[RIGHT_UPPER_THIGH][0] >= 30.0f)
		{
			// system("pause");
			r_isUp = true;
			r_isUp2 = false;
			r_isUp3 = false;
			r_isUp4 = false;
			r_is_return = true;
		}
	}

	// LEFT LEG
	if (l_isUp)
	{
		// leg go up
		if (rotatePart[LEFT_UPPER_THIGH][0] > -30.0f)
		{
			rotatePart[LEFT_UPPER_THIGH][0] -= 0.7f;
		}

		if (!l_isUp2 && !l_is_return)
		{
			rotatePart[LEFT_LOWER_THIGH][0] += 1.6f;
			if (rotatePart[LEFT_LOWER_THIGH][0] >= 35.0f)
			{
				l_isUp2 = true;
			}
		}
		else if (!l_isUp2 && l_is_return)
		{
			if (rotatePart[LEFT_UPPER_THIGH][0] <= 999.0f)
			{
				rotatePart[LEFT_LOWER_THIGH][0] += 1.0f;
				if (rotatePart[LEFT_LOWER_THIGH][0] >= 60.0f)
				{
					l_isUp2 = true;
				}
			}
		}
		else
		{
			rotatePart[LEFT_LOWER_THIGH][0] -= 1.0f;
			if (rotatePart[LEFT_LOWER_THIGH][0] <= 0.0f)
			{
				rotatePart[LEFT_LOWER_THIGH][0] = 0.0f;
				// do nothing
			}
		}

		if (!l_isUp4 && l_is_return)
		{
			rotatePart[LEFT_FOOT_TOES][0] += 0.5f;
			if (rotatePart[LEFT_FOOT_TOES][0] >= 0.0f)
			{
				rotatePart[LEFT_FOOT_TOES][0] = 0.0f;
				l_isUp4 = false;
			}
		}


		if (rotatePart[LEFT_UPPER_THIGH][0] <= -30.0f && rotatePart[LEFT_LOWER_THIGH][0] == 0.0f)
		{
			l_isUp = false;
			l_isUp2 = false;
			l_isUp3 = false;
			l_isUp4 = true;
			// system("pause");
		}
	}
	else
	{
		// leg go down
		rotatePart[LEFT_UPPER_THIGH][0] = -rotatePart[RIGHT_UPPER_THIGH][0];
		if (!l_isUp2 && rotatePart[LEFT_UPPER_THIGH][0] >= 0.0f)
		{
			rotatePart[LEFT_LOWER_THIGH][0] += 0.6f;

			if (rotatePart[LEFT_LOWER_THIGH][0] >= 30.0f)
			{
				rotatePart[LEFT_LOWER_THIGH][0] = 30.0f;
				l_isUp2 = true;
			}
		}
		else if (!l_isUp2 && rotatePart[LEFT_UPPER_THIGH][0] >= -10.0f)
		{
			rotatePart[LEFT_LOWER_THIGH][0] += 0.3f;
			// cout << rotatePart[RIGHT_LOWER_THIGH][0] << endl;
			// system("pause");
		}

		if (!l_isUp3)
		{
			rotatePart[LEFT_FOOT][0] += 0.8f;

			if (rotatePart[LEFT_FOOT][0] >= 10.0f)
			{
				rotatePart[LEFT_FOOT][0] = 10.0f;
				l_isUp3 = true;
			}
		}
		else
		{
			rotatePart[LEFT_FOOT][0] -= 0.8f;
			if (rotatePart[LEFT_FOOT][0] <= 0.0f)
			{
				rotatePart[LEFT_FOOT][0] = 0.0f;
				l_isUp3 = true;
			}
		}

		if (l_isUp4 && rotatePart[LEFT_FOOT][0] <= 0.0f)
		{
			rotatePart[LEFT_FOOT_TOES][0] -= 0.5f;
			if (rotatePart[LEFT_FOOT_TOES][0] <= -20.0f)
			{
				rotatePart[LEFT_FOOT_TOES][0] = -20.0f;
				l_isUp4 = false;
			}
		}



		if (rotatePart[LEFT_UPPER_THIGH][0] >= 30.0f)
		{
			// system("pause");
			l_isUp = true;
			l_isUp2 = false;
			l_isUp3 = false;
			l_isUp4 = false;
			l_is_return = true;
		}
	}

	// RIGHT HAND
	rotatePart[RIGHT_UPPER_ARM][0] = -rotatePart[RIGHT_UPPER_THIGH][0];
	if (rotatePart[RIGHT_UPPER_ARM][0] > 0) // when move back
	{
		rotatePart[RIGHT_LOWER_ARM][0] = clampValMax(rotatePart[RIGHT_LOWER_ARM][0] + 0.8f, 0.0f);
	}
	else
	{
		rotatePart[RIGHT_LOWER_ARM][0] -= 0.5f;
	}

	rotatePart[LEFT_UPPER_ARM][0] = rotatePart[RIGHT_UPPER_THIGH][0];
	if (rotatePart[LEFT_UPPER_ARM][0] > 0) // when move back
	{
		rotatePart[LEFT_LOWER_ARM][0] = clampValMax(rotatePart[LEFT_LOWER_ARM][0] + 0.8f, 0.0f);
	}
	else
	{
		rotatePart[LEFT_LOWER_ARM][0] -= 0.5f;
	}

	translatePart[CROTCH][1] = clampValMin(sin((currentTime - startTime)  * 2 * 3.14 / 1.5) * 0.1f, 0.0f);
	rotatePart[ABS][1] = -sin((currentTime - startTime) * 2 * 3.14 / 3.0f) * 1.5f;


	cout << "updated\n";
	return rotatePart[RIGHT_UPPER_THIGH][0];
	
}

void jumpingJack() // NAIK TURUN BELUM*****************************************
{
	if (!isOpen)
	{
		if (pause)
		{
			Sleep(400);
		}
		pause = false;

		
		if (!squat1)
		{
			rotatePart[LEFT_UPPER_ARM][2] += 10.0f;
			rotatePart[LEFT_LOWER_ARM][1] += 10.0f;

			rotatePart[LEFT_UPPER_THIGH][0] -= 1.0f;
			rotatePart[LEFT_LOWER_THIGH][0] += 3.0f;
			rotatePart[LEFT_FOOT][0] -= 3.0f;

			if (rotatePart[LEFT_LOWER_ARM][1] >= 80.0f)
			{
				rotatePart[LEFT_LOWER_ARM][1] = 80.0f;
			}

			if (rotatePart[LEFT_FOOT][0] <= -20.0f)
			{
				rotatePart[LEFT_FOOT][0] = -20.0f;
				
				if (rotatePart[LEFT_LOWER_THIGH][0] >= 25.0f)
				{
					rotatePart[LEFT_LOWER_THIGH][0] = 25.0f;
					if (rotatePart[LEFT_UPPER_THIGH][0] <= -10.0f)
					{
						rotatePart[LEFT_UPPER_THIGH][0] = -10.0f;
						squat1 = true;
					}
				}
			}

			rotatePart[RIGHT_UPPER_ARM][2] = -rotatePart[LEFT_UPPER_ARM][2];
			rotatePart[RIGHT_LOWER_ARM][1] = -rotatePart[LEFT_LOWER_ARM][1];
			rotatePart[RIGHT_UPPER_THIGH][2] = -rotatePart[LEFT_UPPER_THIGH][2];
			rotatePart[RIGHT_UPPER_THIGH][0] = rotatePart[LEFT_UPPER_THIGH][0];
			rotatePart[RIGHT_LOWER_THIGH][0] = rotatePart[LEFT_LOWER_THIGH][0];
			rotatePart[RIGHT_FOOT][0] = rotatePart[LEFT_FOOT][0];
		}
		else if (!squat2)
		{
			rotatePart[LEFT_UPPER_ARM][2] += 10.0f;
			rotatePart[LEFT_LOWER_ARM][0] -= 10.0f;
			rotatePart[LEFT_UPPER_THIGH][0] += 1.0f;
			rotatePart[LEFT_LOWER_THIGH][0] -= 3.0f;
			rotatePart[LEFT_FOOT][0] += 10.0f;
			rotatePart[LEFT_FOOT][2] -= 3.0f;
			rotatePart[LEFT_FOOT_TOES][0] -= 5.f;

			if (rotatePart[LEFT_UPPER_ARM][2] >= 140.0f)
			{
				rotatePart[LEFT_UPPER_ARM][2] = 140.0f;
			}

			if (rotatePart[LEFT_LOWER_ARM][0] <= -80.0f)
			{
				rotatePart[LEFT_LOWER_ARM][0] = -80.0f;
			}

			if (rotatePart[LEFT_FOOT][0] >= 50.0f)
			{
				rotatePart[LEFT_FOOT][0] = 50.0f;
			}

			if (rotatePart[LEFT_FOOT_TOES][0] <= -35.0f)
			{
				rotatePart[LEFT_FOOT_TOES][0] = -35.0f;
			}

			if (rotatePart[LEFT_FOOT][2] <= -10.0f)
			{
				rotatePart[LEFT_FOOT][2] = -10.0f;
			}

			if (rotatePart[LEFT_UPPER_ARM][2] >= 120.0f)
			{
				rotatePart[LEFT_UPPER_ARM][2] = 120.0f;
			}

			rotatePart[LEFT_UPPER_THIGH][2] += 1.0f;

			if (rotatePart[LEFT_UPPER_THIGH][2] >= 10.0f)
			{
				rotatePart[LEFT_UPPER_THIGH][2] = 10.0f;
				if (rotatePart[LEFT_UPPER_THIGH][0] >= 0.0f)
				{
					rotatePart[LEFT_UPPER_THIGH][0] = 0.0f;
					if (rotatePart[LEFT_LOWER_THIGH][0] <= 0.0f)
					{
						rotatePart[LEFT_LOWER_THIGH][0] = 0.0f;
						isOpen = true;
						squat1 = false;
						squat2 = false;
						pause = true;
					}
				}
			}

			rotatePart[RIGHT_UPPER_ARM][2] = -rotatePart[LEFT_UPPER_ARM][2];
			rotatePart[RIGHT_LOWER_ARM][0] = rotatePart[LEFT_LOWER_ARM][0];
			rotatePart[RIGHT_UPPER_THIGH][0] = rotatePart[LEFT_UPPER_THIGH][0];
			rotatePart[RIGHT_LOWER_THIGH][0] = rotatePart[LEFT_LOWER_THIGH][0];
			rotatePart[RIGHT_FOOT][0] = rotatePart[LEFT_FOOT][0];
			rotatePart[RIGHT_FOOT][2] = -rotatePart[LEFT_FOOT][2];
			rotatePart[RIGHT_FOOT_TOES][0] = rotatePart[LEFT_FOOT_TOES][0];
			rotatePart[RIGHT_UPPER_THIGH][2] = -rotatePart[LEFT_UPPER_THIGH][2];
		}
	}
	else
	{
		if (pause)
		{
			Sleep(300);
		}
		pause = false;
		

		if (!squat1)
		{
			rotatePart[LEFT_UPPER_ARM][2] -= 10.0f;
			rotatePart[LEFT_LOWER_ARM][1] -= 5.0f;
			rotatePart[LEFT_LOWER_ARM][0] += 10.0f;

			rotatePart[LEFT_UPPER_THIGH][0] -= 1.0f;
			rotatePart[LEFT_LOWER_THIGH][0] += 3.0f;

			if (rotatePart[LEFT_LOWER_ARM][0] >= 0.0f)
			{
				rotatePart[LEFT_LOWER_ARM][0] = 0.0f;
			}

			if (rotatePart[LEFT_LOWER_THIGH][0] >= 15.0f)
			{
				rotatePart[LEFT_LOWER_THIGH][0] = 15.0f;
				if (rotatePart[LEFT_UPPER_THIGH][0] <= -10.0f)
				{
					rotatePart[LEFT_UPPER_THIGH][0] = -10.0f;
					squat1 = true;
				}
			}

			rotatePart[RIGHT_UPPER_ARM][2] = -rotatePart[LEFT_UPPER_ARM][2];
			rotatePart[RIGHT_LOWER_ARM][0] = rotatePart[LEFT_LOWER_ARM][0];
			rotatePart[RIGHT_UPPER_THIGH][2] = -rotatePart[LEFT_UPPER_THIGH][2];
			rotatePart[RIGHT_UPPER_THIGH][0] = rotatePart[LEFT_UPPER_THIGH][0];
			rotatePart[RIGHT_LOWER_THIGH][0] = rotatePart[LEFT_LOWER_THIGH][0];
			rotatePart[RIGHT_FOOT][0] = rotatePart[LEFT_FOOT][0];
		}
		else if (!squat2)
		{
			rotatePart[LEFT_UPPER_ARM][2] -= 10.0f;
			rotatePart[LEFT_LOWER_ARM][1] -= 10.0f;
			rotatePart[LEFT_UPPER_THIGH][0] += 1.0f;
			rotatePart[LEFT_LOWER_THIGH][0] -= 3.0f;
			rotatePart[LEFT_FOOT][0] += 10.0f;
			rotatePart[LEFT_FOOT][2] += 3.0f;
			rotatePart[LEFT_FOOT_TOES][0] += 5.0f;

			if (rotatePart[LEFT_UPPER_ARM][2] <= 0.0f)
			{
				rotatePart[LEFT_UPPER_ARM][2] = 0.0f;
			}

			if (rotatePart[LEFT_LOWER_ARM][1] <= 0.0f)
			{
				rotatePart[LEFT_LOWER_ARM][1] = 0.0f;
			}

			if (rotatePart[LEFT_FOOT][0] >= 0.0f)
			{
				rotatePart[LEFT_FOOT][0] = 0.0f;
			}

			if (rotatePart[LEFT_FOOT_TOES][0] >= 0.0f)
			{
				rotatePart[LEFT_FOOT_TOES][0] = 0.0f;
			}

			if (rotatePart[LEFT_FOOT][2] >= 0.0f)
			{
				rotatePart[LEFT_FOOT][2] = 0.0f;
			}

			rotatePart[LEFT_UPPER_THIGH][2] -= 1.0f;

			if (rotatePart[LEFT_UPPER_THIGH][2] <= 0.0f)
			{
				rotatePart[LEFT_UPPER_THIGH][2] = 0.0f;
				if (rotatePart[LEFT_UPPER_THIGH][0] >= 0.0f)
				{
					rotatePart[LEFT_UPPER_THIGH][0] = 0.0f;
					if (rotatePart[LEFT_LOWER_THIGH][0] <= 0.0f)
					{
						rotatePart[LEFT_LOWER_THIGH][0] = 0.0f;
						isOpen = false;
						squat1 = false;
						squat2 = false;
						pause = true;
					}
				}
			}

			rotatePart[RIGHT_UPPER_ARM][2] = -rotatePart[LEFT_UPPER_ARM][2];
			rotatePart[RIGHT_LOWER_ARM][1] = -rotatePart[LEFT_LOWER_ARM][1];
			rotatePart[RIGHT_UPPER_THIGH][0] = rotatePart[LEFT_UPPER_THIGH][0];
			rotatePart[RIGHT_LOWER_THIGH][0] = rotatePart[LEFT_LOWER_THIGH][0];
			rotatePart[RIGHT_FOOT][0] = rotatePart[LEFT_FOOT][0];
			rotatePart[RIGHT_FOOT][2] = -rotatePart[LEFT_FOOT][2];
			rotatePart[RIGHT_FOOT_TOES][0] = rotatePart[LEFT_FOOT_TOES][0];
			rotatePart[RIGHT_UPPER_THIGH][2] = -rotatePart[LEFT_UPPER_THIGH][2];
		}
	}
}
