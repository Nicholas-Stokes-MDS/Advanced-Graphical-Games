#pragma once

// library defines

//#include "CCamera.h"
#include <array>
#include <vector>
#include "LightManager.h"

class Mesh_Model
{
public:
	Mesh_Model(std::string FilePath);
	~Mesh_Model();

	// functions
	virtual void Update(float DeltaTime);
	virtual void Render(GLint _program, GLint _texture, glm::mat4 _matrix, glm::mat4 _viewMat, glm::mat4 _projMat, glm::vec3 _cameraPosition, GLint _skybox, GLint _reflectionMap);
	virtual void Texture(GLint _texture);
	virtual void AddInstances(glm::mat4 _matrix, int _count);
	virtual void TogglePointLight();
	virtual void ToggleDirectionalLight();
	
	bool pointLightToggle = true;
	bool directionalLightToggle = true;

protected:
	GLuint VAO;
	GLuint DrawCount;
	int DrawType;

	std::vector<glm::mat4> m_instanceTransform;

	GLuint VBO_Instanced;

	int ObjectCount;

	// Offset values for positioning
	float offsetX = 500.0f;
	float offsetY = 500.0f;
};

struct VertexStandard
{
public:
	glm::vec3 Position;
	glm::vec2 TexCoord;
	glm::vec3 Normal;

	VertexStandard()
	{
		Position = glm::vec3(0.0f);
		TexCoord = glm::vec2(0.0f);
		Normal = glm::vec3(0.0f);
	};
	VertexStandard(glm::vec3 pos, glm::vec2 texc, glm::vec3 norm)
	{
		Position = pos;
		TexCoord = texc;
		Normal = norm;
	}
};