
#include "md2model.h"

anim_t CMD2Model::animlist[ 21 ] = 
{
	// first, last, fps
	{   0,  30,  9 },	// STAND
	{  40,  45, 10 },	// RUN
	{  46,  53, 10 },	// ATTACK
	{  54,  57,  7 },	// PAIN_A
	{  58,  61,  7 },	// PAIN_B
	{  62,  65,  7 },	// PAIN_C
	{  66,  71,  7 },	// JUMP
	{  72,  83,  7 },	// FLIP
	{  84,  94,  7 },	// SALUTE
	{  95, 111, 10 },	// FALLBACK
	{ 112, 122,  7 },	// WAVE
	{ 123, 134,  6 },	// POINTIING
	{ 135, 153, 10 },	// CROUCH_STAND
	{ 154, 159,  7 },	// CROUCH_WALK
	{ 160, 168, 10 },	// CROUCH_ATTACK
	{ 196, 172,  7 },	// CROUCH_PAIN
	{ 173, 177,  5 },	// CROUCH_DEATH
	{ 178, 183,  7 },	// DEATH_FALLBACK
	{ 184, 189,  7 },	// DEATH_FALLFORWARD
	{ 190, 197,  7 },	// DEATH_FALLBACKSLOW
	{ 198, 198,  5 }	// BOOM
};



string sMD2AnimationNames[MAX_ANIMATIONS] = 
{
	"Stand",
	"Run",
	"Attack",
	"Pain A",
	"Pain B",
	"Pain C",
	"Jump",
	"Flip",
	"Salute",
	"Fallback",
	"Wave",
	"Pointing",
	"Crouch Stand",
	"Crouch Walk",
	"Crouch Attack",
	"Crouch Pain",
	"Crouch Death",
	"Death Fallback",
	"Death Fall Forward",
	"Death Fallback Slow",
	"Boom"
};

#pragma warning( once : 4305 )

