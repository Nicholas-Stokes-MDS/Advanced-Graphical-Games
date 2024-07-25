// Library Includes
//#include "CCamera.h"
#include "ShaderLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Mesh_Model.h"
#include "Skybox.h"

void InitialSetup();
void Update();
void Render();

// pointer to the main window
GLFWwindow* Window = nullptr;

// programs
GLuint Program_FixedTri;

GLuint Program_PositionOnly;

GLuint Program_ColorFade;

GLuint Program_WorldSpace;

GLuint Program_Texture;

GLuint Program_TextureMix;

GLuint Program_3D;

GLuint Program_Pyramid;

GLuint Program_3DCamera;

GLuint Program_3DModel;

GLuint Program_LightingAmbient;

GLuint Program_LightingBlinnPhong;

GLuint Program_Skybox;

GLuint Program_Reflection;

// VAO / VBO / EBO
GLuint VBO_Tri;
GLuint VAO_Tri;

GLuint VBO_Tri2;
GLuint VAO_Tri2;

GLuint VBO_Quad;
GLuint VAO_Quad;
GLuint EBO_Quad;

GLuint VBO_Cube;
GLuint VAO_Cube;
GLuint EBO_Cube;

GLuint VBO_Cube2;
GLuint VAO_Cube2;
GLuint EBO_Cube2;

GLuint VBO_Pyramid;
GLuint VAO_Pyramid;
GLuint EBO_Pyramid;

// textures
GLuint Texture_shuckle;
GLuint Texture_AwesomeFace;
GLuint Texture_Soldier;
GLuint Texture_Tree;
GLuint TextureID_Skybox;
GLuint Texture_Ship;
GLuint Texture_ReflectionMap;

float CurrentTime;
float Oscillate;

float WindowWidth = 800.0f;
float WindowHeight = 800.0f;

// toggle bools
bool cursorToggle = true;
bool wireframeToggle = false;
bool coordsToggle = false;
bool manualToggle = true;
bool pointLightToggle = true;


float deltaTime;


glm::mat4 ProjectionMat;
glm::mat4 ViewMat;

std::vector<std::string> FilePaths = {
	"Resources/Textures/CubeMap/Right.png",
	"Resources/Textures/CubeMap/Left.png",
	"Resources/Textures/CubeMap/Bottom.png",
	"Resources/Textures/CubeMap/Top.png",
	"Resources/Textures/CubeMap/Back.png",
	"Resources/Textures/CubeMap/Front.png"
};

// vertices / indices
GLfloat Vertices_Tri[] = {
	// position			// color
	-1.0f, 1.0f, 0.0f,	1.0f, 0.0f, 0.0f,	// Top Right
	-1.0f, -1.0f, 0.0f,	0.0f, 1.0f, 0.0f,	// Top Left
	1.0f, -1.0f, 0.0f,	0.0f, 0.0f, 1.0f,	// Bottom Center

	// triangle 2
	1.0f, -1.0f, 0.0f,	0.0f, 0.0f, 1.0f,	// Top Right
	1.0f, 1.0f, 0.0f,	0.0f, 1.0f, 0.0f,	// Top Left
	-1.0f, 1.0f, 0.0f,	1.0f, 0.0f, 0.0f,	// Bottom Center
};

// vertices / indices
GLfloat Vertices_Tri2[] = {
	// position			// color
	-0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 1.0f,	// Top Right
	-0.5f, -0.5f, 0.0f,	1.0f, 1.0f, 0.0f,	// Top Left
	0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 1.0f,	// Bottom Center

	// triangle 2
	0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 1.0f,	// Top Right
	0.5f, 0.5f, 0.0f,	1.0f, 1.0f, 0.0f,	// Top Left
	-0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 1.0f,	// Bottom Center
};

GLfloat Vertices_Quad[] = {
	// Index	// position			// color		// texture coords
	/* 0 */ -0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 1.0f,	0.0f, 1.0f,		// Top - Left
	/* 1 */	-0.5f, -0.5f, 0.0f,	1.0f, 1.0f, 1.0f,	0.0f, 0.0f,		// Bot - Left
	/* 2 */	0.5f, -0.5f, 0.0f,	1.0f, 1.0f, 1.0f,	1.0f, 0.0f,		// Bot - Right
	/* 3 */ 0.5f, 0.5f, 0.0f,	1.0f, 1.0f, 0.0f,	1.0f, 1.0f,		// Top - Right
};

GLuint Indices_Quad[] = {
	0, 1, 2,	// First Triangle ( TL -> BL -> BR )
	0, 2, 3,	// Second Triangle ( TL -> BR -> TR )
};

