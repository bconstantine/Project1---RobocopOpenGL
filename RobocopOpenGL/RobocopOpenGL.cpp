#include "main.h"

vec3 camera = vec3(0, 0, 20);
string partsList[51] = { "head",
"neck",
"upper_body",
"abs",
"crotch",
"left_upper_thigh",
"left_lower_thigh",
"left_foot",
"left_foot_toes",
"right_upper_thigh",
"holder3",
"holder2",
"holder1",
"right_lower_thigh",
"right_foot",
"right_foot_toes",
"right_upper_arm",
"right_lower_arm",
"right_palm",
"right_thumb_1",
"right_thumb_2",
"right_pointer_1",
"right_pointer_2",
"right_pointer_3",
"right_middle_1",
"right_middle_2",
"right_middle_3",
"right_index_1",
"right_index_2",
"right_index_3",
"right_pinky_1",
"right_pinky_2",
"right_pinky_3",
"left_upper_arm",
"left_lower_arm",
"left_palm",
"left_thumb_1",
"left_thumb_2",
"left_pointer_1",
"left_pointer_2",
"left_pointer_3",
"left_middle_1",
"left_middle_2",
"left_middle_3",
"left_index_1",
"left_index_2",
"left_index_3",
"left_pinky_1",
"left_pinky_2",
"left_pinky_3"
};

float initialOffset[][3] = {
	{0, 6.094, 0.173}, //head
	{0, 4.581, 0.673}, //neck
	{0,2.3932, 0}, //upper_body
	{0,1.245, 0.085}, //abs
	{0,0,0}, //crotch
	{-1.288, -0.735, 0.172}, //left_upper_thigh 
	{-1.358, -5.544, -0.313}, //left_lower_thigh
	{-1.432, -9.653, 0.182}, //left_foot
	{-1.543, -10.489, -1.762}, //left_foot_toes
	{1.041, -0.919, 0.406}, //right_upper_thigh
	{1.156, 0.847, 0.346}, //holder3 //10 index
	{1.137,-0.787,0.418}, //holder2
	{1.245,-0.812,0.481}, //holder1
	{1.05,-5.564,-0.064}, //right_lower_thigh
	{0.994,-9.684,0.211}, //right_foot
	{1.212,-10.498,-1.3}, //right_foot_toes
	{2.443,4.31,0.292 }, //right_upper_arm
	{2.999,1.417,0.308}, //right_lower_arm
	{3.758,-1.598,0.203}, //right_palm
	{3.506,-2.028,-0.345}, //right_thumb_1 //19 index
	{3.437,-1.965,-0.81}, //right_thumb_2
	{3.786,-2.61,-0.267}, //right_pointer_1
	{3.888,-3.12,-0.254}, //right_pointer_2
	{3.913,-3.469,-0.223}, //right_pointer_3
	{3.856,-2.605,0.089}, //right_middle_1
	{3.911,-3.203,0.099}, //right_middle_2
	{3.936,-3.598,0.081}, //right_middle_3
	{3.884,-2.577,0.401}, //right_index_1
	{3.938,-3.137,0.39}, //right_index_2
	{3.955,-3.443,0.375}, //right_index_3
	{3.871,2.585,0.661}, //right_pinky_1
	{3.91,-2.97,0.663}, //right_pinky_2
	{3.919,-3.234,0.688}, //right_pinky_3 32
	{-2.457,4.095,0.404}, //left_upper_arm
	{-3.003,1.381,0.42}, //left_lower_arm
	{-3.772,-1.634,0.315}, //left_parm
	{-3.52,-2.063,-0.234}, //left_thumb_1 //36
	{-3.451,-2.001,-0.699}, //left_thumb_2
	{-3.8,-2.646,-0.155}, //left_pointer_1
	{-3.902,-3.156,-0.142}, //left_pointer_2
	{-3.928,-3.504,-0.112}, //left_pointer_3
	{-3.87,-2.641,0.201}, //left_middle_1
	{-3.925,-3.239,0.211}, //left_middle_2
	{-3.95,-3.634,0.192}, //left_middle_3
	{-3.898,-2.613,0.512}, //left_index_1
	{-3.952,-3.172,0.501}, //left_index_2
	{-3.969,-3.479,0.486}, //left_index_3
	{-3.886,-2.621,0.773}, //left_pinky_1
	{-3.924,-3.006,0.775}, //left_pinky_3
	{-3.933,-3.27,0.8} //left_pinky_3
};

