#include "Renderer.h"

Renderer::Renderer()
{

}

Renderer::~Renderer()
{
	Cleanup();
}
struct obje
{
	GLfloat x, y;

};
obje arr[5];
void Renderer::Initialize(int WindowWidth, int WindowHeight)
{
	PreTime = glfwGetTime();

	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//////////////////////////////////////////////////////////////////////////
	// Camera
	MyCamera = new FirstPersonCamera();
	// Projection matrix : 
	MyCamera->SetPerspectiveProjection(45.0f, (float)WindowWidth / (float)WindowHeight, 0.1f, 1000.0f);
	// View matrix : 
	ty = -42.5;
	tx = 30.0;
	MyCamera->SetLookAt(vec3(tx, ty - 1, 4), vec3(tx, ty + 10, 0), vec3(0, 1, 0));

	//MyCamera->SetLookAt(
	//	vec3(+30.0f, -43.5f, +4.0f),// Camera Position
	//	vec3(+30.0f, -20.0f, +0.0f),// Look at Point
	//	//vec3(+0.0f, +0.0f, +130.0f),
	//	//vec3(+0.0f, +0.0f, +0.0f),
	//	vec3(+0.0f, +1.0f, +0.0f) // Up Vector
	//);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Create and compile our GLSL program from the shaders
	ProgramID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
	// Use our shader
	glUseProgram(ProgramID);
	// Get the ID of VP from shaders
	ViewProjectionMatrixID = glGetUniformLocation(ProgramID, "ViewProjectionMatrix");
	// Get the ID of ModelMatrix from shaders
	ModelMatrixID = glGetUniformLocation(ProgramID, "ModelMatrix");
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Initialization using the Ordinary Way
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	GLfloat VertexData[] = {
		//base
		-1.0f, +1.0f, 0.0f,		1, 0, 0,	0, 0,
		+1.0f, +1.0f, 0.0f,		1, 0, 0,	1, 0,
		-1.0f, -1.0f, 0.0f,		1, 0, 0,	0, 1,

		+1.0f, +1.0f, 0.0f,		1, 0, 0,	1, 0,
		-1.0f, -1.0f, 0.0f,		1, 0, 0,	0, 1,
		+1.0f, -1.0f, 0.0f,		1, 0, 0,	1, 1,

		//back
		-1.0f, -1.0f, 0.0f,		1, 0, 0,	0, 1,
		+1.0f, -1.0f, 0.0f,		1, 0, 0,	1, 1,
		-1.0f, -1.0f, +1.0f,	1, 0, 0,	0, 0,

		+1.0f, -1.0f, 0.0f,		1, 0, 0,	1, 1,
		+1.0f, -1.0f, +1.0f,	1, 0, 0,	1, 0,
		-1.0f, -1.0f, +1.0f,	1, 0, 0,	0, 0,

		//front
		-1.0f, +1.0f, 0.0f,		1, 0, 0,	0, 1,
		-1.0f, +1.0f, +1.0f,	1, 0, 0,	0, 0,
		+1.0f, +1.0f, +1.0f,	1, 0, 0,	1, 0,

		+1.0f, +1.0f, +1.0f,	1, 0, 0,	1, 0,
		+1.0f, +1.0f, 0.0f,		1, 0, 0,	1, 1,
		-1.0f, +1.0f, 0.0f,		1, 0, 0,	0, 1,

		//left
		-1.0f, -1.0f, 0.0f,		1, 0, 0,	0, 1,
		-1.0f, +1.0f, 0.0f,		1, 0, 0,	1, 1,
		-1.0f, +1.0f, +1.0f,	1, 0, 0,	1, 0,

		-1.0f, -1.0f, 0.0f,		1, 0, 0,	0, 1,
		-1.0f, -1.0f, +1.0f,	1, 0, 0,	0, 0,
		-1.0f, +1.0f, +1.0f,	1, 0, 0,	1, 0,

		//right
		+1.0f, -1.0f, 0.0f,		1, 0, 0,	1, 1,
		+1.0f, +1.0f, 0.0f,		1, 0, 0,	0, 1,
		+1.0f, +1.0f, +1.0f,	1, 0, 0,	0, 0,

		+1.0f, +1.0f, +1.0f,	1, 0, 0,	0, 0,
		+1.0f, -1.0f, +1.0f,	1, 0, 0,	1, 0,
		+1.0f, -1.0f, 0.0f,		1, 0, 0,	1, 1,


		//snake head
		-1.0f, +1.0f, +1.0f,	1, 0, 0,	0, 0,
		+1.0f, +1.0f, +1.0f,	1, 0, 0,	1, 0,
		-1.0f, -1.0f, +1.0f,	1, 0, 0,	0, 1,

		+1.0f, +1.0f, +1.0f,	1, 0, 0,	1, 0,
		-1.0f, -1.0f, +1.0f,	1, 0, 0,	0, 1,
		+1.0f, -1.0f, +1.0f,	1, 0, 0,	1, 1,

		//square
		
	   0.5,1,0.5,                 1, 0, 0,	0, 0,
	   0.5,0.8,0.5,               1, 0, 0,	1, 0,
	   0.4,0.8,0.5,               1, 0, 0,	0, 1,

	   0.5,1,0.5,                 1, 0, 0,	0, 0,
	   0.4,1,0.5,                 1, 0, 0,	1, 0,
	   0.4,0.8,0.5,               1, 0, 0,	0, 1,
   
	};						 

	glGenBuffers(1, &VertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(0 * sizeof(GLfloat)));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));


	S = scale(50, 50, 50);
	T = translate(0.0f, 0.0f, 0.0f);
	R = rotate(0.0f, vec3(0, 0, 1));
	ModelMatrix = T * R * S;
	ty = -42.5;
	tx = 30.0;
	up = rig= lef= dow = false;
	arr[1].x = 20.0f;   arr[1].y = 30.0f; //food
	arr[2].x = -20.0f;   arr[2].y = 0.0f; //death
	arr[3].x = -20.0f;   arr[3].y = -10.0f; //sc++
	arr[4].x = -20.0f;   arr[4].y = -20.0f; //sc++
	//////////////////////////////////////////////////////////////////////////
	//dynamic model
	 ​DynamicShader.LoadProgram();
	 ​DynamicModel.LoadModel("snake.md2");
	 ​DynamicAnimState = ​DynamicModel.StartAnimation(animType_t::STAND);
	//////////////////////////////////////////////////////////////////////////
}
void Renderer::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(ProgramID);

	mat4 ViewProjectionMatrix = MyCamera->GetProjectionMatrix() * MyCamera->GetViewMatrix();
	glUniformMatrix4fv(ViewProjectionMatrixID, 1, GL_FALSE, &ViewProjectionMatrix[0][0]);
	S = scale(50, 50, 50);
	T = translate(0.0f, 0.0f, 0.0f);
	R = rotate(0.0f, vec3(0, 0, 1));
	ModelMatrix = T * R * S;
	//////////////////////////////////////////////////////////////////////////
	// Drawing using the Ordinary Way
	glBindVertexArray(VertexArrayID);
	t1 = new Texture("0arrakisday_dn.jpg", 0);
	t1->Bind();
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	t1 = new Texture("1arrakisday_ft.jpg", 0);
	t1->Bind();
	glDrawArrays(GL_TRIANGLES, 6, 24);

	//food 1

		t1 = new Texture("food.png", 0);
		t1->Bind();
		T = translate(arr[1].x, arr[1].y, 0.0f);
		S = scale(3, 3, 3);
		R = rotate(0.0f, 0.0f, 1.0f, 0.0f);
		ModelMatrix = T*R*S;
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 30, 6);
	
	//death

	t1 = new Texture("death.png", 0);
	t1->Bind();
	T = translate(arr[2].x, arr[2].y, 0.0f);
	S = scale(3, 3, 3);
	R = rotate(0.0f, 0.0f, 1.0f, 0.0f);
	ModelMatrix = T*R*S;
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 30, 6);

	//sc+
	
		t1 = new Texture("score+.png", 0);
		t1->Bind();
		T = translate(arr[3].x, arr[3].y, 0.0f);
		S = scale(3, 3, 3);
		R = rotate(0.0f, 0.0f, 1.0f, 0.0f);
		ModelMatrix = T*R*S;
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 30, 6);
	
	//sc--
	
		t1 = new Texture("sc--.png", 0);
		t1->Bind();
		T = translate(arr[4].x, arr[4].y, 0.0f);
		S = scale(3, 3, 3);
		R = rotate(0.0f, 0.0f, 1.0f, 0.0f);
		ModelMatrix = T*R*S;
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 30, 6);
	

	//////////////////////////////////////////////////////////////////////////
	//dynamic model
	ViewProjectionMatrix = MyCamera->GetProjectionMatrix() * MyCamera->GetViewMatrix();
	​DynamicShader.UseProgram();
	​DynamicShader.BindViewProjectionMatrix(&ViewProjectionMatrix[0][0]);
	R = rotate(90.0f, 0.0f, 0.0f, 1.0f);
	snake.X_old = tx;
	snake.Y_old = ty;
	T = translate(tx, ty, 2.0f);
	S = scale(0.1f, 0.1f, 0.1f);
	​DynamicModelMatrix = T * snake.R * R * S;
	​DynamicShader.BindModelMatrix(&​DynamicModelMatrix[0][0]);
	​DynamicModel.RenderModel(&​DynamicAnimState, &​DynamicShader);

}

