
#include <iostream>
#include <sstream>


/*GLEW
/ use static lib and reference before others
*/
#define GLEW_STATIC
#include <GL/eglew.h>

//GLFW
#include <GLFW/glfw3.h>

//SOIL
#include <SOIL/SOIL.h>

//Math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Shader
#include "Shader.h"
#include "DrawStart.h"
#include "Camera.h"

using namespace std;

const GLuint WIDTH = 1280, HEIGHT = 720;

Camera *_camera;

float mixValue = 45.0f;

float FoV = 45.0f;
float AspectRatio = (float)WIDTH / HEIGHT;
double Aspect = 45.0f;

GLfloat DELTA_TIME = 0.0f;
GLfloat LAST_FRAME = 0.0f;

//GLfloat PITCH = 0.0f;
//GLfloat YAW = -90.0f;

GLfloat lastX = 640, lastY = 360;

string IMAGE[] = { "Resource/img/muban.jpg" , "Resource/img/mao.jpg" };
bool KEYS[1024];

//View matrix
glm::mat4 VIEW_MATRIX;
glm::vec3 CAMERA_POS;
glm::vec3 CAMERA_FRONT;
glm::vec3 CAMERA_UP;

bool FIRST_MOUSE = true;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void mouse_callback(GLFWwindow *window, double xpos, double ypos);

void scroll_callback(GLFWwindow *window, double xOffset, double yOffset);

void InitGF()
{
	glfwInit();
}

void SetGFRequired()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}

GLFWwindow* CreateGFwindowObj()
{
	GLFWwindow * window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	return window;
}

bool InitGE()
{
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return false;
	}
	return true;
}

void DefineViewPort(GLFWwindow* window)
{
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
}

void SetKeyCallback(GLFWwindow *window)
{
	glfwSetKeyCallback(window, key_callback);
}

void SetMouseCallback(GLFWwindow *window, Camera *camera)
{
	//if(camera != nullptr)
	
	glfwSetCursorPosCallback(window, mouse_callback);
}

void SetScrollCallback(GLFWwindow *window, Camera *camera)
{
	glfwSetScrollCallback(window, scroll_callback);
}

void Ready(GLFWwindow* &window)
{
	InitGF();
	SetGFRequired();

	window = CreateGFwindowObj();

	InitGE();
	//camera = new Camera()
	DefineViewPort(window);

	//Event
	SetKeyCallback(window);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST);
}

void OnKeyPress(Camera *camera)
{
	/*GLfloat cameraSpeed = 5.0f * DELTA_TIME;
	if (KEYS[GLFW_KEY_W])
		CAMERA_POS += cameraSpeed * CAMERA_FRONT;
	if (KEYS[GLFW_KEY_S])
		CAMERA_POS -= cameraSpeed * CAMERA_FRONT;
	if (KEYS[GLFW_KEY_A])
		CAMERA_POS -= glm::normalize(glm::cross(CAMERA_FRONT, CAMERA_UP)) * cameraSpeed;
	if (KEYS[GLFW_KEY_D])
		CAMERA_POS += glm::normalize(glm::cross(CAMERA_FRONT, CAMERA_UP)) * cameraSpeed;*/

	if (KEYS[GLFW_KEY_W])
		camera->ProcessKeyboard(FORWARD, DELTA_TIME);
	if (KEYS[GLFW_KEY_S])
		camera->ProcessKeyboard(BACKWARD, DELTA_TIME);
	if (KEYS[GLFW_KEY_A])
		camera->ProcessKeyboard(LEFT, DELTA_TIME);
	if (KEYS[GLFW_KEY_D])
		camera->ProcessKeyboard(RIGHT, DELTA_TIME);
}

