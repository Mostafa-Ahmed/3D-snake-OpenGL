
#include "Md3.h"



void CQuaternion::CreateMatrix(float *pMatrix)
{
	if(!pMatrix) return;
	
	pMatrix[ 0] = 1.0f - 2.0f * ( y * y + z * z );  
	pMatrix[ 1] = 2.0f * ( x * y - w * z );  
	pMatrix[ 2] = 2.0f * ( x * z + w * y );  
	pMatrix[ 3] = 0.0f;  
	pMatrix[ 4] = 2.0f * ( x * y + w * z );  
	pMatrix[ 5] = 1.0f - 2.0f * ( x * x + z * z );  
	pMatrix[ 6] = 2.0f * ( y * z - w * x );  
	pMatrix[ 7] = 0.0f;  
	pMatrix[ 8] = 2.0f * ( x * z - w * y );  
	pMatrix[ 9] = 2.0f * ( y * z + w * x );  
	pMatrix[10] = 1.0f - 2.0f * ( x * x + y * y );  
	pMatrix[11] = 0.0f;  
	pMatrix[12] = 0;  
	pMatrix[13] = 0;  
	pMatrix[14] = 0;  
	pMatrix[15] = 1.0f;
}

void CQuaternion::CreateFromMatrix(float *pTheMatrix, int rowColumnCount)
{
	if(!pTheMatrix || ((rowColumnCount != 3) && (rowColumnCount != 4))) return;
	float *pMatrix = pTheMatrix;

	float m4x4[16] = {0};

	if(rowColumnCount == 3)
	{
		m4x4[0]  = pTheMatrix[0];	m4x4[1]  = pTheMatrix[1];	m4x4[2]  = pTheMatrix[2];
		m4x4[4]  = pTheMatrix[3];	m4x4[5]  = pTheMatrix[4];	m4x4[6]  = pTheMatrix[5];
		m4x4[8]  = pTheMatrix[6];	m4x4[9]  = pTheMatrix[7];	m4x4[10] = pTheMatrix[8];
		m4x4[15] = 1;
		pMatrix = &m4x4[0];
	}
	float diagonal = pMatrix[0] + pMatrix[5] + pMatrix[10] + 1;
	float scale = 0.0f;
	if(diagonal > 0.00000001)
	{
		scale = float(sqrt(diagonal ) * 2);
		x = ( pMatrix[9] - pMatrix[6] ) / scale;
		y = ( pMatrix[2] - pMatrix[8] ) / scale;
		z = ( pMatrix[4] - pMatrix[1] ) / scale;
		w = 0.25f * scale;
	}
	else 
	{
		if ( pMatrix[0] > pMatrix[5] && pMatrix[0] > pMatrix[10] )  
		{	
			scale  = (float)sqrt( 1.0f + pMatrix[0] - pMatrix[5] - pMatrix[10] ) * 2.0f;
			x = 0.25f * scale;
			y = (pMatrix[4] + pMatrix[1] ) / scale;
			z = (pMatrix[2] + pMatrix[8] ) / scale;
			w = (pMatrix[9] - pMatrix[6] ) / scale;	
		} 
		else if ( pMatrix[5] > pMatrix[10] ) 
		{
			scale  = (float)sqrt( 1.0f + pMatrix[5] - pMatrix[0] - pMatrix[10] ) * 2.0f;
			x = (pMatrix[4] + pMatrix[1] ) / scale;
			y = 0.25f * scale;
			z = (pMatrix[9] + pMatrix[6] ) / scale;
			w = (pMatrix[2] - pMatrix[8] ) / scale;
		} 
		else 
		{	
			scale  = (float)sqrt( 1.0f + pMatrix[10] - pMatrix[0] - pMatrix[5] ) * 2.0f;
			x = (pMatrix[2] + pMatrix[8] ) / scale;
			y = (pMatrix[9] + pMatrix[6] ) / scale;
			z = 0.25f * scale;
			w = (pMatrix[4] - pMatrix[1] ) / scale;
		}
	}
}

CQuaternion CQuaternion::Slerp(CQuaternion &q1, CQuaternion &q2, float t)
{
	CQuaternion qInterpolated;

	if(q1.x == q2.x && q1.y == q2.y && q1.z == q2.z && q1.w == q2.w) 
		return q1;
	float result = (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z) + (q1.w * q2.w);
	if(result < 0.0f)
	{
		q2 = CQuaternion(-q2.x, -q2.y, -q2.z, -q2.w);
		result = -result;
	}
	float scale0 = 1 - t, scale1 = t;
	if(1 - result > 0.1f)
	{
		float theta = (float)acos(result);
		float sinTheta = (float)sin(theta);
		scale0 = (float)sin( ( 1 - t ) * theta) / sinTheta;
		scale1 = (float)sin( ( t * theta) ) / sinTheta;
	}	

	qInterpolated.x = (scale0 * q1.x) + (scale1 * q2.x);
	qInterpolated.y = (scale0 * q1.y) + (scale1 * q2.y);
	qInterpolated.z = (scale0 * q1.z) + (scale1 * q2.z);
	qInterpolated.w = (scale0 * q1.w) + (scale1 * q2.w);
	return qInterpolated;
}

bool IsInString(string strString, string strSubString)
{
	if(strString.length() <= 0 || strSubString.length() <= 0) return false;

	unsigned int index = strString.find(strSubString);

	if(index >= 0 && index < strString.length())
		return true;

	return false;
}

CModelMD3::CModelMD3()
{
	memset(&m_Head,  0, sizeof(t3DModel));
	memset(&m_Upper, 0, sizeof(t3DModel));
	memset(&m_Lower, 0, sizeof(t3DModel));
	memset(&m_Weapon, 0, sizeof(t3DModel));
}

