#include "CCamera.h"

CCamera::CCamera()
{
	m_CameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
	m_CameraLookDir = glm::vec3(0.0f, 0.0f, -1.0f);
	m_CameraUpDir = glm::vec3(0.0f, 1.0f, 0.0f);
	m_WindowWidth = 800.0f;
	m_WindowHeight = 800.0f;

	m_radius = 600.0f;
	m_angle = 1.5f;

	m_lookPos = glm::vec3(0.0f, 0.0f, -10.0f);
	
	m_moveSpeed = 5.0f;
	m_mouseSpeed = 10.0f;

	m_yaw = 270.0f;
	
	m_position = glm::vec3(0.0f, 100.0f, 600.0f);
}

CCamera::~CCamera()
{
}

void CCamera::Update(float _dt, bool _manualToggle)
{
	// calculate the perspective projection matrix
	m_projMat = glm::perspective(glm::radians(45.0f), (float)m_WindowWidth / (float)m_WindowHeight, 0.1f, 100000.0f);

	//free camera
	glfwGetCursorPos(m_window, &Xpos, &Ypos);
	glm::vec2 mouse(Xpos, Ypos);
	glm::vec2 mouseDelta = mouse - m_lastMouse;
	mouseDelta.y = -mouseDelta.y;
	m_lastMouse = mouse;

	// changes camera pitch and yaw based on mouse movement
	if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		mouseDelta *= _dt;
		m_yaw += mouseDelta.x * m_mouseSpeed;
		m_pitch += mouseDelta.y * m_mouseSpeed;

		if (m_pitch > 89.0f)
		{
			m_pitch = 89.0f;
		}
		else if (m_pitch < -89.0f)
		{
			m_pitch = -89.0f;
		}
	}

	glm::vec3 dir;
	float ryaw = glm::radians(m_yaw), rpitch = glm::radians(m_pitch);

	dir.x = cos(ryaw) * cos(rpitch);
	dir.y = sin(rpitch);
	dir.z = sin(ryaw) * cos(rpitch);
	m_lookDir = glm::normalize(dir);

	// forward vector
	glm::vec3 fwd = glm::normalize(-m_lookDir);
	glm::vec3 right = glm::normalize(glm::cross(fwd, glm::vec3(0, 1, 0)));
	glm::vec3 up = glm::vec3(0, 1, 0);
	m_CameraUpDir = glm::normalize(glm::cross(right, fwd));

	// move camera based on WSADQE keys
	glm::vec3 move = ((float)(TriHoriz()) * -right)
		+ ((float)(TriVerti()) * fwd) + ((float)(-TriUpDown()) * up);

	move *= m_moveSpeed;
	m_position += move;

	// view matrix calculation
	m_viewMat = glm::lookAt(m_position, m_position + m_lookDir, m_CameraUpDir);
}

glm::mat4 CCamera::GetViewMat()
{
	return m_viewMat;
}

glm::mat4 CCamera::GetProjMat()
{
	return m_projMat;
}

glm::vec3 CCamera::GetCameraPos()
{
	return m_position;
}

glm::vec3 CCamera::GetForVector()
{
	
	glm::vec3 forward = glm::normalize(m_lookPos - m_position);
	return forward;
}

glm::vec3 CCamera::GetRightVector()
{
	glm::vec3 forward = glm::normalize(m_lookPos - m_position);
	glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));
	return right;
}

signed char CCamera::TriHoriz()
{
	signed char ret = 0;
	if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
	{
		ret += -1;
	}

	if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
	{
		ret += 1;
	}


	return ret;
}

signed char CCamera::TriVerti()
{
	signed char ret = 0;
	if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
	{
		ret += 1;
	}

	if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
	{
		ret += -1;
	}

	return ret;
}

signed char CCamera::TriUpDown()
{
	signed char ret = 0;
	if (glfwGetKey(m_window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		ret += -1;
	}

	if (glfwGetKey(m_window, GLFW_KEY_E) == GLFW_PRESS)
	{
		ret += 1;
	}

	return ret;
}