template<typename Func>
void InputVertexInfo(GLuint *VAO, Func func)
{
	//Creae a buffer Array ID
	glGenVertexArrays(1, VAO);
	//Bind the vertex array object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(*VAO);

	//Custom
	func();

	//Unbind
	//Unbind VAO First for saveing EBO, that would bind EBO autoly when bind VAO 
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template<typename Func>
void Draw(GLFWwindow *window, Camera *camera, Func func)
{
	//Loop
	while (!glfwWindowShouldClose(window))
	{
		//Check if any events
		glfwPollEvents();

		//Key Events
		OnKeyPress(camera);

		//Clear
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//Custom
		func();

		//Swap the screen buffers
		glfwSwapBuffers(window);
	}
}

template<typename Func>
void Draw(GLFWwindow *window, Func func)
{
	//Loop
	while (!glfwWindowShouldClose(window))
	{
		//Check if any events
		glfwPollEvents();

		//Key Events
		//OnKeyPress(camera);

		//Clear
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//Custom
		func();

		//Swap the screen buffers
		glfwSwapBuffers(window);
	}
}

void Clear(GLuint *VAO, GLuint *VBO, GLuint *EBO)
{
	glDeleteVertexArrays(1, VAO);
	glDeleteBuffers(1, VBO);

	if (EBO != nullptr)
	{
		glDeleteBuffers(1, EBO);
	}

	glfwTerminate();
}

void Clear(GLuint VAOs[], GLuint VBOs[])
{
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
}

//Use 2 texture
void Lesson8(GLFWwindow *window)
{
	cout << "Lesson8." << endl;
	Shader shader("shader2.vs", "shader2.fs");

	string imageName1 = "Resource/img/muban.jpg";
	string imageName2 = "Resource/img/mao.jpg";

	GLfloat triangle[] = {
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, //bottom right
		0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, //top right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, //bottom left
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f //top left
	};

	GLuint indices[] = {
		0, 1, 2, //first triangles
		1, 2, 3  //second triangles
	};

	GLuint VAO, VBO, EBO, texture1, texture2;
	cout << "Input vertexInfo..." << endl;
	//InputVertexInfo(&VAO, &VBO, triangle);
	InputVertexInfo(&VAO, [&]() {
		//Texture 1
		//Load image
		int width, height;
		unsigned char* image = SOIL_load_image(imageName1.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		glGenTextures(1, &texture1);
		glBindTexture(GL_TEXTURE_2D, texture1);
		//Create Texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0);

		//Texture 2
		//Load image
		int width1, height1;

		unsigned char* image2 = SOIL_load_image(imageName2.c_str(), &width1, &height1, 0, SOIL_LOAD_RGB);
		glGenTextures(1, &texture2);
		glBindTexture(GL_TEXTURE_2D, texture2);
		//Creaet Texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image2);
		glBindTexture(GL_TEXTURE_2D, 0);

		//Create a buffer ID
		glGenBuffers(1, &VBO);
		//Bind new Buffer
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//Copy user info to current buffer
		glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(GLfloat), triangle, GL_STATIC_DRAW);

		//Create a index buffer ID
		glGenBuffers(1, &EBO);
		//Bind new index Buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indices, GL_STATIC_DRAW);

		//Tell openGL how to explain vertex data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		//Color Attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		//TexCoord
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

	});

	Draw(window,
		[&]() {

		//Use shaderProgram
		glUseProgram(shader.Program);

		//Active Texture0 
		glActiveTexture(GL_TEXTURE0);
		//Bind Texture before bind VAO
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(shader.Program, "ourTexture1"), 0);

		//Active Texture1
		glActiveTexture(GL_TEXTURE1);
		//Bind
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(shader.Program, "ourTexture2"), 1);

		//Bind VAO
		glBindVertexArray(VAO);

		//Draw
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		glBindVertexArray(0);
	});

	Clear(&VAO, &VBO, nullptr);
}