GLfloat Vertices_Cube[] = {
	// Index	// position				// color				// tex coords	// position index
	-1.0f, 1.0f, 1.0f,        1.0f,0.0f,0.0f,        0.0f, 1.0f,//front quad
	-1.0f, -1.0f, 1.0f,        1.0f,0.0f,0.0f,        0.0f, 0.0f,
	1.0f, -1.0f, 1.0f,        1.0f,0.0f,0.0f,        1.0f, 0.0f,
	1.0f, 1.0f, 1.0f,        1.0f,0.0f,0.0f,        1.0f, 1.0f,

	1.0f, 1.0f, -1.0f,        1.0f,0.0f,0.0f,        0.0f, 1.0f,    //back qd
	1.0f, -1.0f, -1.0f,        1.0f,0.0f,0.0f,        0.0f, 0.0f,
	-1.0f, -1.0f, -1.0f,    1.0f,0.0f,0.0f,        1.0f, 0.0f,
	-1.0f,    1.0f, -1.0f,    1.0f,0.0f,0.0f,        1.0f, 1.0f,

	1.0f, 1.0f, 1.0f,        1.0f,0.0f,0.0f,        0.0f, 1.0f, //right qd
	1.0f, -1.0f, 1.0f,        1.0f,0.0f,0.0f,        0.0f, 0.0f,
	1.0f, -1.0f, -1.0f,        1.0f,0.0f,0.0f,        1.0f, 0.0f,
	1.0f, 1.0f, -1.0f,        1.0f,0.0f,0.0f,        1.0f, 1.0f,

	-1.0f, 1.0f, -1.0f,        1.0f,0.0f,0.0f,        0.0f, 1.0f, //left qd
	-1.0f, -1.0f, -1.0f,    1.0f,0.0f,0.0f,        0.0f, 0.0f,
	-1.0f, -1.0f, 1.0f,        1.0f,0.0f,0.0f,        1.0f, 0.0f,
	-1.0f, 1.0f, 1.0f,        1.0f,0.0f,0.0f,        1.0f, 1.0f,

	-1.0f, 1.0f, -1.0f,        1.0f,0.0f,0.0f,        0.0f, 1.0f, //top qd
	-1.0f, 1.0f, 1.0f,        1.0f,0.0f,0.0f,        0.0f, 0.0f,
	1.0f, 1.0f, 1.0f,        1.0f,0.0f,0.0f,        1.0f, 0.0f,
	1.0f, 1.0f, -1.0f,        1.0f,0.0f,0.0f,        1.0f, 1.0f,

	-1.0f, -1.0f, 1.0f,        1.0f,0.0f,0.0f,        0.0f, 1.0f, //btm qd
	-1.0f, -1.0f, -1.0f,    1.0f,0.0f,0.0f,        0.0f, 0.0f,
	1.0f, -1.0f, -1.0f,        1.0f,0.0f,0.0f,        1.0f, 0.0f,
	1.0f, -1.0f, 1.0f,        1.0f,0.0f,0.0f,        1.0f, 1.0f
};

GLfloat Vertices_Cube2[] = {
	// Index	// position				// color				// tex coords	// position index
	-1.0f, 1.0f, 1.0f,        0.0f,1.0f,0.0f,        0.0f, 1.0f,//front quad
	-1.0f, -1.0f, 1.0f,        0.0f,1.0f,0.0f,        0.0f, 0.0f,
	1.0f, -1.0f, 1.0f,        0.0f,1.0f,0.0f,        1.0f, 0.0f,
	1.0f, 1.0f, 1.0f,        0.0f,1.0f,0.0f,        1.0f, 1.0f,

	1.0f, 1.0f, -1.0f,        0.0f,1.0f,0.0f,        0.0f, 1.0f,    //back qd
	1.0f, -1.0f, -1.0f,        0.0f,1.0f,0.0f,        0.0f, 0.0f,
	-1.0f, -1.0f, -1.0f,    0.0f,1.0f,0.0f,        1.0f, 0.0f,
	-1.0f,    1.0f, -1.0f,    0.0f,1.0f,0.0f,        1.0f, 1.0f,

	1.0f, 1.0f, 1.0f,        0.0f,1.0f,0.0f,        0.0f, 1.0f, //right qd
	1.0f, -1.0f, 1.0f,        0.0f,1.0f,0.0f,        0.0f, 0.0f,
	1.0f, -1.0f, -1.0f,        0.0f,1.0f,0.0f,        1.0f, 0.0f,
	1.0f, 1.0f, -1.0f,        0.0f,1.0f,0.0f,        1.0f, 1.0f,

	-1.0f, 1.0f, -1.0f,        0.0f,1.0f,0.0f,        0.0f, 1.0f, //left qd
	-1.0f, -1.0f, -1.0f,    0.0f,1.0f,0.0f,        0.0f, 0.0f,
	-1.0f, -1.0f, 1.0f,        0.0f,1.0f,0.0f,        1.0f, 0.0f,
	-1.0f, 1.0f, 1.0f,        0.0f,1.0f,0.0f,        1.0f, 1.0f,

	-1.0f, 1.0f, -1.0f,        0.0f,1.0f,0.0f,        0.0f, 1.0f, //top qd
	-1.0f, 1.0f, 1.0f,        0.0f,1.0f,0.0f,        0.0f, 0.0f,
	1.0f, 1.0f, 1.0f,        0.0f,1.0f,0.0f,        1.0f, 0.0f,
	1.0f, 1.0f, -1.0f,        0.0f,1.0f,0.0f,        1.0f, 1.0f,

	-1.0f, -1.0f, 1.0f,        0.0f,1.0f,0.0f,        0.0f, 1.0f, //btm qd
	-1.0f, -1.0f, -1.0f,    0.0f,1.0f,0.0f,        0.0f, 0.0f,
	1.0f, -1.0f, -1.0f,        0.0f,1.0f,0.0f,        1.0f, 0.0f,
	1.0f, -1.0f, 1.0f,        0.0f,1.0f,0.0f,        1.0f, 1.0f
};

