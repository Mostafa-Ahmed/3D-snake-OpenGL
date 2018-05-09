#pragma once;

#include <string>
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
#include "../Model.h"
#include"../ShaderProgram.h"
#include"MD5Animation.h"

class MD5Model 
{
private:
	void RemoveQuotes1(std::string& str);

	// Get's the size of the file in bytes.
	int GetFileLength1(std::istream& file);

	// Ignore everything else that comes on the line, up to 'length' characters.
	void IgnoreLine1(std::istream& file, int length);

	// Computes the W component of the quaternion based on the X, Y, and Z components.
	// This method assumes the quaternion is of unit length.
	void ComputeQuatW1(glm::quat& quat);

	std::vector<Model> modelMeshes;
	std::vector<GLuint> texes;

public:
	glm::mat4x4         m_LocalToWorldMatrix;
	std::vector<glm::vec3> actualVertices;
    MD5Model();
    virtual ~MD5Model();
	void ComputeVertices();
	void TransformToModel();
    bool LoadModel( const std::string& filename );
    bool LoadAnim( const std::string& filename );
	bool Update(float fDeltaTime, bool loop=true);
	void Render();
	void RenderModel(ShaderProgram* shaderProgram, const glm::mat4 &parentTransforms);

protected:
    typedef std::vector<glm::vec3> PositionBuffer;
    typedef std::vector<glm::vec3> NormalBuffer;
    typedef std::vector<glm::vec2> Tex2DBuffer;
    typedef std::vector<GLuint> IndexBuffer;

    struct Vertex
    {
        glm::vec3   m_Pos;
        glm::vec3   m_Normal;
        glm::vec2   m_Tex0;
        int         m_StartWeight;
        int         m_WeightCount;
    };
    typedef std::vector<Vertex> VertexList;

    struct Triangle
    {
        int             m_Indices[3];
    };
    typedef std::vector<Triangle> TriangleList;

    struct Weight
    {
        int             m_JointID;
        float           m_Bias;
        glm::vec3       m_Pos;
    };
    typedef std::vector<Weight> WeightList;

    struct Joint
    {
        std::string     m_Name;
        int             m_ParentID;
        glm::vec3       m_Pos;
        glm::quat       m_Orient;
    };
    typedef std::vector<Joint> JointList;

    struct Mesh
    {
        std::string     m_Shader;
        // This vertex list stores the vertices in the bind pose.
        VertexList      m_Verts;
        TriangleList    m_Tris;
        WeightList      m_Weights;

        // A texture ID for the material
        GLuint          m_TexID;

        // These buffers are used for rendering the animated mesh
        PositionBuffer  m_PositionBuffer;   // Vertex position stream
        NormalBuffer    m_NormalBuffer;     // Vertex normals stream
        Tex2DBuffer     m_Tex2DBuffer;      // Texture coordinate set
        IndexBuffer     m_IndexBuffer;      // Vertex index buffer
    };
    typedef std::vector<Mesh> MeshList;
	
    // Prepare the mesh for rendering
    // Compute vertex positions and normals
    bool PrepareMesh( Mesh& mesh );
    bool PrepareMesh( Mesh& mesh, const MD5Animation::FrameSkeleton& skel );
    bool PrepareNormals( Mesh& mesh );

    // Render a single mesh of the model
    void RenderMesh( const Mesh& mesh );
    void RenderNormals( const Mesh& mesh );

    // Draw the skeleton of the mesh for debugging purposes.
    void RenderSkeleton( const JointList& joints );
    
    bool CheckAnimation( const MD5Animation& animation ) const;
private:

    int                 m_iMD5Version;
    int                 m_iNumJoints;
    int                 m_iNumMeshes;

    bool                m_bHasAnimation;

    JointList           m_Joints;
    MeshList            m_Meshes;

    MD5Animation        m_Animation;


};