void Lesson9(GLFWwindow *window)
{
	cout << "Lesson9:" << endl;
	Shader shader1("shader2-1.vs", "shader2-1.fs");

	string imageName1 = "Resource/img/muban.jpg";
	string imageName2 = "Resource/img/mao.jpg";

	GLfloat vertexts[] = {
		-0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f
	};

	GLint indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	GLuint VAO, VBO, EBO, texture1, texture2;
	cout << "Input vertexInfo..." << endl;
	InputVertexInfo(&VAO, [&]() {

		//VBO
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(GLfloat), vertexts, GL_STATIC_DRAW);

		//EBO
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLint), indices, GL_STATIC_DRAW);

		//Texture 1
		//Load image
		int width, height;
		unsigned char *image1 = SOIL_load_image(imageName1.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		//Create a texture ID
		glGenTextures(1, &texture1);
		glBindTexture(GL_TEXTURE_2D, texture1);
		//Set wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		//Set filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		//Create texture(texure target, mipmap level, which formate as save texture, texture width, texture height, ?, oringe img formate, oringe img data type, real img data)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
		//Generate mipmap autoly
		glGenerateMipmap(GL_TEXTURE_2D);
		//Free image data RAM 
		SOIL_free_image_data(image1);
		//Unbind
		glBindTexture(GL_TEXTURE_2D, 0);

		//Texture 2
		unsigned char *image2 = SOIL_load_image(imageName2.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		glGenTextures(1, &texture2);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image2);
		glBindTexture(GL_TEXTURE_2D, 0);

		//Tell openGL how to explain vertex data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		//Color Attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		//TexCoord
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
	});

	

	//matrix transform order
	// * translate * rotate * scale === scale -> rotate -> translate
	//=================================================================
	Draw(window, [&]() {
		//Program 1
		shader1.Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(shader1.Program, "ourTexture1"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(shader1.Program, "ourTexture2"), 1);

		glUniform1f(glGetUniformLocation(shader1.Program, "mixValue"), mixValue);

		//First gameObj
		glm::mat4 trans;
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, glm::radians((GLfloat)glfwGetTime() * 50.0f), glm::vec3(0.0, 0.0, 1.0));

		glUniformMatrix4fv(glGetUniformLocation(shader1.Program, "transform"), 1, GL_FALSE, glm::value_ptr(trans));

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//Second gameObj
		trans = glm::mat4();//reset trans
		
		// * translate * scale
		GLfloat scaleAmount = sin(glfwGetTime());
		trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
		trans = glm::scale(trans, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
		
		glUniformMatrix4fv(glGetUniformLocation(shader1.Program, "transform"), 1, GL_FALSE, glm::value_ptr(trans));

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	});

	Clear(&VAO, &VBO, &EBO);
}

//============================================
//Enter 3D, use model,view,projection matrix
//============================================
void Lesson10(GLFWwindow *window)
{
	cout << "Lesson10:" << endl;
	cout << "Create Shader..." << endl;
	Shader shader("shader3.vs", "shader3.fs");

	string imageName1 = "Resource/img/muban.jpg";
	string imageName2 = "Resource/img/mao.jpg";

	GLfloat vertexs[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {

		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)

	};

	GLint indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	GLuint VAO, VBO, EBO;
	GLuint TT[2];

	cout << "Input vertexInfo..." << endl;
	InputVertexInfo(&VAO, [&]() {
		//VBO
		//======
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);

		//EBO
		//======
		//glGenBuffers(1, &EBO);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		//Texture
		//=============
		int width, height;
		for (int i = 0; i < 2; i++)
		{
			unsigned char* image = SOIL_load_image(IMAGE[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);
			//TT
			glGenTextures(1, &TT[i]);
			//Pointer texture target!!!!
			glBindTexture(GL_TEXTURE_2D, TT[i]);
			//Set wrapping
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			//Set filter
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			//Generate
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(GL_TEXTURE_2D);
			//Free RAM and Unbind
			SOIL_free_image_data(image);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		//VertexAttribArray
		//=======================
		//Set vertex attribute pointer
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		//Set texCoord attibute pointer
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
	});

	//View matrix
	glm::mat4 view;
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	VIEW_MATRIX = glm::translate(VIEW_MATRIX, glm::vec3(0.0f, 0.0f, -3.0f));


	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	glm::vec3 cameraUp = glm::normalize(glm::cross(cameraDirection, cameraRight));
	
	Draw(window, [&]() {
		shader.Use();

		//Matrix
		//==================
		//Model matrix
		//glm::mat4 model;
		//model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		//View matrix
		GLfloat radius = 10.0f;
		GLfloat camX = sin(glfwGetTime()) * radius;
		GLfloat camZ = cos(glfwGetTime()) * radius;

		cameraPos = glm::vec3(camX, 0.0f, camZ);
		VIEW_MATRIX = glm::lookAt(cameraPos, cameraTarget, cameraUp);

		//Projection matrix
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(FoV), AspectRatio, 0.1f, 100.0f);
		

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TT[0]);
		glUniform1i(glGetUniformLocation(shader.Program, "ourTexture1"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, TT[1]);
		glUniform1i(glGetUniformLocation(shader.Program, "ourTexture2"), 1);

		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(VIEW_MATRIX));

		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		//Draw multi cube with the same vertex and different translate
		for (GLuint i = 0; i < 10; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			GLfloat angle = 20.0f * i;
			if(i % 3 == 0)
				angle = glm::radians((GLfloat)glfwGetTime() * 20.0f) * (i + 1);
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(0);
	});

	Clear(&VAO, &VBO, &EBO);
}

