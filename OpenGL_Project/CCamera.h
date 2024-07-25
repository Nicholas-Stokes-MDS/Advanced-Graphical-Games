#pragma once
#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "CInput.h"
class CCamera
{
public:
	CCamera();
	~CCamera();

	void Update(float _dt, bool m_anualToggle);
	glm::mat4 GetViewMat();
	glm::mat4 GetProjMat();
	glm::vec3 GetCameraPos();

	void setWindow(GLFWwindow* _window) { m_window = _window; }
	
	glm::vec3 GetForVector();
	glm::vec3 GetRightVector();

	void ChangeRadius(float _radius) { m_radius += _radius; }

private:
	glm::mat4 m_viewMat;
	glm::mat4 m_projMat;

	float m_fov;
	float m_aspectRatio;
	glm::vec3 m_CameraPos;
	glm::vec3 m_CameraLookDir;
	glm::vec3 m_CameraUpDir;
	glm::vec3 m_position;
	float m_WindowWidth;
	float m_WindowHeight;
	
	// orbiting camera
	float m_radius;
	float m_angle;
	glm::vec3 m_lookPos;

	GLFWwindow* m_window;

	signed char TriHoriz();

	signed char TriVerti();

	signed char TriUpDown();

	float m_moveSpeed;
	float m_mouseSpeed;

	glm::vec2 m_lastMouse;
	float m_yaw = 0, m_pitch = 0;

	double Xpos;
	double Ypos;

	glm::vec3 m_lookDir;
	glm::vec3 m_up;
};

