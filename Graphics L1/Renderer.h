#ifndef Renderer_h__
#define Renderer_h__
#include <iostream>
#include <gl\glew.h>
#include <gl\glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "shader.hpp"
#include "Model.h"
#include "FirstPersonCamera.h"
#include "texture.h"
#include "ShaderProgram.h"
#include "Model3D.h"
#include "KeyFrameAnimationShader.h"
#include "md2model.h"
#include "Snake.h"
using namespace glm;
using namespace std;

class Renderer
{
	GLuint ProgramID;
	GLuint VertexBufferID;
	GLuint VertexArrayID;
	GLuint ViewProjectionMatrixID;
	GLuint ModelMatrixID;

	FirstPersonCamera* MyCamera;

	Model* MySquare;
	GLfloat ty,tx,r_v;
	char c;
	mat4 ModelMatrix, S, R, T;
	Texture* t1;
	//static modls
	ShaderProgram ​StaticShader;
	Model3D ​StaticModel;
	mat4 ​StaticModelMatrix;

	double PreTime;
	bool up, rig, lef , dow ;
	float tim;
	//md2model
	 KeyFrameAnimationShader ​DynamicShader;
	 CMD2Model ​DynamicModel;
	 animState_t ​DynamicAnimState;
	 mat4 ​DynamicModelMatrix;

	 Snake snake;

public:
	Renderer();
	~Renderer();

	void Initialize(int WindowWidth, int WindowHeight);
	void Draw();
	void HandleKeyboardInput(int Key, int Action);
	void HandleMouseMove(double MouseXPos, double MouseYPos);
	void HandleMouseClick(int MouseAction, int MouseButton, double MouseXPos, double MouseYPos);
	void HandleWindowResize(int WindowWidth, int WindowHeight);
	void Update();
	void Cleanup();
	void colide(GLfloat &, GLfloat &, int);
};

#endif