
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
#include "Camera.h"

#include "Vertex.h"
#include "URL.h"

using namespace std;

const GLuint WIDTH = 1280, HEIGHT = 720;

Camera *_camera;

GLfloat DELTA_TIME = 0.0f;
GLfloat LAST_FRAME = 0.0f;

GLfloat lastX = 640, lastY = 360;

string IMAGE[] = { "Resource/img/muban.jpg" , "Resource/img/mao.jpg" };
bool KEYS[1024];

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
	if (EBO != nullptr)
	{
		glDeleteVertexArrays(1, VAO);
	}
	if (EBO != nullptr)
	{
		glDeleteBuffers(1, VBO);
	}
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
	
	//Input vertex
	cout << "Input vertexInfo" << endl;
	GLuint VAO, VBO, TT[2];
	InputVertexInfo(&VAO, [&]() {
		//=======
		//VBO	
		//=======
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(TEST_COORDS::vertexsPT), TEST_COORDS::vertexsPT, GL_STATIC_DRAW);

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
			model = glm::translate(model, TEST_COORDS::cubePositions[i]);
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


void CreateLightVAO(GLuint* lightVAO, GLuint* VBO, int stride)
{
	//Light VAO
	glGenVertexArrays(1, lightVAO);
	glBindVertexArray(*lightVAO);
	//VBO to Light VAO
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	//Light VAO vertex attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//Unbind
	glBindVertexArray(0);
}


template<typename Func>
void CreateVAO(GLuint* VAO, GLuint* VBO, GLfloat vertexs[], int elemNum, Func func)
{
	//VAO
	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);

	//VBO
	glGenBuffers(1, VBO);
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, elemNum * sizeof(GLfloat), vertexs, GL_STATIC_DRAW);

	func();

	//Unbind
	glBindVertexArray(0);
}

void CreateTexture(GLuint* texture, string imageName)
{
	int width, height;
	//create texture ID
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);
	//set wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	//set filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//load img
	unsigned char* image = SOIL_load_image(imageName.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	//generate
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	//free
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
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
	//lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
	//lightPos.y = sin(glfwGetTime() / 2.0f) - 1.0f;
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

enum MaterialMap {
	DIFFUSE,
	SPECULAR,
	ALL
};

struct MaterialAttribute
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLfloat shininess;
};