CModelMD3::~CModelMD3()
{
	DestroyModel(&m_Head);
	DestroyModel(&m_Upper);
	DestroyModel(&m_Lower);
	DestroyModel(&m_Weapon);
}	

void CModelMD3::DestroyModel(t3DModel *pModel)
{
	for(int i = 0; i < pModel->numOfObjects; i++)
	{
		if(pModel->pObject[i].pFaces)		delete [] pModel->pObject[i].pFaces;
		if(pModel->pObject[i].pNormals)		delete [] pModel->pObject[i].pNormals;
		if(pModel->pObject[i].pVerts)		delete [] pModel->pObject[i].pVerts;
		if(pModel->pObject[i].pTexVerts)	delete [] pModel->pObject[i].pTexVerts;
	}

	if(pModel->pTags)		delete [] pModel->pTags;
	if(pModel->pLinks)		free(pModel->pLinks);
}
	
t3DModel *CModelMD3::GetModel(int whichPart)
{
	if(whichPart == kLower) 
		return &m_Lower;
	if(whichPart == kUpper) 
		return &m_Upper;
	if(whichPart == kHead) 
		return &m_Head;
	return &m_Weapon;
}

bool CModelMD3::LoadModel(LPSTR strPath)
{
	char strLowerModel[255] = {0};
	char strUpperModel[255] = {0};
	char strHeadModel[255]  = {0};
	char strLowerSkin[255]  = {0};
	char strUpperSkin[255]  = {0};	
	char strHeadSkin[255]   = {0};	
	CLoadMD3 loadMd3;				
	if(!strPath) return false;
	sprintf(strLowerModel, "%s\\lower.md3", strPath);
	sprintf(strUpperModel, "%s\\upper.md3", strPath);
	sprintf(strHeadModel,  "%s\\head.md3",  strPath);
	sprintf(strLowerSkin, "%s\\lower.skin", strPath);
	sprintf(strUpperSkin, "%s\\upper.skin", strPath);
	sprintf(strHeadSkin,  "%s\\head.skin",  strPath);
	if(!loadMd3.ImportMD3(&m_Head,  strHeadModel))
	{
		MessageBox(NULL, "Unable to load the HEAD model!", "Error", MB_OK);
		return false;
	}
	m_Head.index = 0;

	if(!loadMd3.ImportMD3(&m_Upper, strUpperModel))		
	{
		MessageBox(NULL, "Unable to load the UPPER model!", "Error", MB_OK);
		return false;
	}

	m_Upper.index = 1;
	if(!loadMd3.ImportMD3(&m_Lower, strLowerModel))
	{
		MessageBox(NULL, "Unable to load the LOWER model!", "Error", MB_OK);
		return false;
	}

	m_Lower.index = 2;
	if(!loadMd3.LoadSkin(&m_Lower, strLowerSkin))
	{
		MessageBox(NULL, "Unable to load the LOWER skin!", "Error", MB_OK);
		return false;
	}

	if(!loadMd3.LoadSkin(&m_Upper, strUpperSkin))
	{
		MessageBox(NULL, "Unable to load the UPPER skin!", "Error", MB_OK);
		return false;
	}

	if(!loadMd3.LoadSkin(&m_Head,  strHeadSkin))
	{
		MessageBox(NULL, "Unable to load the HEAD skin!", "Error", MB_OK);
		return false;
	}

	LoadModelTextures(&m_Lower, strPath);
	LoadModelTextures(&m_Upper, strPath);
	LoadModelTextures(&m_Head,  strPath);
	char strConfigFile[255] = {0};	
	sprintf(strConfigFile,  "%s\\animation.cfg",  strPath);
	if(!LoadAnimations(strConfigFile))
	{
		MessageBox(NULL, "Unable to load the Animation Config File!", "Error", MB_OK);
		return false;
	}
	LinkModel(&m_Lower, &m_Upper, "tag_torso");
	LinkModel(&m_Upper, &m_Head, "tag_head");

	SetTorsoAnimation("TORSO_STAND");
	SetLegsAnimation("LEGS_IDLE");


	ConvertToModel(&m_Head, m_Head.index);
	ConvertToModel(&m_Upper, m_Upper.index);
	ConvertToModel(&m_Lower, m_Lower.index);
	return true;
}

bool CModelMD3::LoadWeapon(LPSTR strPath, LPSTR strModel)
{
	char strWeaponModel[255]  = {0};	
	char strWeaponShader[255] = {0};	
	CLoadMD3 loadMd3;					
	if(!strPath || !strModel) return false;
	sprintf(strWeaponModel, "%s\\%s.md3", strPath, strModel);
	if(!loadMd3.ImportMD3(&m_Weapon,  strWeaponModel))
	{
		MessageBox(NULL, "Unable to load the WEAPON model!", "Error", MB_OK);
		return false;
	}

	sprintf(strWeaponShader, "%s\\%s.shader", strPath, strModel);
	if(!loadMd3.LoadShader(&m_Weapon, strWeaponShader))
	{
		MessageBox(NULL, "Unable to load the SHADER file!", "Error", MB_OK);
		return false;
	}
	LoadModelTextures(&m_Weapon, strPath);
	m_Weapon.index = 3;

	ConvertToModel(&m_Weapon, m_Weapon.index);
	LinkModel(&m_Upper, &m_Weapon, "tag_weapon");
		
	return true;
}

