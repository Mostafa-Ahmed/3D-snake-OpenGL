

#include "MD5Model.h"

namespace fs = boost::filesystem;


void MD5Model::RemoveQuotes1(std::string& str)
{
	size_t n;
	while ((n = str.find('\"')) != std::string::npos) str.erase(n, 1);
}

// Get's the size of the file in bytes.
int MD5Model::GetFileLength1(std::istream& file)
{
	int pos = file.tellg();
	file.seekg(0, std::ios::end);
	int length = file.tellg();
	// Restore the position of the get pointer
	file.seekg(pos);

	return length;
}

// Ignore everything else that comes on the line, up to 'length' characters.
void MD5Model::IgnoreLine1(std::istream& file, int length)
{
	file.ignore(length, '\n');
}

// Computes the W component of the quaternion based on the X, Y, and Z components.
// This method assumes the quaternion is of unit length.
void MD5Model::ComputeQuatW1(glm::quat& quat)
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


MD5Model::MD5Model()
: m_iMD5Version(-1)
, m_iNumJoints(0)
, m_iNumMeshes(0)
, m_bHasAnimation(false)
, m_LocalToWorldMatrix(1)
{
}

MD5Model::~MD5Model()
{
}

bool MD5Model::LoadModel( const std::string &filename )
{
    if ( !fs::exists(filename) )
    {
        std::cerr << "MD5Model::LoadModel: Failed to find file: " << filename << std::endl;
        return false;
    }
    
    fs::path filePath = filename;
    // store the parent path used for loading images relative to this file.
    fs::path parent_path = filePath.parent_path();

    std::string param;
    std::string junk;   // Read junk from the file

    fs::ifstream file(filename);
    int fileLength = GetFileLength1( file );   
    assert( fileLength > 0 );

    m_Joints.clear();
    m_Meshes.clear();
    
    file >> param;

    while ( !file.eof() )
    {
        if ( param == "MD5Version" )
        {
            file >> m_iMD5Version;
            assert( m_iMD5Version == 10 );
        }
        else if ( param == "commandline" )
        {
            IgnoreLine1(file, fileLength ); // Ignore the contents of the line
        }
        else if ( param == "numJoints" )
        {
            file >> m_iNumJoints;
            m_Joints.reserve(m_iNumJoints);
        }
        else if ( param == "numMeshes" )
        {
            file >> m_iNumMeshes;
            m_Meshes.reserve(m_iNumMeshes);
        }
        else if ( param == "joints" )
        {
            Joint joint;
            file >> junk; // Read the '{' character
            for ( int i = 0; i < m_iNumJoints; ++i )
            {
                file >> joint.m_Name >> joint.m_ParentID >> junk
                     >> joint.m_Pos.x >> joint.m_Pos.y >> joint.m_Pos.z >> junk >> junk
                     >> joint.m_Orient.x >> joint.m_Orient.y >> joint.m_Orient.z >> junk;
                
                RemoveQuotes1( joint.m_Name );
                MD5Model::ComputeQuatW1( joint.m_Orient );

                m_Joints.push_back(joint);
                // Ignore everything else on the line up to the end-of-line character.
                IgnoreLine1( file, fileLength );
            }
            file >> junk; // Read the '}' character
        }
        else if ( param == "mesh" )
        {
            Mesh mesh;
            int numVerts, numTris, numWeights;

            file >> junk; // Read the '{' character
            file >> param;
            while ( param != "}" )  // Read until we get to the '}' character
            {
                if ( param == "shader" )
                {
                    file >> mesh.m_Shader;
                    RemoveQuotes1( mesh.m_Shader );

                    fs::path shaderPath( mesh.m_Shader );
                    fs::path texturePath;
                    if ( shaderPath.has_parent_path() )
                    {
                        texturePath = shaderPath;
                    }
                    else
                    {
                        texturePath = parent_path / shaderPath;
                    }

                    if ( !texturePath.has_extension() )
                    {
                        texturePath.replace_extension( ".tga" );
                    }

					std::string s = "data\\" + texturePath.string();
					Texture* tex = new Texture(s.c_str(),1);

                    mesh.m_TexID = tex->m_texture;

                    file.ignore(fileLength, '\n' ); // Ignore everything else on the line
                }
                else if ( param == "numverts")
                {
                    file >> numVerts;               // Read in the vertices
                    IgnoreLine1(file, fileLength);
                    for ( int i = 0; i < numVerts; ++i )
                    {
                        Vertex vert;

                        file >> junk >> junk >> junk                    // vert vertIndex (
                            >> vert.m_Tex0.x >> vert.m_Tex0.y >> junk  //  s t )
                            >> vert.m_StartWeight >> vert.m_WeightCount;

                        IgnoreLine1(file, fileLength);

                        mesh.m_Verts.push_back(vert);
                        mesh.m_Tex2DBuffer.push_back(vert.m_Tex0);
                    }  
                }
                else if ( param == "numtris" )
                {
                    file >> numTris;
                    IgnoreLine1(file, fileLength);
                    for ( int i = 0; i < numTris; ++i )
                    {
                        Triangle tri;
                        file >> junk >> junk >> tri.m_Indices[0] >> tri.m_Indices[1] >> tri.m_Indices[2];

                        IgnoreLine1( file, fileLength );

                        mesh.m_Tris.push_back(tri);
                        mesh.m_IndexBuffer.push_back( (GLuint)tri.m_Indices[0] );
                        mesh.m_IndexBuffer.push_back( (GLuint)tri.m_Indices[1] );
                        mesh.m_IndexBuffer.push_back( (GLuint)tri.m_Indices[2] );
                    }              
                }
                else if ( param == "numweights" )
                {
                    file >> numWeights;
                    IgnoreLine1( file, fileLength );
                    for ( int i = 0; i < numWeights; ++i )
                    {
                        Weight weight;
                        file >> junk >> junk >> weight.m_JointID >> weight.m_Bias >> junk
                            >> weight.m_Pos.x >> weight.m_Pos.y >> weight.m_Pos.z >> junk;

                        IgnoreLine1( file, fileLength );
                        mesh.m_Weights.push_back(weight);
                    }
                }
                else
                {
                    IgnoreLine1(file, fileLength);
                }
            
                file >> param;
            }
			

            PrepareMesh(mesh);
            PrepareNormals(mesh);

            m_Meshes.push_back(mesh);

        }

        file >> param;
    }

    assert( m_Joints.size() == m_iNumJoints );
    assert( m_Meshes.size() == m_iNumMeshes );

	TransformToModel();

    return true;
}
void MD5Model::TransformToModel()
{
	for (size_t i = 0; i < m_Meshes.size(); i++)
	{
		Model m;
		m.VertexData = m_Meshes[i].m_PositionBuffer;
		m.NormalsData = m_Meshes[i].m_NormalBuffer;
		m.UVData = m_Meshes[i].m_Tex2DBuffer;
		for (size_t j = 0; j < m_Meshes[i].m_IndexBuffer.size(); j++)
		{
			m.IndicesData.push_back((unsigned short)m_Meshes[i].m_IndexBuffer[j]);
		}
		m.Initialize();
		modelMeshes.push_back(m);
		texes.push_back(m_Meshes[i].m_TexID);
	}
}
bool MD5Model::LoadAnim( const std::string& filename )
{
    if ( m_Animation.LoadAnimation( filename ) )
    {
        // Check to make sure the animation is appropriate for this model
        m_bHasAnimation = CheckAnimation( m_Animation );
    }

    return m_bHasAnimation;
}

