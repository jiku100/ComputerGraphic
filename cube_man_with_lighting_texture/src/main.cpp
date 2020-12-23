//
// Display a illuminated sphere
//

#include "initShader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "texture.hpp"

enum eShadeMode { NO_LIGHT, GOURAUD, PHONG, NUM_LIGHT_MODE };

glm::mat4 projectMat;
glm::mat4 viewMat;
glm::mat4 modelMat;

int shadeMode = NO_LIGHT;
int isTexture = false;
int isRotate = false;

float setAngle = -90.0f;
float rotAngle = 0.0f;
float armAngle = 0.0f;
float underLegAngle = 0.0f;
float underArmAngle = 0.0f;

int armDecide = 1;

typedef glm::vec4  normal4;
typedef glm::vec4  point4;
typedef glm::vec2  texcoord2;
const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

point4 points[NumVertices];
normal4 normals[NumVertices];
texcoord2 texCoords[NumVertices];

GLuint projectMatrixID;
GLuint viewMatrixID;
GLuint modelMatrixID;
GLuint shadeModeID;
GLuint textureModeID;

GLuint Texture1;
GLuint Texture2;
GLuint TextureID;

point4 vertices[8] = {
	point4(-0.5, -0.5, 0.5, 1.0),
	point4(-0.5, 0.5, 0.5, 1.0),
	point4(0.5, 0.5, 0.5, 1.0),
	point4(0.5, -0.5, 0.5, 1.0),
	point4(-0.5, -0.5, -0.5, 1.0),
	point4(-0.5, 0.5, -0.5, 1.0),
	point4(0.5, 0.5, -0.5, 1.0),
	point4(0.5, -0.5, -0.5, 1.0)
};

int Index = 0;
void
quad(int a, int b, int c, int d)
{
	points[Index] = vertices[a]; texCoords[Index] = glm::vec2(0.0, 1.0); normals[Index] = vertices[a]; normals[Index][3] = 0.0; Index++;
	points[Index] = vertices[b]; texCoords[Index] = glm::vec2(0.0, 0.0); normals[Index] = vertices[b]; normals[Index][3] = 0.0; Index++;
	points[Index] = vertices[c]; texCoords[Index] = glm::vec2(1.0, 0.0); normals[Index] = vertices[c]; normals[Index][3] = 0.0; Index++;
	points[Index] = vertices[a]; texCoords[Index] = glm::vec2(0.0, 1.0); normals[Index] = vertices[a]; normals[Index][3] = 0.0; Index++;
	points[Index] = vertices[c]; texCoords[Index] = glm::vec2(1.0, 0.0); normals[Index] = vertices[c]; normals[Index][3] = 0.0; Index++;
	points[Index] = vertices[d]; texCoords[Index] = glm::vec2(1.0, 1.0); normals[Index] = vertices[d]; normals[Index][3] = 0.0; Index++;
}

void
colorcube()
{
	quad(1, 0, 3, 2);
	quad(3, 7, 6, 2);
	quad(0, 4, 7, 3);
	quad(4, 5, 6, 7);
	quad(1, 5, 4, 0);
	quad(2, 6, 5, 1);
}