bool CModelMD3::Load(LPSTR strPath, LPSTR strModel)
{
	char strWeaponModel[255]  = {0};	
	char strWeaponShader[255] = {0};	
	CLoadMD3 loadMd3;					
	if(!strPath || !strModel) return false;
	sprintf(strWeaponModel, "%s\\%s.md3", strPath, strModel);
	if(!loadMd3.ImportMD3(&m_Weapon,  strWeaponModel))
	{
		MessageBox(NULL, "Unable to load the WEAPON model!", "Error", MB_OK);
		return false;
	}

	sprintf(strWeaponShader, "%s\\%s.shader", strPath, strModel);
	if(!loadMd3.LoadShader(&m_Weapon, strWeaponShader))
	{
		MessageBox(NULL, "Unable to load the SHADER file!", "Error", MB_OK);
		return false;
	}
	LoadModelTextures(&m_Weapon, strPath);	
	return true;
}

void CModelMD3::LoadModelTextures(t3DModel *pModel, LPSTR strPath)
{
	for(int i = 0; i < pModel->numOfMaterials; i++)
	{
		if(strlen(pModel->pMaterials[i].strFile) > 0)
		{
			bool bNewTexture = true;
			for(unsigned int j = 0; j < strTextures.size(); j++)
			{
				if(!strcmp(pModel->pMaterials[i].strFile, strTextures[j].c_str()) )
				{
					bNewTexture = false;
					pModel->pMaterials[i].texureId = j;
				}
			}

			if(bNewTexture == false) continue;			
			char strFullPath[255] = {0};

			char str[255] = { 0 };
			char str2[255] = { 0 };
			int len;
			for (size_t l = 0; l < 255; l++)
			{
				if (pModel->pMaterials[i].strFile[l] == '\0')
				{
					len = l-1;
					break;
				}
			}
			for (int j = len ; j >= 0; j--)
			{
				if (pModel->pMaterials[i].strFile[j] == '\\')
				{
					len = len-j;
					break;
				}
				str[len -j] = pModel->pMaterials[i].strFile[j];
			}
			for (size_t k = 0; k < len+1; k++)
			{
				str2[k] = str[len-k];
			}
			str2[len+1] = '\0';

			if (str2[0] == '\0')
			{
				str2[0] = 'a';
			}
			//sprintf(strFullPath, "%s\\%s", strPath, pModel->pMaterials[i].strFile);
			sprintf(strFullPath, "%s\\%s", strPath, str2);
			Texture* tex = new Texture(strFullPath,1);
			m_Textures[strTextures.size()] = tex->m_texture;
			pModel->pMaterials[i].texureId = strTextures.size();
			strTextures.push_back(pModel->pMaterials[i].strFile);
		}
	}
}
bool CModelMD3::LoadAnimations(LPSTR strConfigFile)
{
	tAnimationInfo animations[LastAnime] = {0};

	ifstream fin(strConfigFile);
	if( fin.fail() )
	{
		return false;
	}

	string strWord = "";				
	string strLine = "";				
	int currentAnim = 0;				
	int torsoOffset = 0;				
	while( fin >> strWord)
	{
		if(!isdigit( strWord[0] ))
		{
			getline(fin, strLine);
			continue;
		}

		int startFrame = atoi(strWord.c_str());
		int numOfFrames = 0, loopingFrames = 0, framesPerSecond = 0;
		fin >> numOfFrames >> loopingFrames >> framesPerSecond;

		animations[currentAnim].startFrame		= startFrame;
		animations[currentAnim].endFrame		= startFrame + numOfFrames;
		animations[currentAnim].loopingFrames	= loopingFrames;
		animations[currentAnim].framesPerSecond = framesPerSecond;

		fin >> strLine >> strLine;
		strcpy(animations[currentAnim].strName, strLine.c_str());

		if(IsInString(strLine, "BOTH"))
		{
			m_Upper.pAnimations.push_back(animations[currentAnim]);
			m_Lower.pAnimations.push_back(animations[currentAnim]);
		}
		else if(IsInString(strLine, "TORSO"))
		{
			m_Upper.pAnimations.push_back(animations[currentAnim]);
		}
		else if(IsInString(strLine, "LEGS"))
		{	
			if(!torsoOffset)
				torsoOffset = animations[LEGS_WALKCR].startFrame - animations[TORSO_GESTURE].startFrame;

			animations[currentAnim].startFrame -= torsoOffset;
			animations[currentAnim].endFrame -= torsoOffset;

			m_Lower.pAnimations.push_back(animations[currentAnim]);
		}
	
		currentAnim++;
	}	

	m_Lower.numOfAnimations = m_Lower.pAnimations.size();
	m_Upper.numOfAnimations = m_Upper.pAnimations.size();
	m_Head.numOfAnimations = m_Head.pAnimations.size();
	m_Weapon.numOfAnimations = m_Head.pAnimations.size();

	return true;
}

void  CModelMD3::LinkModel(t3DModel *pModel, t3DModel *pLink, LPSTR strTagName)
{
	if(!pModel || !pLink || !strTagName) return;

	for(int i = 0; i < pModel->numOfTags; i++)
	{
		if( !_stricmp(pModel->pTags[i].strName, strTagName) )
		{
			pModel->pLinks[i] = pLink;
			return;
		}
	}
}


bool CModelMD3::UpdateModel(t3DModel *pModel,bool loop,float deltatime)
{
	int startFrame = 0;
	int endFrame   = 1;

	tAnimationInfo *pAnim = &(pModel->pAnimations[pModel->currentAnim]);

	if(pModel->numOfAnimations)
	{
		startFrame = pAnim->startFrame;
		endFrame   = pAnim->endFrame;
	}
	
	pModel->nextFrame = (pModel->currentFrame + 1) % endFrame;

	if(pModel->nextFrame == 0)
	{
		pModel->nextFrame =  startFrame;
		if(!loop)
			return true;
	}
	SetCurrentTime(pModel, deltatime);
	return false;
}