bool MD5Model::CheckAnimation( const MD5Animation& animation ) const
{
    if ( m_iNumJoints != animation.GetNumJoints() )
    {
        return false;
    }

    // Check to make sure the joints match up
    for ( unsigned int i = 0; i < m_Joints.size(); ++i )
    {
        const Joint& meshJoint = m_Joints[i];
        const MD5Animation::JointInfo& animJoint = animation.GetJointInfo( i );

        if ( meshJoint.m_Name != animJoint.m_Name || 
             meshJoint.m_ParentID != animJoint.m_ParentID )
        {
            return false;
        }
    }

    return true;
}


// Compute the position of the vertices in object local space
// in the skeleton's bind pose
bool MD5Model::PrepareMesh( Mesh& mesh )
{
    mesh.m_PositionBuffer.clear();
    mesh.m_Tex2DBuffer.clear();

    // Compute vertex positions
    for ( unsigned int i = 0; i < mesh.m_Verts.size(); ++i )
    {
        glm::vec3 finalPos(0);
        Vertex& vert = mesh.m_Verts[i];

        vert.m_Pos = glm::vec3(0);
        vert.m_Normal = glm::vec3(0);

        // Sum the position of the weights
        for ( int j = 0; j < vert.m_WeightCount; ++j )
        {
            Weight& weight = mesh.m_Weights[vert.m_StartWeight + j];
            Joint& joint = m_Joints[weight.m_JointID];
            
            // Convert the weight position from Joint local space to object space
            glm::vec3 rotPos = joint.m_Orient * weight.m_Pos;

            vert.m_Pos += ( joint.m_Pos + rotPos ) * weight.m_Bias;
        }

        mesh.m_PositionBuffer.push_back(vert.m_Pos);
        mesh.m_Tex2DBuffer.push_back(vert.m_Tex0);
    }

    return true;
}