int handIndices(bool isRight, int fingerNumber, int fingerPart) {
	//fingerNumber:
	// 1 thumb
	// 2 pointer
	// 3 middle
	// 4 index
	// 5 pinky

	//fingerPart: 
	// 1 thumb
	// 2 pointer
	// 3 middle
	// 4 index
	// 5 pinky
	int baseIndex= 19;
	if (!handIndices) {
		baseIndex = 36;
	}
	for (int i = 0; i < fingerNumber-1; i++) {
		if (i == 0) {
			baseIndex += 2;
		}
		else {
			baseIndex += 3;
		}
	}
	for (int i = 0; i < fingerPart - 1; i++) {
		baseIndex++;
	}
	return baseIndex;
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitContextVersion(4, 3);//�HOpenGL version4.3���������
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);//�O�_�V�U�ۮe,GLUT_FORWARD_COMPATIBLE���䴩(?
	glutInitContextProfile(GLUT_CORE_PROFILE);

	//multisample for golygons smooth
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Project 1 - Robocop");

	glewExperimental = GL_TRUE; //�m��glewInit()���e
	if (glewInit()) {
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;//c error
		exit(EXIT_FAILURE);
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(ChangeSize);
	glutKeyboardFunc(Keyboard);
	//int ActionMenu, ModeMenu, ShaderMenu;
	//ActionMenu = glutCreateMenu(ActionMenuEvents);//�إߥk����
	////�[�J�k�䪫��
	//glutAddMenuEntry("idle", 0);
	//glutAddMenuEntry("walk", 1);
	//glutAttachMenu(GLUT_RIGHT_BUTTON);	//�P�k�����p

	//ModeMenu = glutCreateMenu(ModeMenuEvents);//�إߥk����
	////�[�J�k�䪫��
	//glutAddMenuEntry("Line", 0);
	//glutAddMenuEntry("Fill", 1);
	//glutAttachMenu(GLUT_RIGHT_BUTTON);	//�P�k�����p


	//glutCreateMenu(menuEvents);//�إߥk����
	////�[�J�k�䪫��
	//glutAddSubMenu("action", ActionMenu);
	//glutAddSubMenu("mode", ModeMenu);
	//glutAttachMenu(GLUT_RIGHT_BUTTON);	//�P�k�����p

	glutMouseFunc(Mouse);
	glutTimerFunc(100, idle, 0);
	glutMainLoop();
	return 0;
}
void ChangeSize(int w, int h) {
	if (h == 0) h = 1;
	glViewport(0, 0, w, h);
	Projection = perspective(80.0f, (float)w / h, 0.1f, 100.0f);
}
void Mouse(int button, int state, int x, int y) {
	if (button == 2) isFrame = false;
}
void idle(int dummy) {
	isFrame = true;
	int out = 0;
	if (action == WALK) {
		updateObj(dummy);
		out = dummy + 1;
		if (out > 12) out = 1;
	}
	else if (action == IDLE) {
		resetObj(dummy);
		out = 0;
	}
	glutPostRedisplay();

	glutTimerFunc(150, idle, out);
}
void resetObj(int f) {
	for (int i = 0; i < PARTSNUM; i++) {
		angles[i] = 0.0f;
	}
}
void updateObj(int frame) {
	switch (frame) {
	case 0:
		//����
		angles[2] = -45;
		//�k��

		//�L
		angles[13] = 45;

		break;
	case 1:
	case 2:
	case 3:
		angles[1] += 10;
		angles[12] -= 15;
		position += 0.1;
		break;
	case 4:
	case 5:
	case 6:
		angles[1] -= 10;
		angles[12] += 15;
		angles[13] -= 15;
		position -= 0.1;
		break;
	case 7:
	case 8:
	case 9:
		angles[1] -= 10;
		angles[12] += 15;
		angles[13] = 0;
		position += 0.1;
		break;
	case 10:
	case 11:
	case 12:
		angles[1] += 10;
		angles[12] -= 15;
		angles[13] += 15;
		position -= 0.1;
		break;
	}
}


