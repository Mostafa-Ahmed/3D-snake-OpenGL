#ifndef Model_h__
#define Model_h__

#include "glm/glm.hpp"
#include "gl/glew.h"
#include <vector>

using namespace std;
using namespace glm;

class Model
{
	//VAO
	GLuint mVertexArrayObjectID;
	
	//VBOs
	GLuint mVertexDataBufferID;
	GLuint mColorDataBufferID;
	GLuint mIndicesDataBufferID;
	GLuint mUVDataBufferID;
	GLuint mNormalsBufferID;
public:
	Model();
	~Model();

	vector<vec3> VertexData;
	vector<vec3> ColorData;
	vector<vec2> UVData;
	vector<vec3> NormalsData;
	vector<unsigned short> IndicesData;

	mat4 ModelMatrix;

	void Initialize();
	virtual void Update();
	virtual void Draw();
	virtual void DrawStrip();
	virtual void DrawWireFrame();
	void Cleanup();
};
#endif // Model_h__