struct DirLightAttribute
{
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct PointLightAttribute
{
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLfloat constant;
	GLfloat linear;
	GLfloat quadratic;
};

struct SpotLightAttribute
{
	glm::vec3 position;
	glm::vec3 direction;
	GLfloat cutOff;
	GLfloat outerCutOff;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLfloat constant;
	GLfloat linear;
	GLfloat quadratic;
};


struct LightAttribute
{
	glm::vec3 position;
	glm::vec3 direction;
	GLfloat cutOff;
	GLfloat outerCutOff;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	GLfloat constant;
	GLfloat linear;
	GLfloat quadratic;
};

void TransformLightAttribute(Shader* shader, LightAttribute light)
{
	glUniform3f(glGetUniformLocation(shader->Program, "light.ambient"), light.ambient.x, light.ambient.y, light.ambient.z);
	glUniform3f(glGetUniformLocation(shader->Program, "light.diffuse"), light.diffuse.x, light.diffuse.y, light.diffuse.z);
	glUniform3f(glGetUniformLocation(shader->Program, "light.specular"), light.specular.x, light.specular.y, light.specular.z);
	glUniform3f(glGetUniformLocation(shader->Program, "light.position"), light.position.x, light.position.y, light.position.z);
	glUniform3f(glGetUniformLocation(shader->Program, "light.direction"), light.direction.x, light.direction.y, light.direction.z);
	glUniform1f(glGetUniformLocation(shader->Program, "light.cutOff"), glm::cos(glm::radians(light.cutOff)));
	glUniform1f(glGetUniformLocation(shader->Program, "light.outerCutOff"), glm::cos(glm::radians(light.outerCutOff)));
	glUniform1f(glGetUniformLocation(shader->Program, "light.constant"), light.constant);
	glUniform1f(glGetUniformLocation(shader->Program, "light.linear"), light.linear);
	glUniform1f(glGetUniformLocation(shader->Program, "light.quadratic"), light.quadratic);
}

void TransformDirLightAttribute(Shader* shader, DirLightAttribute light)
{
	glUniform3f(glGetUniformLocation(shader->Program, "dirLight.ambient"), light.ambient.x, light.ambient.y, light.ambient.z);
	glUniform3f(glGetUniformLocation(shader->Program, "dirLight.diffuse"), light.diffuse.x, light.diffuse.y, light.diffuse.z);
	glUniform3f(glGetUniformLocation(shader->Program, "dirLight.specular"), light.specular.x, light.specular.y, light.specular.z);
	glUniform3f(glGetUniformLocation(shader->Program, "dirLight.direction"), light.direction.x, light.direction.y, light.direction.z);
}

void TransformPointLightAttribute(Shader* shader, PointLightAttribute light, int index)
{
	stringstream ambient, diffuse, specular, position, constant, linear, quadratic;
	ambient << "pointLights[" << index << "].ambient";
	diffuse << "pointLights[" << index << "].diffuse";
	specular << "pointLights[" << index << "].specular";
	position << "pointLights[" << index << "].position";
	constant << "pointLights[" << index << "].constant";
	linear << "pointLights[" << index << "].linear";
	quadratic << "pointLights[" << index << "].quadratic";

	//cout << ambient.str().c_str() << endl;

	glUniform3f(glGetUniformLocation(shader->Program, ambient.str().c_str()), light.ambient.x, light.ambient.y, light.ambient.z);
	glUniform3f(glGetUniformLocation(shader->Program, diffuse.str().c_str()), light.diffuse.x, light.diffuse.y, light.diffuse.z);
	glUniform3f(glGetUniformLocation(shader->Program, specular.str().c_str()), light.specular.x, light.specular.y, light.specular.z);
	glUniform3f(glGetUniformLocation(shader->Program, position.str().c_str()), light.position.x, light.position.y, light.position.z);
	glUniform1f(glGetUniformLocation(shader->Program, constant.str().c_str()), light.constant);
	glUniform1f(glGetUniformLocation(shader->Program, linear.str().c_str()), light.linear);
	glUniform1f(glGetUniformLocation(shader->Program, quadratic.str().c_str()), light.quadratic);
}

void TransformSpotLightAttribute(Shader* shader, SpotLightAttribute light)
{
	glUniform3f(glGetUniformLocation(shader->Program, "spotLight.ambient"), light.ambient.x, light.ambient.y, light.ambient.z);
	glUniform3f(glGetUniformLocation(shader->Program, "spotLight.diffuse"), light.diffuse.x, light.diffuse.y, light.diffuse.z);
	glUniform3f(glGetUniformLocation(shader->Program, "spotLight.specular"), light.specular.x, light.specular.y, light.specular.z);
	glUniform3f(glGetUniformLocation(shader->Program, "spotLight.position"), light.position.x, light.position.y, light.position.z);
	glUniform3f(glGetUniformLocation(shader->Program, "spotLight.direction"), light.direction.x, light.direction.y, light.direction.z);
	glUniform1f(glGetUniformLocation(shader->Program, "spotLight.cutOff"), glm::cos(glm::radians(light.cutOff)));
	glUniform1f(glGetUniformLocation(shader->Program, "spotLight.outerCutOff"), glm::cos(glm::radians(light.outerCutOff)));
	glUniform1f(glGetUniformLocation(shader->Program, "spotLight.constant"), light.constant);
	glUniform1f(glGetUniformLocation(shader->Program, "spotLight.linear"), light.linear);
	glUniform1f(glGetUniformLocation(shader->Program, "spotLight.quadratic"), light.quadratic);
}

void TransformMaterialAttribute(Shader* shader, MaterialMap map, MaterialAttribute material)
{
	if (map == DIFFUSE)
	{
		glUniform3f(glGetUniformLocation(shader->Program, "material.specular"), material.specular.x, material.specular.y, material.specular.z);
	}

	if (map == SPECULAR)
	{
		glUniform3f(glGetUniformLocation(shader->Program, "material.diffuse"), material.diffuse.x, material.diffuse.y, material.diffuse.z);
	}

	if (map == ALL)
		;
	//glUniform3f(glGetUniformLocation(shader->Program, "material.ambient"), 0.0f, 0.1f, 0.06f);
	//glUniform3f(glGetUniformLocation(shader->Program, "material.diffuse"), 0.0f, 0.50980392f, 0.50980392f);
	//glUniform3f(glGetUniformLocation(shader->Program, "material.specular"), 0.0f, 0.0f, 0.0f);
	glUniform1f(glGetUniformLocation(shader->Program, "material.shininess"), material.shininess);
}

void TransformMatrix(Shader* shader, glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
	//Model
	TransformMatrixUniform(shader, model, MODEL);
	//View
	TransformMatrixUniform(shader, view, VIEW);
	//Projection
	TransformMatrixUniform(shader, projection, PROJECTION);
}

void TransformViewPos(Shader* shader, Camera* camera)
{
	glUniform3f(glGetUniformLocation(shader->Program, "viewPos"), camera->Position.x, camera->Position.y, camera->Position.z);
}

void TransformTexture(Shader* shader, GLuint* texture, GLuint index, string location)
{
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, *texture);
	glUniform1i(glGetUniformLocation(shader->Program, location.c_str()), index);
}