void drawMan()
{
	glm::mat4 bodyMat, headMat, LArmMat, LForMat,
		RArmMat, RForMat, LULMat, LLLMat, RULMat, RLLMat;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture1);
	glUniform1i(TextureID, 0);

	modelMat = glm::rotate(glm::mat4(1.0f), glm::radians(setAngle), glm::vec3(1.0f, 0.0f, 0.0f));
	modelMat = glm::rotate(modelMat, rotAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	// body
	bodyMat = glm::scale(modelMat, glm::vec3(0.5, 0.5, 0.5));
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &bodyMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	// L Arm
	LArmMat = glm::translate(bodyMat, glm::vec3(-0.625, 0, 0));
	LArmMat = glm::scale(LArmMat, glm::vec3(0.2, 0.4, 0.8));
	LArmMat = glm::rotate(LArmMat, glm::radians(15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	LArmMat = glm::rotate(LArmMat, armAngle, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &LArmMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	// L Forearm
	LForMat = glm::translate(LArmMat, glm::vec3(0, 0, -1));
	LForMat = glm::rotate(LForMat, underArmAngle, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &LForMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	// R Arm
	RArmMat = glm::translate(bodyMat, glm::vec3(0.625, 0, 0));
	RArmMat = glm::scale(RArmMat, glm::vec3(0.2, 0.4, 0.8));
	RArmMat = glm::rotate(RArmMat, glm::radians(-15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	RArmMat = glm::rotate(RArmMat, -armAngle, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &RArmMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	// R Forearm
	RForMat = glm::translate(RArmMat, glm::vec3(0, 0, -1));
	RForMat = glm::rotate(RForMat, underArmAngle, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &RForMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	// L Upper Leg
	LULMat = glm::translate(bodyMat, glm::vec3(-0.25, 0, -1));
	LULMat = glm::scale(LULMat, glm::vec3(0.25, 0.5, 1));
	LULMat = glm::rotate(LULMat, -armAngle, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &LULMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	// L Lower Leg
	LLLMat = glm::translate(LULMat, glm::vec3(0, 0, -1));
	LLLMat = glm::rotate(LLLMat, -underLegAngle, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &LLLMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	//R Upper Leg
	RULMat = glm::translate(bodyMat, glm::vec3(0.25, 0, -1));
	RULMat = glm::scale(RULMat, glm::vec3(0.25, 0.5, 1));
	RULMat = glm::rotate(RULMat, armAngle, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &RULMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	// R Lower Leg
	RLLMat = glm::translate(RULMat, glm::vec3(0, 0, -1));
	RLLMat = glm::rotate(RLLMat, -underLegAngle, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &RLLMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	//head
	headMat = glm::translate(bodyMat, glm::vec3(0, 0, 0.75));
	headMat = glm::scale(headMat, glm::vec3(0.5, 0.5, 0.5));
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &headMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 30);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, Texture2);
	glUniform1i(TextureID, 1);
	glDrawArrays(GL_TRIANGLES, 30, 6);
}

//----------------------------------------------------------------------------

// OpenGL initialization
void init()
{
	colorcube();
	projectMat = glm::perspective(glm::radians(65.0f), 1.0f, 0.1f, 100.0f);
	viewMat = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	modelMat = glm::mat4(1.0f);
	// Create a vertex array object
	GLuint vao[1];
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	// Create and initialize a buffer object
	GLuint buffer[1];
	glGenBuffers(1, buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(normals) + sizeof(texCoords),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(normals), normals);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(normals), sizeof(texCoords), texCoords);

	// Load shaders and use the resulting shader program
	GLuint program = InitShader("src/vshader.glsl", "src/fshader.glsl");
	glUseProgram(program);

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(points)));

	GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(points) + sizeof(normals)));

	projectMatrixID = glGetUniformLocation(program, "mProject");
	glUniformMatrix4fv(projectMatrixID, 1, GL_FALSE, &projectMat[0][0]);

	viewMatrixID = glGetUniformLocation(program, "mView");
	glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &viewMat[0][0]);

	modelMatrixID = glGetUniformLocation(program, "mModel");
	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMat[0][0]);

	shadeModeID = glGetUniformLocation(program, "shadeMode");
	glUniform1i(shadeModeID, shadeMode);

	textureModeID = glGetUniformLocation(program, "isTexture");
	glUniform1i(textureModeID, isTexture);

	// Load the texture using any two methods
	Texture1 = loadBMP_custom("earth.bmp");
	Texture2 = loadBMP_custom("face.bmp");
	//GLuint Texture = loadDDS("uvtemplate.DDS");

	// Get a handle for our "myTextureSampler" uniform
	TextureID = glGetUniformLocation(program, "Texture");

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture1);

	//// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(TextureID, 0);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

//----------------------------------------------------------------------------

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawMan();

	glutSwapBuffers();
}

//----------------------------------------------------------------------------

void idle()
{
	static int prevTime = glutGet(GLUT_ELAPSED_TIME);
	int currTime = glutGet(GLUT_ELAPSED_TIME);


	if (abs(currTime - prevTime) >= 20) {
		float t = abs(currTime - prevTime);
		armAngle += glm::radians(armDecide * t * 360.0f / 2000.0f);
		underLegAngle = abs(armAngle);
		underArmAngle = abs(armAngle);
		if (armAngle >= glm::radians(45.0f))
			armDecide = -1;
		else if (armAngle <= glm::radians(-45.0f))
			armDecide = 1;
		if(isRotate)
			rotAngle += glm::radians(t * 360.0f / 5000.0f);
		prevTime = currTime;
	}
	
	glutPostRedisplay();
}

//----------------------------------------------------------------------------

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'l': case 'L':
		shadeMode = (++shadeMode % NUM_LIGHT_MODE);
		glUniform1i(shadeModeID, shadeMode);
		glutPostRedisplay();
		break;
	case 'r': case 'R':
		isRotate = !isRotate;
		glutPostRedisplay();
		break;
	case 't': case 'T':
		isTexture = !isTexture;
		glUniform1i(textureModeID, isTexture);
		glutPostRedisplay();
		break;
	case 033:  // Escape key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	}
}

//----------------------------------------------------------------------------

void resize(int w, int h)
{
	float ratio = (float)w / (float)h;
	glViewport(0, 0, w, h);

	projectMat = glm::perspective(glm::radians(65.0f), ratio, 0.1f, 100.0f);
	glUniformMatrix4fv(projectMatrixID, 1, GL_FALSE, &projectMat[0][0]);
	glutPostRedisplay();
}

//----------------------------------------------------------------------------

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Color Cube");

	glewInit();

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(resize);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}