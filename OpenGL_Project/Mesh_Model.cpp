#include "Mesh_Model.h"

// library defines
#define TINYOBJLOADER_IMPLEMENTATION

// library includes
#include "tiny_obj_loader.h"
#include <iostream>

LightManager lightManager;

Mesh_Model::Mesh_Model(std::string FilePath)
{
	std::vector<VertexStandard> Vertices;
	tinyobj::ObjReaderConfig ReaderConfig;
	tinyobj::ObjReader Reader;

	if (!Reader.ParseFromFile(FilePath, ReaderConfig))
	{
		if (!Reader.Error().empty())
		{
			std::cerr << "TinyObjReader: " << Reader.Error();
		}
		exit(1);
	}

	if (!Reader.Warning().empty())
	{
		std::cout << "TinyObjReader: " << Reader.Warning();
	}

	auto& Attrib = Reader.GetAttrib();
	auto& Shapes = Reader.GetShapes();

	// loop through the shapes of the object
	for (size_t ShapeIndex = 0; ShapeIndex < Shapes.size(); ShapeIndex++)
	{
		// loop through the faces of the shape
		size_t ReadIndexOffset = 0;
		for (size_t FaceIndex = 0; FaceIndex < Shapes[ShapeIndex].mesh.num_face_vertices.size(); FaceIndex++)
		{
			size_t FaceVertexCount = size_t(Shapes[ShapeIndex].mesh.num_face_vertices[FaceIndex]);
			// loop through the vertices of the face 
			for (size_t VertexIndex = 0; VertexIndex < FaceVertexCount; VertexIndex++)
			{
				VertexStandard Vertex{};
				tinyobj::index_t TinyObjVertex = Shapes[ShapeIndex].mesh.indices[ReadIndexOffset + VertexIndex];
				Vertex.Position = {
					Attrib.vertices[3 * size_t(TinyObjVertex.vertex_index) + 0],
					Attrib.vertices[3 * size_t(TinyObjVertex.vertex_index) + 1],
					Attrib.vertices[3 * size_t(TinyObjVertex.vertex_index) + 2],
				};
				if (TinyObjVertex.texcoord_index >= 0) // negative states no TexCoord data
				{
					Vertex.TexCoord = {
						Attrib.texcoords[2 * size_t(TinyObjVertex.texcoord_index) + 0],
						Attrib.texcoords[2 * size_t(TinyObjVertex.texcoord_index) + 1],
					};
				}

				if (TinyObjVertex.normal_index >= 0) // Negative states no Normal data
				{
					Vertex.Normal = {
						Attrib.normals[3 * size_t(TinyObjVertex.normal_index) + 0],
						Attrib.normals[3 * size_t(TinyObjVertex.normal_index) + 1],
						Attrib.normals[3 * size_t(TinyObjVertex.normal_index) + 2],
					};
				}

				Vertices.push_back(Vertex);
			}
			ReadIndexOffset += FaceVertexCount;
		}

	}

	// store for use in rendering
	DrawType = GL_TRIANGLES;
	DrawCount = (GLuint)Vertices.size();

	// create the vertex array and associated buffers
	GLuint VBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexStandard) * Vertices.size(), Vertices.data(), GL_STATIC_DRAW);

	// attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexStandard),
		(void*)(offsetof(VertexStandard, VertexStandard::Position)));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexStandard),
		(void*)(offsetof(VertexStandard, VertexStandard::TexCoord)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexStandard),
		(void*)(offsetof(VertexStandard, VertexStandard::Normal)));

	// put normals here too 
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);


}

Mesh_Model::~Mesh_Model()
{
}

void Mesh_Model::Update(float DeltaTime)
{
}

void Mesh_Model::AddInstances(glm::mat4 _matrix, int _count)
{
	// for the soldier
	if (_count == 1)
	{
		glm::mat4 singleTransform = glm::scale(_matrix, glm::vec3(20.0f, 20.0f, 20.0f));
		m_instanceTransform.push_back(singleTransform);
	}
	else
	{
		// Initialize each matrix in the array with position offsets for the 1000 trees
		for (int i = 0; i < 32; ++i) {
			for (int j = 0; j < 32; ++j) {
				int index = i * 32 + j;
				glm::mat4 transform = glm::translate(_matrix, glm::vec3((j * offsetX) - 8000.0f, 0.0f, (i * offsetY) - 8000.0f));
				m_instanceTransform.push_back(transform);
			}
		}
	}




	
	// instanced drawing through instanced arrays
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO_Instanced);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Instanced);
	glBufferData(GL_ARRAY_BUFFER, m_instanceTransform.size() * sizeof(glm::mat4), m_instanceTransform.data(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)0);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(1 * sizeof(glm::vec4)));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	glBindVertexArray(0);
}

void Mesh_Model::TogglePointLight()
{
	if (pointLightToggle == true)
	{
		lightManager.PointLightCount = 0;
		pointLightToggle = false;
	}
	else if (pointLightToggle == false)
	{
		lightManager.PointLightCount = 2;
		pointLightToggle = true;
	}
}

void Mesh_Model::ToggleDirectionalLight()
{
	if (directionalLightToggle == true)
	{
		lightManager.DirectionalLight1.Color = glm::vec3(0.0f, 0.0f, 0.0f);
		directionalLightToggle = false;
	}
	else if (directionalLightToggle == false)
	{
		lightManager.DirectionalLight1.Color = glm::vec3(1.0f, 1.0f, 0.0f);
		directionalLightToggle = true;
	}
}

