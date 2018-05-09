#include "KeyFrameAnimationShader.h"


KeyFrameAnimationShader::KeyFrameAnimationShader()
{
}


KeyFrameAnimationShader::~KeyFrameAnimationShader()
{
}

void KeyFrameAnimationShader::LoadProgram()
{
	ProgramID = LoadShaders( "KeyframeAnimation.vertexshader", "TextureLighting.fragmentshader");

	ModelMatrixID = glGetUniformLocation(ProgramID, "ModelMatrix");

	ViewProjectionMatrixID = glGetUniformLocation(ProgramID, "ViewProjectionMatrix");

	interpolationID = glGetUniformLocation(ProgramID, "Interpolation");
}

void KeyFrameAnimationShader::SetInterpolation(float interpolation)
{
	glUniform1f(interpolationID,interpolation);
}