void CModelMD3::DrawModel(ShaderProgram* shader,const glm::mat4 &mat)
{
	DrawLink(m_Lower.index, &m_Lower, shader, mat);

	//RenderModel(m_Lower.index, &m_Lower, shader, mat);
	//RenderModel(m_Head.index, &m_Head, shader, mat);
	//RenderModel(m_Upper.index, &m_Upper, shader, mat);
	//RenderModel(m_Weapon.index, &m_Weapon, shader, mat);
}


void CModelMD3::DrawModel()
{
	glRotatef(-90, 1, 0, 0);
	DrawLink(&m_Lower);
}
void CModelMD3::Draw(ShaderProgram* shader, const glm::mat4 &mat)
{
	RenderModel(m_Weapon.index,&m_Weapon,shader,mat);
}

void CModelMD3::Draw()
{
	glRotatef(-90, 1, 0, 0);
	RenderModel(&m_Weapon);
}
bool CModelMD3::UpdateLowerModel(float deltatime)
{
	int Animation = GetLegsCurrentAnimation();
	if(Animation > 5)
		Animation += 7;
	bool loop = false;
	if(Animation == BOTH_DEATH1)
	{
		if (UpdateModel(&m_Lower, loop, deltatime))
			SetLegsAnimation(BOTH_DEAD1);
		return true;
	}
	if(Animation == LEGS_JUMPB)
	{
		if (UpdateModel(&m_Lower, loop, deltatime))
			SetLegsAnimation(LEGS_LAND);
		return true;
	}
	if(Animation == LEGS_LAND)
	{
		if (UpdateModel(&m_Lower, loop, deltatime))
			SetLegsAnimation(LEGS_IDLE);
		return true;
	}
	else
		loop = true;
	return UpdateModel(&m_Lower, loop, deltatime);
}
bool CModelMD3::UpdateUpperModel(float deltatime)
{
	int Animation = GetTorsoCurrentAnimation();
	bool loop = false;
	if(Animation == BOTH_DEATH1)
	{
		loop = false;
		if (UpdateModel(&m_Upper, loop, deltatime))
			SetTorsoAnimation(BOTH_DEAD1);
		return true;
	}
	if(Animation == TORSO_ATTACK)
	{
		loop = false;
		if (UpdateModel(&m_Upper, loop, deltatime))
			SetTorsoAnimation(TORSO_STAND);
		return true;
	}
	if(Animation == TORSO_DROP)
	{
		loop = false;
		if (UpdateModel(&m_Upper, loop, deltatime))
			SetTorsoAnimation(TORSO_STAND);
		return true;
	}
	if(Animation == TORSO_ATTACK2)
	{
		loop = false;
		if (UpdateModel(&m_Upper, loop, deltatime))
			SetTorsoAnimation(TORSO_STAND2);
		return true;
	}
	else
		loop = true;
	return UpdateModel(&m_Upper, loop, deltatime);
}
bool CModelMD3::UpdateUpperModel(int weapon,float deltatime)
{
	int Animation = GetTorsoCurrentAnimation();
	bool loop = false;
	if(Animation == BOTH_DEATH1)
	{
		loop = false;
		if (UpdateModel(&m_Upper, loop, deltatime))
			SetTorsoAnimation(BOTH_DEAD1);
		return true;
	}
	if(Animation == TORSO_ATTACK)
	{
		loop = false;
		if (UpdateModel(&m_Upper, loop, deltatime))
			SetTorsoAnimation(TORSO_STAND);
		return true;
	}
	if(Animation == TORSO_DROP)
	{
		loop = false;
		if (UpdateModel(&m_Upper, loop, deltatime))
			if(!weapon)
				SetTorsoAnimation(TORSO_STAND2);
			else
				SetTorsoAnimation(TORSO_STAND);
		return true;
	}
	if(Animation == TORSO_ATTACK2)
	{
		loop = false;
		if (UpdateModel(&m_Upper, loop, deltatime))
			SetTorsoAnimation(TORSO_STAND2);
		return true;
	}
	else
		loop = true;
	return UpdateModel(&m_Upper, loop, deltatime);
}


void CModelMD3::DrawLink(int index,t3DModel *pModel,ShaderProgram* shader,const glm::mat4 &modelMatrix)
{
	RenderModel(index,pModel,shader,modelMatrix);
	CQuaternion qQuat, qNextQuat, qInterpolatedQuat;
	float *pMatrix, *pNextMatrix;
	float finalMatrix[16] = { 0 };

	for (int i = 0; i < pModel->numOfTags; i++)
	{
		t3DModel *pLink = pModel->pLinks[i];

		if (pLink)
		{
			CVector3 vPosition = pModel->pTags[pModel->currentFrame * pModel->numOfTags + i].vPosition;
			CVector3 vNextPosition = pModel->pTags[pModel->nextFrame * pModel->numOfTags + i].vPosition;
			vPosition.x = vPosition.x + pModel->interpol * (vNextPosition.x - vPosition.x),
				vPosition.y = vPosition.y + pModel->interpol * (vNextPosition.y - vPosition.y),
				vPosition.z = vPosition.z + pModel->interpol * (vNextPosition.z - vPosition.z);
			pMatrix = &pModel->pTags[pModel->currentFrame * pModel->numOfTags + i].rotation[0][0];
			pNextMatrix = &pModel->pTags[pModel->nextFrame * pModel->numOfTags + i].rotation[0][0];
			qQuat.CreateFromMatrix(pMatrix, 3);
			qNextQuat.CreateFromMatrix(pNextMatrix, 3);
			qInterpolatedQuat = qQuat.Slerp(qQuat, qNextQuat, pModel->interpol);
			qInterpolatedQuat.CreateMatrix(finalMatrix);
			finalMatrix[12] = vPosition.x;
			finalMatrix[13] = vPosition.y;
			finalMatrix[14] = vPosition.z;


			glm::mat4 modelmat = glm::mat4(1);
			for (size_t j = 0; j < 4; j++)
			{
				for (size_t k = 0; k < 4; k++)
				{
					modelmat[j][k] = finalMatrix[j * 4 + k];
				}
			}

			glm::mat4 curTransform = modelMatrix * modelmat;

			DrawLink(pLink->index,pLink,shader,curTransform);
		}
	}

}


