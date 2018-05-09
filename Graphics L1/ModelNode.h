#ifndef ModelNode_h__
#define ModelNode_h__

#include <vector>
#include <string>
#include "glm/glm.hpp"
#include "gl/glew.h"

#include "TexturedModel.h"
#include "ShaderProgram.h"
#include "texture.h"

class ModelNode
{

public:
	std::string name;
	ModelNode* parent;
	std::vector<ModelNode*> children;
	std::vector<TexturedModel*> meshes;
	//std::vector<Texture> textures;
	glm::mat4 transforms;

	ModelNode();
	~ModelNode();

	void TraverseTree(std::vector<glm::vec3> &VertexData, const glm::mat4 &parentTransforms);
	void ModelNode::TraverseTree(std::vector<glm::vec3> &VertexData);
	void Render(ShaderProgram* shaderProgram, const glm::mat4 &parentTransforms);
	void Render(GLuint programID, const glm::mat4 &parentTransforms);
	ModelNode* FindNode(std::string name);
};

#endif // Model_h__