void Lesson11(GLFWwindow *window)
{
	cout << "Lesson11:" << endl;
	//Set Shader
	cout << "Create Shader..." << endl;
	Shader shader("shader3.vs", "shader3.fs");

	//Vertex
	GLfloat vertexs[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {

		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)

	};

	//Input vertex
	cout << "Input vertexInfo" << endl;
	GLuint VAO, VBO, TT[2];
	InputVertexInfo(&VAO, [&]() {
		//=======
		//VBO	
		//=======
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);

		//========
		//TT
		//========
		int width, height;
		for (int i = 0; i < 2; i++)
		{
			//create texture ID
			glGenTextures(1, &TT[i]);
			glBindTexture(GL_TEXTURE_2D, TT[i]);
			//set wrapping
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			//set filter
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			//load img
			unsigned char* image = SOIL_load_image(IMAGE[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);
			//generate
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(GL_TEXTURE_2D);
			//free
			SOIL_free_image_data(image);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		
		//========================
		//VertexAttributeArray
		//========================
		//position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		//texCoord
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
	});

	CAMERA_POS = glm::vec3(0.0f, 0.0f, 3.0f);
	CAMERA_FRONT = glm::vec3(0.0f, 0.0f, -1.0f);
	CAMERA_UP = glm::vec3(0.0f, 1.0f, 0.0f);

	
	//Draw
	cout << "Draw Start..." << endl;
	Draw(window, [&]() {
		//Set DeltaTime
		GLfloat currentFrame = glfwGetTime();
		DELTA_TIME = currentFrame - LAST_FRAME;
		LAST_FRAME = currentFrame;
		
		//Use program
		shader.Use();
		
		//Bind TT
		for (GLint i = 0; i < 2; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, TT[i]);
			stringstream ourTexture;
			ourTexture << "ourTexture" << i + 1;
			//string ourTexture = "ourTexture" + (i + 1) ;
			glUniform1i(glGetUniformLocation(shader.Program, ourTexture.str().c_str()), i);
		}

		//Model matrix

		//View matrix
		
		VIEW_MATRIX = glm::lookAt(CAMERA_POS, CAMERA_POS + CAMERA_FRONT, CAMERA_UP);
		//Projection matrix
		glm::mat4 projection = glm::perspective(glm::radians((GLfloat)Aspect), AspectRatio, 0.1f, 100.0f);


		//Send uniform
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(VIEW_MATRIX));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		//Bind VAO
		glBindVertexArray(VAO);

		//Draw
		for (int i = 0; i < 10; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			GLfloat angle = 20.0f * i;
			if (i % 3 == 0)
				angle = glm::radians((GLfloat)glfwGetTime() * 20.0f) * (i + 1);
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//UnBind VAO
		glBindVertexArray(0);
	});
	cout << "Draw end..." << endl;


	//Clear
	cout << "Clear..." << endl;
	Clear(&VAO, &VBO, nullptr);
}