GLuint Indices_Cube[] = {
	0,  1,  2,
	0,  2,  3,
	4,  5,  6,
	4,  6,  7,
	8,  9,  10,
	8,  10, 11,
	12, 13, 14,
	12, 14, 15,
	16, 17, 18,
	16, 18, 19,
	20, 21, 22,
	20, 22, 23
};

GLfloat Vertices_Pyramid[] = {
	// Index	// position				//color				// tex coords
				// bottom quad
	/* 00 */	-0.5f, 0.0f, -0.5f,		1.0f,1.0f,1.0f,		0.0f, 1.0f,
	/* 01 */	-0.5f, 0.0f, 0.5f,		1.0f,1.0f,1.0f,		0.0f, 0.0f,
	/* 02 */	0.5f, 0.0f, 0.5f,		1.0f,1.0f,1.0f,		1.0f, 0.0f,
	/* 03 */	0.5f, 0.0f, -0.5f,		1.0f,1.0f,1.0f,		1.0f, 1.0f,
				// side 1
	/* 04 */	-0.5f, 0.0f, -0.5f,		1.0f,1.0f,1.0f,		0.0f, 0.0f,
	/* 05 */	-0.5f, 0.0f, 0.5f,		1.0f,1.0f,1.0f,		1.0f, 0.0f,
				// side 2
	/* 06 */	-0.5f, 0.0f, 0.5f,		1.0f,1.0f,1.0f,		0.0f, 0.0f,
	/* 07 */	0.5f, 0.0f, 0.5f,		1.0f,1.0f,1.0f,		1.0f, 0.0f,
				// side 3
	/* 08 */	0.5f, 0.0f, 0.5f,		1.0f,1.0f,1.0f,		0.0f, 0.0f,
	/* 09 */	0.5f, 0.0f, -0.5f,		1.0f,1.0f,1.0f,		1.0f, 0.0f,
				// side 4
	/* 10 */	0.5f, 0.0f, -0.5f,		1.0f,1.0f,1.0f,		0.0f, 0.0f,
	/* 11 */	-0.5f, 0.0f, -0.5f,		1.0f,1.0f,1.0f,		1.0f, 0.0f,
				// top point
	/* 12 */	0.0f, 0.5f, 0.0f,		1.0f,1.0f,1.0f,		0.5f, 1.0f, // top point
};


GLuint Indices_Pyramid[] = {
	1, 0, 3, // Bottom (Quad) Tri 1
	1, 3, 2, // Bottom (Quad) Tri 2

	4, 5, 12, // Side 1
	6, 7, 12, // Side 2
	8, 9, 12, // Side 3
	10, 11, 12, // Side 4
};


// Object Matrices and Components
glm::vec3 QuadPosition = glm::vec3(0.5f, 0.5f, 0.0f);
glm::mat4 TranslationMat;

float QuadRotationAngle = 45.0f;
glm::mat4 RotationMat;

glm::vec3 QuadScale = glm::vec3(0.5f, 0.5f, 1.0f);
glm::mat4 ScaleMat;

glm::mat4 QuadModelMat;

// cube
glm::vec3 CubePosition = glm::vec3(200.0f, 200.0f, 0.0f);
float CubeRotationAngle = 0.0f;
glm::vec3 CubeScale = glm::vec3(10.0f, 10.0f, 10.0f);
glm::mat4 CubeModelMat;

glm::mat4 CubeModelMat2;
glm::vec3 CubePosition2 = glm::vec3(-200.0f, 200.0f, 0.0f);

// 3D model
glm::vec3 ThreeDPosition = glm::vec3(0.0f, 0.0f, -10.0f);
float ThreeDRotationAngle = 0.0f;
glm::vec3 ThreeDScale = glm::vec3(0.05f, 0.05f, 0.05f);
glm::mat4 ThreeDModelMat;


CCamera Camera;

Mesh_Model* MeshModel = nullptr;

Mesh_Model* TreeModel = nullptr;

Mesh_Model* ShipModel = nullptr;

Skybox* Skybox1 = nullptr;




