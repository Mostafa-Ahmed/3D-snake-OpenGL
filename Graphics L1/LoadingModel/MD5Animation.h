#pragma once;


#include <string>
#include "Helpers.h"
#include "../../middleware/includes/boost/filesystem.hpp"
#include "../../middleware/includes/boost/filesystem/fstream.hpp"


#include <vector>
#include <string>
#include <iostream>
#include <ctime>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "../texture.h"


namespace fs = boost::filesystem;

class MD5Animation
{

	void RemoveQuotes(std::string& str)
	{
		size_t n;
		while ((n = str.find('\"')) != std::string::npos) str.erase(n, 1);
	}

	// Get's the size of the file in bytes.
	int GetFileLength(std::istream& file)
	{
		int pos = file.tellg();
		file.seekg(0, std::ios::end);
		int length = file.tellg();
		// Restore the position of the get pointer
		file.seekg(pos);

		return length;
	}

	// Ignore everything else that comes on the line, up to 'length' characters.
	void IgnoreLine(std::istream& file, int length)
	{
		file.ignore(length, '\n');
	}

	// Computes the W component of the quaternion based on the X, Y, and Z components.
	// This method assumes the quaternion is of unit length.
	void ComputeQuatW(glm::quat& quat)
	{
		float t = 1.0f - (quat.x * quat.x) - (quat.y * quat.y) - (quat.z * quat.z);
		if (t < 0.0f)
		{
			quat.w = 0.0f;
		}
		else
		{
			quat.w = -sqrtf(t);
		}
	}
public:
    MD5Animation();
    virtual ~MD5Animation();

    // Load an animation from the animation file
    bool LoadAnimation( const std::string& filename );
    // Update this animation's joint set.
    //void Update( float fDeltaTime );
	bool Update(float fDeltaTime, bool loop=true);
    // Draw the animated skeleton
    void Render();

    // The JointInfo stores the information necessary to build the 
    // skeletons for each frame
    struct JointInfo
    {
        std::string m_Name;
        int         m_ParentID;
        int         m_Flags;
        int         m_StartIndex;
    };
    typedef std::vector<JointInfo> JointInfoList;

    struct Bound
    {
        glm::vec3   m_Min;
        glm::vec3   m_Max;
    };
    typedef std::vector<Bound> BoundList;

    struct BaseFrame
    {
        glm::vec3   m_Pos;
        glm::quat   m_Orient;
    };
    typedef std::vector<BaseFrame> BaseFrameList;

    struct FrameData
    {
        int m_iFrameID;
        std::vector<float> m_FrameData;
    };
    typedef std::vector<FrameData> FrameDataList;

    // A Skeleton joint is a joint of the skeleton per frame
    struct SkeletonJoint
    {
        SkeletonJoint()
            : m_Parent(-1)
            , m_Pos(0)
        {}

        SkeletonJoint( const BaseFrame& copy )
            : m_Pos( copy.m_Pos )
            , m_Orient( copy.m_Orient )
        {}

        int         m_Parent;
        glm::vec3   m_Pos;
        glm::quat   m_Orient;
    };
    typedef std::vector<SkeletonJoint> SkeletonJointList;

    // A frame skeleton stores the joints of the skeleton for a single frame.
    struct FrameSkeleton
    {
        SkeletonJointList   m_Joints;
    };
    typedef std::vector<FrameSkeleton> FrameSkeletonList;

    const FrameSkeleton& GetSkeleton() const
    {
        return m_AnimatedSkeleton;
    }

    int GetNumJoints() const
    {
        return m_iNumJoints;
    }

    const JointInfo& GetJointInfo( unsigned int index ) const
    {
        assert( index < m_JointInfos.size() );
        return m_JointInfos[index];
    }

protected:

    JointInfoList       m_JointInfos;
    BoundList           m_Bounds;
    BaseFrameList       m_BaseFrames;
    FrameDataList       m_Frames;
    FrameSkeletonList   m_Skeletons;    // All the skeletons for all the frames

    FrameSkeleton       m_AnimatedSkeleton;

    // Build the frame skeleton for a particular frame
    void BuildFrameSkeleton( FrameSkeletonList& skeletons, const JointInfoList& jointInfo, const BaseFrameList& baseFrames, const FrameData& frameData );
    void InterpolateSkeletons( FrameSkeleton& finalSkeleton, const FrameSkeleton& skeleton0, const FrameSkeleton& skeleton1, float fInterpolate );


private:
    int m_iMD5Version;
    int m_iNumFrames;
    int m_iNumJoints;
    int m_iFramRate;
    int m_iNumAnimatedComponents;

    float m_fAnimDuration;
    float m_fFrameDuration;
    float m_fAnimTime;
};