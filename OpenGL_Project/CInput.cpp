#include "CInput.h"

signed char CInput::TriHoriz()
{
    signed char ret = 0;
    if (glfwGetKey(m_window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
		ret -= 1;
	}
    else if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
    {
        ret -= 1;
    }

    if (glfwGetKey(m_window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
		ret += 1;
	}
    else if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
    {
		ret += 1;
	}

    return ret;
}