bool MD5Model::PrepareMesh( Mesh& mesh, const MD5Animation::FrameSkeleton& skel )
{
    for ( unsigned int i = 0; i < mesh.m_Verts.size(); ++i )
    {
        const Vertex& vert = mesh.m_Verts[i];
        glm::vec3& pos = mesh.m_PositionBuffer[i];
        glm::vec3& normal = mesh.m_NormalBuffer[i];

        glm::vec3 pos1 = glm::vec3(0);
		normal = glm::vec3(0);

        for ( int j = 0; j < vert.m_WeightCount; ++j )
        {
            const Weight& weight = mesh.m_Weights[vert.m_StartWeight + j];
            const MD5Animation::SkeletonJoint& joint = skel.m_Joints[weight.m_JointID];

            glm::vec3 rotPos = joint.m_Orient * weight.m_Pos;
            pos1 += ( joint.m_Pos + rotPos ) * weight.m_Bias;

			float distance = sqrt((pos1.x - pos.x)*(pos1.x - pos.x) + (pos1.y - pos.y)*(pos1.y - pos.y) + (pos1.z - pos.z)*(pos1.z - pos.z));
			if (distance > 1000)
			{
				continue;
			}
			else
			{
				pos = pos1;
			}
			normal += (joint.m_Orient * vert.m_Normal) * weight.m_Bias;
        }
    }
    return true;
}


// Compute the vertex normals in the Mesh's bind pose
bool MD5Model::PrepareNormals( Mesh& mesh )
{
    mesh.m_NormalBuffer.clear();

    // Loop through all triangles and calculate the normal of each triangle
    for ( unsigned int i = 0; i < mesh.m_Tris.size(); ++i )
    {
        glm::vec3 v0 = mesh.m_Verts[ mesh.m_Tris[i].m_Indices[0] ].m_Pos;
        glm::vec3 v1 = mesh.m_Verts[ mesh.m_Tris[i].m_Indices[1] ].m_Pos;
        glm::vec3 v2 = mesh.m_Verts[ mesh.m_Tris[i].m_Indices[2] ].m_Pos;

        glm::vec3 normal = glm::cross( v2 - v0, v1 - v0 );

        mesh.m_Verts[ mesh.m_Tris[i].m_Indices[0] ].m_Normal += normal;
        mesh.m_Verts[ mesh.m_Tris[i].m_Indices[1] ].m_Normal += normal;
        mesh.m_Verts[ mesh.m_Tris[i].m_Indices[2] ].m_Normal += normal;
    }

    // Now normalize all the normals
    for ( unsigned int i = 0; i < mesh.m_Verts.size(); ++i )
    {
        Vertex& vert = mesh.m_Verts[i];

        glm::vec3 normal = glm::normalize( vert.m_Normal );
        mesh.m_NormalBuffer.push_back( normal );

        // Reset the normal to calculate the bind-pose normal in joint space
        vert.m_Normal = glm::vec3(0);

        // Put the bind-pose normal into joint-local space
        // so the animated normal can be computed faster later
        for ( int j = 0; j < vert.m_WeightCount; ++j )
        {
            const Weight& weight = mesh.m_Weights[vert.m_StartWeight + j];
            const Joint& joint = m_Joints[weight.m_JointID];
            vert.m_Normal += ( normal * joint.m_Orient ) * weight.m_Bias;
        }
    }

    return true;
}

bool MD5Model::Update( float fDeltaTime , bool loop)
{
    if ( m_bHasAnimation )
    {
		if (m_Animation.Update(fDeltaTime) && !loop)
			return true;
        const MD5Animation::FrameSkeleton& skeleton = m_Animation.GetSkeleton();

        for ( unsigned int i = 0; i < m_Meshes.size(); ++i )
        {
            PrepareMesh( m_Meshes[i], skeleton );


			/*modelMeshes[i].VertexData.clear();
			for (unsigned int j = 0; j < m_Meshes[i].m_Verts.size(); ++j)
			{
				glm::vec3 v0 = m_Meshes[i].m_Verts[j].m_Pos;
				modelMeshes[i].VertexData.push_back(v0);
			}*/


			modelMeshes[i].VertexData = m_Meshes[i].m_PositionBuffer;
			modelMeshes[i].NormalsData = m_Meshes[i].m_NormalBuffer;
			/*for (size_t j = 0; j < m_Meshes[i].m_IndexBuffer.size(); j++)
			{
				modelMeshes[i].IndicesData.push_back((unsigned short)m_Meshes[i].m_IndexBuffer[j]);
			}*/
			modelMeshes[i].Update();
        }
    }
	return false;
}


