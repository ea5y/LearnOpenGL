#pragma once

#include <GL/eglew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>

#include "Shader.h"
using namespace std;

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec3 TexCoords;
};

struct Texture
{
	GLuint id;
	string type;
};

class Mesh
{
public:
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;
	
	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> texture);
	void Draw(Shader* shader);

private:
	GLuint VAO, VBO, EBO;
	void SetupMesh();
};
