#include "FirstPersonCamera.h"


FirstPersonCamera::FirstPersonCamera()
{
	this->SetLookAt(0, 0, 1,
		0, 0, 0,
		0, 1, 0);

	SetPerspectiveProjection(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
}


FirstPersonCamera::~FirstPersonCamera()
{
}

vec3 FirstPersonCamera::GetLookDirection()
{
	return -mDirection;
}

void FirstPersonCamera::SetLookAt(float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ)
{
	vec3 eyePt(eyeX, eyeY, eyeZ);
	vec3 centerPt(centerX, centerY, centerZ);
	vec3 upVec(upX, upY, upZ);

	this->SetLookAt(eyePt, centerPt, upVec);
}

void FirstPersonCamera::SetLookAt(const vec3 &eye, const vec3 &center, vec3 &up)
{
	mPosition = eye;

	mDirection = normalize(mPosition - center);
	mRight = normalize(cross(up, mDirection));
	mUp = normalize(cross(mDirection, mRight));
}

mat4 FirstPersonCamera::GetViewMatrix()
{
	vec3 mCenter = mPosition + this->GetLookDirection();
	return lookAt(mPosition, mCenter, mUp);
}

mat4 FirstPersonCamera::GetProjectionMatrix()
{
	return mProjectionMatrix;
}

void FirstPersonCamera::SetPerspectiveProjection(float FOV, float aspectRatio, float near, float far)
{
	mProjectionMatrix = perspective(FOV, aspectRatio, near, far);
}

void FirstPersonCamera::Slide(float stepR, float stepU, float stepD)
{
	mPosition += stepR * mRight;
	mPosition += stepU * mUp;
	mPosition += stepD * this->GetLookDirection();
}

void FirstPersonCamera::Yaw(float angleDegrees)
{
	mDirection = glm::rotate(mDirection, angleDegrees, mUp);
	mRight = glm::rotate(mRight, angleDegrees, mUp);
}

void FirstPersonCamera::Pitch(float angleDegrees)
{
	mUp = glm::rotate(mUp, angleDegrees, mRight);
	mDirection = glm::rotate(mDirection, angleDegrees, mRight);
}

void FirstPersonCamera::Roll(float angleDegrees)
{
	mRight = glm::rotateZ(mRight, angleDegrees);
	mDirection = glm::rotateZ(mDirection, angleDegrees);
	mUp = glm::rotateZ(mUp, angleDegrees);
}

void FirstPersonCamera::Walk(float dist)
{
	float tempZ = mPosition.z;
	mPosition += dist * this->GetLookDirection();
	mPosition.z = tempZ;
	//mPosition -= dist * mPosition;
}

void FirstPersonCamera::Strafe(float dist)
{
	mPosition += dist *mRight;
}

void FirstPersonCamera::Fly(float dist)
{
	mPosition += dist * mUp;
}

glm::vec3 FirstPersonCamera::GetEyePosition()
{
	return mPosition;
}