void Lesson12(GLFWwindow *window, Camera *camera)
{
	cout << "Lesson11:" << endl;
	//Set Shader
	cout << "Create Shader..." << endl;
	Shader shader("shader3.vs", "shader3.fs");

	//Vertex
	GLfloat vertexs[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {

		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)

	};

	//Input vertex
	cout << "Input vertexInfo" << endl;
	GLuint VAO, VBO, TT[2];
	InputVertexInfo(&VAO, [&]() {
		//=======
		//VBO	
		//=======
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexs), vertexs, GL_STATIC_DRAW);

		//========
		//TT
		//========
		int width, height;
		for (int i = 0; i < 2; i++)
		{
			//create texture ID
			glGenTextures(1, &TT[i]);
			glBindTexture(GL_TEXTURE_2D, TT[i]);
			//set wrapping
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			//set filter
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			//load img
			unsigned char* image = SOIL_load_image(IMAGE[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);
			//generate
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			glGenerateMipmap(GL_TEXTURE_2D);
			//free
			SOIL_free_image_data(image);
			glBindTexture(GL_TEXTURE_2D, 0);
		}


		//========================
		//VertexAttributeArray
		//========================
		//position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		//texCoord
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
	});

	//Draw
	cout << "Draw Start..." << endl;
	Draw(window, _camera, [&]() {
		//Set DeltaTime
		GLfloat currentFrame = glfwGetTime();
		DELTA_TIME = currentFrame - LAST_FRAME;
		LAST_FRAME = currentFrame;

		//Use program
		shader.Use();

		//Bind TT
		for (GLint i = 0; i < 2; i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, TT[i]);
			stringstream ourTexture;
			ourTexture << "ourTexture" << i + 1;
			//string ourTexture = "ourTexture" + (i + 1) ;
			glUniform1i(glGetUniformLocation(shader.Program, ourTexture.str().c_str()), i);
		}

		//Model matrix

		//View matrix

		VIEW_MATRIX = camera->GetViewMatrix();
		//Projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), AspectRatio, 0.1f, 100.0f);


		//Send uniform
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(VIEW_MATRIX));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		//Bind VAO
		glBindVertexArray(VAO);

		//Draw
		for (int i = 0; i < 10; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			GLfloat angle = 20.0f * i;
			if (i % 3 == 0)
				angle = glm::radians((GLfloat)glfwGetTime() * 20.0f) * (i + 1);
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//UnBind VAO
		glBindVertexArray(0);
	});
	cout << "Draw end..." << endl;


	//Clear
	cout << "Clear..." << endl;
	Clear(&VAO, &VBO, nullptr);
}

void SetCamera(Camera* &camera, GLFWwindow *window)
{
	camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
	SetMouseCallback(window, camera);
	SetScrollCallback(window, camera);
}

int main()
{
	GLFWwindow* window;
	Ready(window);

	SetCamera(_camera, window);

	//1
	//Lesson1(window);

	//2
	//Lesson2(window, shaderProgram);

	//3
	//Lesson3(window);

	//4
	//Lesson4(window);

	//5
	//Lesson5(window);

	//6
	//Lesson6(window);

	//7
	//Lesson7(window);

	//8
	//Lesson8(window);

	//9
	//Lesson9(window);

	//10
	//Lesson10(window);

	//11
	//Lesson11(window);
	
	//12
	Lesson12(window, _camera);

	return 0;
}

//Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	// Change value of uniform with arrow keys (sets amount of textre mix)
	if (key == GLFW_KEY_RIGHT)
	{
		/*AspectRatio += 0.1f;
		//if (mixValue >= 1.0f)
		//	mixValue = 1.0f;
		cout << "AspectRatio: " << AspectRatio << endl;*/
		VIEW_MATRIX = glm::translate(VIEW_MATRIX, glm::vec3(-1.0f, 0.0f, 0.0f));
	}
	if (key == GLFW_KEY_LEFT)
	{
		/*AspectRatio -= 0.1f;
		//if (mixValue <= 0.0f)
		//	mixValue = 0.0f;
		cout << "AspectRatio: " << AspectRatio << endl;*/
		VIEW_MATRIX = glm::translate(VIEW_MATRIX, glm::vec3(1.0f, 0.0f, 0.0f));
	}

	if (key == GLFW_KEY_UP)
	{
		FoV += 1.0f;
		//if (mixValue >= 1.0f)
		//	mixValue = 1.0f;
		cout << "FoV: " << FoV << endl;
	}
	if (key == GLFW_KEY_DOWN)
	{
		FoV -= 1.0f;
		//if (mixValue <= 0.0f)
		//	mixValue = 0.0f;
		cout << "FoV: " << FoV << endl;
	}

	if (action == GLFW_PRESS)
		KEYS[key] = true;
	else if (action == GLFW_RELEASE)
		KEYS[key] = false;

}

void mouse_callback(GLFWwindow *window, double xPos, double yPos)
{
	if (FIRST_MOUSE)
	{
		lastX = xPos;
		lastY = yPos;
		FIRST_MOUSE = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = yPos - lastY;

	lastX = xPos;
	lastY = yPos;

	xOffset *= _camera->MouseSensitivity;
	yOffset *= _camera->MouseSensitivity;

	_camera->ProcessMouseMovement(xOffset, yOffset, true);
	
}

void scroll_callback(GLFWwindow *window, double xOffset, double yOffset)
{
	_camera->ProcessMouseScroll(yOffset);
}