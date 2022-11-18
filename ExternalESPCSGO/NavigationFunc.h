#pragma once
#include <iostream>
#include <GLFW/glfw3.h>

void ShowWindow(GLFWwindow* Window)
{
	glfwSetWindowAttrib(Window, GLFW_MOUSE_PASSTHROUGH, false);
}

void HideWindow(GLFWwindow* Window)
{
	glfwSetWindowAttrib(Window, GLFW_MOUSE_PASSTHROUGH, true);
}