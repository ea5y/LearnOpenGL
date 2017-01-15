
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

GLfloat DELTA_TIME = 0.0f;
GLfloat LAST_FRAME = 0.0f;

GLfloat lastX = 640, lastY = 360;

string IMAGE[] = { "Resource/img/muban.jpg" , "Resource/img/mao.jpg" };
bool KEYS[1024];

bool FIRST_MOUSE = true;

//LightColor


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
void InputVertexInfo(GLuint *VAO, GLuint* LightVAO, Func func)
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
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
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
			glUniform1i(glGetUniformLocation(shader.Program, ourTexture.str().c_str()), i);
		}

		//Projection matrix
		glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (GLfloat) WIDTH/ HEIGHT, 0.1f, 100.0f);

		//Send uniform
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
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

void CreateLightVAO(GLuint* lightVAO, GLuint* VBO)
{
	//Light VAO
	glGenVertexArrays(1, lightVAO);
	glBindVertexArray(*lightVAO);
	//VBO to Light VAO
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	//Light VAO vertex attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//Unbind
	glBindVertexArray(0);
}

void CreateVAO(GLuint* VAO, GLuint* VBO, GLfloat vertexs[], int vertexNum)
{
	//VAO
	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);

	//VBO
	glGenBuffers(1, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexNum * sizeof(GLfloat), vertexs, GL_STATIC_DRAW);

	//VAP position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//VAP normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Unbind
	glBindVertexArray(0);
}

GLuint* CreateTexture(GLuint texture[], GLuint textureNum)
{
	int width, height;
	for (int i = 0; i < textureNum; i++)
	{		
		//TT
		glGenTextures(1, &texture[i]);
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		//Set wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//Set filter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//Load img
		unsigned char* image = SOIL_load_image(IMAGE[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		//Generate
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateTextureMipmap(0);
		//Free
		SOIL_free_image_data(image);
		//Unbind
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	return texture;
}

enum MATRIX
{
	MODEL,
	VIEW,
	PROJECTION
};

void TransformMatrixUniform(Shader *shader, glm::mat4 matrix, MATRIX matrixType)
{
	if(matrixType == MODEL)
		glUniformMatrix4fv(glGetUniformLocation(shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(matrix));
	if (matrixType == VIEW)
		glUniformMatrix4fv(glGetUniformLocation(shader->Program, "view"), 1, GL_FALSE, glm::value_ptr(matrix));
	if (matrixType == PROJECTION)
		glUniformMatrix4fv(glGetUniformLocation(shader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(matrix));
}

glm::vec3 CreateLight(Shader* lightShader, Camera* camera, glm::mat4 projection, GLuint* LightVAO)
{
	//Light pos
	glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);

	//Use
	lightShader->Use();
	
	//Transform uniform
	//Model
	glm::mat4 model;
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.2f));
	TransformMatrixUniform(lightShader, model, MODEL);
	//View
	TransformMatrixUniform(lightShader, camera->GetViewMatrix(), VIEW);
	//Projection
	TransformMatrixUniform(lightShader, projection, PROJECTION);
	
	//Draw
	glBindVertexArray(*LightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	return lightPos;
}

void CreateGameObject(Shader* shader, Camera* camera, glm::mat4 projection, GLuint* VAO, glm::vec3 lightPos)
{
	//Use
	shader->Use();

	//Transform uniform
	//Model
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	TransformMatrixUniform(shader, model, MODEL);
	//View
	TransformMatrixUniform(shader, camera->GetViewMatrix(), VIEW);
	//Projection
	TransformMatrixUniform(shader, projection, PROJECTION);

	//Transform color
	glUniform3f(glGetUniformLocation(shader->Program, "objectColor"), 1.0f, 0.5f, 0.31f);
	glUniform3f(glGetUniformLocation(shader->Program, "lightColor"), 1.0f, 1.0f, 1.0f);

	//
	glUniform3f(glGetUniformLocation(shader->Program, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	//
	glUniform3f(glGetUniformLocation(shader->Program, "viewPos"), camera->Position.x, camera->Position.y, camera->Position.z);

	//Draw
	glBindVertexArray(*VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void SetDeltaTime()
{
	GLfloat currentFrame = glfwGetTime();
	DELTA_TIME = currentFrame - LAST_FRAME;
	LAST_FRAME = currentFrame;
}

void Lesson13(GLFWwindow* window, Camera* camera)
{
	//Vertex
	GLfloat vertexs[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	//Shader
	Shader shader("shader4.vs", "shader4.fs");
	Shader lightShader("shader4.vs", "shader4-1.fs");
	//VAO VBO
	GLuint VAO, VBO, LightVAO;
	CreateVAO(&VAO, &VBO, vertexs, 36 * 6);
	CreateLightVAO(&LightVAO, &VBO);
	//TT@Null
	
	glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (GLfloat)WIDTH / HEIGHT, 0.1f, 100.0f);

	//Draw
	Draw(window, camera, [&]() {
		//Set DeltaTime
		SetDeltaTime();

		//Light
		glm::vec3 lightPos = CreateLight(&lightShader, camera, projection, &LightVAO);

		//GameObject
		CreateGameObject(&shader, camera, projection, &VAO, lightPos);
		
	});

	//Clear
	Clear(&VAO, &VBO);
}

int main()
{
	GLFWwindow* window;
	Ready(window);

	SetCamera(_camera, window);
	
	//12
	//Lesson12(window, _camera);

	//13
	Lesson13(window, _camera);

	return 0;
}

//Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	
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