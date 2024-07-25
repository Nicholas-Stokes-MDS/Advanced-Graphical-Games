#pragma once

//#include "CCamera.h"
#include "Mesh_Model.h"
class Skybox : public Mesh_Model
{
public:
	
	Skybox(std::string FilePath);
	~Skybox();

	void Render(GLint _program, GLint _texture, glm::mat4 _matrix, glm::mat4 _viewMat, glm::mat4 _projMat, glm::vec3 _cameraPosition, GLint _skybox);

};