GLuint M_KaID;
GLuint M_KdID;
GLuint M_KsID;

void init() {
	isFrame = false;
	pNo = 0;
	for (int i = 0; i < PARTSNUM; i++)//��l�ƨ��װ}�C
		angles[i] = 0.0;

	//VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	ShaderInfo shaders[] = {
		{ GL_VERTEX_SHADER, "DSPhong_Material.vp" },//vertex shader
		{ GL_FRAGMENT_SHADER, "DSPhong_Material.fp" },//fragment shader
		{ GL_NONE, NULL } };
	program = LoadShaders(shaders);//Ū��shader

	glUseProgram(program);//uniform�ѼƼƭȫe������use shader

	MatricesIdx = glGetUniformBlockIndex(program, "MatVP");
	ModelID = glGetUniformLocation(program, "Model");
	M_KaID = glGetUniformLocation(program, "Material.Ka");
	M_KdID = glGetUniformLocation(program, "Material.Kd");
	M_KsID = glGetUniformLocation(program, "Material.Ks");
	//or
	M_KdID = M_KaID + 1;
	M_KsID = M_KaID + 2;

	Projection = glm::perspective(80.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

	// Camera matrix
	View = glm::lookAt(
		glm::vec3(0, 10, 25), // Camera is at (0,10,25), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
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


	glClearColor(0.0, 0.0, 0.0, 1);//black screen
}

#define DOR(angle) (angle*3.1415/180);
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(VAO);
	glUseProgram(program);//uniform�ѼƼƭȫe������use shader
	float eyey = DOR(eyeAngley);
	View = lookAt(
		vec3(eyedistance * sin(eyey), 2, eyedistance * cos(eyey)), // Camera is at (0,0,20), in World Space
		vec3(0, 0, 0), // and looks at the origin
		vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	myUpdateModel();
	//update data to UBO for MVP
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), &View);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), &Projection);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	GLuint offset[3] = { 0,0,0 };//offset for vertices , uvs , normals
	for (int i = 0; i < PARTSNUM; i++) {
		glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Models[i][0][0]);

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

		int vertexIDoffset = 0;//glVertexID's offset 
		string mtlname;//material name
		vec3 Ks = vec3(1, 1, 1);//because .mtl excluding specular , so give it here.
		for (int j = 0; j < mtls[i].size(); j++) {//
			mtlname = mtls[i][j];
			//find the material diffuse color in map:KDs by material name.
			glUniform3fv(M_KdID, 1, &KDs[mtlname][0]);
			glUniform3fv(M_KsID, 1, &Ks[0]);
			//          (primitive   , glVertexID base , vertex count    )
			glDrawArrays(GL_TRIANGLES, vertexIDoffset, faces[i][j + 1] * 3);
			//we draw triangles by giving the glVertexID base and vertex count is face count*3
			vertexIDoffset += faces[i][j + 1] * 3;//glVertexID's base offset is face count*3
		}//end for loop for draw one part of the robot	

	}//end for loop for updating and drawing model
	glFlush();//�j�����W����OpenGL commands
	glutSwapBuffers();//�մ��e�x�M��xbuffer ,���Obuffer�e���M�e�xbuffer�洫�ϧڭ̬ݨ���
}

