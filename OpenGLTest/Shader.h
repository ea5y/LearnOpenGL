#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

#include <GL/glew.h>

class Shader 
{
public:
	//Program ID
	GLuint Program;
	//Constructor
	Shader(const GLchar *vertexSourcePath, const GLchar *fragmentSourcePath);
	//Use Program
	void Use();
};

#endif