void CModelMD3::DrawLink(t3DModel *pModel)
{
	RenderModel(pModel);
	CQuaternion qQuat, qNextQuat, qInterpolatedQuat;
	float *pMatrix, *pNextMatrix;
	float finalMatrix[16] = {0};

	for(int i = 0; i < pModel->numOfTags; i++)
	{
		t3DModel *pLink = pModel->pLinks[i];

		if(pLink)
		{			
			CVector3 vPosition = pModel->pTags[pModel->currentFrame * pModel->numOfTags + i].vPosition;
			CVector3 vNextPosition = pModel->pTags[pModel->nextFrame * pModel->numOfTags + i].vPosition;
			vPosition.x = vPosition.x + pModel->t * (vNextPosition.x - vPosition.x),
			vPosition.y	= vPosition.y + pModel->t * (vNextPosition.y - vPosition.y),
			vPosition.z	= vPosition.z + pModel->t * (vNextPosition.z - vPosition.z);			
			pMatrix = &pModel->pTags[pModel->currentFrame * pModel->numOfTags + i].rotation[0][0];
			pNextMatrix = &pModel->pTags[pModel->nextFrame * pModel->numOfTags + i].rotation[0][0];
			qQuat.CreateFromMatrix( pMatrix, 3);
			qNextQuat.CreateFromMatrix( pNextMatrix, 3 );
			qInterpolatedQuat = qQuat.Slerp(qQuat, qNextQuat, pModel->t);
			qInterpolatedQuat.CreateMatrix( finalMatrix );
			finalMatrix[12] = vPosition.x;
			finalMatrix[13] = vPosition.y;
			finalMatrix[14] = vPosition.z;

			glPushMatrix();
			glMultMatrixf( finalMatrix );

			DrawLink(pLink);
			glPopMatrix();
		}
	}

}

void CModelMD3::SetCurrentTime(t3DModel *pModel,float deltatime)
{
	/*float elapsedTime   = 0.0f;

	if(!pModel->pAnimations.size()) return;

	float time = (float)GetTickCount();

	elapsedTime = time - pModel->lastTime;
	*/
	int animationSpeed = pModel->pAnimations[pModel->currentAnim].framesPerSecond;

	//float t = elapsedTime * animationSpeed;
	/*if (elapsedTime >= animationSpeed)
	{
		pModel->currentFrame = pModel->nextFrame;
		pModel->lastTime = time;
	}*/
	pModel->t += deltatime;
	
	if (pModel->t - pModel->lastTime >= 1/float(animationSpeed))
	{
		pModel->currentFrame = pModel->nextFrame;
		pModel->lastTime = pModel->t;
	}

	pModel->interpol = float(animationSpeed)*(pModel->t - pModel->lastTime);
}

void CModelMD3::ConvertToModel(t3DModel* pModel, int index)
{
	for (size_t i = 0; i < pModel->pObject.size(); i++)
	{
		Model m;
		int textureID = pModel->pMaterials[pModel->pObject[i].materialID].texureId;
		for (size_t j = 0; j < pModel->pObject[i].numOfVerts; j++)
		{
			m.VertexData.push_back(glm::vec3(pModel->pObject[i].pVerts[j].x, pModel->pObject[i].pVerts[j].y, pModel->pObject[i].pVerts[j].z));
			//m.NormalsData.push_back(glm::vec3(pModel->pObject[i].pNormals[j].x, pModel->pObject[i].pNormals[j].y, pModel->pObject[i].pNormals[j].z));
			m.UVData.push_back(glm::vec2(pModel->pObject[i].pTexVerts[j].x, pModel->pObject[i].pTexVerts[j].y));

		}

		for (size_t j = 0; j < pModel->pObject[i].numOfFaces; j++)
		{
			for (int whichVertex = 0; whichVertex < 3; whichVertex++)
			{
				int index = pModel->pObject[i].pFaces[j].vertIndex[whichVertex];
				m.IndicesData.push_back(index);
			}
		}
		m.Initialize();
		if (index == 0)
		{
			mHead.push_back(m);
			mHeadTex.push_back(m_Textures[textureID]);
		}
		else if (index == 1)
		{
			mUpper.push_back(m);
			mUpperTex.push_back(m_Textures[textureID]);
		}
		else if (index == 2)
		{
			mLower.push_back(m);
			mLowerTex.push_back(m_Textures[textureID]);
		}
		else if (index == 3)
		{
			mWeapon.push_back(m);
			mWeaponTex.push_back(m_Textures[textureID]);
		}
	}
}


void CModelMD3::ComputeActualVertices()
{
	actualVertices.clear();
	TraverseLink(m_Lower.index, &m_Lower, glm::mat4(1));
}