void Renderer::Update()
{
	double CurrentTime = glfwGetTime();
	double DeltaTime = CurrentTime - PreTime;
	PreTime = CurrentTime;
	
	PreTime = CurrentTime;
	if (snake.in_y == 1)
	{
		ty = snake.Y_old + 2 * DeltaTime;
		//MyCamera->Walk(2.1 * DeltaTime);
		MyCamera->SetLookAt(vec3(tx, ty - 1, 4), vec3(tx, ty + 10, 0), vec3(0, 1, 0));
	}
	if (snake.in_y == -1)
	{
		ty = snake.Y_old - 2 * DeltaTime;
		//MyCamera->Walk(2.1 * DeltaTime);
		MyCamera->SetLookAt(vec3(tx, ty + 1, 4), vec3(tx, ty - 10, 0), vec3(0, -1, 0));
	}

	if (snake.in_x == 1)
	{
		tx = snake.X_old + 2 * DeltaTime;
		//MyCamera->Walk(2.1 * DeltaTime);
		MyCamera->SetLookAt(vec3(tx - 1, ty, 4), vec3(tx + 10, ty, 0), vec3(1, 0, 0));
	}
	if (snake.in_x == -1)
	{
		tx = snake.X_old - 2 * DeltaTime;
		//MyCamera->Walk(2.1 * DeltaTime);
		MyCamera->SetLookAt(vec3(tx + 1, ty, 4), vec3(tx - 10, ty, 0), vec3(-1, 0, 0));
	}
	//cout << "tx   " << tx << "  ty  " << ty << endl;
	for (int i = 0; i<5; i++)
		colide(arr[i].x, arr[i].y, i);

}

