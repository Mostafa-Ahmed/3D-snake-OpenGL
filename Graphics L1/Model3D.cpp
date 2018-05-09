#include "Model3D.h"

Model3D::Model3D(void)
{
}

Model3D::~Model3D(void)
{
}

glm::mat4 Model3D::aiMat4x4ToglmMat4(aiMatrix4x4 aimat)
{
	glm::mat4 gmat;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			// Transpose matrix, as aiMatrix4x4 is column major while opengl is row major.
			gmat[i][j] = aimat[j][i];
		}
	}
	return gmat;
}

std::string Model3D::GetDirectoryPath(std::string sFilePath)
{
	std::string sDirectory = "";
	for (int i = sFilePath.length(); i >= 0; i--)
	{
		if(sFilePath[i] == '\\' || sFilePath[i] == '/')
		{
			sDirectory = sFilePath.substr(0, i+1);
			break;
		}
	}
	return sDirectory;
}

bool Model3D::LoadFromFile(std::string modelFileName, bool flipUVs)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(modelFileName.c_str(),
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		(flipUVs?aiProcess_FlipUVs:0));

	if(!scene){
		printf("ERROR LOADING MODEL: %s\n", importer.GetErrorString());
		return false;
	}

	std::string modelDir = GetDirectoryPath(modelFileName);
	for (int i = 0; i < scene->mNumMaterials; i++)
	{
		textures.push_back(convertAIMaterialToTexture(scene->mMaterials[i], modelDir));
	}

	for (int i = 0; i < scene->mNumMeshes; i++)
	{
		meshes.push_back(convertAIMeshToTexturedModel(scene->mMeshes[i]));
	}

	rootNode = convertAINodeToModelNode(scene->mRootNode, 0);
}

Texture* Model3D::convertAIMaterialToTexture(aiMaterial* aimaterial, std::string dir)
{
	Texture* tex = 0;
	aiString path;
	if(aimaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
	{
		std::string texturePath = dir + path.data;
		printf("Loading Texture: %s\n", texturePath.c_str());
		tex = new Texture(texturePath, 0);
	}
	//if(!tex || !(tex->loaded))
	//{
	//	// handle default texture.
	//	printf("Error Loading Texture. Using default texture.\n");
	//	tex = new Texture("data\\textures\\default.png", 0);
	//}
	return tex;
}

TexturedModel* Model3D::convertAIMeshToTexturedModel(aiMesh* aimesh)
{
	TexturedModel* mesh = new TexturedModel();
	for (int i = 0; i < aimesh->mNumVertices; i++)
	{
		aiVector3D vv = aimesh->mVertices[i];
		mesh->VertexData.push_back(glm::vec3(vv.x, vv.y, vv.z));
		AllVertexData.push_back(glm::vec3(vv.x, vv.y, vv.z));
		if(aimesh->GetNumUVChannels() > 0){
			aiVector3D uvv = aimesh->mTextureCoords[0][i];
			mesh->UVData.push_back(glm::vec2(uvv.x, uvv.y));
		}

		aiVector3D nv = aimesh->mNormals[i];
		mesh->NormalsData.push_back(glm::vec3(nv.x, nv.y, nv.z));
	}

	for (int i = 0; i < aimesh->mNumFaces; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			mesh->IndicesData.push_back(aimesh->mFaces[i].mIndices[j]);
		}
	}

	mesh->texture = textures[aimesh->mMaterialIndex];

	return mesh;
}

void Model3D::computeVertexData(const glm::mat4 &transformationMatrix)
{
	AllVertexData.clear();
	rootNode->TraverseTree(AllVertexData, transformationMatrix);
}

void Model3D::computeVertexData()
{
	AllVertexData.clear();
	rootNode->TraverseTree(AllVertexData);
}


ModelNode* Model3D::convertAINodeToModelNode(aiNode* ainode, ModelNode* parent)
{
	ModelNode* node = new ModelNode();
	node->name = ainode->mName.data;
	node->parent = parent;
	node->transforms = aiMat4x4ToglmMat4(ainode->mTransformation);
	for (int i = 0; i < ainode->mNumMeshes; i++)
	{
		node->meshes.push_back(meshes[ainode->mMeshes[i]]);
	}
	for (int i = 0; i < ainode->mNumChildren; i++)
	{
		node->children.push_back(convertAINodeToModelNode(ainode->mChildren[i], node));
	}
	return node;
}

void Model3D::Initialize()
{
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i]->Initialize();
	}
}

void Model3D::Render(ShaderProgram* shaderProgram, const glm::mat4 &transformationMatrix)
{
	rootNode->Render(shaderProgram, transformationMatrix);

	// Render meshes ignoring nodes structure
	/*for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i]->Render(graphicsDevice);
	}*/
}


void Model3D::Render(GLuint programID, const glm::mat4 &transformationMatrix)
{
	rootNode->Render(programID, transformationMatrix);

	// Render meshes ignoring nodes structure
	/*for (int i = 0; i < meshes.size(); i++)
	{
	meshes[i]->Render(graphicsDevice);
	}*/
}

void Model3D::Render(ShaderProgram* shaderProgram)
{
	rootNode->Render(shaderProgram, glm::mat4(1.0f));
}

void Model3D::Cleanup()
{
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i]->Cleanup();
	}
	for (int i = 0; i < textures.size(); i++)
	{
		delete textures[i];
	}
}