int main()
{
	// initialise GLFW library
	glfwInit();
	
	// window hints net to be set before creation of a window
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);


	// creates window
	Window = glfwCreateWindow(WindowWidth, WindowHeight, "First OpenGL window", NULL, NULL);
	if (Window == NULL)
	{
		std::cout << "GLFW failed to initialise properly. Terminating program." << std::endl;
		system("pause");

		// destroys remaining windows and cursors
		glfwTerminate();
		return -1;
	}
	
	// all drawing happens to this context
	glfwMakeContextCurrent(Window);


	// must be called after GLFW window intialisation and before gl function calls
	// Populates all gl prefixed functions with correct calls based on current pc hardware and drivers
	// initialise GLEW to populate OpenGl function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW failed to initialise properly. Terminating program." << std::endl;
		system("pause");

		// destroys remaining windows and cursors
		glfwTerminate();
		return -1;
	}

	MeshModel = new Mesh_Model("Resources/Models/AncientEmpire/SM_Prop_Statue_01.obj");

	TreeModel = new Mesh_Model("Resources/Models/AncientEmpire/SM_Env_Tree_Palm_01.obj");

	ShipModel = new Mesh_Model("Resources/Models/SM_Ship_Stealth_02.obj");

	Skybox1 = new Skybox("Resources/Models/cube.obj");

	// set up initial elements of the program
	InitialSetup();

	// create the programs
	Program_PositionOnly = ShaderLoader::CreateProgram("Resources/Shaders/VertexColor.vert.txt",
		"Resources/Shaders/VertexColor.frag.txt");

	Program_ColorFade = ShaderLoader::CreateProgram("Resources/Shaders/VertexColor.vert.txt",
		"Resources/Shaders/VertexColorFade.frag.txt");

	Program_WorldSpace = ShaderLoader::CreateProgram("Resources/Shaders/WorldSpace.vert.txt",
		"Resources/Shaders/VertexColorFade.frag.txt");

	Program_Texture = ShaderLoader::CreateProgram("Resources/Shaders/Texture.vert.txt",
		"Resources/Shaders/Texture.frag.txt");

	Program_TextureMix = ShaderLoader::CreateProgram("Resources/Shaders/Texture.vert.txt",
		"Resources/Shaders/TextureMix.frag.txt");

	Program_3D = ShaderLoader::CreateProgram("Resources/Shaders/Texture3D.vert.txt",
		"Resources/Shaders/TextureMix3D.frag.txt");
		
	Program_3DCamera = ShaderLoader::CreateProgram("Resources/Shaders/CameraTexture3D.vert.txt",
		"Resources/Shaders/VertexColor.frag.txt");


	//Program_3DCamera = ShaderLoader::CreateProgram("Resources/Shaders/CameraTexture3D.vert.txt",
	//	"Resources/Shaders/TextureMix3D.frag.txt");

	Program_3DModel = ShaderLoader::CreateProgram("Resources/Shaders/3DModel.vert.txt",
		"Resources/Shaders/TextureMix3D.frag.txt");

	Program_LightingAmbient = ShaderLoader::CreateProgram("Resources/Shaders/LightingModel.vert.txt",
		"Resources/Shaders/LightingModel.frag.txt");

	Program_LightingBlinnPhong = ShaderLoader::CreateProgram("Resources/Shaders/LightingModel.vert.txt",
		"Resources/Shaders/Lighting_BlinnPhong.frag.txt");

	Program_Skybox = ShaderLoader::CreateProgram("Resources/Shaders/Skybox.vert.txt",
		"Resources/Shaders/Skybox.frag.txt");


	Program_Reflection = ShaderLoader::CreateProgram("Resources/Shaders/LightingModel.vert.txt",
		"Resources/Shaders/Reflection.frag.txt");

	// generate the VAO for a triangle
	glGenVertexArrays(1, &VAO_Tri);
	glBindVertexArray(VAO_Tri);
	
	// generate the VBO for a triangle
	glGenBuffers(1, &VBO_Tri);
	// bind the buffer to a specified slot based on the target
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Tri);
	// copy the actual data into the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_Tri), Vertices_Tri, GL_STATIC_DRAW);

	// set the vertex attribute information (how to interpret the vertex data)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);




	// generate the VAO 2 for second triangle
	glGenVertexArrays(1, &VAO_Tri2);
	glBindVertexArray(VAO_Tri2);

	// generate the VBO 2 for second triangle
	glGenBuffers(1, &VBO_Tri2);
	// bind the buffer to a specified slot based on the target
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Tri2);
	// copy the actual data into the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_Tri2), Vertices_Tri2, GL_STATIC_DRAW);

	// set the vertex attribute information (how to interpret the vertex data)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	
	
	// generate the VAO for a quad
	glGenVertexArrays(1, &VAO_Quad);
	glBindVertexArray(VAO_Quad);

	// generate the EBO for a quad
	glGenBuffers(1, &EBO_Quad);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Quad);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices_Quad), Indices_Quad, GL_STATIC_DRAW);

	// generate the VBO for a quad
	glGenBuffers(1, &VBO_Quad);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Quad);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_Quad), Vertices_Quad, GL_STATIC_DRAW);



	// set the vertex attribute information (how to interpret the vertex data)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	

	
	// generate the VAO for a cube
	glGenVertexArrays(1, &VAO_Cube);
	glBindVertexArray(VAO_Cube);

	// generate the EBO for a cube
	glGenBuffers(1, &EBO_Cube);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Cube);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices_Cube), Indices_Cube, GL_STATIC_DRAW);

	// generate the VBO for a cube
	glGenBuffers(1, &VBO_Cube);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Cube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_Cube), Vertices_Cube, GL_STATIC_DRAW);

	// set the vertex attribute information (how to interpret the vertex data)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// generate the VAO for a cube
	glGenVertexArrays(1, &VAO_Cube2);
	glBindVertexArray(VAO_Cube2);

	// generate the EBO for a cube
	glGenBuffers(1, &EBO_Cube2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Cube2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices_Cube), Indices_Cube, GL_STATIC_DRAW);

	// generate the VBO for a cube
	glGenBuffers(1, &VBO_Cube2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Cube2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_Cube2), Vertices_Cube2, GL_STATIC_DRAW);

	// set the vertex attribute information (how to interpret the vertex data)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	
	// generate the VAO for a cube
	glGenVertexArrays(1, &VAO_Pyramid);
	glBindVertexArray(VAO_Pyramid);

	// generate the EBO for a cube
	glGenBuffers(1, &EBO_Pyramid);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Pyramid);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices_Pyramid), Indices_Pyramid, GL_STATIC_DRAW);

	// generate the VBO for a cube
	glGenBuffers(1, &VBO_Pyramid);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Pyramid);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_Pyramid), Vertices_Pyramid, GL_STATIC_DRAW);

	// set the vertex attribute information (how to interpret the vertex data)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);	


	// culling set up
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	// Enable Depth testing for	3D
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// set camera window
	Camera.setWindow(Window);


	// Load the Image data
	stbi_set_flip_vertically_on_load(true);
	int ImageWidth;
	int ImageHeight;
	int ImageComponents;
	unsigned char* ImageData = stbi_load("Resources/Textures/shuckle.jpg",
		&ImageWidth, &ImageHeight, &ImageComponents, 0);


	// Create and bind a new texture variable
	glGenTextures(1, &Texture_shuckle);
	glBindTexture(GL_TEXTURE_2D, Texture_shuckle);

	// Setting the filtering and mipmap parameters for this texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Check how many components the loaded image has (RGBA or RBG?)
	GLint LoadedComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;

	// Populate the texture with the image data
	glTexImage2D(	GL_TEXTURE_2D, 0, LoadedComponents, ImageWidth, ImageHeight, 0,
					LoadedComponents, GL_UNSIGNED_BYTE, ImageData);

