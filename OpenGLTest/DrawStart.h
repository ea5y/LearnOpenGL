#pragma once

#include <iostream>

#define GLEW_STATIC
#include <GL/eglew.h>
#include <GLFW/glfw3.h>

//typedef void(*pFunc)();

class DrawStartC
{
public:
	template<typename Func>
	static void Draw (GLFWwindow *window,Func func)
	{
		//Loop
		while (!glfwWindowShouldClose(window))
		{
			//Check if any events
			glfwPollEvents();

			//Clear
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			//Custom
			func();

			//Swap the screen buffers
			glfwSwapBuffers(window);
		}
	}
};
