#include <gl\glew.h>
#include <gl\glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "shader.hpp"
#include "Model.h"
#include "FirstPersonCamera.h"
#include "texture.h"
#include <math.h>

#pragma once

using namespace glm;

class Snake
{
public:
	GLfloat X_old, Y_old, X_new, Y_new;

	int in_x;
	int in_y;

	mat4 R;
	float angle;
	Snake();
	mat4 turn_Right(int y_Dir);
	mat4 turn_Left(int y_Dir);
	mat4 turn_Up(int x_Dir);
	mat4 turn_Down(int x_Dir);
	~Snake();
};