#pragma  region precomputed Normals
vec3_t anorms[NUMVERTEXNORMALS] = 
{
	{-0.525731, 0.000000, 0.850651}, 
	{-0.442863, 0.238856, 0.864188}, 
	{-0.295242, 0.000000, 0.955423}, 
	{-0.309017, 0.500000, 0.809017}, 
	{-0.162460, 0.262866, 0.951056}, 
	{0.000000, 0.000000, 1.000000}, 
	{0.000000, 0.850651, 0.525731}, 
	{-0.147621, 0.716567, 0.681718}, 
	{0.147621, 0.716567, 0.681718}, 
	{0.000000, 0.525731, 0.850651}, 
	{0.309017, 0.500000, 0.809017}, 
	{0.525731, 0.000000, 0.850651}, 
	{0.295242, 0.000000, 0.955423}, 
	{0.442863, 0.238856, 0.864188}, 
	{0.162460, 0.262866, 0.951056}, 
	{-0.681718, 0.147621, 0.716567}, 
	{-0.809017, 0.309017, 0.500000}, 
	{-0.587785, 0.425325, 0.688191}, 
	{-0.850651, 0.525731, 0.000000}, 
	{-0.864188, 0.442863, 0.238856}, 
	{-0.716567, 0.681718, 0.147621}, 
	{-0.688191, 0.587785, 0.425325}, 
	{-0.500000, 0.809017, 0.309017}, 
	{-0.238856, 0.864188, 0.442863}, 
	{-0.425325, 0.688191, 0.587785}, 
	{-0.716567, 0.681718, -0.147621}, 
	{-0.500000, 0.809017, -0.309017}, 
	{-0.525731, 0.850651, 0.000000}, 
	{0.000000, 0.850651, -0.525731}, 
	{-0.238856, 0.864188, -0.442863}, 
	{0.000000, 0.955423, -0.295242}, 
	{-0.262866, 0.951056, -0.162460}, 
	{0.000000, 1.000000, 0.000000}, 
	{0.000000, 0.955423, 0.295242}, 
	{-0.262866, 0.951056, 0.162460}, 
	{0.238856, 0.864188, 0.442863}, 
	{0.262866, 0.951056, 0.162460}, 
	{0.500000, 0.809017, 0.309017}, 
	{0.238856, 0.864188, -0.442863}, 
	{0.262866, 0.951056, -0.162460}, 
	{0.500000, 0.809017, -0.309017}, 
	{0.850651, 0.525731, 0.000000}, 
	{0.716567, 0.681718, 0.147621}, 
	{0.716567, 0.681718, -0.147621}, 
	{0.525731, 0.850651, 0.000000}, 
	{0.425325, 0.688191, 0.587785}, 
	{0.864188, 0.442863, 0.238856}, 
	{0.688191, 0.587785, 0.425325}, 
	{0.809017, 0.309017, 0.500000}, 
	{0.681718, 0.147621, 0.716567}, 
	{0.587785, 0.425325, 0.688191}, 
	{0.955423, 0.295242, 0.000000}, 
	{1.000000, 0.000000, 0.000000}, 
	{0.951056, 0.162460, 0.262866}, 
	{0.850651, -0.525731, 0.000000}, 
	{0.955423, -0.295242, 0.000000}, 
	{0.864188, -0.442863, 0.238856}, 
	{0.951056, -0.162460, 0.262866}, 
	{0.809017, -0.309017, 0.500000}, 
	{0.681718, -0.147621, 0.716567}, 
	{0.850651, 0.000000, 0.525731}, 
	{0.864188, 0.442863, -0.238856}, 
	{0.809017, 0.309017, -0.500000}, 
	{0.951056, 0.162460, -0.262866}, 
	{0.525731, 0.000000, -0.850651}, 
	{0.681718, 0.147621, -0.716567}, 
	{0.681718, -0.147621, -0.716567}, 
	{0.850651, 0.000000, -0.525731}, 
	{0.809017, -0.309017, -0.500000}, 
	{0.864188, -0.442863, -0.238856}, 
	{0.951056, -0.162460, -0.262866}, 
	{0.147621, 0.716567, -0.681718}, 
	{0.309017, 0.500000, -0.809017}, 
	{0.425325, 0.688191, -0.587785}, 
	{0.442863, 0.238856, -0.864188}, 
	{0.587785, 0.425325, -0.688191}, 
	{0.688191, 0.587785, -0.425325}, 
	{-0.147621, 0.716567, -0.681718}, 
	{-0.309017, 0.500000, -0.809017}, 
	{0.000000, 0.525731, -0.850651}, 
	{-0.525731, 0.000000, -0.850651}, 
	{-0.442863, 0.238856, -0.864188}, 
	{-0.295242, 0.000000, -0.955423}, 
	{-0.162460, 0.262866, -0.951056}, 
	{0.000000, 0.000000, -1.000000}, 
	{0.295242, 0.000000, -0.955423}, 
	{0.162460, 0.262866, -0.951056}, 
	{-0.442863, -0.238856, -0.864188}, 
	{-0.309017, -0.500000, -0.809017}, 
	{-0.162460, -0.262866, -0.951056}, 
	{0.000000, -0.850651, -0.525731}, 
	{-0.147621, -0.716567, -0.681718}, 
	{0.147621, -0.716567, -0.681718}, 
	{0.000000, -0.525731, -0.850651}, 
	{0.309017, -0.500000, -0.809017}, 
	{0.442863, -0.238856, -0.864188}, 
	{0.162460, -0.262866, -0.951056}, 
	{0.238856, -0.864188, -0.442863}, 
	{0.500000, -0.809017, -0.309017}, 
	{0.425325, -0.688191, -0.587785}, 
	{0.716567, -0.681718, -0.147621}, 
	{0.688191, -0.587785, -0.425325}, 
	{0.587785, -0.425325, -0.688191}, 
	{0.000000, -0.955423, -0.295242}, 
	{0.000000, -1.000000, 0.000000}, 
	{0.262866, -0.951056, -0.162460}, 
	{0.000000, -0.850651, 0.525731}, 
	{0.000000, -0.955423, 0.295242}, 
	{0.238856, -0.864188, 0.442863}, 
	{0.262866, -0.951056, 0.162460}, 
	{0.500000, -0.809017, 0.309017}, 
	{0.716567, -0.681718, 0.147621}, 
	{0.525731, -0.850651, 0.000000}, 
	{-0.238856, -0.864188, -0.442863}, 
	{-0.500000, -0.809017, -0.309017}, 
	{-0.262866, -0.951056, -0.162460}, 
	{-0.850651, -0.525731, 0.000000}, 
	{-0.716567, -0.681718, -0.147621}, 
	{-0.716567, -0.681718, 0.147621}, 
	{-0.525731, -0.850651, 0.000000}, 
	{-0.500000, -0.809017, 0.309017}, 
	{-0.238856, -0.864188, 0.442863}, 
	{-0.262866, -0.951056, 0.162460}, 
	{-0.864188, -0.442863, 0.238856}, 
	{-0.809017, -0.309017, 0.500000}, 
	{-0.688191, -0.587785, 0.425325}, 
	{-0.681718, -0.147621, 0.716567}, 
	{-0.442863, -0.238856, 0.864188}, 
	{-0.587785, -0.425325, 0.688191}, 
	{-0.309017, -0.500000, 0.809017}, 
	{-0.147621, -0.716567, 0.681718}, 
	{-0.425325, -0.688191, 0.587785}, 
	{-0.162460, -0.262866, 0.951056}, 
	{0.442863, -0.238856, 0.864188}, 
	{0.162460, -0.262866, 0.951056}, 
	{0.309017, -0.500000, 0.809017}, 
	{0.147621, -0.716567, 0.681718}, 
	{0.000000, -0.525731, 0.850651}, 
	{0.425325, -0.688191, 0.587785}, 
	{0.587785, -0.425325, 0.688191}, 
	{0.688191, -0.587785, 0.425325}, 
	{-0.955423, 0.295242, 0.000000}, 
	{-0.951056, 0.162460, 0.262866}, 
	{-1.000000, 0.000000, 0.000000}, 
	{-0.850651, 0.000000, 0.525731}, 
	{-0.955423, -0.295242, 0.000000}, 
	{-0.951056, -0.162460, 0.262866}, 
	{-0.864188, 0.442863, -0.238856}, 
	{-0.951056, 0.162460, -0.262866}, 
	{-0.809017, 0.309017, -0.500000}, 
	{-0.864188, -0.442863, -0.238856}, 
	{-0.951056, -0.162460, -0.262866}, 
	{-0.809017, -0.309017, -0.500000}, 
	{-0.681718, 0.147621, -0.716567}, 
	{-0.681718, -0.147621, -0.716567}, 
	{-0.850651, 0.000000, -0.525731}, 
	{-0.688191, 0.587785, -0.425325}, 
	{-0.587785, 0.425325, -0.688191}, 
	{-0.425325, 0.688191, -0.587785}, 
	{-0.425325, -0.688191, -0.587785}, 
	{-0.587785, -0.425325, -0.688191}, 
	{-0.688191, -0.587785, -0.425325}, 
};
#pragma endregion

