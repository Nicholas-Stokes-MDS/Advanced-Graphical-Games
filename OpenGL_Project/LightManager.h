#pragma once

#include "CCamera.h"

struct PointLight
{
	glm::vec3 Position;
	glm::vec3 Color;
	float SpecularStrength;

	float AttenuationConstant;
	float AttenuationLinear;
	float AttenuationExponent;
};

struct DirectionalLight
{
	glm::vec3 Direction;
	glm::vec3 Color;
	float SpecularStrength;
};

//stores the lighting properties and pass the variables into the shader instead of using hard coded values
class LightManager
{
public:
	LightManager();
	~LightManager();

	glm::vec3 LightColor;
	glm::vec3 LightPos;

	float AmbientStrength;
	glm::vec3 AmbientColor;

	float LightSpecularStrength;
	float ObjectShininess;

	static const int MAX_POINT_LIGHTS = 2;
	PointLight PointLightArray[MAX_POINT_LIGHTS];
	unsigned int PointLightCount;

	DirectionalLight DirectionalLight1;

	float Testing;

};