// Generate the mipmaps, free the memory and unbind the texture
glGenerateMipmap(GL_TEXTURE_2D);
stbi_image_free(ImageData);
glBindTexture(GL_TEXTURE_2D, 0);




// Load the Image data
stbi_set_flip_vertically_on_load(true);
ImageData = stbi_load("Resources/Textures/AwesomeFace.png",
	&ImageWidth, &ImageHeight, &ImageComponents, 0);


// Create and bind a new texture variable
glGenTextures(1, &Texture_AwesomeFace);
glBindTexture(GL_TEXTURE_2D, Texture_AwesomeFace);

// Setting the filtering and mipmap parameters for this texture
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

// Check how many components the loaded image has (RGBA or RBG?)
LoadedComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;

// Populate the texture with the image data
glTexImage2D(GL_TEXTURE_2D, 0, LoadedComponents, ImageWidth, ImageHeight, 0,
	LoadedComponents, GL_UNSIGNED_BYTE, ImageData);

// Generate the mipmaps, free the memory and unbind the texture
glGenerateMipmap(GL_TEXTURE_2D);
stbi_image_free(ImageData);
glBindTexture(GL_TEXTURE_2D, 0);



// soldier
// Load the Image data
stbi_set_flip_vertically_on_load(true);
ImageData = stbi_load("Resources/Textures/PolygonAncientWorlds_Statue_01.png",
	&ImageWidth, &ImageHeight, &ImageComponents, 0);


// Create and bind a new texture variable
glGenTextures(1, &Texture_Soldier);
glBindTexture(GL_TEXTURE_2D, Texture_Soldier);

// Setting the filtering and mipmap parameters for this texture
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

// Check how many components the loaded image has (RGBA or RBG?)
LoadedComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;

// Populate the texture with the image data
glTexImage2D(GL_TEXTURE_2D, 0, LoadedComponents, ImageWidth, ImageHeight, 0,
	LoadedComponents, GL_UNSIGNED_BYTE, ImageData);