void MD5Model::ComputeVertices()
{
	actualVertices.clear();
	for (unsigned int i = 0; i < m_Meshes.size(); ++i)
	{
		for (size_t j = 0; j < modelMeshes[i].VertexData.size(); j++)
		{
			actualVertices.push_back(modelMeshes[i].VertexData[j]);
		}
	}
}

void MD5Model::RenderModel(ShaderProgram* shaderProgram, const glm::mat4 &parentTransforms)
{
	glm::mat4 curTransform = parentTransforms;
	shaderProgram->BindModelMatrix(&curTransform[0][0]);
	// Render the meshes
	for (unsigned int i = 0; i < m_Meshes.size(); ++i)
	{
		glBindTexture(GL_TEXTURE_2D, texes[i]);
		modelMeshes[i].Draw();
	}
}


void MD5Model::Render()
{
    glPushMatrix();
    glMultMatrixf( glm::value_ptr(m_LocalToWorldMatrix) );

    // Render the meshes
    for ( unsigned int i = 0; i < m_Meshes.size(); ++i )
    {
        RenderMesh( m_Meshes[i] );
    }
    
    m_Animation.Render();

    for ( unsigned int i = 0; i < m_Meshes.size(); ++i )
    {
        RenderNormals( m_Meshes[i] );
    }

    glPopMatrix();
}

void MD5Model::RenderMesh( const Mesh& mesh )
{
    glColor3f( 1.0f, 1.0f, 1.0f );
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    glEnableClientState( GL_NORMAL_ARRAY );

    glBindTexture( GL_TEXTURE_2D, mesh.m_TexID );
    glVertexPointer( 3, GL_FLOAT, 0, &(mesh.m_PositionBuffer[0]) );
    glNormalPointer( GL_FLOAT, 0, &(mesh.m_NormalBuffer[0]) );
    glTexCoordPointer( 2, GL_FLOAT, 0, &(mesh.m_Tex2DBuffer[0]) );
    
    glDrawElements( GL_TRIANGLES, mesh.m_IndexBuffer.size(), GL_UNSIGNED_INT, &(mesh.m_IndexBuffer[0]) );

    glDisableClientState( GL_NORMAL_ARRAY );
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    glDisableClientState( GL_VERTEX_ARRAY );

    glBindTexture( GL_TEXTURE_2D, 0 );
}

void MD5Model::RenderNormals(  const Mesh& mesh )
{

    glPushAttrib( GL_ENABLE_BIT );
    glDisable( GL_LIGHTING );

    glColor3f( 1.0f, 1.0f, 0.0f );// Yellow

    glBegin( GL_LINES );
    {
        for ( unsigned int i = 0; i < mesh.m_PositionBuffer.size(); ++i )
        {
            glm::vec3 p0 = mesh.m_PositionBuffer[i];
            glm::vec3 p1 = ( mesh.m_PositionBuffer[i] + mesh.m_NormalBuffer[i] );

            glVertex3fv( glm::value_ptr(p0) );
            glVertex3fv( glm::value_ptr(p1) );
        }
    }
    glEnd();

    glPopAttrib();
}

void MD5Model::RenderSkeleton( const JointList& joints )
{
    glPointSize( 5.0f );
    glColor3f( 1.0f, 0.0f, 0.0f );
    
    glPushAttrib( GL_ENABLE_BIT );

    glDisable(GL_LIGHTING );
    glDisable( GL_DEPTH_TEST );

    // Draw the joint positions
    glBegin( GL_POINTS );
    {
        for ( unsigned int i = 0; i < joints.size(); ++i )
        {
            glVertex3fv( glm::value_ptr(joints[i].m_Pos) );
        }
    }
    glEnd();

    // Draw the bones
    glColor3f( 0.0f, 1.0f, 0.0f );
    glBegin( GL_LINES );
    {
        for ( unsigned int i = 0; i < joints.size(); ++i )
        {
            const Joint& j0 = joints[i];
            if ( j0.m_ParentID != -1 )
            {
                const Joint& j1 = joints[j0.m_ParentID];
                glVertex3fv( glm::value_ptr(j0.m_Pos) );
                glVertex3fv( glm::value_ptr(j1.m_Pos) );
            }
        }
    }
    glEnd();

    glPopAttrib();

}