/*-----------------------------------------------

Name:	LoadModel

Params:	sFilename - md2 model file path

Result:	Loads MD2 model from file.

/*---------------------------------------------*/

void CMD2Model::LoadExportedModel(char* sFilename)
{
	FILE* fp = fopen(sFilename, "rb");

	fread(&header, sizeof(md2_t), 1, fp); // Read header where all info about model is stored

	char* buffer = new char[header.num_frames * header.framesize]; // Read all frame data to one big buffer
	fseek(fp, header.ofs_frames, SEEK_SET);
	fread(buffer, sizeof(char), header.num_frames * header.framesize, fp);

	vVertices.resize(header.num_frames, vector<glm::vec3>(header.num_xyz)); // Allocate space for vertices
	vNormals.resize(header.num_frames, vector<int>(header.num_xyz)); // And normals

	// Extract vertices and normals from frame data
	for (int i = 0; i < header.num_frames; i++)
	{
		frame_t* frame_ptr = (frame_t*)&buffer[header.framesize * i]; // Convert buffer to frame_t pointer
		for (int j = 0; j < header.num_xyz; j++)
		{
			vVertices[i][j].x = frame_ptr->translate[0] + (float(frame_ptr->verts[j].v[0]) * frame_ptr->scale[0]);
			vVertices[i][j].y = frame_ptr->translate[1] + (float(frame_ptr->verts[j].v[1]) * frame_ptr->scale[1]);
			vVertices[i][j].z = frame_ptr->translate[2] + (float(frame_ptr->verts[j].v[2]) * frame_ptr->scale[2]);

			vNormals[i][j] = frame_ptr->verts[j].lightnormalindex;
		}
	}
	vector<glm::vec2> uv;
	int position = header.ofs_st;

	fseek(fp, position, SEEK_SET);
	vector<short> uvdata;
	uvdata.resize(header.num_st * 2);
	fread(&uvdata[0], sizeof(short)*2, header.num_st, fp);
	for (int i = 0; i < header.num_st*2; i+=2)
		uv.push_back(glm::vec2(float(uvdata[i]) / header.skinwidth, (float(uvdata[i+1]) / header.skinheight)));
	int x = header.num_xyz - header.num_st;
	if (x > 0)
		for (size_t i = 0; i < x; i++)
		{
			uv.push_back(glm::vec2(0));
		}
	mVertices = vVertices[0];
	
	position = header.ofs_tris;

	fseek(fp, position, SEEK_SET);
	vector<unsigned short> trisData;
	trisData.resize(header.num_tris * 6);
	fread(&trisData[0], sizeof(unsigned short)* 6, header.num_tris, fp);
	for (int i = 0; i < header.num_tris * 6; i += 6)
	{
		int a = trisData[i];
		int b = trisData[i+1];
		int c = trisData[i+2];
		int ta = trisData[i+3];
		int tb = trisData[i+4];
		int tc = trisData[i+5];
		m.IndicesData.push_back(a);
		m.IndicesData.push_back(b);
		m.IndicesData.push_back(c);
		/*m.VertexData.push_back(vVertices[0][a]);
		m.VertexData.push_back(vVertices[0][b]);
		m.VertexData.push_back(vVertices[0][c]);
		m.UVData.push_back(uv[ta]);
		m.UVData.push_back(uv[tb]);
		m.UVData.push_back(uv[tc]);
		m.NormalsData.push_back(glm::vec3(anorms[vNormals[0][a]][0], anorms[vNormals[0][a]][1], anorms[vNormals[0][a]][2]));
		m.NormalsData.push_back(glm::vec3(anorms[vNormals[0][b]][0], anorms[vNormals[0][b]][1], anorms[vNormals[0][b]][2]));
		m.NormalsData.push_back(glm::vec3(anorms[vNormals[0][c]][0], anorms[vNormals[0][c]][1], anorms[vNormals[0][c]][2]));*/
	}

	m.VertexData = vVertices[0];
	for (size_t i = 0; i < vNormals[0].size(); i++)
	{
		m.NormalsData.push_back(glm::vec3(anorms[vNormals[0][i]][0], anorms[vNormals[0][i]][1], anorms[vNormals[0][i]][2]));
	}
	m.UVData = uv;
	m.Initialize();

	//int i = 0;
	//int iTotalVertices = 0;

	//// And start with creating VBOs for vertices, textue coordinates and normals
	//vboFrameVertices.resize(header.num_frames);
	//for (int i = 0; i < header.num_frames; i++)
	//	vboFrameVertices[i].CreateVBO();

	//vboTextureCoords.CreateVBO();

	//int renderMode = GL_TRIANGLE_STRIP; // Extract rendering mode

	//renderModes.push_back(renderMode); // Remember the values

	//int numVertices = vVertices[0].size();
	//numRenderVertices.push_back(numVertices);

	//for (int j = 0; j < numVertices; j++)
	//{
	//	float s = uv[j].x; // Extract texture coordinates
	//	float t = uv[j].y;

	//	vboTextureCoords.AddData(&s, 4); // Add texture coords to VBO
	//	vboTextureCoords.AddData(&t, 4);


	//	int vi = j;

	//	for (int k = 0; k < header.num_frames; k++)
	//	{
	//		vboFrameVertices[k].AddData(&vVertices[k][vi], 12); // Add vertex to VBO
	//		vboFrameVertices[k].AddData(&anorms[vNormals[k][vi]], 12); // Add normal to VBO from normal table
	//	}
	//}



	//// Now all necessary data are extracted, let's create VAO for rendering MD2 model

	//glGenVertexArrays(1, &uiVAO);
	//glBindVertexArray(uiVAO);

	//for (int i = 0; i < header.num_frames; i++)
	//{
	//	vboFrameVertices[i].BindVBO();
	//	vboFrameVertices[i].UploadDataToGPU(GL_STATIC_DRAW);
	//}

	//vboFrameVertices[0].BindVBO(); // Vertex and normals data parameters

	//// Vertex positions
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), 0);
	//glEnableVertexAttribArray(3); // Vertices for next keyframe, now we can set it to same VBO
	//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), 0);
	//// Normal vectors
	//glEnableVertexAttribArray(2);
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)(sizeof(glm::vec3)));
	//glEnableVertexAttribArray(4); // Normals for next keyframe, now we can set it to same VBO
	//glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (void*)(sizeof(glm::vec3)));

	//// Texture coordinates
	//vboTextureCoords.BindVBO();
	//vboTextureCoords.UploadDataToGPU(GL_STATIC_DRAW);

	//// Texture coordinates
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);

	
	string sPath = sFilename;
	int index = sPath.find_last_of("\\/");
	string sDirectory = index != -1 ? sPath.substr(0, index + 1) : "";
	string sPureFilename = index != -1 ? sPath.substr(index + 1) : sFilename;

	string sTextureExtensions[] = { "jpg", "jpeg", "png", "bmp", "tga" };
	index = sPureFilename.find_last_of(".");
	if (index != -1)
	{
		string sStripped = sPureFilename.substr(0, index + 1);
		for (int i = 0; i < 5; i++)
		{
			string sTry = sDirectory + sStripped + sTextureExtensions[i];
			tSkin = new Texture(sTry, 0);
			if (tSkin->width > 0)
				break;
		}
	}
	fclose(fp);

}