// Generate the mipmaps, free the memory and unbind the texture
glGenerateMipmap(GL_TEXTURE_2D);
stbi_image_free(ImageData);
glBindTexture(GL_TEXTURE_2D, 0);



// tree
// Load the Image data
stbi_set_flip_vertically_on_load(true);
ImageData = stbi_load("Resources/Textures/PolygonAncientWorlds_Texture_01_A.png",
	&ImageWidth, &ImageHeight, &ImageComponents, 0);


// Create and bind a new texture variable
glGenTextures(1, &Texture_Tree);
glBindTexture(GL_TEXTURE_2D, Texture_Tree);

// Setting the filtering and mipmap parameters for this texture
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

// Check how many components the loaded image has (RGBA or RBG?)
LoadedComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;

// Populate the texture with the image data
glTexImage2D(GL_TEXTURE_2D, 0, LoadedComponents, ImageWidth, ImageHeight, 0,
	LoadedComponents, GL_UNSIGNED_BYTE, ImageData);

// Generate the mipmaps, free the memory and unbind the texture
glGenerateMipmap(GL_TEXTURE_2D);
stbi_image_free(ImageData);
glBindTexture(GL_TEXTURE_2D, 0);



// ship
// Load the Image data
stbi_set_flip_vertically_on_load(true);
ImageData = stbi_load("Resources/Textures/PolygonSciFiSpace_Texture_01_A.png",
	&ImageWidth, &ImageHeight, &ImageComponents, 0);


// Create and bind a new texture variable
glGenTextures(1, &Texture_Ship);
glBindTexture(GL_TEXTURE_2D, Texture_Ship);

// Setting the filtering and mipmap parameters for this texture
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

// Check how many components the loaded image has (RGBA or RBG?)
LoadedComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;

// Populate the texture with the image data
glTexImage2D(GL_TEXTURE_2D, 0, LoadedComponents, ImageWidth, ImageHeight, 0,
	LoadedComponents, GL_UNSIGNED_BYTE, ImageData);

// Generate the mipmaps, free the memory and unbind the texture
glGenerateMipmap(GL_TEXTURE_2D);
stbi_image_free(ImageData);
glBindTexture(GL_TEXTURE_2D, 0);


// ship
// Load the Image data
stbi_set_flip_vertically_on_load(true);
ImageData = stbi_load("Resources/ReflectionMap_White.png",
	&ImageWidth, &ImageHeight, &ImageComponents, 0);


// Create and bind a new texture variable
glGenTextures(1, &Texture_ReflectionMap);
glBindTexture(GL_TEXTURE_2D, Texture_ReflectionMap);

// Setting the filtering and mipmap parameters for this texture
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

// Check how many components the loaded image has (RGBA or RBG?)
LoadedComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;

// Populate the texture with the image data
glTexImage2D(GL_TEXTURE_2D, 0, LoadedComponents, ImageWidth, ImageHeight, 0,
	LoadedComponents, GL_UNSIGNED_BYTE, ImageData);

// Generate the mipmaps, free the memory and unbind the texture
glGenerateMipmap(GL_TEXTURE_2D);
stbi_image_free(ImageData);
glBindTexture(GL_TEXTURE_2D, 0);



// Create and bind a new texture variable
glGenTextures(1, &TextureID_Skybox);
glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID_Skybox);
stbi_set_flip_vertically_on_load(true);

// Storage variables
//int ImageWidth;
//int ImageHeight;
//int ImageComponents;

for (int i = 0; i < 6; ++i)
{
	// Load each image separately
	unsigned char* ImageData = stbi_load(FilePaths[i].c_str(), // need an array of file paths for the images
		&ImageWidth, &ImageHeight, &ImageComponents, 0);

	// cube map should be full color (RGB). Image might ome with Alpha
	GLint LoadedComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;

	// populate the texture with the image data
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
		0, LoadedComponents, ImageWidth, ImageHeight, 0,
		LoadedComponents, GL_UNSIGNED_BYTE, ImageData);

	// free each image data after creating the OpenGL texture
	stbi_image_free(ImageData);
}

// Setting the address mode for this texture
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

// generate the mipmaps, free the memory and unbind the texture
glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
glBindTexture(GL_TEXTURE_CUBE_MAP, 0);


float lastFrame = 0.0f;


glfwWindowHint(GLFW_SAMPLES, 4);
glEnable(GL_MULTISAMPLE);

// main loop
while (glfwWindowShouldClose(Window) == false)
{
	// delta time calculations
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	// update all objects and run the process
	Update();

	// render all the objects
	Render();
}
// ensure correct shutdown of GLFW
glfwTerminate();
return 0;
}

