#include "Skybox.h"

// runs default constructor of Mesh_Model
Skybox::Skybox(std::string FilePath)
	:Mesh_Model(FilePath)
{

}

Skybox::~Skybox()
{
}

void Skybox::Render(GLint _program, GLint _texture, glm::mat4 _matrix, glm::mat4 _viewMat, glm::mat4 _projMat, glm::vec3 _cameraPosition, GLint _skybox)
{
	// bind program and VAO
	glUseProgram(_program);

	// change culling for the skybox so it faces correctly
	glCullFace(GL_FRONT);

	// Activate and bind the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _texture);
	glUniform1i(glGetUniformLocation(_program, "Texture_Skybox"), 0);

	
	// send variables to shader via uniform
	glm::mat4 ViewMat = glm::mat4(glm::mat3(_viewMat));
	GLint ViewMatLoc = glGetUniformLocation(_program, "ViewMat");
	glUniformMatrix4fv(ViewMatLoc, 1, GL_FALSE, glm::value_ptr(ViewMat));


	GLint ProjectionMatLoc = glGetUniformLocation(_program, "ProjectionMat");
	glUniformMatrix4fv(ProjectionMatLoc, 1, GL_FALSE, glm::value_ptr(_projMat));

	glBindVertexArray(VAO);

	// render instances
	glDepthFunc(GL_LEQUAL);
	glDrawArraysInstanced(DrawType, 0, DrawCount, m_instanceTransform.size());
	glDepthFunc(GL_LESS);

	// unbinding
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glUseProgram(0);
}
