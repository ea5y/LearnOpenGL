#include "Shader.h"

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	string vertexCode;
	string fragmentCode;
	ifstream vShaderFile;
	ifstream fShaderFile;

	//Ensures ifstream obj can throw exceptions
	vShaderFile.exceptions(ifstream::badbit);
	fShaderFile.exceptions(ifstream::badbit);

	try 
	{
		//Open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		//Read file's buffer contents into streams
		stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		//Close file handles
		vShaderFile.close();
		fShaderFile.close();

		//Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch(ifstream::failure e)
	{
		cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << endl;
	}

	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();

	//Create Shader
	GLuint vertexShader, fragmentShader;
	GLint success;
	GLchar infoLog[512];
	
	//Vertex Shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);
	//Error info
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	}

	//Fragment Shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
	//Error info
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
	}

	//Link Shader
	this->Program = glCreateProgram();
	glAttachShader(this->Program, vertexShader);
	glAttachShader(this->Program, fragmentShader);
	glLinkProgram(this->Program);
	//Error info
	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
	}

	//Delete Shader
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Use() 
{
	glUseProgram(this->Program);
}