void CursorPositionInput(GLFWwindow* Window, double Xpos, double Ypos)
{
	// print cursor position to console if the cursor position changes
	if (coordsToggle == true)
	{
		std::cout << "Cursor Position: " << Xpos << ", " << Ypos << std::endl;
	}
}

void KeyInput(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods)
{
	if (Key == GLFW_KEY_ESCAPE && Action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(Window, true);
	}

	//// toggle cursor visibility with key C
	//if (Key == GLFW_KEY_C && Action == GLFW_PRESS)
	//{
	//	if (cursorToggle == true)
	//	{
	//		glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	//		cursorToggle = false;
	//	}
	//	else if (cursorToggle == false)
	//	{
	//		glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	//		cursorToggle = true;
	//	}
	//}

	// toggle wireframe mode with key F
	if (Key == GLFW_KEY_F && Action == GLFW_PRESS)
	{
		if (wireframeToggle == true)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			wireframeToggle = false;
		}
		else if (wireframeToggle == false)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			wireframeToggle = true;
		}
	}

	//// toggle coordinate display with key P
	//if (glfwGetKey(Window, GLFW_KEY_P) == GLFW_PRESS)
	//{
	//	if (coordsToggle == true)
	//	{
	//		coordsToggle = false;
	//	}
	//	else if (coordsToggle == false)
	//	{
	//		coordsToggle = true;
	//	}
	//}

	// toggle point light with key 1
	if (glfwGetKey(Window, GLFW_KEY_1) == GLFW_PRESS)
	{
		MeshModel->TogglePointLight();
	}

	// toggle directional light with key 2
	if (glfwGetKey(Window, GLFW_KEY_2) == GLFW_PRESS)
	{
		MeshModel->ToggleDirectionalLight();
	}


}

void MouseButtonInput(GLFWwindow* Window, int Button, int Action, int Mods)
{
	if (Button == GLFW_MOUSE_BUTTON_LEFT && Action == GLFW_PRESS)
	{
		//std::cout << "Mouse button left: Press" << std::endl;
	}
	else if (Button == GLFW_MOUSE_BUTTON_LEFT && Action == GLFW_RELEASE)
	{
		//std::cout << "Mouse button left: Release" << std::endl;
	}


	if (Button == GLFW_MOUSE_BUTTON_RIGHT && Action == GLFW_PRESS)
	{
		//std::cout << "Mouse button right: Press" << std::endl;
	}
	else if (Button == GLFW_MOUSE_BUTTON_RIGHT && Action == GLFW_RELEASE)
	{
		//std::cout << "Mouse button right: Release" << std::endl;
	}
}

void TextInput(GLFWwindow* Window, unsigned int Codepoint)
{
	
	//std::cout << "Key Pressed: " << (unsigned char)Codepoint << std::endl;
}

// setup the initial elements of the program
void InitialSetup()
{
	// set the color of the window for when the buffer is cleared
	glClearColor(1.0f, 0.647f, 0.0f, 1.0f);

	// maps the range of the window size to NDC (-1 -> 1)
	// mapping size of GLFW window to the viewport helps OpenGl place the vertex data in relation to the window dimensions
	glViewport(0, 0, 800, 800);

	// Set the callback functions for input processing
	glfwSetKeyCallback(Window, KeyInput);
	glfwSetCharCallback(Window, TextInput);
	glfwSetMouseButtonCallback(Window, MouseButtonInput);
	glfwSetCursorPosCallback(Window, CursorPositionInput);
	


	glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// add instances of tree and soldier model using Mesh_Model class
	glm::mat4 Mat4Instance = glm::mat4(1.0f);

	TranslationMat = glm::translate(glm::mat4(1.0f), ThreeDPosition);
	RotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(ThreeDRotationAngle) * 1, glm::vec3(1.0f, 1.0f, 1.0f));
	ScaleMat = glm::scale(glm::mat4(1.0f), ThreeDScale);
	Mat4Instance = TranslationMat * RotationMat * ScaleMat;

	TreeModel->AddInstances(Mat4Instance, 256);
	MeshModel->AddInstances(Mat4Instance, 1);
	ShipModel->AddInstances(Mat4Instance, 1);
	Skybox1->AddInstances(Mat4Instance, 1);


}









void ProcessInput(float DeltaTime)
{
	float CameraSpeed = 100.0f;

	double Xpos;
	double Ypos;
	glfwGetCursorPos(Window, &Xpos, &Ypos);
	if (Xpos > (WindowWidth * 0.5f) && Ypos > (WindowHeight * 0.5f))
	{
		//std::cout << "Cursor is in lower right quadrant" << std::endl;
	}
	
	if (glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		//std::cout << "Mouse button left: Press" << std::endl;
	}




}