void CMD2Model::LoadModel(char* sFilename, bool flipUVCoords /*= false*/)
{
	FILE* fp = fopen(sFilename, "rb");
	
	fread(&header, sizeof(md2_t), 1, fp); // Read header where all info about model is stored

	char* buffer = new char[header.num_frames * header.framesize]; // Read all frame data to one big buffer
	fseek(fp, header.ofs_frames, SEEK_SET);
	fread(buffer, sizeof(char), header.num_frames * header.framesize, fp);

	vVertices.resize(header.num_frames, vector<glm::vec3>(header.num_xyz)); // Allocate space for vertices
	vNormals.resize(header.num_frames, vector<int>(header.num_xyz)); // And normals

	// Extract vertices and normals from frame data
	for (int i = 0; i < header.num_frames; i++)
	{
		frame_t* frame_ptr = (frame_t*)&buffer[header.framesize * i]; // Convert buffer to frame_t pointer
		for (int j = 0; j < header.num_xyz; j++)
		{
			vVertices[i][j].x = frame_ptr->translate[0] + (float(frame_ptr->verts[j].v[0]) * frame_ptr->scale[0]);
			vVertices[i][j].y = frame_ptr->translate[1] + (float(frame_ptr->verts[j].v[1]) * frame_ptr->scale[1]);
			vVertices[i][j].z = frame_ptr->translate[2] + (float(frame_ptr->verts[j].v[2]) * frame_ptr->scale[2]);

			vNormals[i][j] = frame_ptr->verts[j].lightnormalindex;
		}
	}

	// Now let's read OpenGL rendering commands
	glCommands.resize(header.num_glcmds);
	fseek(fp, header.ofs_glcmds, SEEK_SET);
	if (glCommands.size()!=0)
		fread(&glCommands[0], sizeof(int), header.num_glcmds, fp);
	
	int i = 0;
	int iTotalVertices = 0;

	// And start with creating VBOs for vertices, textue coordinates and normals
	vboFrameVertices.resize(header.num_frames);
	for (int i = 0; i < header.num_frames; i++)
		vboFrameVertices[i].CreateVBO();

	vboTextureCoords.CreateVBO();
	
	while (1) // We while loop until we come to 0 value, which is the end of OpenGL commands
	{
		int action;
		if (glCommands.size() != 0)
		{
			action = glCommands[i];
			if (action == 0)break;
		}
		else
			action = 1;

		int renderMode = action < 0 ? GL_TRIANGLE_FAN : GL_TRIANGLE_STRIP; // Extract rendering mode
		int numVertices = action < 0 ? -action : action; // And number of vertices
		i++;

		renderModes.push_back(renderMode); // Remember the values
		numRenderVertices.push_back(numVertices);

		for (int j = 0; j < numVertices; j++)
		{
			float s = *((float*)(&glCommands[i++])); // Extract texture coordinates
			float t = *((float*)(&glCommands[i++]));
			if(flipUVCoords == true)
				t = 1.0f - t; // Flip t, because it is (from some reasons) stored from top to bottom
			int vi = glCommands[i++];

			vboTextureCoords.AddData(&s, 4); // Add texture coords to VBO
			vboTextureCoords.AddData(&t, 4);

			for (int k = 0; k < header.num_frames; k++)
			{
				vboFrameVertices[k].AddData(&vVertices[k][vi], 12); // Add vertex to VBO
				vboFrameVertices[k].AddData(&anorms[vNormals[k][vi]], 12); // Add normal to VBO from normal table
			}
		}
	}


	mVertices = vVertices[0];





	// Now all necessary data are extracted, let's create VAO for rendering MD2 model

	glGenVertexArrays(1, &uiVAO);
	glBindVertexArray(uiVAO);

	for (int i = 0; i < header.num_frames; i++)
	{
		vboFrameVertices[i].BindVBO();
		vboFrameVertices[i].UploadDataToGPU(GL_STATIC_DRAW);
	}

	vboFrameVertices[0].BindVBO(); // Vertex and normals data parameters
	
	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3), 0);
	glEnableVertexAttribArray(3); // Vertices for next keyframe, now we can set it to same VBO
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3), 0);
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3), (void*)(sizeof(glm::vec3)));
	glEnableVertexAttribArray(4); // Normals for next keyframe, now we can set it to same VBO
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3), (void*)(sizeof(glm::vec3)));

	// Texture coordinates
	vboTextureCoords.BindVBO();
	vboTextureCoords.UploadDataToGPU(GL_STATIC_DRAW);

	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
	
	
	// I have read, that if you read the data from header.num_skins and header.ofs_skins,
	// these data are Quake2 specific paths. So usually you will find models on internet
	// with header.num_skins 0 and texture with the same filename as model filename

	// Find texture name (modelname.jpg, modelname.png...)

	string sPath = sFilename;
	int index = sPath.find_last_of("\\/");
	string sDirectory = index != -1 ? sPath.substr(0, index+1) : "";
	string sPureFilename = index != -1 ? sPath.substr(index+1) : sFilename;

	string sTextureExtensions[] = {"jpg", "jpeg", "png", "bmp", "tga"};
	index = sPureFilename.find_last_of(".");
	if(index != -1)
	{
		string sStripped = sPureFilename.substr(0, index+1);
		for (int i = 0; i < 5; i++)
		{
			string sTry = sDirectory+sStripped+sTextureExtensions[i];
			if(tSkin = new Texture(sTry,0))
			{
				break;
			}
		}
	}
	fclose(fp);
}

