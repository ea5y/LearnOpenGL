#ifndef MODEL_H
#define MODEL_H

//GLEW
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>

//Math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//SOIL
#include <SOIL/SOIL.h>

#include "Shader.h"
#include "Mesh.h"

using namespace std;

class Model
{
public:
	Model(GLchar* path)
	{
		this->LoadModel(path);
	}
	void Draw(Shader* shader);

private:
	//data
	vector<Mesh> meshes;
	string directory;
	vector<Texture> textures_loaded;

	void LoadModel(string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	GLint TextureFromFile(const char* path, string directory);

};

#endif // !MODEL_H
