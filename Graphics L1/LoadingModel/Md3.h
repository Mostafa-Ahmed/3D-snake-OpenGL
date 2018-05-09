#ifndef _MD3_H
#define _MD3_H



#define LegConvertor 7
#define kLower	0			
#define kUpper	1			
#define kHead	2			
#define kWeapon	3

#define MAX_TEXTURES 100	
#include"../Model.h"
#include "../texture.h"
#include <vector>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <fstream>
#include <vector>
#include <crtdbg.h>
#include "../ShaderProgram.h"
using namespace std;

enum TORSO
{
	BOTH_DEATH1 = 0,		
	BOTH_DEAD1,				
	BOTH_DEATH2,			
	BOTH_DEAD2,				
	BOTH_DEATH3,			
	BOTH_DEAD3,			
	TORSO_GESTURE,			
	TORSO_ATTACK,			
	TORSO_ATTACK2,			
	TORSO_DROP,				
	TORSO_RAISE,			
	TORSO_STAND,			
	TORSO_STAND2,
	LEGS_WALKCR,
	LEGS_WALK,				
	LEGS_RUN,				
	LEGS_BACK,				
	LEGS_SWIM,				
	LEGS_JUMP,				
	LEGS_LAND,				
	LEGS_JUMPB,				
	LEGS_LANDB,				
	LEGS_IDLE,				
	LEGS_IDLECR,			
	LEGS_TURN,				
	LastAnime			
} ;
//6 walkcr
//7 walk
//8 run
//enum LEGS
//{
//	LEGS_DEATH1 = 0,		
//	LEGS_DEAD1,				
//	LEGS_DEATH2,			
//	LEGS_DEAD2,				
//	LEGS_DEATH3,			
//	LEGS_DEAD3,			
//	LEGS_WALKCR
//};

class CVector3
{
public:
	float x, y, z;
};

class CVector2
{
public:
	float x, y;
};

struct tFace
{
	int vertIndex[3];
	int coordIndex[3];
};
struct tMaterialInfo
{
	char  strName[255];
	char  strFile[255];
	BYTE  color[3];
	int   texureId;
	float uTile;
	float vTile;
	float uOffset;
	float vOffset;
};
struct t3DObject
{
	int  numOfVerts;
	int  numOfFaces;
	int  numTexVertex;
	int  materialID;
	bool bHasTexture;
	char strName[255];
	CVector3  *pVerts;
	CVector3  *pNormals;
	CVector2  *pTexVerts;
	tFace *pFaces;
};

struct tAnimationInfo
{
	char strName[255];
	int startFrame;
	int endFrame;
	int loopingFrames;
	int framesPerSecond;
};
struct t3DModel
{
	int numOfObjects;
	int numOfMaterials;
	vector<tMaterialInfo> pMaterials;
	vector<t3DObject> pObject;
	int numOfAnimations;
	int currentAnim;
	int currentFrame;
	float interpol;
	int nextFrame;
	int index;
	float t;
	float lastTime;
	vector<tAnimationInfo> pAnimations;
	int numOfTags;
	t3DModel			**pLinks;
	struct tMd3Tag		*pTags;
};

struct tMd3Header
{ 
	char	fileID[4];					
	int		version;					
	char	strFile[68];				
	int		numFrames;					
	int		numTags;					
	int		numMeshes;					
	int		numMaxSkins;				
	int		headerSize;					
	int		tagStart;					
	int		tagEnd;						
	int		fileSize;					
};
struct tMd3MeshInfo
{
	char	meshID[4];					
	char	strName[68];				
	int		numMeshFrames;				
	int		numSkins;					
	int     numVertices;				
	int		numTriangles;				
	int		triStart;					
	int		headerSize;					
	int     uvStart;					
	int		vertexStart;				
	int		meshSize;					
};

struct tMd3Tag
{
	char		strName[64];			
	CVector3	vPosition;				
	float		rotation[3][3];			
};
struct tMd3Bone
{
	float	mins[3];					
	float	maxs[3];					
	float	position[3];				
	float	scale;						
	char	creator[16];				
};

struct tMd3Triangle
{
   signed short	 vertex[3];				
   unsigned char normal[2];				
};