/*-----------------------------------------------

Name:	RenderModel

Params:	animState - structure with animation state - 
		if it's NULL, then only static model is rendered

Result:	Renders model.

/*---------------------------------------------*/


void CMD2Model::RenderModel(animState_t* animState)
{
	tSkin->Bind();
	m.Draw();
}


void CMD2Model::RenderModel(animState_t* animState, KeyFrameAnimationShader* shader)
{
	glBindVertexArray(uiVAO);

	int iTotalOffset = 0;
	tSkin->Bind();
	if(animState == NULL)
	{
		glEnableVertexAttribArray(0);
		vboFrameVertices[0].BindVBO();
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3), 0);
		
		// Set interpolation to negative number, so that vertex shader knows
		shader->SetInterpolation(-1.0f);
		for (int i = 0; i < renderModes.size(); i++)// Just render using previously extracted render modes
		{
			glDrawArrays(renderModes[i], iTotalOffset, numRenderVertices[i]);
			iTotalOffset += numRenderVertices[i];
		}
	}
	else
	{
		// Change vertices pointers to current frame
		glEnableVertexAttribArray(0);
		vboFrameVertices[animState->curr_frame].BindVBO();
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3), 0);

		glEnableVertexAttribArray(3);
		vboFrameVertices[animState->next_frame].BindVBO();
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3), 0);

		// Change normal pointers to current frame

		glEnableVertexAttribArray(2);
		vboFrameVertices[animState->curr_frame].BindVBO();
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3), (void*)sizeof(glm::vec3));

		glEnableVertexAttribArray(4);
		vboFrameVertices[animState->next_frame].BindVBO();
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 2*sizeof(glm::vec3), (void*)sizeof(glm::vec3));

		shader->SetInterpolation(animState->interpol);
		for (int i = 0; i < renderModes.size(); i++)
		{
			glDrawArrays(renderModes[i], iTotalOffset, numRenderVertices[i]);
			iTotalOffset += numRenderVertices[i];
		}
	}
}

