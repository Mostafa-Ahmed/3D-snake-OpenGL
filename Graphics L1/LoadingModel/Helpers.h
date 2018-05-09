#pragma once;


#include <string>

#include<ctime>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

// Helper class to count frame time
class ElapsedTime
{
public:
	ElapsedTime(float maxTimeStep = 0.03333f) : m_fMaxTimeStep(maxTimeStep)
		, m_fPrevious(std::clock() / (float)CLOCKS_PER_SEC)
	{
		
	}
	float GetElapsedTime() const
	{
		float fCurrentTime = std::clock() / (float)CLOCKS_PER_SEC;
		float fDeltaTime = fCurrentTime - m_fPrevious;
		m_fPrevious = fCurrentTime;

		// Clamp to the max time step
		if (fDeltaTime > m_fMaxTimeStep)
			fDeltaTime = m_fMaxTimeStep;
		

		return fDeltaTime;
	}

private:
    float m_fMaxTimeStep;
    mutable float m_fPrevious;
};

