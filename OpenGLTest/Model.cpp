#include "Model.h"

void Model::Draw(Shader* shader)
{
	for (GLuint i = 0; i < this->meshes.size(); i++)
		this->meshes[i].Draw(shader);
}

void Model::LoadModel(string path)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
		return;
	}
	this->directory = path.substr(0, path.find_last_of('/'));

	this->ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	//Add all mesh of current node
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		//get mesh through meshe's index
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		this->meshes.push_back(this->ProcessMesh(mesh, scene));
	}

	//deal child node
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		this->ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	//deal vertex
	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		//deal position, normal, texCoords
		//@TODO

		vertices.push_back(vertex);
	}

	//deal indices

	//deal material


	return Mesh(vertices, indices, textures);
}