/*-----------------------------------------------

Name:	StartAnimation

Params:	type - one of 19 types of MD2 animation
		(RUN, ATTACK, STAND...)

Result:	Returns animState_t structure with data
		at start of animation.

/*---------------------------------------------*/

animState_t CMD2Model::StartAnimation(animType_t type)
{
	animState_t res;
	res.startframe = animlist[type].first_frame;
	res.endframe = animlist[type].last_frame;
	res.curr_frame = animlist[type].first_frame;
	res.next_frame = animlist[type].first_frame + 1;

	res.fps = animlist[type].fps;
	res.type = type;

	res.curr_time = 0.0f;
	res.old_time = 0.0f;

	res.interpol = 0.0f;

	return res;
}

/*-----------------------------------------------

Name:	UpdateAnimation

Params:	animState - animation state to update
		fTimePassed - time passed since the last
		frame

Result:	Updates data in animation structure (performs
		animation basically).

/*---------------------------------------------*/


bool CMD2Model::UpdateExportedAnimation(animState_t* animState, float fTimePassed,bool loop)
{
	if (!loop)
		if(animState->curr_frame == animState->endframe)
			return true;
	animState->curr_time += fTimePassed;

	if (animState->curr_time - animState->old_time > (1.0f / float(animState->fps)))
	{
		animState->old_time = animState->curr_time;

		animState->curr_frame = animState->next_frame;
		animState->next_frame++;
		if (animState->next_frame > animState->endframe)
			animState->next_frame = animState->startframe;
	}

	animState->interpol = float(animState->fps) * (animState->curr_time - animState->old_time);
	UpdateVertices(animState);
	return false;
}

