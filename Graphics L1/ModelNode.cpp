#include "ModelNode.h"


ModelNode::ModelNode(void)
{
}


ModelNode::~ModelNode(void)
{
}



void ModelNode::Render(GLuint programID, const glm::mat4 &parentTransforms)
{
	glm::mat4 curTransform = parentTransforms * this->transforms;
	//glm::mat4 curTransform = parentTransforms;

	int modelmatID = glGetUniformLocation(programID, "ModelMatrix");
	glUniformMatrix4fv(modelmatID, 1, false, &curTransform[0][0]);
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i]->Draw();
	}
	for (int i = 0; i < children.size(); i++)
	{
		//children[i]->Render(shaderProgram, parentTransforms);
		children[i]->Render(programID, curTransform);
	}
}


void ModelNode::Render(ShaderProgram* shaderProgram, const glm::mat4 &parentTransforms)
{
	glm::mat4 curTransform = parentTransforms * this->transforms;
	//glm::mat4 curTransform = parentTransforms;
	shaderProgram->BindModelMatrix(&curTransform[0][0]);
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i]->Draw();
	}
	for (int i = 0; i < children.size(); i++)
	{
		//children[i]->Render(shaderProgram, parentTransforms);
		children[i]->Render(shaderProgram, curTransform);
	}
}

void ModelNode::TraverseTree(std::vector<glm::vec3> &VertexData, const glm::mat4 &parentTransforms)
{
	glm::mat4 curTransform = parentTransforms * this->transforms;
	//glm::mat4 curTransform = parentTransforms;
	for (int i = 0; i < meshes.size(); i++)
	{
		for (int j = 0; j < meshes[i]->VertexData.size(); j++)
		{
		
			glm::vec4 temp = glm::vec4(meshes[i]->VertexData[j], 1);
			temp = curTransform*temp;
			VertexData.push_back(glm::vec3(temp.x, temp.y, temp.z));
		}
	}
	for (int i = 0; i < children.size(); i++)
	{
		children[i]->TraverseTree(VertexData, curTransform);
	}
}

void ModelNode::TraverseTree(std::vector<glm::vec3> &VertexData)
{
	//glm::mat4 curTransform = parentTransforms;
	for (int i = 0; i < meshes.size(); i++)
	{
		for (int j = 0; j < meshes[i]->VertexData.size(); j++)
		{

			VertexData.push_back(meshes[i]->VertexData[j]);
		}
	}
	for (int i = 0; i < children.size(); i++)
	{
		children[i]->TraverseTree(VertexData);
	}
}

ModelNode* ModelNode::FindNode(std::string name)
{
	// Not Tested.
	if(this->name == name)
		return this;
	ModelNode* node = 0;
	for (int i = 0; i < children.size(); i++)
	{
		if(node = children[i]->FindNode(name))
			return node;
	}
	return node;
}