struct tMd3Face
{
   int vertexIndices[3];				
};


struct tMd3TexCoord
{
   float textureCoord[2];
};

struct tMd3Skin 
{
	char strName[68];
};

class CLoadMD3
{

public:

	CLoadMD3();								
	bool ImportMD3(t3DModel *pModel, char *strFileName);
	bool LoadSkin(t3DModel *pModel, LPSTR strSkin);
	bool LoadShader(t3DModel *pModel, LPSTR strShader);

private:


	void ReadMD3Data(t3DModel *pModel);
	void ConvertDataStructures(t3DModel *pModel, tMd3MeshInfo meshHeader);
	void CleanUp();
	FILE *m_FilePointer;

	tMd3Header				m_Header;			

	tMd3Skin				*m_pSkins;			
	tMd3TexCoord			*m_pTexCoords;		
	tMd3Face				*m_pTriangles;		
	tMd3Triangle			*m_pVertices;		
	tMd3Bone				*m_pBones;			
};
class CModelMD3
{

public:

	CModelMD3();
	~CModelMD3();



	void TraverseModel(int index, t3DModel *pModel, const glm::mat4 &modelMatrix);
	void ComputeActualVertices();
	void TraverseLink(int index, t3DModel *pModel, const glm::mat4 &modelMatrix);

	bool LoadModel(LPSTR strPath);

	bool LoadWeapon(LPSTR strPath, LPSTR strModel);
	bool Load(LPSTR strPath, LPSTR strModel);

	void LinkModel(t3DModel *pModel, t3DModel *pLink, LPSTR strTagName);
	void DrawLink(int index, t3DModel *pModel, ShaderProgram* shader,const glm::mat4 &modelMatrix);

	void SetTorsoAnimation(LPSTR strAnimation);
	void SetTorsoAnimation(int AnimationNumber);
	void SetLegsAnimation(LPSTR strAnimation);
	void SetLegsAnimation(int AnimationNumber);
	void DrawModel();
	void DrawModel(ShaderProgram* shader,const glm::mat4 &mat);
	void Draw(ShaderProgram* shader,const glm::mat4 &mat);
	void Draw();
	void RenderModel(int index, t3DModel *pModel, ShaderProgram* shader, const glm::mat4 &modelMatrix);

	void DestroyModel(t3DModel *pModel);
	bool UpdateLowerModel(float deltatime);
	bool UpdateUpperModel(float deltatime);
	bool UpdateUpperModel(int, float deltatime);
	int GetTorsoCurrentAnimation();
	int GetLegsCurrentAnimation();

	t3DModel *GetModel(int whichPart);
	vector<glm::vec3> actualVertices;
private:
	void LoadModelTextures(t3DModel *pModel, LPSTR strPath);
	void ConvertToModel(t3DModel* pModel, int index);
	bool LoadAnimations(LPSTR strConfigFile);

	bool UpdateModel(t3DModel *pModel, bool loop, float deltatime);
	void SetCurrentTime(t3DModel *pModel,float deltatime);

	void DrawLink(t3DModel *pModel);
	void RenderModel(t3DModel *pModel);
	UINT m_Textures[MAX_TEXTURES];	

	vector<string> strTextures;
	t3DModel m_Head;
	t3DModel m_Upper;
	t3DModel m_Lower;
	t3DModel m_Weapon;

	vector<int> mHeadTex;
	vector<int> mUpperTex;
	vector<int> mLowerTex;
	vector<int> mWeaponTex;

	vector<Model> mHead;
	vector<Model> mUpper;
	vector<Model> mLower;
	vector<Model> mWeapon;
};


class CQuaternion 
{

public:

	CQuaternion() { x = y = z = 0.0f;   w = 1.0f;  }
	CQuaternion(float X, float Y, float Z, float W) 
	{ 
		x = X;		y = Y;		z = Z;		w = W;
	}

	void CreateMatrix(float *pMatrix);
	void CreateFromMatrix(float *pMatrix, int rowColumnCount);

	CQuaternion Slerp(CQuaternion &q1, CQuaternion &q2, float t);

private:

	float x, y, z, w;
};


#endif