void CModelMD3::TraverseLink(int index, t3DModel *pModel, const glm::mat4 &modelMatrix)
{
	TraverseModel(index, pModel, modelMatrix);
	CQuaternion qQuat, qNextQuat, qInterpolatedQuat;
	float *pMatrix, *pNextMatrix;
	float finalMatrix[16] = { 0 };

	for (int i = 0; i < pModel->numOfTags; i++)
	{
		t3DModel *pLink = pModel->pLinks[i];

		if (pLink)
		{
			CVector3 vPosition = pModel->pTags[pModel->currentFrame * pModel->numOfTags + i].vPosition;
			CVector3 vNextPosition = pModel->pTags[pModel->nextFrame * pModel->numOfTags + i].vPosition;
			vPosition.x = vPosition.x + pModel->interpol * (vNextPosition.x - vPosition.x),
				vPosition.y = vPosition.y + pModel->interpol * (vNextPosition.y - vPosition.y),
				vPosition.z = vPosition.z + pModel->interpol * (vNextPosition.z - vPosition.z);
			pMatrix = &pModel->pTags[pModel->currentFrame * pModel->numOfTags + i].rotation[0][0];
			pNextMatrix = &pModel->pTags[pModel->nextFrame * pModel->numOfTags + i].rotation[0][0];
			qQuat.CreateFromMatrix(pMatrix, 3);
			qNextQuat.CreateFromMatrix(pNextMatrix, 3);
			qInterpolatedQuat = qQuat.Slerp(qQuat, qNextQuat, pModel->interpol);
			qInterpolatedQuat.CreateMatrix(finalMatrix);
			finalMatrix[12] = vPosition.x;
			finalMatrix[13] = vPosition.y;
			finalMatrix[14] = vPosition.z;


			glm::mat4 modelmat = glm::mat4(1);
			for (size_t j = 0; j < 4; j++)
			{
				for (size_t k = 0; k < 4; k++)
				{
					modelmat[j][k] = finalMatrix[j * 4 + k];
				}
			}

			glm::mat4 curTransform = modelMatrix * modelmat;
			TraverseLink(pLink->index, pLink, curTransform);
		}
	}
}

void CModelMD3::TraverseModel(int index, t3DModel *pModel, const glm::mat4 &modelMatrix)
{
	vector<Model> toBeDrawn;
	vector<int> texes;
	if (index == 0)
	{
		toBeDrawn = mHead;
		texes = mHeadTex;
	}
	else if (index == 1)
	{
		toBeDrawn = mUpper;
		texes = mUpperTex;
	}

	else if (index == 2)
	{
		toBeDrawn = mLower;
		texes = mLowerTex;
	}
	else if (index == 3)
	{
		return;
		toBeDrawn = mWeapon;
		texes = mWeaponTex;
	}

	for (int i = 0; i < pModel->numOfObjects; i++)
	{
		t3DObject *pObject = &pModel->pObject[i];
		int currentIndex = pModel->currentFrame * pObject->numOfVerts;
		int nextIndex = pModel->nextFrame * pObject->numOfVerts;
		if (pObject->bHasTexture)
		{
			int textureID = pModel->pMaterials[pObject->materialID].texureId;
			glBindTexture(GL_TEXTURE_2D, m_Textures[textureID]);
		}

		for (int j = 0; j < pObject->numOfFaces; j++)
		{
			for (int whichVertex = 0; whichVertex < 3; whichVertex++)
			{
				int index = pObject->pFaces[j].vertIndex[whichVertex];
				CVector3 vPoint1 = pObject->pVerts[currentIndex + index];
				CVector3 vPoint2 = pObject->pVerts[nextIndex + index];
				glm::vec3 v = glm::vec3(vPoint1.x + pModel->interpol * (vPoint2.x - vPoint1.x),
					vPoint1.y + pModel->interpol * (vPoint2.y - vPoint1.y),
					vPoint1.z + pModel->interpol * (vPoint2.z - vPoint1.z));
				if (index >= toBeDrawn[i].VertexData.size())
					continue;
				toBeDrawn[i].VertexData[index] = v;
			}
		}

		toBeDrawn[i].Update();


		glm::mat4 curTransform = modelMatrix;
		for (size_t j = 0; j < toBeDrawn[i].VertexData.size(); j++)
		{
			glm::vec4 t = curTransform*glm::vec4(toBeDrawn[i].VertexData[j], 1);
			actualVertices.push_back(glm::vec3(t.x,t.y,t.z));
		}
	}
}

void CModelMD3::RenderModel(int index, t3DModel *pModel,ShaderProgram* shader,const glm::mat4 &modelMatrix)
{
	vector<Model> toBeDrawn;
	vector<int> texes;
	if (index == 0)
	{
		toBeDrawn = mHead;
		texes = mHeadTex;
	}
	else if (index == 1)
	{
		toBeDrawn = mUpper;
		texes = mUpperTex;
	}

	else if (index == 2)
	{
		toBeDrawn = mLower;
		texes = mLowerTex;
	}
	else if (index == 3)
	{
		toBeDrawn = mWeapon;
		texes = mWeaponTex;
	}
	
	for (int i = 0; i < pModel->numOfObjects; i++)
	{
		t3DObject *pObject = &pModel->pObject[i];
		int currentIndex = pModel->currentFrame * pObject->numOfVerts;
		int nextIndex = pModel->nextFrame * pObject->numOfVerts;
		if (pObject->bHasTexture)
		{
			int textureID = pModel->pMaterials[pObject->materialID].texureId;
			glBindTexture(GL_TEXTURE_2D, m_Textures[textureID]);
		}

		for (int j = 0; j < pObject->numOfFaces; j++)
		{
			for (int whichVertex = 0; whichVertex < 3; whichVertex++)
			{
				int index = pObject->pFaces[j].vertIndex[whichVertex];
				CVector3 vPoint1 = pObject->pVerts[currentIndex+index];
				CVector3 vPoint2 = pObject->pVerts[nextIndex + index];
				glm::vec3 v = glm::vec3(vPoint1.x + pModel->interpol * (vPoint2.x - vPoint1.x),
					vPoint1.y + pModel->interpol * (vPoint2.y - vPoint1.y),
					vPoint1.z + pModel->interpol * (vPoint2.z - vPoint1.z));
				if (index >= toBeDrawn[i].VertexData.size())
					continue;
				toBeDrawn[i].VertexData[index] = v;
			}
		}
		
		toBeDrawn[i].Update();


		glm::mat4 curTransform = modelMatrix;
		shader->BindModelMatrix(&curTransform[0][0]);
		toBeDrawn[i].Draw();
	}

}

