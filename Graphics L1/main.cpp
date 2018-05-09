#pragma once
#include <iostream>
#include <Windows.h>
#include <gl/glew.h>
#include <gl/glfw3.h>
#include "Renderer.h"

int KeyPressed = -1;
int KeyAction = -1;
int MouseButton = -1;
int MouseAction = -1;
double MouseXPos = -1.0;
double MouseYPos = -1.0;
bool MouseMove = false;
int WindowWidth = 1024;
int WindowHeight = 768;
bool WindowSizeChanged = false;

bool InitGraphics();
void SpecialKeyPressed(GLFWwindow*, int, int, int, int);
void MouseMoved(GLFWwindow*, double, double);
void MouseClicked(GLFWwindow*, int, int, int);
void WindowResized(GLFWwindow*, int, int);

int main(void)
{
	InitGraphics();
	return 0;
}

/**
Creates the main window and initializes OpenGL stuff.
*/
bool InitGraphics()
{
	// ******************** Initialise GLFW ******************** //
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return false;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //OpenGL version 3.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 3.3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //If requesting an OpenGL version below 3.2, GLFW_OPENGL_ANY_PROFILE

	// ******************** Create Window ******************** //
	GLFWwindow* window = glfwCreateWindow(WindowWidth, WindowHeight, "Graphics Project Template 2017/2018", NULL, NULL);
	if (window == NULL){
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	
	// ******************** Initialize GLEW ******************** //
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return false;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	//getting window size and setting mouse position
	glfwGetWindowSize(window, &WindowWidth, &WindowHeight);
	//MouseXPos = WindowWidth / 2;
	//MouseYPos = WindowHeight / 2;
	//glfwSetCursorPos(window, MouseXPos, MouseYPos);

	// ******************** Binding Event Handlers ******************** //
	// Keyboard pressing event handler binding
	glfwSetKeyCallback(window, &SpecialKeyPressed);
	// Mouse movement event handler binding
	glfwSetCursorPosCallback(window, &MouseMoved);
	// Mouse click event handler binding
	glfwSetMouseButtonCallback(window, &MouseClicked);
	// Window resize event handler binding
	glfwSetWindowSizeCallback(window, &WindowResized);

	// ******************** Initialize OpenGL ******************** //
	Renderer *renderer = new Renderer(); //All drawing code goes here
	renderer->Initialize(WindowWidth, WindowHeight);

	do{ // Loop
		renderer->Draw();
		// call the handle keyboard only when a button is pressed.
		if (KeyPressed != -1)
		{
			renderer->HandleKeyboardInput(KeyPressed, KeyAction);
			//reset the pressed key.
			KeyPressed = -1;
		}
		// call mouse click handle only when a mouse button is cliked.
		if (MouseAction != -1)
		{
			renderer->HandleMouseClick(MouseAction, MouseButton, MouseXPos, MouseYPos);
			MouseAction = -1;
		}
		// call mouse move handel only when the mouse position is changed.
		if (MouseMove)
		{
			renderer->HandleMouseMove(MouseXPos, MouseYPos);
			MouseMove = false;
		}
		// call window size changed handle only when the window size changes.
		if (WindowSizeChanged)
		{
			renderer->HandleWindowResize(WindowWidth, WindowHeight);
			WindowSizeChanged = false;
		}
		// update self-animated objects each time frame.
		renderer->Update();
		// Swap buffers
		glfwSwapBuffers(window); //Displaying our finished scene
		glfwPollEvents(); //try to comment it

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
	glfwWindowShouldClose(window) == 0);
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	glfwDestroyWindow(window);
	// Deleting renderer object will deallocate it's memory and call it's destructor (which calls the renderer's Cleanup method)
	delete renderer;
	return true;
}

void SpecialKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// saving the pressed key: GLFW_KEY_{ANY_KEY}
	KeyPressed = key;
	// saving the key action: GLFW_PRESS, GLFW_RELEASE, GLFW_REPEAT
	KeyAction = action;
}

void MouseMoved(GLFWwindow* window, double xpos, double ypos)
{
	MouseMove = true;
	// saving the mouse position 
	MouseXPos = xpos;
	MouseYPos = ypos;
	printf("X = %f, Y = %f\n", xpos, ypos);
}

void MouseClicked(GLFWwindow* window, int button, int action, int mods)
{
	// saving the mouse button action: GLFW_PRESS, GLFW_RELEASE, GLFW_REPEAT
	MouseAction = action;
	// saving the mouse button: GLFW_MOUSE_BUTTON_LEFT, GLFW_MOUSE_BUTTON_RIGHT, GLFW_MOUSE_BUTTON_MIDDLE
	MouseButton = button;
	// saving the mouse position 
	glfwGetCursorPos(window, &MouseXPos, &MouseYPos);
}

void WindowResized(GLFWwindow* window, int width, int height)
{
	WindowSizeChanged = true;
	// saving the new window size 
	WindowWidth = width;
	WindowHeight = height;
	// updating the area used for drawing
	glViewport(0, 0, width, height);
}