void Renderer::HandleKeyboardInput(int Key, int Action)
{
	if (Action == GLFW_PRESS || Action == GLFW_REPEAT)
	{
		switch (Key)
		{
			//Moving forward
		case GLFW_KEY_UP:
		case GLFW_KEY_W:
			/*cout << "tx   "<<tx<<"  ty  "<<ty << endl;
			cout << "x   " << arr[1].x << "  y  " << arr[1].y<< endl;*/
			/*for(int i=0;i<5;i++)
			colide(arr[i].x, arr[i].y, i);*/
			if (up == false & dow==false) {
				MyCamera->Roll(snake.in_x * 90);
				ty = snake.Y_old + 1;
				snake.R = snake.turn_Up(snake.in_x);
				snake.in_y = 1;
				snake.in_x = 0;
				up = true;
				rig = lef = dow = false;
			}
			break;

			//Moving backword
		case GLFW_KEY_DOWN:
		case GLFW_KEY_S:
			/*for (int i = 0; i<5; i++)
				colide(arr[i].x, arr[i].y, i);*/
			if (dow == false && up==false)
			{
				MyCamera->Roll(snake.in_x * -90);
				ty = snake.Y_old - 1;
				snake.R = snake.turn_Down(snake.in_x);
				snake.in_y = -1;
				snake.in_x = 0;
				dow = true;
				rig = lef = up = false;
			}
			break;

			// Moving right
		case GLFW_KEY_RIGHT:
		case GLFW_KEY_D:
			/*cout << "tx   " << tx << "  ty  " << ty << endl;
			cout << "x   " << arr[1].x << "  y  " << arr[1].y << endl;*/
			/*for (int i = 0; i < 5; i++) {
				cout << "x   " << arr[i].x << "  y  " << arr[i].y << "   " << i << endl;
				colide(arr[i].x, arr[i].y, i);
			}*/
			if (rig == false && lef==false) {
				MyCamera->Walk(5);
				MyCamera->Roll(snake.in_y * -90);
				tx = snake.X_old + 1;
				snake.R = snake.turn_Right(snake.in_y);
				snake.in_x = 1;
				snake.in_y = 0;
				rig = true;
				dow = lef = up = false;
			}
			break;

			// Moving left
		case GLFW_KEY_LEFT:
		case GLFW_KEY_A:
			//cout << "tx   " << tx << "  ty  " << ty << endl;
			/*for (int i = 0; i < 5; i++) {
				cout << "x   " << arr[i].x << "  y  " << arr[i].y << "   " << i << endl;
				colide(arr[i].x, arr[i].y, i);
			}*/
			if (lef == false && rig==false) {
				MyCamera->Walk(5);
				MyCamera->Roll(snake.in_y * 90);
				tx = snake.X_old - 1;
				snake.R = snake.turn_Left(snake.in_y);
				snake.in_x = -1;
				snake.in_y = 0;
				lef = true;
				dow = rig = up = false;
			}
			break;

			// Moving up
		case GLFW_KEY_SPACE:
		case GLFW_KEY_R:
			MyCamera->Fly(0.1);
			break;

			// Moving down
		case GLFW_KEY_LEFT_CONTROL:
		case GLFW_KEY_F:
			MyCamera->Fly(-0.1);
			break;

		case GLFW_KEY_I:
			MyCamera->Pitch(1);
			break;

		case GLFW_KEY_K:
			MyCamera->Pitch(-1);
			break;

		case GLFW_KEY_L:
			MyCamera->Yaw(-1);
			break;

		case GLFW_KEY_J:
			MyCamera->Yaw(1);
			break;

		case GLFW_KEY_O:
			MyCamera->Roll(-90);
			break;

		case GLFW_KEY_U:
			MyCamera->Roll(90);
			break;

		default:
			break;
		}
	}
}
void Renderer::colide(GLfloat & x, GLfloat &y, int index) {


	GLfloat  dx = tx - x;
	GLfloat  dy = (ty + 4);
	int c = rand() % 10;
	int c2= rand() % 10;
	if (tx >= (arr[index].x - 4) && tx <= (arr[index].x + 4)) {
		if (dy >= (arr[index].y) && dy <= (arr[index].y + 4)) {
			while (true) {
				int x1 = rand() % 45;
				int y1 = rand() % 45 ;
				if (c % 2 != 0) {
					if (c2 % 2 == 0)
						y1 = -y1;
					else
						x1 = -x1;
				}
				bool fl = false;
				cout << "here" << endl;
				for (int i = 0; i < 5; i++)
				{
					if (i == index)
						continue;
					if (x1 >= (arr[i].x - 4) && x1 <= (arr[i].x + 4)) {
						if (y1 >= (arr[i].y) && y1 <= (arr[i].y + 4))
							fl = true;

					}
				}
				if (fl == false) {
					arr[index].x = x1;
					arr[index].y = y1;
					break;
				}

			}
		}

	}
}
void Renderer::HandleMouseMove(double MouseXPos, double MouseYPos)
{

}

void Renderer::HandleMouseClick(int MouseAction, int MouseButton, double MouseXPos, double MouseYPos)
{

}

void Renderer::HandleWindowResize(int WindowWidth, int WindowHeight)
{
	MyCamera->SetPerspectiveProjection(45.0f, (float)WindowWidth / (float)WindowHeight, 0.1f, 1000.0f);
}

void Renderer::Cleanup()
{
	glDeleteProgram(ProgramID);
	delete MyCamera;
}