#ifndef Model3D_h__
#define Model3D_h__

#include <string>

#include <glm\glm.hpp>
#include <vector>
#include <gl\glew.h>

#include "../../middleware/includes/assimp/Importer.hpp"
#include "../../middleware/includes/assimp/scene.h"
#include "../../middleware/includes/assimp/postprocess.h"

#include "TexturedModel.h"
#include "ModelNode.h"
#include "texture.h"
#include "ShaderProgram.h"
class Model3D
{
	//can be considered as the scenegraph's leafs
	std::vector<TexturedModel*> meshes;
	std::vector<Texture*> textures;
	ModelNode* rootNode;

	std::string GetDirectoryPath(std::string sFilePath);
	TexturedModel* convertAIMeshToTexturedModel(aiMesh* aimesh);
	Texture* convertAIMaterialToTexture(aiMaterial* aimaterial, std::string dir);
	ModelNode* convertAINodeToModelNode(aiNode* ainode, ModelNode* parent);
	glm::mat4 aiMat4x4ToglmMat4(aiMatrix4x4 aimat);

public:
	glm::mat4 ModelMatrix;
	std::vector<glm::vec3> AllVertexData;
	Model3D();
	~Model3D();
	bool LoadFromFile(std::string modelFileName, bool flipUVs = true);
	void Initialize();
	void computeVertexData(const glm::mat4 &transformationMatrix);
	void computeVertexData();
	void Render(ShaderProgram* shaderProgram);
	void Render(ShaderProgram* shaderProgram, const glm::mat4 &transformationMatrix);
	void Render(GLuint programID, const glm::mat4 &transformationMatrix);
	void Cleanup();

};
#endif // Model3D_h__