void Obj2Buffer() {
	std::vector<vec3> Kds;
	std::vector<vec3> Kas;
	std::vector<vec3> Kss;
	std::vector<std::string> Materials;//mtl-name
	std::string texture;
	for (int i = 0; i < PARTSNUM; i++) {
		cout << partsList[i] << endl;
		loadMTL(("../Assets/Obj/" + partsList[i] + ".mtl").c_str(), Kds, Kas, Kss, Materials, texture);
	}
	//printf("%d\n",texture);
	for (int i = 0; i < Materials.size(); i++) {
		string mtlname = Materials[i];
		//  name            vec3
		KDs[mtlname] = Kds[i];
	}

	for (int i = 0; i < PARTSNUM; i++) {
		load2Buffer("../Assets/Obj/"+partsList[i]+".obj", i);
	}
	GLuint totalSize[3] = { 0,0,0 };
	GLuint offset[3] = { 0,0,0 };
	for (int i = 0; i < PARTSNUM; i++) {
		totalSize[0] += vertices_size[i] * sizeof(vec3);
		totalSize[1] += uvs_size[i] * sizeof(vec2);
		totalSize[2] += normals_size[i] * sizeof(vec3);
	}
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


	for (int i = 0; i < PARTSNUM; i++) {
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
	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
}

void myUpdateModel() {
	//reset state
	for (int i = 0; i < PARTSNUM; i++) {
		Models[i] = mat4(1.0f);
	}


	//placing to initial codes
	for (int i = 0; i < PARTSNUM; i++) {
		Models[i] = translate(Models[i], vec3(initialOffset[i][0], initialOffset[i][1], initialOffset[i][2]));
	}
}

void updateModels() {
	mat4 Rotatation[PARTSNUM];
	mat4 Translation[PARTSNUM];
	for (int i = 0; i < PARTSNUM; i++) {
		Models[i] = mat4(1.0f);
		Rotatation[i] = mat4(1.0f);
		Translation[i] = mat4(1.0f);
	}
	float r, pitch, yaw, roll;
	float alpha, beta, gamma;

	//Body
	beta = angleMain;
	Rotatation[0] = rotate(beta, 0, 1, 0);
	Translation[0] = translate(0, 2.9 + position, 0);
	Models[0] = Translation[0] * Rotatation[0];
	//����=======================================================
	//���W���u
	yaw = DOR(beta); r = 3.7;
	alpha = angles[1];
	gamma = 10;
	Rotatation[1] = rotate(alpha, 1, 0, 0) * rotate(gamma, 0, 0, 1);//�V�e����*�V�k����
	Translation[1] = translate(3.7, 1, -0.5);

	Models[1] = Models[0] * Translation[1] * Rotatation[1];

	//���ӻH
	Rotatation[4] = rotate(alpha, 1, 0, 0) * rotate(gamma, 0, 0, 1);//�V�e����*�V�k����
	Translation[4] = translate(3.7, 1, -0.5);//�첾�쥪�W���u�B
	Models[4] = Models[0] * Translation[1] * Rotatation[1];

	//���U���u
	pitch = DOR(alpha); r = 3;
	roll = DOR(gamma);
	static int i = 0;
	i += 5;
	alpha = angles[2] - 20;
	//�W���u+�U���u�V�e����*�V�k����
	Rotatation[2] = rotate(alpha, 1, 0, 0);
	//��x�b�첾�H�W���u���b�|����P��:translate(0,r*cos,r*sin)
	//��z�b�첾�H�W���u���b�|����:translate(r*sin,-rcos,0)
	Translation[2] = translate(0, -3, 0);

	Models[2] = Models[1] * Translation[2] * Rotatation[2];


	pitch = DOR(alpha);
	//b = DOR(angles[2]);
	roll = DOR(gamma);
	//��x���׻P�U���u�ۦP
	//Rotatation[3] = Rotatation[2];
	//��x�b�첾�H�W���u���b�|����P��:translate(0,r*cos,r*sin) ,���׬��W���u+�U���u
	Translation[3] = translate(0, -4.8, 0);
	Models[3] = Models[2] * Translation[3] * Rotatation[3];
	//============================================================
	//�Y==========================================================
	Translation[5] = translate(0, 3.9, -0.5);
	Models[5] = Models[0] * Translation[5] * Rotatation[5];
	//============================================================
	//�k��=========================================================
	gamma = -10; alpha = angles[6] = -angles[1];
	Rotatation[6] = rotate(alpha, 1, 0, 0) * rotate(gamma, 0, 0, 1);
	Translation[6] = translate(-3.9, 1.7, -0.2);
	Models[6] = Models[0] * Translation[6] * Rotatation[6];

	Rotatation[9] = rotate(alpha, 1, 0, 0) * rotate(gamma, 0, 0, 1);
	Translation[9] = translate(-3.9, 1.1, -0.2);
	Models[9] = Models[0] * Translation[9] * Rotatation[9];

	angles[7] = angles[2];
	pitch = DOR(alpha); r = -3;
	roll = DOR(gamma);
	alpha = angles[7] - 20;
	Rotatation[7] = rotate(alpha, 1, 0, 0);
	Translation[7] = translate(0, -3, 0);
	Models[7] = Models[6] * Translation[7] * Rotatation[7];

	pitch = DOR(alpha);
	//b = DOR(angles[7]);
	roll = DOR(gamma);
	Translation[8] = translate(0, -6, 0);
	Models[8] = Models[7] * Translation[8] * Rotatation[8];
	//=============================================================
	//back&DBody===================================================
	Translation[10] = translate(0, 2, -4.5);
	Models[10] = Models[0] * Translation[10] * Rotatation[10];

	Translation[11] = translate(0, -5.3, 0);
	Models[11] = Models[0] * Translation[11] * Rotatation[11];
	//=============================================================
	//���}
	alpha = angles[12]; gamma = 10;
	Rotatation[12] = rotate(alpha, 1, 0, 0) * rotate(gamma, 0, 0, 1);
	Translation[12] = translate(1.8, -4.5, 0);
	Models[12] = Translation[12] * Rotatation[12] * Models[12];

	pitch = DOR(alpha); r = -7;
	roll = DOR(gamma);
	alpha = angles[13] + angles[12];
	Translation[13] = translate(-r * sin(roll), r * cos(pitch), r * sin(pitch)) * Translation[12];
	Rotatation[13] = rotate(alpha, 1, 0, 0);
	Models[13] = Translation[13] * Rotatation[13] * Models[13];

	pitch = DOR(alpha); r = -5;
	//b = DOR(angles[13]);
	roll = DOR(gamma);
	Translation[14] = translate(-(r + 2) * sin(roll), r * cos(pitch), r * sin(pitch) - 1) * Translation[13];
	Rotatation[14] = rotate(alpha, 1, 0, 0);
	Models[14] = Translation[14] * Rotatation[14] * Models[14];
	//=============================================================
	//�k�}
	alpha = angles[15] = -angles[12];
	gamma = -10;
	Rotatation[15] = rotate(alpha, 1, 0, 0) * rotate(gamma, 0, 0, 1);
	Translation[15] = translate(-1.8, -4.5, 0);
	Models[15] = Translation[15] * Rotatation[15] * Models[15];

	angles[16] = angles[13];
	pitch = DOR(alpha); r = -7;
	roll = DOR(gamma);
	alpha = angles[16] + angles[15];
	Rotatation[16] = rotate(alpha, 1, 0, 0);
	Translation[16] = translate(-r * sin(roll), r * cos(pitch), r * sin(pitch)) * Translation[15];
	Models[16] = Translation[16] * Rotatation[16] * Models[16];

	pitch = DOR(alpha); r = -5;
	//b = DOR(angles[16]);
	roll = DOR(gamma);
	alpha = angles[15] + angles[16];
	Translation[17] = translate(-(r + 2) * sin(roll), r * cos(pitch), r * sin(pitch) - 0.5) * Translation[16];
	Rotatation[17] = rotate(alpha, 1, 0, 0);
	Models[17] = Translation[17] * Rotatation[17] * Models[17];
	//=============================================================
}

void load2Buffer(string obj, int i) {
	std::vector<vec3> vertices;
	std::vector<vec2> uvs;
	std::vector<vec3> normals; // Won't be used at the moment.
	std::vector<unsigned int> materialIndices;

	const char* c = obj.c_str();
	bool res = loadOBJ(c, vertices, uvs, normals, faces[i], mtls[i]);
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
mat4 translate(float x, float y, float z) {
	vec4 t = vec4(x, y, z, 1);//w = 1 ,�hx,y,z=0�ɤ]��translate
	vec4 c1 = vec4(1, 0, 0, 0);
	vec4 c2 = vec4(0, 1, 0, 0);
	vec4 c3 = vec4(0, 0, 1, 0);
	mat4 M = mat4(c1, c2, c3, t);
	return M;
}
mat4 scale(float x, float y, float z) {
	vec4 c1 = vec4(x, 0, 0, 0);
	vec4 c2 = vec4(0, y, 0, 0);
	vec4 c3 = vec4(0, 0, z, 0);
	vec4 c4 = vec4(0, 0, 0, 1);
	mat4 M = mat4(c1, c2, c3, c4);
	return M;
}

mat4 rotate(float angle, float x, float y, float z) {
	float r = DOR(angle);
	mat4 M = mat4(1);

	vec4 c1 = vec4(cos(r) + (1 - cos(r)) * x * x, (1 - cos(r)) * y * x + sin(r) * z, (1 - cos(r)) * z * x - sin(r) * y, 0);
	vec4 c2 = vec4((1 - cos(r)) * y * x - sin(r) * z, cos(r) + (1 - cos(r)) * y * y, (1 - cos(r)) * z * y + sin(r) * x, 0);
	vec4 c3 = vec4((1 - cos(r)) * z * x + sin(r) * y, (1 - cos(r)) * z * y - sin(r) * x, cos(r) + (1 - cos(r)) * z * z, 0);
	vec4 c4 = vec4(0, 0, 0, 1);
	M = mat4(c1, c2, c3, c4);
	return M;
}
void Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '1':
		angleMain += 5;
		if (angleMain >= 360) angleMain = 0;
		printf("beta:%f\n", angleMain);
		break;
	case '2':
		angleMain -= 5;
		if (angleMain <= 0) angleMain = 360;
		printf("beta:%f\n", angleMain);
		break;
	case 'w':
		eyedistance -= 0.2;
		break;
	case 's':
		eyedistance += 0.2;
		break;
	case 'a':
		eyeAngley -= 10;
		break;
	case 'd':
		eyeAngley += 10;
		break;
	case 'r':
		angles[1] -= 5;
		if (angles[1] == -360) angles[1] = 0;
		movey = 0;
		movex = 0;
		break;
	case 't':
		angles[2] -= 5;
		if (angles[2] == -360) angles[2] = 0;
		movey = 0;
		movex = 0;
		break;
	case 'q':
		break;
	case 'e':
		break;
	}
	glutPostRedisplay();
}
void menuEvents(int option) {}
void ActionMenuEvents(int option) {
	switch (option) {
	case 0:
		action = IDLE;
		break;
	case 1:
		action = WALK;
		break;
	}
}
void ModeMenuEvents(int option) {
	switch (option) {
	case 0:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 1:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	}
}
void ShaderMenuEvents(int option) {
	pNo = option;
}