void CMD2Model::UpdateVertices(animState_t* animState)
{
	for (size_t i = 0; i < m.VertexData.size(); i++)
	{
		if (animState->interpol >= 0)
		{
			glm::vec3 newVertex = vVertices[animState->curr_frame][i] + (vVertices[animState->next_frame][i] - vVertices[animState->curr_frame][i])*animState->interpol;
			glm::vec3 newNormal = glm::vec3(anorms[vNormals[animState->curr_frame][i]][0] + (anorms[vNormals[animState->next_frame][i]][0] - anorms[vNormals[animState->curr_frame][i]][0])*animState->interpol,
				anorms[vNormals[animState->curr_frame][i]][1] + (anorms[vNormals[animState->next_frame][i]][1] - anorms[vNormals[animState->curr_frame][i]][1])*animState->interpol,
				anorms[vNormals[animState->curr_frame][i]][2] + (anorms[vNormals[animState->next_frame][i]][2] - anorms[vNormals[animState->curr_frame][i]][2])*animState->interpol);
			m.VertexData[i] = newVertex;
			m.NormalsData[i] = newNormal;
		}
	}
	m.Update();
}

bool CMD2Model::UpdateAnimation(animState_t* animState, float fTimePassed,bool loop)
{
	if (!loop)
		if(animState->curr_frame == animState->endframe)
			return true;
	animState->curr_time += fTimePassed;

	if(animState->curr_time - animState->old_time > (1.0f / float(animState->fps)))
	{
		animState->old_time = animState->curr_time;

		animState->curr_frame = animState->next_frame;
		animState->next_frame++;
		if(animState->next_frame > animState->endframe)
			animState->next_frame = animState->startframe;
	}

	animState->interpol = float(animState->fps) * (animState->curr_time - animState->old_time);
	return false;
}
