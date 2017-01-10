
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

//Shader
#include "Shader.h"
#include "DrawStart.h"
using namespace std;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

//Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

//Shaders
const GLchar * vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;//point input position variable for config vertex attributes\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";

//Out put color as in put color of fragmentShader
const GLchar * vertexShaderSource1 = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"out vec4 vertexColor;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position, 1.0);\n"
"vertexColor = vec4(0.5f, 0.0f, 0.0f, 1.0f);\n"
"}\0";

//Add a color attribute as out put color
const GLchar * vertexShaderSource2 = "#version 330 core\n"
"layout (location = 0) in vec3 position;//point input position variable for config vertex attributes\n"
"layout (location = 1) in vec3 color;//point input color variable for config vertex attributes\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position, 1.0);\n"
"ourColor = color;\n"
"}\0";

const GLchar * fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

const GLchar * fragmentShaderSource1 = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\n\0";

//Set fragmentShader color with color of vertexShader out put
const GLchar * fragmentShaderSource2 = "#version 330 core\n"
"in vec4 vertexColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vertexColor;\n"
"}\n\0";

//Set fragmentShader color with uniform
const GLchar * fragmentShaderSource3 = "#version 330 core\n"
"out vec4 color;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"color = ourColor;\n"
"}\n\0";

//Set fragmentShader color with color of vertexShader out put which is vertexShader's color attribute
const GLchar * fragmentShaderSource4 = "#version 330 core\n"
"in vec3 ourColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(ourColor, 1.0f);\n"
"}\n\0";

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

GLuint CreateVertexShader(const GLchar **vertexShaderS)
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, vertexShaderS, NULL);
	glCompileShader(vertexShader);

	//Check for compile errors
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return vertexShader;
}

GLuint CreateFragmentShader(const GLchar **fragmentShaderS)
{
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, fragmentShaderS, NULL);
	glCompileShader(fragmentShader);

	//Check for compile errors
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return fragmentShader;
}

GLuint LinkShader(GLuint vertexShader, GLuint fragmentShader)
{
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//Check for linking errors
	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	return shaderProgram;
}

void DeleteShader(GLuint *shaders, GLuint shaderNum)
{
	for (int i = 0; i < shaderNum; i++)
	{
		glDeleteShader(shaders[i]);
	}
}


