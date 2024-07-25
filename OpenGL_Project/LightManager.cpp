#include "LightManager.h"

//stores the lighting properties and pass the variables into the shader instead of using hard coded values
LightManager::LightManager()
{
	LightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	LightPos = glm::vec3(-300.0f, 200.0f, 0.0f);

	// ambient
	AmbientColor = glm::vec3(1.0f, 1.0f, 1.0f);
	AmbientStrength = 0.15f;
	

	// specular
	LightSpecularStrength = 1.0f;
	ObjectShininess = 32.0f;

	//Testing = 1.0f;

	// point light 1 values
	PointLightArray[0].Position					= glm::vec3(-200.0f, 200.0f, 0.0f);
	PointLightArray[0].Color					= glm::vec3(0.0f, 1.0f, 0.0f);
	PointLightArray[0].SpecularStrength			= 1.0f;
	PointLightArray[0].AttenuationConstant		= 1.0f;
	PointLightArray[0].AttenuationLinear		= 0.0014f;
	PointLightArray[0].AttenuationExponent		= 0.000007f;

	// point light 2 values
	PointLightArray[1].Position					= glm::vec3(200.0f, 200.0f, 0.0f);
	PointLightArray[1].Color					= glm::vec3(1.0f, 0.0f, 0.0f);
	PointLightArray[1].SpecularStrength			= 1.0f;
	PointLightArray[1].AttenuationConstant		= 1.0f;
	PointLightArray[1].AttenuationLinear		= 0.0014f;
	PointLightArray[1].AttenuationExponent		= 0.000007f;

	PointLightCount = 2;

	// directional light values
	DirectionalLight1.Direction = glm::vec3(0.5f, 0.0f, 1.0f);
	DirectionalLight1.Color = glm::vec3(1.0f, 1.0f, 0.0f);
	DirectionalLight1.SpecularStrength = 1.0f;


}

LightManager::~LightManager()
{
}