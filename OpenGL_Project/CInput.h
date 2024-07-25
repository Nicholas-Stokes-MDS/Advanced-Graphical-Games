#pragma once
#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
class CInput
{
public:
	signed char TriHoriz();
	void setWindow(GLFWwindow* _window) { m_window = _window; }
	
private:
	GLFWwindow* m_window;
	
};

