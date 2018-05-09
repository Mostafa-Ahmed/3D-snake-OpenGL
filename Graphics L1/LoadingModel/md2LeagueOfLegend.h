#ifndef md2LeagueOfLegend_h__
#define md2LeagueOfLegend_h__

#include <string>
#include <vector>
#include "gl/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "../md2model.h"







/*****************************************************

Class:		CMD2Model

Purpose:	Handles MD2 models - loading, rendering,
			animation etc.

*****************************************************/

extern string LOLMD2AnimationNames[7];

class MD2LeagueOfLegend
{
public:
	typedef enum {
		STAND,
		ATTACK1,
		ATTACK2,
		RUN,
		DEATH,
		SPELL1,
		SPELL2,
		MAX_ANIMATIONS

	} animType_LOL;
	void LoadExportedModel(char* sFilename);
	void LoadModel(char* sFilename, bool flipUVCoords = false);
	void RenderModel(animState_t* animState);
	void RenderModel(animState_t* animState,KeyFrameAnimationShader* shader);

	void UpdateVertices(animState_t* animState);
	animState_t StartAnimation(animType_LOL type);
	bool UpdateExportedAnimation(animState_t* animState, float fTimePassed,bool loop=true);
	bool UpdateAnimation(animState_t* animState, float fTimePassed,bool loop = true);
	void PauseAnimation();
	void StopAnimation();
	vector<glm::vec3> getCurrentVertices(animState_t* animState);

	static anim_t	animlist[7];
	vector< vector<glm::vec3> > vVertices;// Vertices extracted for every frame
	vector< glm::vec3 > mVertices; 
private:
	UINT uiModelVAO;
	Model m;
	vector<UINT> uiFramesBuffer;

	md2_t header;
	
	vector <vector< int > > vNormals; // Normal indices extracted for every frame
	vector<int> glCommands; // Rendering OpenGL commands

	vector<CVertexBufferObject> vboFrameVertices; // All frames (keyframes) of model
	CVertexBufferObject vboTextureCoords; // Texture coords are same for all frames

	vector<int> renderModes; // Rendering modes
	vector<int> numRenderVertices; // with number of vertices

	Texture* tSkin;

	UINT uiVAO;
};

#endif // md2model_h__