void RealDraw(Shader* shader, GLuint* VAO,GLuint objNum, glm::vec3 cubePos[])
{
	glBindVertexArray(*VAO);
	for (GLuint i = 0; i < objNum; i++)
	{
		glm::mat4 model = glm::mat4();
		model = glm::translate(model, cubePos[i]);
		GLfloat angle = 20.0f * (i + 1);

		if (i % 3 == 0)
		{
			angle = (GLfloat)glfwGetTime() * angle;

		}
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		TransformMatrixUniform(shader, model, MODEL);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	glBindVertexArray(0);
}

void CreateLight(Shader* lightShader, Camera* camera, glm::mat4 projection, GLuint* LightVAO, GLuint objNum, glm::vec3 lightPos[])
{
	//Use
	lightShader->Use();

	//Transform matrix
	glm::mat4 model;
	TransformMatrix(lightShader, model, camera->GetViewMatrix(), projection);


	//Draw
	glBindVertexArray(*LightVAO);
	for (GLuint i = 0; i < objNum; i++)
	{
		model = glm::mat4();
		model = glm::translate(model, lightPos[i]);
		model = glm::scale(model, glm::vec3(0.2f));
		TransformMatrixUniform(lightShader, model, MODEL);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindVertexArray(0);
}

template<typename Func>
void CreateGameObject(Shader* shader, Camera* camera, glm::mat4 projection, GLuint* VAO, GLuint objNum, glm::vec3 cubePos[], Func func)
{
	//Use
	shader->Use();
	//viewPos
	TransformViewPos(shader, camera);

	//Transform
	//=============
	//matrix
	glm::mat4 model;
	TransformMatrix(shader, model, camera->GetViewMatrix(), projection);

	//material attribute,light attribute,texture
	func();
		
	//Draw
	RealDraw(shader, VAO, objNum, cubePos);
}

void SetDeltaTime()
{
	GLfloat currentFrame = glfwGetTime();
	DELTA_TIME = currentFrame - LAST_FRAME;
	LAST_FRAME = currentFrame;
}

void Lesson14(GLFWwindow* window, Camera* camera)
{
	//Shader
	Shader shader("shader5.vs", "shader5.fs");
	Shader lightShader("shader5.vs", "shader4-1.fs");
	//VAO VBO
	GLuint VAO, VBO, LightVAO;
	CreateVAO(&VAO, &VBO, TEST_COORDS::vertexsPNT, 48 * 6, []() {
		//VAP
		//=======
		//position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		//normal
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		//texCoords
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
	});
	CreateLightVAO(&LightVAO, &VBO, 8);
	
	//TT
	GLuint diffuseMap, specularMap;
	CreateTexture(&diffuseMap, "Resource/img/muban2.png");
	CreateTexture(&specularMap, "Resource/img/muban2_specular.png");

	//Projection
	glm::mat4 projection = glm::perspective(camera->Zoom, (GLfloat)WIDTH / HEIGHT, 0.1f, 100.0f);

	Draw(window, camera, [&]() {
		//Set deltaTime
		SetDeltaTime();

		//Create light
		glm::vec3 lightPos = CreateLight(&lightShader, camera, projection, &LightVAO);

		MaterialAttribute material;
		material.shininess = 32.0f;

		LightAttribute light;
		light.ambient = glm::vec3(0.1f);
		light.diffuse = glm::vec3(0.5f);
		light.specular = glm::vec3(1.0f);
		light.position = lightPos;
		
		light.constant = 1.0f;
		light.linear = 0.09f;
		light.quadratic = 0.032f;

		//Create gameObject
		CreateGameObject(&shader, camera, projection, &VAO, 1, TEST_COORDS::cubePositions, [&]() {
			//material
			TransformMaterialAttribute(&shader, ALL, material);
			//light
			TransformLightAttribute(&shader, light);
			//texture
			TransformTexture(&shader, &diffuseMap, 0, "material.diffuse");
			TransformTexture(&shader, &specularMap, 1, "material.specular");
		});
	});
}

//Direction light
void Lesson15(GLFWwindow* window, Camera* camera)
{
	//Shader
	Shader shader("shader-direction-light.vs", "shader-direction-light.fs");

	//VAO VBO
	GLuint VAO, VBO;
	CreateVAO(&VAO, &VBO, TEST_COORDS::vertexsPNT, 48 * 6, []() {
		//VAP
		//=======
		//position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		//normal
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		//texCoords
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
	});

	//TT
	GLuint diffMap, specularMap;
	CreateTexture(&diffMap, "Resource/img/muban2.png");
	CreateTexture(&specularMap, "Resource/img/muban2_specular.png");

	//Projection
	glm::mat4 projection = glm::perspective(camera->Zoom, (GLfloat)WIDTH / HEIGHT, 0.1f, 100.0f);

	//Draw
	Draw(window, camera, [&]() {
		//Set deltaTime
		SetDeltaTime();

		MaterialAttribute material;
		material.shininess = 32.0f;

		LightAttribute light;
		light.ambient = glm::vec3(0.1f);
		light.diffuse = glm::vec3(0.5f);
		light.specular = glm::vec3(1.0f);
		light.direction = glm::vec3(0.5f);
		light.constant = 1.0f;
		light.linear = 0.09f;
		light.quadratic = 0.032f;

		//GameObject
		CreateGameObject(&shader, camera, projection, &VAO, 10, TEST_COORDS::cubePositions, [&]() {
			//material
			TransformMaterialAttribute(&shader, ALL, material);
			//light
			TransformLightAttribute(&shader, light);
			//texture
			TransformTexture(&shader, &diffMap, 0, "material.diffuse");
			TransformTexture(&shader, &specularMap, 1, "material.specular");
		});
	});

	Clear(&VAO, &VBO, nullptr);
}

//Point light
void Lesson16(GLFWwindow* window, Camera* camera)
{
	
	//GameObject shader
	Shader gameObjShader("shader-point-light.vs", "shader-point-light.fs");
	//Light shader
	Shader lightShader("shader-point-light.vs", "shader4-1.fs");

	//VAO VBO
	GLuint VAO, VBO;
	CreateVAO(&VAO, &VBO, TEST_COORDS::vertexsPNT, 48 * 6, [&]() {
		//VAP 
		//=======
		//position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		//normal
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		//texCoord
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
	});

	//Light VAO
	GLuint LightVAO;
	CreateLightVAO(&LightVAO, &VBO, 8);
	
	//TT
	GLuint diffuseMap, specularMap;
	CreateTexture(&diffuseMap, "Resource/img/muban2.png");
	CreateTexture(&specularMap, "Resource/img/muban2_specular.png");

	//Projection
	glm::mat4 projection = glm::perspective(camera->Zoom, (GLfloat)WIDTH / HEIGHT, 0.1f, 100.0f);
	

	//Draw
	Draw(window, camera, [&]() {
		//Set deltaTime
		SetDeltaTime();

		//Create Light
		auto lightPos = CreateLight(&lightShader, camera, projection, &LightVAO);

		//Config material
		MaterialAttribute material;
		material.shininess = 32.0f;

		//Config light
		LightAttribute light;
		light.ambient = glm::vec3(0.2f);
		light.diffuse = glm::vec3(0.5f);
		light.specular = glm::vec3(1.0f);
		light.position = lightPos;
		light.constant = 1.0f;
		light.linear = 0.09f;
		light.quadratic = 0.032f;

		//Create GameObject
		CreateGameObject(&gameObjShader, camera, projection, &VAO, 10, TEST_COORDS::cubePositions, [&]() {
			//Transform
			//=============
			//material
			TransformMaterialAttribute(&gameObjShader, ALL, material);

			//light
			TransformLightAttribute(&gameObjShader, light);

			//texture
			TransformTexture(&gameObjShader, &diffuseMap, 0, "material.diffuse");
			TransformTexture(&gameObjShader, &specularMap, 1, "material.specular");
		});
	});

	//Clear
	Clear(&VAO, &VBO, nullptr);
	Clear(&LightVAO, nullptr, nullptr);
}

//Spotlight
void Lesson17(GLFWwindow* window, Camera* camera)
{
	//Shader
	Shader gameObjShader("shader-spot-light.vs", "shader-spot-light.fs");
	Shader lightShader("shader-spot-light.vs", "shader4-1.fs");

	//VAO VBO
	GLuint VAO, VBO;
	CreateVAO(&VAO, &VBO, TEST_COORDS::vertexsPNT, 48 * 6, []() {
		//VAP 
		//=======
		//position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		//normal
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		//texCoord
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
	});

	//LightVAO
	GLuint LightVAO;
	CreateLightVAO(&LightVAO, &VBO, 8);

	//TT
	GLuint diffuseMap, specularMap;
	CreateTexture(&diffuseMap, "Resource/img/muban2.png");
	CreateTexture(&specularMap, "Resource/img/muban2_specular.png");

	//Projection
	glm::mat4 projection = glm::perspective(camera->Zoom, (GLfloat)WIDTH / HEIGHT, 0.1f, 100.0f);

	//Draw
	Draw(window, camera, [&]() {
		//Set deltaTime
		SetDeltaTime();

		//Create Light
		auto lightPos = CreateLight(&lightShader, camera, projection, &LightVAO);

		//Config material
		MaterialAttribute material;
		material.shininess = 32.0f;

		//Config light
		LightAttribute light;
		light.ambient = glm::vec3(0.2f);
		light.diffuse = glm::vec3(0.5f);
		light.specular = glm::vec3(1.0f);
		light.position = camera->Position;
		light.direction = camera->Front;
		light.cutOff = 12.5f;
		light.outerCutOff = 17.5f;
		light.constant = 1.0f;
		light.linear = 0.09f;
		light.quadratic = 0.032f;

		//Create GameObject
		CreateGameObject(&gameObjShader, camera, projection, &VAO, 10, TEST_COORDS::cubePositions, [&]() {
			//Transform
			//=============
			//material
			TransformMaterialAttribute(&gameObjShader, ALL, material);

			//light
			TransformLightAttribute(&gameObjShader, light);

			//texture
			TransformTexture(&gameObjShader, &diffuseMap, 0, "material.diffuse");
			TransformTexture(&gameObjShader, &specularMap, 1, "material.specular");
		});
	});

	//Clear
	Clear(&VAO, &VBO, nullptr);
	Clear(&LightVAO, nullptr, nullptr);
}

glm::mat4 CreateProjection(Camera* camera)
{
	glm::mat4 projection = glm::perspective(camera->Zoom, (GLfloat)WIDTH / HEIGHT, 0.1f, 100.0f);
	return projection;
}

DirLightAttribute ConfigDirLight(glm::vec3 ambient = glm::vec3(0.2f), glm::vec3 diffuse = glm::vec3(0.5f), glm::vec3 specular = glm::vec3(1.0f), glm::vec3 direction = glm::vec3(-0.2f, -0.5f, -0.3f))
{
	DirLightAttribute dirLight;
	dirLight.ambient = ambient;
	dirLight.diffuse = diffuse;
	dirLight.specular = specular;
	dirLight.direction = direction;
	return dirLight;
}

PointLightAttribute ConfigPointLight(glm::vec3 position)
{
	PointLightAttribute pointLight;
	pointLight.ambient = glm::vec3(0.2f);
	pointLight.diffuse = glm::vec3(0.3f);
	pointLight.specular = glm::vec3(0.5f);
	pointLight.position = position;
	pointLight.constant = 1.0f;
	pointLight.linear = 0.09f;
	pointLight.quadratic = 0.032f;
	return pointLight;
}

SpotLightAttribute ConfigSpotLight(glm::vec3 position, glm::vec3 direction)
{
	SpotLightAttribute spotLight;
	spotLight.ambient = glm::vec3(0.2f);
	spotLight.diffuse = glm::vec3(1.0f);
	spotLight.specular = glm::vec3(1.0f);
	spotLight.position = position;
	spotLight.direction = direction;
	spotLight.cutOff = 8.5f;
	spotLight.outerCutOff = 10.5f;
	spotLight.constant = 1.0f;
	spotLight.linear = 0.09f;
	spotLight.quadratic = 0.032f;
	return spotLight;
}

//Multi Light
void Lesson18(GLFWwindow* window, Camera* camera)
{
	//Shader
	Shader gameShader("shader-multi-light.vs", "shader-multi-light.fs");
	Shader lightShader("shader-multi-light.vs", "shader4-1.fs");

	//VAO VBO
	GLuint VAO, VBO;
	CreateVAO(&VAO, &VBO, TEST_COORDS::vertexsPNT, 48 * 6, []() {
		//VAP
		//========
		//position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		//normal
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		//texCoords
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
	});

	//Light VAO
	GLuint LightVAO;
	CreateLightVAO(&LightVAO, &VBO, 8);

	//TT
	GLuint diffuseMap, specularMap;
	CreateTexture(&diffuseMap, URL::GetMuban2_PNG());
	CreateTexture(&specularMap, URL::GetMuban2_Specular_PNG());

	//Projection
	glm::mat4 projection = CreateProjection(camera);

	
	//Draw
	Draw(window, camera, [&]() {
		//Set deltaTime
		SetDeltaTime();

		//Create light
		CreateLight(&lightShader, camera, projection, &LightVAO, 4, TEST_COORDS::pointLightPositions);

		//Config Material and Light
		MaterialAttribute material;
		material.shininess = 32.0f;

		DirLightAttribute dirLight = ConfigDirLight();

		PointLightAttribute pointLights[4];
		for (int i = 0; i < 4; i++)
		{
			PointLightAttribute pointLight = ConfigPointLight(TEST_COORDS::pointLightPositions[i]);
			pointLights[i] = pointLight;
		}

		SpotLightAttribute spotLight = ConfigSpotLight(camera->Position, camera->Front);

		//Create gameObject
		CreateGameObject(&gameShader, camera, projection, &VAO, 10, TEST_COORDS::cubePositions, [&]() {
			//Transform
			//============
			//material
			TransformMaterialAttribute(&gameShader, ALL, material);

			//light
			TransformDirLightAttribute(&gameShader, dirLight);
			for (int i = 0; i < 4; i++)
			{
				TransformPointLightAttribute(&gameShader, pointLights[i], i);
			}
			TransformSpotLightAttribute(&gameShader, spotLight);
			

			//texture
			TransformTexture(&gameShader, &diffuseMap, 0, "material.diffuse");
			TransformTexture(&gameShader, &specularMap, 1, "material.specular");
		});
	});

	//Clear
	Clear(&VAO, &VBO, nullptr);
	Clear(&LightVAO, nullptr, nullptr);
}

int main()
{
	GLFWwindow* window;
	Ready(window);

	SetCamera(_camera, window);
	
	//12
	//Lesson12(window, _camera);

	//13
	//Lesson13(window, _camera);

	//14
	//Lesson14(window, _camera);

	//15
	//Lesson15(window, _camera);

	//16
	//Lesson16(window, _camera);

	//17
	//Lesson17(window, _camera);

	//18
	Lesson18(window, _camera);
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