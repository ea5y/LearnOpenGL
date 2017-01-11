
#include <iostream>
#include <vector>

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

using namespace std;


float mixValue = 0.4f;

//Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	// Change value of uniform with arrow keys (sets amount of textre mix)
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		mixValue += 0.1f;
		if (mixValue >= 1.0f)
			mixValue = 1.0f;
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		mixValue -= 0.1f;
		if (mixValue <= 0.0f)
			mixValue = 0.0f;
	}
}


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
	GLFWwindow * window = glfwCreateWindow(1280, 720, "LearnOpenGL", nullptr, nullptr);
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

void Ready(GLFWwindow* &window)
{
	InitGF();
	SetGFRequired();

	window = CreateGFwindowObj();

	InitGE();

	DefineViewPort(window);

	SetKeyCallback(window);
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
void Draw(GLFWwindow *window, Func func)
{
	//Loop
	while (!glfwWindowShouldClose(window))
	{
		//Check if any events
		glfwPollEvents();

		//Clear
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

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

	//Model matrix
	glm::mat4 model;
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	//View matrix
	glm::mat4 view;
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	//Projection matrix
	glm::mat4 projection;
	projection = glm::perspective(45.0f, (float)1280 / 720, 0.1f, 100.0f);

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
void Lesson10() 
{
	
}

int main()
{
	GLFWwindow* window;
	Ready(window);

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
	Lesson9(window);

	

	return 0;
}

