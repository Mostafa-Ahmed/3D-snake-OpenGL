#ifndef GraphicsDevice_h__
#define GraphicsDevice_h__

#include "gl/glew.h"
#include "glm/glm.hpp"
#include "shader.hpp"


class ShaderProgram
{
public:
	GLuint ProgramID;

	GLuint ModelMatrixID;
	GLuint ViewProjectionMatrixID;

	ShaderProgram();
	~ShaderProgram();

	void BindModelMatrix(GLfloat* value);
	void BindViewProjectionMatrix(GLfloat* value);

	//calls glUseProgram.
	void UseProgram();
	//load shaders from files.
	void LoadProgram();
	void CleanUp();
};

#endif // GraphicsDevice_h__