void CModelMD3::RenderModel(t3DModel *pModel)
{
	if(pModel->pObject.size() <= 0) return;
	for(int i = 0; i < pModel->numOfObjects; i++)
	{
		t3DObject *pObject = &pModel->pObject[i];
		int currentIndex = pModel->currentFrame * pObject->numOfVerts; 
		int nextIndex = pModel->nextFrame * pObject->numOfVerts; 
		if(pObject->bHasTexture)
		{
			glEnable(GL_TEXTURE_2D);
			int textureID = pModel->pMaterials[pObject->materialID].texureId;
			glBindTexture(GL_TEXTURE_2D, m_Textures[textureID]);
		}
		else
		{
			glDisable(GL_TEXTURE_2D);
		}
		glBegin(GL_TRIANGLES);
		for(int j = 0; j < pObject->numOfFaces; j++)
		{
			for(int whichVertex = 0; whichVertex < 3; whichVertex++)
			{
				int index = pObject->pFaces[j].vertIndex[whichVertex];
				if(pObject->pTexVerts) 
				{
					glTexCoord2f(pObject->pTexVerts[ index ].x, 
						pObject->pTexVerts[ index ].y);
				}
				CVector3 vPoint1 = pObject->pVerts[ currentIndex + index ];
				CVector3 vPoint2 = pObject->pVerts[ nextIndex + index];

				glVertex3f(vPoint1.x + pModel->t * (vPoint2.x - vPoint1.x),
					vPoint1.y + pModel->t * (vPoint2.y - vPoint1.y),
					vPoint1.z + pModel->t * (vPoint2.z - vPoint1.z));
			}
		}
		glEnd();
	}
}

void CModelMD3::SetTorsoAnimation(LPSTR strAnimation)
{
	for(int i = 0; i < m_Upper.numOfAnimations; i++)
	{
		if( !_stricmp(m_Upper.pAnimations[i].strName, strAnimation) )
		{
			m_Upper.currentAnim = i;
			m_Upper.currentFrame = m_Upper.pAnimations[m_Upper.currentAnim].startFrame;
			return;
		}
	}
}
void CModelMD3::SetTorsoAnimation(int AnimationNumber)
{
	m_Upper.currentAnim = AnimationNumber;
	m_Upper.currentFrame = m_Upper.pAnimations[m_Upper.currentAnim].startFrame;
}
int CModelMD3::GetTorsoCurrentAnimation()
{
	return m_Upper.currentAnim;
}
int CModelMD3::GetLegsCurrentAnimation()
{
	return m_Lower.currentAnim;
}

void CModelMD3::SetLegsAnimation(LPSTR strAnimation)
{
	for(int i = 0; i < m_Lower.numOfAnimations; i++)
	{
		if( !_stricmp(m_Lower.pAnimations[i].strName, strAnimation) )
		{
			m_Lower.currentAnim = i;
			m_Lower.currentFrame = m_Lower.pAnimations[m_Lower.currentAnim].startFrame;
			return;
		}
	}
}
void CModelMD3::SetLegsAnimation(int AnimationNumber)
{
	if(AnimationNumber > 5)
		m_Lower.currentAnim = AnimationNumber - LegConvertor;
	else
		m_Lower.currentAnim = AnimationNumber;
	m_Lower.currentFrame = m_Lower.pAnimations[m_Lower.currentAnim].startFrame;
}
CLoadMD3::CLoadMD3()
{
	memset(&m_Header, 0, sizeof(tMd3Header));

	// Set the pointers to null
	m_pSkins=NULL;
	m_pTexCoords=NULL;
	m_pTriangles=NULL;
	m_pBones=NULL;
}

bool CLoadMD3::ImportMD3(t3DModel *pModel, char *strFileName)
{
	char strMessage[255] = {0};

	m_FilePointer = fopen(strFileName, "rb");
	if(!m_FilePointer) 
	{
		sprintf(strMessage, "Unable to find the file: %s!", strFileName);
		MessageBox(NULL, strMessage, "Error", MB_OK);
		return false;
	}
	fread(&m_Header, 1, sizeof(tMd3Header), m_FilePointer);
	char *ID = m_Header.fileID;
	if((ID[0] != 'I' || ID[1] != 'D' || ID[2] != 'P' || ID[3] != '3') || m_Header.version != 15)
	{
		sprintf(strMessage, "Invalid file format (Version not 15): %s!", strFileName);
		MessageBox(NULL, strMessage, "Error", MB_OK);
		return false;
	}
	ReadMD3Data(pModel);
	CleanUp();
	return true;
}