void DrawStart(GLFWwindow* window, GLuint shaderProgram, GLuint VAO, GLuint* EBO)
{
	//Loop
	while (!glfwWindowShouldClose(window))
	{
		//Check if any events
		glfwPollEvents();

		//Render
		//Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Use shaderProgram
		glUseProgram(shaderProgram);

		//Bind VAO
		glBindVertexArray(VAO);
		//Bind EBO autoly
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		//Draw
		if (EBO != NULL)
		{
			//cout << "*EBO != NULL" << endl;
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		glBindVertexArray(0);

		//Swap the screen buffers
		glfwSwapBuffers(window);
	}
}

void DrawStart3(GLFWwindow* window, GLuint *shaderPrograms, GLuint shaderProgramNum, GLuint *VAOs, GLuint VAOnum, GLuint* EBO)
{
	//Loop
	while (!glfwWindowShouldClose(window))
	{
		//Check if any events
		glfwPollEvents();

		//Render
		//Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		for (int i = 0; i < shaderProgramNum; i++)
		{
			//Use shaderProgram
			glUseProgram(shaderPrograms[i]);

			{
				//Bind VAO
				glBindVertexArray(VAOs[i]);
				//Bind EBO autoly
				//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

				//Draw
				if (EBO != NULL)
				{
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				}
				else
				{
					glDrawArrays(GL_TRIANGLES, 0, 3);
				}
			}
		}


		glBindVertexArray(0);

		//Swap the screen buffers
		glfwSwapBuffers(window);
	}
}

void DrawStart4(GLFWwindow* window, GLuint *shaderProgram, GLuint *VAO, GLuint* EBO)
{
	//Loop
	while (!glfwWindowShouldClose(window))
	{
		//Check if any events
		glfwPollEvents();

		//Render
		//Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Use shaderProgram
		glUseProgram(*shaderProgram);

		//Update color
		GLfloat timeValue = glfwGetTime();
		GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
		GLint vertexColorLocation = glGetUniformLocation(*shaderProgram, "ourColor");

		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		//Bind VAO
		glBindVertexArray(*VAO);
		//Bind EBO autoly
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		//Draw
		if (EBO != NULL)
		{
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		glBindVertexArray(0);

		//Swap the screen buffers
		glfwSwapBuffers(window);
	}
}

//Include color attribute of vertex
void DrawStart(GLFWwindow* window, Shader shader, GLuint *VAO)
{
	GLfloat offset = 0.5f;
	//Loop
	while (!glfwWindowShouldClose(window))
	{
		//Check if any events
		glfwPollEvents();

		//Render
		//Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Use shaderProgram
		shader.Use();

		//Set xOffset through location, translate a parameter as well; 
		glUniform1f(glGetUniformLocation(shader.Program, "xOffset"), offset);

		//Bind VAO
		glBindVertexArray(*VAO);

		//Draw
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);

		//Swap the screen buffers
		glfwSwapBuffers(window);
	}
}

void DrawStart2(GLFWwindow* window, GLuint shaderProgram, GLuint VAOs[2])
{
	//Loop
	while (!glfwWindowShouldClose(window))
	{
		//Check if any events
		glfwPollEvents();

		//Render
		//Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Use shaderProgram
		glUseProgram(shaderProgram);

		for (int i = 0; i < 2; i++)
		{
			//Bind VAO
			glBindVertexArray(VAOs[i]);
			//Draw
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}


		glBindVertexArray(0);

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

void Clear2(GLuint VAOs[], GLuint VBOs[])
{
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
}

void Ready(GLFWwindow* &window)
{
	InitGF();
	SetGFRequired();

	window = CreateGFwindowObj();

	InitGE();

	DefineViewPort(window);
}

#define ELEM_COUNT(a) (sizeof(a)/sizeof(a[0]))

void InputVertexInfo(GLuint *VAO, GLuint *VBO, GLuint *EBO, GLfloat vertices[], GLuint indices[])
{
	//Creae a buffer Array ID
	glGenVertexArrays(1, VAO);
	//Create a buffer ID
	glGenBuffers(1, VBO);

	if (EBO != nullptr)
	{
		//Create a Index buffer ID
		glGenBuffers(1, EBO);
	}

	//Bind the vertex array object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(*VAO);

	//Bind new Buffer
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	//Copy user info to current buffer
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	//cout << ELEM_COUNT(vertices) << endl;
	if (EBO != nullptr)
	{
		//Bind new index Buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
		//Copy index info to current element array buffer

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indices, GL_STATIC_DRAW);

		//cout << "size = " + 6 * sizeof(GLuint) << endl;
	}



	//Tell openGL how to explain vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Unbind
	//Unbind VAO First for saveing EBO, that would bind EBO autoly when bind VAO 
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (EBO != nullptr)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}

//Base only include 1 VAO, 1 VBO
void InputVertexInfo(GLuint *VAO, GLuint *VBO, GLfloat vertices[])
{
	//Creae a buffer Array ID
	glGenVertexArrays(1, VAO);
	//Create a buffer ID
	glGenBuffers(1, VBO);

	//Bind the vertex array object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(*VAO);

	//Bind new Buffer
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	//Copy user info to current buffer
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	
	//Tell openGL how to explain vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Color Attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Unbind
	//Unbind VAO First for saveing EBO, that would bind EBO autoly when bind VAO 
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}



void InputVertexInfo(GLuint *VAO, GLuint *VBO, GLuint *EBO, GLuint vertices[], GLuint verticesNum, GLfloat indices[], GLuint indicesNum, GLuint stride, string imageName)
{
	//Creae a buffer Array ID
	glGenVertexArrays(1, VAO);
	//Create a buffer ID
	glGenBuffers(1, VBO);
	//Create a index buffer
	glGenBuffers(1, EBO);

	//Bind the vertex array object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(*VAO);



	//Load image
	int width, height;
	unsigned char* image = SOIL_load_image(imageName.c_str(), &width, &height, 0, SOIL_LOAD_RGB);


	//Texture
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	//Create Texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);


	//Bind new Buffer
	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	//Copy user info to current buffer
	glBufferData(GL_ARRAY_BUFFER, verticesNum * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	//Bind new index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesNum * sizeof(GLuint), indices, GL_STATIC_DRAW);

	//Tell openGL how to explain vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Color Attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	//Unbind
	//Unbind VAO First for saveing EBO, that would bind EBO autoly when bind VAO 
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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

void InputVertexInfo(GLuint *VAOs, GLuint VAOnum, GLuint *VBOs, GLuint VBOnum, GLfloat **triangles, GLuint triangleL)
{
	glGenVertexArrays(VAOnum, VAOs);
	glGenBuffers(VBOnum, VBOs);

	for (int i = 0; i < VAOnum; i++)
	{
		glBindVertexArray(VAOs[i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
		glBufferData(GL_ARRAY_BUFFER, triangleL * sizeof(GLfloat), triangles[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
		glEnableVertexAttribArray(0);

		printf("Size = %d\n", sizeof(triangles[i]));
	}
}

/*
* Draw the 2 triangles with indices
*/
void Lesson1(GLFWwindow* window)
{
	//Set shader
	GLuint vertextShader = CreateVertexShader(&vertexShaderSource);
	GLuint fragmentShader = CreateFragmentShader(&fragmentShaderSource);

	//Link shader
	GLuint shaderProgram = LinkShader(vertextShader, fragmentShader);

	//Delete shader
	GLuint shaders[2];
	shaders[0] = vertextShader;
	shaders[1] = fragmentShader;
	DeleteShader(shaders, 2);

	//Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[12] = {
		0.5f, 0.5f, 0.0f,   //right up
		0.5f, -0.5f, 0.0f,  //right down

		-0.5f, -0.5f, 0.0f, //left down
		-0.5f, 0.5f, 0.0f   //left up
	};

	GLuint indices[] = {
		0, 1, 3, //first triangles
		1, 2, 3  //second triangles
	};

	GLuint VAO = 1, VBO = 1, EBO = 1;

	cout << "Input VertexInfo..." << endl;
	InputVertexInfo(&VAO, &VBO, &EBO, vertices, indices);

	//Use wireframe mode
	cout << "Use polygon mode" << endl;
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Start
	cout << "Draw start..." << endl;
	DrawStart(window, shaderProgram, VAO, &EBO);
	cout << "Draw end..." << endl;

	//clear
	cout << "Clear..." << endl;
	Clear(&VAO, &VBO, &EBO);

	cout << "Exit!" << endl;
}

/*
* Draw the 2 triangles with 2 different VAOs and 2 different VBOs
*/
void Lesson2(GLFWwindow* window, GLuint shaderProgram)
{
	GLfloat vertices1[] = {
		0.5f, 0.5f, 0.0f,   //right up
		0.5f, -0.5f, 0.0f,  //right down
		0.0f, -0.5f, 0.0f
	};

	GLfloat vertices2[] = {
		-0.5f, 0.5f, 0.0f,   //right up
		-0.5f, -0.5f, 0.0f,
		0.0f, -0.5f, 0.0f
	};

	printf("ssize: %d\n", sizeof(vertices1));

	GLuint VAOs[2], VBOs[2];

	GLfloat *triangles[2];
	triangles[0] = vertices1;
	triangles[1] = vertices2;

	cout << "Input VertexInfo..." << endl;
	InputVertexInfo(VAOs, 2, VBOs, 2, triangles, 9);

	//Use wireframe mode
	cout << "Use polygon mode" << endl;
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Start
	cout << "Draw start..." << endl;
	DrawStart2(window, shaderProgram, VAOs);
	cout << "Draw end..." << endl;

	//clear
	cout << "Clear..." << endl;
	Clear2(VAOs, VBOs);

	cout << "Exit!" << endl;
}

/*
* Draw the 2 triangles with 2 different VAOs and 2 different VBOs, 2 different shaderPrograms and 2 different fragmentShaders
*/
void Lesson3(GLFWwindow* window)
{
	//Set Shader
	GLuint vertexShader = CreateVertexShader(&vertexShaderSource);
	GLuint fragmentShader = CreateFragmentShader(&fragmentShaderSource);
	GLuint fragmentShader1 = CreateFragmentShader(&fragmentShaderSource1);

	//Link Shader
	GLuint shaderProgram = LinkShader(vertexShader, fragmentShader);
	GLuint shaderProgram1 = LinkShader(vertexShader, fragmentShader1);

	GLuint shaders[3];
	shaders[0] = vertexShader;
	shaders[1] = fragmentShader;
	shaders[2] = fragmentShader1;

	//Delete Shader
	DeleteShader(shaders, 3);

	//
	GLfloat vertices1[] = {
		0.5f, 0.5f, 0.0f,   //right up
		0.5f, -0.5f, 0.0f,  //right down
		0.0f, -0.5f, 0.0f
	};

	GLfloat vertices2[] = {
		-0.5f, 0.5f, 0.0f,   //right up
		-0.5f, -0.5f, 0.0f,
		0.0f, -0.5f, 0.0f
	};

	printf("ssize: %d\n", sizeof(vertices1));

	GLuint VAOs[2], VBOs[2];

	GLfloat *triangles[2];
	triangles[0] = vertices1;
	triangles[1] = vertices2;

	cout << "Input VertexInfo..." << endl;
	InputVertexInfo(VAOs, 2, VBOs, 2, triangles, 9);

	//Use wireframe mode
	cout << "Use polygon mode" << endl;
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Start
	GLuint shaderPrograms[2];
	shaderPrograms[0] = shaderProgram;
	shaderPrograms[1] = shaderProgram1;
	cout << "Draw start..." << endl;
	DrawStart3(window, shaderPrograms, 2, VAOs, 2, nullptr);
	cout << "Draw end..." << endl;

	//clear
	cout << "Clear..." << endl;
	Clear2(VAOs, VBOs);

	cout << "Exit!" << endl;
}

/*
*Set trriangle's color through fragmentShader recieve a color parameter from vertexShader
*/
void Lesson4(GLFWwindow * window) 
{
	//Set Shader
	cout << "Set Shader..." << endl;
	GLuint vShader = CreateVertexShader(&vertexShaderSource1);
	GLuint fShader = CreateFragmentShader(&fragmentShaderSource2);

	//Link Shader
	cout << "Link Shader..." << endl;
	GLuint shaderProgram = LinkShader(vShader, fShader);

	//Delete Shader
	cout << "Delete Shader..." << endl;
	GLuint shaders[2];
	shaders[0] = vShader;
	shaders[1] = fShader;
	DeleteShader(shaders, 2);

	GLfloat vertices[] = {
		0.5f, 0.5f, 0.0f,   //right up
		0.5f, -0.5f, 0.0f,  //right down
		0.0f, -0.5f, 0.0f
	};

	GLuint VAO, VBO;
	cout << "Input vertex info" << endl;
	InputVertexInfo(&VAO, &VBO, nullptr, vertices, nullptr);

	//Draw
	cout << "Draw Start..." << endl;
	DrawStart(window, shaderProgram, VAO, nullptr);
	cout << "Draw End..." << endl;

	//Clear
	cout << "Clear..." << endl;
	Clear(&VAO, &VBO, nullptr);

	cout << "Exit!" << endl;
}

/*
 Set fragmentShader color with uniform
*/
void Lesson5(GLFWwindow *window)
{
	GLuint vShader = CreateVertexShader(&vertexShaderSource);
	GLuint fShader = CreateFragmentShader(&fragmentShaderSource3);

	GLuint sP = LinkShader(vShader, fShader);

	GLuint shaders[2];
	shaders[0] = vShader;
	shaders[1] = fShader;
	DeleteShader(shaders, 2);

	GLfloat trriangle[] = {
		-0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f
	};

	GLuint VAO, VBO;
	InputVertexInfo(&VAO, &VBO, nullptr, trriangle, nullptr);

	DrawStart4(window, &sP, &VAO, nullptr);

	Clear(&VAO, &VBO, nullptr);
}

/*
Set fragmentShader color with color of vertexShader out put which is vertexShader's color attribute
*/
void Lesson6(GLFWwindow *window)
{
	GLuint vShader = CreateVertexShader(&vertexShaderSource2);
	GLuint fShader = CreateFragmentShader(&fragmentShaderSource4);

	GLuint sP = LinkShader(vShader, fShader);

	GLuint shaders[2];
	shaders[0] = vShader;
	shaders[1] = fShader;
	DeleteShader(shaders, 2);

	GLfloat trriangle[] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	GLuint VAO, VBO;
	InputVertexInfo(&VAO, &VBO, trriangle);

	DrawStart4(window, &sP, &VAO, nullptr);

	Clear(&VAO, &VBO, nullptr);
}

//Use Shader class
void Lesson7(GLFWwindow* window)
{
	cout << "Lesson7." << endl;
	Shader shader("shader.vs", "shader.fs");

	GLfloat triangle[] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	GLuint VAO, VBO;
	cout << "Input vertexInfo..." << endl;
	InputVertexInfo(&VAO, &VBO, triangle);

	DrawStart(window, shader, &VAO);

	Clear(&VAO, &VBO, nullptr);
}

//Draw with static func with DrawStartC
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
		unsigned char* image2 = SOIL_load_image(imageName2.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		glGenTextures(1, &texture2);
		glBindTexture(GL_TEXTURE_2D, texture2);
		//Creaet Texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
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

		//
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
	Lesson8(window);
	return 0;
}