void Update()
{
	
	glfwPollEvents();

	// get the current time
	CurrentTime = (float)glfwGetTime();
	Oscillate = ((sin(CurrentTime*5)) / 2)*2;

	// Calculate the Model Matrix
	TranslationMat = glm::translate(glm::mat4(1.0f), QuadPosition * Oscillate);
	RotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(QuadRotationAngle) * 1, glm::vec3(0.0f, 0.0f, 1.0f));
	ScaleMat = glm::scale(glm::mat4(1.0f), QuadScale * Oscillate);
	QuadModelMat = TranslationMat * RotationMat * ScaleMat;

	// Calculate Cube Model Matrix
	TranslationMat = glm::translate(glm::mat4(1.0f), CubePosition);
	RotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(CubeRotationAngle) * 1, glm::vec3(1.0f, 1.0f, 1.0f));
	ScaleMat = glm::scale(glm::mat4(1.0f), CubeScale);
	CubeModelMat = TranslationMat * RotationMat * ScaleMat;

	TranslationMat = glm::translate(glm::mat4(1.0f), CubePosition2);
	CubeModelMat2 = TranslationMat * RotationMat * ScaleMat;

	TranslationMat = glm::translate(glm::mat4(1.0f), ThreeDPosition);
	RotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(ThreeDRotationAngle) * 1, glm::vec3(1.0f, 1.0f, 1.0f));
	ScaleMat = glm::scale(glm::mat4(1.0f), ThreeDScale);
	ThreeDModelMat = TranslationMat * RotationMat * ScaleMat;
	
	ScaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(100.0f, 100.0f, 100.0f));

	ProcessInput(deltaTime);
	Camera.Update(deltaTime, manualToggle);
}

// render all the objects
void Render()
{
	// uses double buffering where front buffer contain all color data of final image shown on the screen
	// back buffer where all graphics rendering commands are taking place changing the color information of the image
	// when render complete buffers swapped, where back buffer becomes new front buffer displayed on the screen
	// front buffer becomes back buffer ready for new commands to change the immage
	// single buffers have info written and displayed at the same time, causing flickering if imagine not fully updated between frames
	
	glClear(GL_COLOR_BUFFER_BIT);

	// camera cube
	glUseProgram(Program_3DCamera);
	glBindVertexArray(VAO_Cube);


	// send CubeModel to the shader via uniform
	GLint CameraModelMatLoc = glGetUniformLocation(Program_3DCamera, "CubeModelMat");
	glUniformMatrix4fv(CameraModelMatLoc, 1, GL_FALSE, glm::value_ptr(CubeModelMat));
	GLint ViewMatLoc = glGetUniformLocation(Program_3DCamera, "ViewMat");
	glUniformMatrix4fv(ViewMatLoc, 1, GL_FALSE, glm::value_ptr(Camera.GetViewMat()));
	GLint ProjectionMatLoc = glGetUniformLocation(Program_3DCamera, "ProjectionMat");
	glUniformMatrix4fv(ProjectionMatLoc, 1, GL_FALSE, glm::value_ptr(Camera.GetProjMat()));

	// render the cube
	glDrawElements(GL_TRIANGLES, sizeof(Indices_Cube), GL_UNSIGNED_INT, 0);


	// camera cube
	glUseProgram(Program_3DCamera);
	glBindVertexArray(VAO_Cube2);


	// send CubeModel to the shader via uniform
	glUniformMatrix4fv(CameraModelMatLoc, 1, GL_FALSE, glm::value_ptr(CubeModelMat2));
	glUniformMatrix4fv(ViewMatLoc, 1, GL_FALSE, glm::value_ptr(Camera.GetViewMat()));
	glUniformMatrix4fv(ProjectionMatLoc, 1, GL_FALSE, glm::value_ptr(Camera.GetProjMat()));

	// render the cube
	glDrawElements(GL_TRIANGLES, sizeof(Indices_Cube), GL_UNSIGNED_INT, 0);
	
	// render instances of Mesh_Model class
	MeshModel->Render(Program_LightingBlinnPhong, Texture_Soldier, ThreeDModelMat, Camera.GetViewMat(), Camera.GetProjMat(), Camera.GetCameraPos(), TextureID_Skybox, Texture_ReflectionMap);
	TreeModel->Render(Program_LightingBlinnPhong, Texture_Tree, ThreeDModelMat, Camera.GetViewMat(), Camera.GetProjMat(), Camera.GetCameraPos(), TextureID_Skybox, Texture_ReflectionMap);
	ShipModel->Render(Program_Reflection, Texture_Ship, ThreeDModelMat, Camera.GetViewMat(), Camera.GetProjMat(), Camera.GetCameraPos(), TextureID_Skybox, Texture_ReflectionMap);

	// render the skybox using an instance of Skybox class
	Skybox1->Render(Program_Skybox, TextureID_Skybox, ThreeDModelMat, Camera.GetViewMat(), Camera.GetProjMat(), Camera.GetCameraPos(), TextureID_Skybox);

	// unbind assets to prevent accidental use/modification
	glBindVertexArray(0);
	glUseProgram(0);

	// swaps front and back buffers of specified window
	glfwSwapBuffers(Window);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}