void CLoadMD3::ReadMD3Data(t3DModel *pModel)
{
	int i = 0;

		m_pBones = new tMd3Bone [m_Header.numFrames];
	fread(m_pBones, sizeof(tMd3Bone), m_Header.numFrames, m_FilePointer);
	delete [] m_pBones;
	pModel->pTags = new tMd3Tag [m_Header.numFrames * m_Header.numTags];
	fread(pModel->pTags, sizeof(tMd3Tag), m_Header.numFrames * m_Header.numTags, m_FilePointer);
	pModel->numOfTags = m_Header.numTags;
	
	pModel->pLinks = (t3DModel **) malloc(sizeof(t3DModel) * m_Header.numTags);
	
	for (i = 0; i < m_Header.numTags; i++)
		pModel->pLinks[i] = NULL;

	long meshOffset = ftell(m_FilePointer);

	tMd3MeshInfo meshHeader;

	for (i = 0; i < m_Header.numMeshes; i++)
	{
		fseek(m_FilePointer, meshOffset, SEEK_SET);
		fread(&meshHeader, sizeof(tMd3MeshInfo), 1, m_FilePointer);

		m_pSkins     = new tMd3Skin [meshHeader.numSkins];
		m_pTexCoords = new tMd3TexCoord [meshHeader.numVertices];
		m_pTriangles = new tMd3Face [meshHeader.numTriangles];
		m_pVertices  = new tMd3Triangle [meshHeader.numVertices * meshHeader.numMeshFrames];

		fread(m_pSkins, sizeof(tMd3Skin), meshHeader.numSkins, m_FilePointer);
		
		fseek(m_FilePointer, meshOffset + meshHeader.triStart, SEEK_SET);
		fread(m_pTriangles, sizeof(tMd3Face), meshHeader.numTriangles, m_FilePointer);

		fseek(m_FilePointer, meshOffset + meshHeader.uvStart, SEEK_SET);
		fread(m_pTexCoords, sizeof(tMd3TexCoord), meshHeader.numVertices, m_FilePointer);

		fseek(m_FilePointer, meshOffset + meshHeader.vertexStart, SEEK_SET);
		fread(m_pVertices, sizeof(tMd3Triangle), meshHeader.numMeshFrames * meshHeader.numVertices, m_FilePointer);
		ConvertDataStructures(pModel, meshHeader);

		delete [] m_pSkins;    
		delete [] m_pTexCoords;
		delete [] m_pTriangles;
		delete [] m_pVertices;   

		meshOffset += meshHeader.meshSize;
	}
}


void CLoadMD3::ConvertDataStructures(t3DModel *pModel, tMd3MeshInfo meshHeader)
{
	int i = 0;
	pModel->numOfObjects++;
	t3DObject currentMesh = {0};
	strcpy(currentMesh.strName, meshHeader.strName);

	currentMesh.numOfVerts   = meshHeader.numVertices;
	currentMesh.numTexVertex = meshHeader.numVertices;
	currentMesh.numOfFaces   = meshHeader.numTriangles;
	currentMesh.pVerts    = new CVector3 [currentMesh.numOfVerts * meshHeader.numMeshFrames];
	currentMesh.pTexVerts = new CVector2 [currentMesh.numOfVerts];
	currentMesh.pFaces    = new tFace [currentMesh.numOfFaces];
	for (i=0; i < currentMesh.numOfVerts * meshHeader.numMeshFrames; i++)
	{
		currentMesh.pVerts[i].x =  m_pVertices[i].vertex[0] / 64.0f;
		currentMesh.pVerts[i].y =  m_pVertices[i].vertex[1] / 64.0f;
		currentMesh.pVerts[i].z =  m_pVertices[i].vertex[2] / 64.0f;
	}

	for (i=0; i < currentMesh.numTexVertex; i++)
	{
		currentMesh.pTexVerts[i].x =  m_pTexCoords[i].textureCoord[0];
		currentMesh.pTexVerts[i].y = -m_pTexCoords[i].textureCoord[1];
	}
	for(i=0; i < currentMesh.numOfFaces; i++)
	{
		currentMesh.pFaces[i].vertIndex[0] = m_pTriangles[i].vertexIndices[0];
		currentMesh.pFaces[i].vertIndex[1] = m_pTriangles[i].vertexIndices[1];
		currentMesh.pFaces[i].vertIndex[2] = m_pTriangles[i].vertexIndices[2];
		currentMesh.pFaces[i].coordIndex[0] = m_pTriangles[i].vertexIndices[0];
		currentMesh.pFaces[i].coordIndex[1] = m_pTriangles[i].vertexIndices[1];
		currentMesh.pFaces[i].coordIndex[2] = m_pTriangles[i].vertexIndices[2];
	}
	pModel->pObject.push_back(currentMesh);
}

bool CLoadMD3::LoadSkin(t3DModel *pModel, LPSTR strSkin)
{
	if(!pModel || !strSkin) return false;

	ifstream fin(strSkin);

	if(fin.fail())
	{
		MessageBox(NULL, "Unable to load skin!", "Error", MB_OK);
		return false;
	}

	string strLine = "";
	int textureNameStart = 0;

	while(getline(fin, strLine))
	{
		for(int i = 0; i < pModel->numOfObjects; i++)
		{
			if( IsInString(strLine, pModel->pObject[i].strName) )			
			{			
				for(int j = strLine.length() - 1; j > 0; j--)
				{
					if(strLine[j] == '/')
					{
						textureNameStart = j + 1;
						break;
					}	
				}
				tMaterialInfo texture;
				strcpy(texture.strFile, &strLine[textureNameStart]);
				texture.uTile = texture.uTile = 1;
				pModel->pObject[i].materialID = pModel->numOfMaterials;
				pModel->pObject[i].bHasTexture = true;
				pModel->numOfMaterials++;
				pModel->pMaterials.push_back(texture);
			}
		}
	}

	fin.close();
	return true;
}





bool CLoadMD3::LoadShader(t3DModel *pModel, LPSTR strShader)
{
	if(!pModel || !strShader) return false;

	ifstream fin(strShader);

	if(fin.fail())
	{
		MessageBox(NULL, "Unable to load shader!", "Error", MB_OK);
		return false;
	}

	string strLine = "";
	int currentIndex = 0;
	
	while(getline(fin, strLine))
	{
		tMaterialInfo texture;

		strcpy(texture.strFile, strLine.c_str());
				
		texture.uTile = texture.uTile = 1;

		pModel->pObject[currentIndex].materialID = pModel->numOfMaterials;
		pModel->pObject[currentIndex].bHasTexture = true;

		pModel->numOfMaterials++;

		pModel->pMaterials.push_back(texture);

		currentIndex++;
	}

	fin.close();
	return true;
}

void CLoadMD3::CleanUp()
{
	fclose(m_FilePointer);						
}