void Mesh_Model::Render(GLint _program, GLint _texture, glm::mat4 _matrix, glm::mat4 _viewMat, glm::mat4 _projMat, glm::vec3 _cameraPosition, GLint _skybox, GLint _reflectionMap)
{
	
	// bind program and VAO
	glUseProgram(_program);

	glBindVertexArray(VAO);

	glCullFace(GL_BACK);

	// send variables to shader via uniform
	GLint ViewMatLoc = glGetUniformLocation(_program, "ViewMat");
	glUniformMatrix4fv(ViewMatLoc, 1, GL_FALSE, glm::value_ptr(_viewMat));
	GLint ProjectionMatLoc = glGetUniformLocation(_program, "ProjectionMat");
	glUniformMatrix4fv(ProjectionMatLoc, 1, GL_FALSE, glm::value_ptr(_projMat));
	
	GLint CameraPosition = glGetUniformLocation(_program, "CameraPosition");
	glUniform3fv(CameraPosition, 1, glm::value_ptr(_cameraPosition));



	// passing in uniforms from Light Manager class
	glUniform3fv(glGetUniformLocation(_program, "LightColor"), 1, glm::value_ptr(lightManager.LightColor));

	GLint LightPos = glGetUniformLocation(_program, "LightPos");
	glUniform3fv(LightPos, 1, glm::value_ptr(lightManager.LightPos));

	GLint AmbientColor = glGetUniformLocation(_program, "AmbientColor");
	glUniform3fv(AmbientColor, 1, glm::value_ptr(lightManager.AmbientColor));

	GLint AmbientStrength = glGetUniformLocation(_program, "AmbientStrength");
	glUniform1f(AmbientStrength, lightManager.AmbientStrength);


	glUniform1f(glGetUniformLocation(_program, "LightSpecularStrength"), lightManager.LightSpecularStrength);

	GLint ObjectShininess = glGetUniformLocation(_program, "ObjectShininess");
	glUniform1f(ObjectShininess, lightManager.ObjectShininess);

	GLint Testing = glGetUniformLocation(_program, "Testing");
	glUniform1f(Testing, lightManager.Testing);
	
	// pass in point light array manually from LightManager class
	glUniform3fv(glGetUniformLocation(_program, "PointLightArray[0].Position"), 1, glm::value_ptr(lightManager.PointLightArray[0].Position));
	glUniform3fv(glGetUniformLocation(_program, "PointLightArray[0].Color"), 1, glm::value_ptr(lightManager.PointLightArray[0].Color));
	glUniform1f(glGetUniformLocation(_program, "PointLightArray[0].SpecularStrength"), lightManager.PointLightArray[0].SpecularStrength);
	glUniform1f(glGetUniformLocation(_program, "PointLightArray[0].AttenuationConstant"), lightManager.PointLightArray[0].AttenuationConstant);
	glUniform1f(glGetUniformLocation(_program, "PointLightArray[0].AttenuationLinear"), lightManager.PointLightArray[0].AttenuationLinear);
	glUniform1f(glGetUniformLocation(_program, "PointLightArray[0].AttenuationExponent"), lightManager.PointLightArray[0].AttenuationExponent);


	glUniform3fv(glGetUniformLocation(_program, "PointLightArray[1].Position"), 1, glm::value_ptr(lightManager.PointLightArray[1].Position));
	glUniform3fv(glGetUniformLocation(_program, "PointLightArray[1].Color"), 1, glm::value_ptr(lightManager.PointLightArray[1].Color));
	glUniform1f(glGetUniformLocation(_program, "PointLightArray[1].SpecularStrength"), lightManager.PointLightArray[1].SpecularStrength);
	glUniform1f(glGetUniformLocation(_program, "PointLightArray[1].AttenuationConstant"), lightManager.PointLightArray[1].AttenuationConstant);
	glUniform1f(glGetUniformLocation(_program, "PointLightArray[1].AttenuationLinear"), lightManager.PointLightArray[1].AttenuationLinear);
	glUniform1f(glGetUniformLocation(_program, "PointLightArray[1].AttenuationExponent"), lightManager.PointLightArray[1].AttenuationExponent);

	glUniform1ui(glGetUniformLocation(_program, "PointLightCount"), lightManager.PointLightCount);

	// pass in directional light from LightManager class
	glUniform3fv(glGetUniformLocation(_program, "DirectionalLight1.Direction"), 1, glm::value_ptr(lightManager.DirectionalLight1.Direction));
	glUniform3fv(glGetUniformLocation(_program, "DirectionalLight1.Color"), 1, glm::value_ptr(lightManager.DirectionalLight1.Color));
	glUniform1f(glGetUniformLocation(_program, "DirectionalLight1.SpecularStrength"), lightManager.DirectionalLight1.SpecularStrength);

	// Activate and bind the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glUniform1i(glGetUniformLocation(_program, "Texture0"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _skybox);
	glUniform1i(glGetUniformLocation(_program, "Texture_Skybox"), 1);
	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, _reflectionMap);
	glUniform1i(glGetUniformLocation(_program, "Texture_ReflectionMap"), 2);

	// render instances
	glDrawArraysInstanced(DrawType, 0, DrawCount, m_instanceTransform.size());

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);

}

void Mesh_Model::Texture(GLint _texture)
{

}


