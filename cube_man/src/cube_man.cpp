 //
// Display a color cube
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.  We us an orthographic projection
//   as the default projetion.

#include "cube.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

glm::mat4 projectMat;
glm::mat4 viewMat;

GLuint pvmMatrixID;

float setAngle = -90.0f;
float rotAngle = 0.0f;
float armAngle = 0.0f;
float underLegAngle = 0.0f;
float underArmAngle = 0.0f;

int armDecide = 1;

typedef glm::vec4  color4;
typedef glm::vec4  point4;

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

point4 points[NumVertices];
color4 colors[NumVertices];

// Vertices of a unit cube centered at origin, sides aligned with axes
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

// RGBA colors
color4 vertex_colors[8] = {
	color4(0.0, 0.0, 0.0, 1.0),  // black
	color4(0.0, 1.0, 1.0, 1.0),   // cyan
	color4(1.0, 0.0, 1.0, 1.0),  // magenta
	color4(1.0, 1.0, 0.0, 1.0),  // yellow
	color4(1.0, 0.0, 0.0, 1.0),  // red
	color4(0.0, 1.0, 0.0, 1.0),  // green
	color4(0.0, 0.0, 1.0, 1.0),  // blue
	color4(1.0, 1.0, 1.0, 1.0)  // white
};

//----------------------------------------------------------------------------

// quad generates two triangles for each face and assigns colors
//    to the vertices
int Index = 0;
void
quad(int a, int b, int c, int d)
{
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a];  Index++;
	colors[Index] = vertex_colors[b]; points[Index] = vertices[b];  Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c];  Index++;
	colors[Index] = vertex_colors[a]; points[Index] = vertices[a];  Index++;
	colors[Index] = vertex_colors[c]; points[Index] = vertices[c];  Index++;
	colors[Index] = vertex_colors[d]; points[Index] = vertices[d];  Index++;
}

//----------------------------------------------------------------------------

// generate 12 triangles: 36 vertices and 36 colors
void
colorcube()
{
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);
}

//----------------------------------------------------------------------------

// OpenGL initialization
void
init()
{
	colorcube();

	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);

	// Load shaders and use the resulting shader program
	GLuint program = InitShader("src/vshader.glsl", "src/fshader.glsl");
	glUseProgram(program);

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sizeof(points)));

	pvmMatrixID = glGetUniformLocation(program, "mPVM");

	projectMat = glm::perspective(glm::radians(65.0f), 1.0f, 0.1f, 100.0f);
	viewMat = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

//----------------------------------------------------------------------------

void drawMan(glm::mat4 worldMat)
{
	glm::mat4 bodyMat, headMat, LArmMat, LForMat,
		RArmMat, RForMat, LULMat, LLLMat, RULMat, RLLMat, pvmMat;

	// body
	bodyMat = glm::scale(worldMat, glm::vec3(0.5, 0.5, 0.5));
	pvmMat = projectMat * viewMat * bodyMat;
	glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	//head
	headMat = glm::translate(bodyMat, glm::vec3(0, 0, 0.75));
	headMat = glm::scale(headMat, glm::vec3(0.5, 0.5, 0.5));
	pvmMat = projectMat * viewMat * headMat;
	glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	// L Arm
	LArmMat = glm::translate(bodyMat, glm::vec3(-0.625, 0, 0));
	LArmMat = glm::scale(LArmMat, glm::vec3(0.2, 0.4, 0.8));
	LArmMat = glm::rotate(LArmMat, glm::radians(15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	LArmMat = glm::rotate(LArmMat, armAngle, glm::vec3(1.0f, 0.0f, 0.0f));
	pvmMat = projectMat * viewMat * LArmMat;
	glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	// L Forearm
	LForMat = glm::translate(LArmMat, glm::vec3(0, 0, -1));
	LForMat = glm::rotate(LForMat, underArmAngle, glm::vec3(1.0f, 0.0f, 0.0f));
	pvmMat = projectMat * viewMat * LForMat;
	glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	// R Arm
	RArmMat = glm::translate(bodyMat, glm::vec3(0.625, 0, 0));
	RArmMat = glm::scale(RArmMat, glm::vec3(0.2, 0.4, 0.8));
	RArmMat = glm::rotate(RArmMat, glm::radians(-15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	RArmMat = glm::rotate(RArmMat, -armAngle, glm::vec3(1.0f, 0.0f, 0.0f));
	pvmMat = projectMat * viewMat * RArmMat;
	glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	// R Forearm
	RForMat = glm::translate(RArmMat, glm::vec3(0, 0, -1));
	RForMat = glm::rotate(RForMat, underArmAngle, glm::vec3(1.0f, 0.0f, 0.0f));
	pvmMat = projectMat * viewMat * RForMat;
	glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	// L Upper Leg
	LULMat = glm::translate(bodyMat, glm::vec3(-0.25, 0, -1));
	LULMat = glm::scale(LULMat, glm::vec3(0.25, 0.5, 1));
	LULMat = glm::rotate(LULMat, -armAngle, glm::vec3(1.0f, 0.0f, 0.0f));
	pvmMat = projectMat * viewMat * LULMat;
	glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	// L Lower Leg
	LLLMat = glm::translate(LULMat, glm::vec3(0, 0, -1));
	LLLMat = glm::rotate(LLLMat, -underLegAngle, glm::vec3(1.0f, 0.0f, 0.0f));
	pvmMat = projectMat * viewMat * LLLMat;
	glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	//R Upper Leg
	RULMat = glm::translate(bodyMat, glm::vec3(0.25,0, -1));
	RULMat = glm::scale(RULMat, glm::vec3(0.25, 0.5, 1));
	RULMat = glm::rotate(RULMat, armAngle, glm::vec3(1.0f, 0.0f, 0.0f));
	pvmMat = projectMat * viewMat * RULMat;
	glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	// R Lower Leg
	RLLMat = glm::translate(RULMat, glm::vec3(0, 0, -1));
	RLLMat = glm::rotate(RLLMat, -underLegAngle, glm::vec3(1.0f, 0.0f, 0.0f));
	pvmMat = projectMat * viewMat * RLLMat;
	glUniformMatrix4fv(pvmMatrixID, 1, GL_FALSE, &pvmMat[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}


void display(void)
{
	glm::mat4 worldMat, pvmMat;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	worldMat = glm::rotate(glm::mat4(1.0f), glm::radians(setAngle), glm::vec3(1.0f, 0.0f, 0.0f));
	worldMat = glm::rotate(worldMat, rotAngle, glm::vec3(0.0f, 0.0f, 1.0f));
	drawMan(worldMat);

	glutSwapBuffers();
}

//----------------------------------------------------------------------------

void idle()
{
	static int prevTime = glutGet(GLUT_ELAPSED_TIME);
	int currTime = glutGet(GLUT_ELAPSED_TIME);

	if (abs(currTime - prevTime) >= 20)
	{
		float t = abs(currTime - prevTime);
		rotAngle += glm::radians(t * 360.0f / 5000.0f);
		armAngle += glm::radians(armDecide * t * 360.0f / 2000.0f);
		underLegAngle = abs(armAngle);
		underArmAngle = abs(armAngle);
		if (armAngle >= glm::radians(45.0f))
			armDecide = -1;
		else if (armAngle <= glm::radians(-45.0f))
			armDecide = 1;
		prevTime = currTime;
		glutPostRedisplay();
	}
}

//----------------------------------------------------------------------------

void
keyboard(unsigned char key, int x, int y)
{
	switch (key) {
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

	glutPostRedisplay();
}

//----------------------------------------------------------------------------

int
main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Cube Man");

	glewInit();

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(resize);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}
