#include "ShaderProgram.h"

ShaderProgram::ShaderProgram()
{
}

ShaderProgram::~ShaderProgram()
{
}

void ShaderProgram::LoadProgram()
{
	ProgramID = LoadShaders( "model.vertexshader", "model.fragmentshader");

	ModelMatrixID = glGetUniformLocation(ProgramID, "ModelMatrix");
	ViewProjectionMatrixID = glGetUniformLocation(ProgramID, "ViewProjectionMatrix");
}

void ShaderProgram::BindModelMatrix(GLfloat* value)
{
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, value);
}

void ShaderProgram::BindViewProjectionMatrix(GLfloat* value)
{
	glUniformMatrix4fv(ViewProjectionMatrixID, 1, GL_FALSE, value);
}

void ShaderProgram::UseProgram()
{
	glUseProgram(ProgramID);
}

void ShaderProgram::CleanUp()
{
	glDeleteProgram(ProgramID);
}