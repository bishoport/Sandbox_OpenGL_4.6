#include "ShadowsLayer.h"

#include<filesystem>
namespace fs = std::filesystem;

using namespace GLCore;
using namespace GLCore::Utils;

ShadowsLayer::ShadowsLayer(){}

ShadowsLayer::~ShadowsLayer(){}

void ShadowsLayer::OnAttach()
{
	EnableGLDebugging();

	shaderProgram = new GLCore::Shader("assets/shaders/shadows/default.vert", "assets/shaders/shadows/default.frag");
	framebufferProgram = new GLCore::Shader("assets/shaders/shadows/framebuffer.vert", "assets/shaders/shadows/framebuffer.frag");
	shadowMapProgram = new GLCore::Shader("assets/shaders/shadows/shadowMap.vert", "assets/shaders/shadows/shadowMap.frag");

	shaderProgram->use();
	shaderProgram->setVec4("lightColor", lightColor);
	shaderProgram->setVec3("lightPos", lightPos);

	framebufferProgram->use();
	framebufferProgram->setInt("screenTexture", 0);
	framebufferProgram->setFloat("gamma", gamma);

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Enables Multisampling
	glEnable(GL_MULTISAMPLE);

	// Enables Cull Facing
	glEnable(GL_CULL_FACE);
	// Keeps front faces
	glCullFace(GL_FRONT);

	// Uses counter clock-wise standard
	glFrontFace(GL_CCW);

	camera = new Camera(1280,720,glm::vec3(0.0f, 0.0f, 2.0f));

	std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
	std::string modelPath = "/OpenGL-Sandbox/assets/meshes/GLTF/Cubo.gltf";

	// Load in models
	model = new Model((parentDir + modelPath).c_str());

	window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
}

void ShadowsLayer::OnDetach(){}

void ShadowsLayer::OnEvent(GLCore::Event& event){}

void ShadowsLayer::OnUpdate(GLCore::Timestep ts)
{
	// Updates counter and times
	crntTime = glfwGetTime();
	timeDiff = crntTime - prevTime;
	counter++;

	if (timeDiff >= 1.0 / 30.0)
	{
		// Creates new title
		std::string FPS = std::to_string((1.0 / timeDiff) * counter);
		std::string ms = std::to_string((timeDiff / counter) * 1000);
		std::string newTitle = "PAJARRACO - " + FPS + "FPS / " + ms + "ms";
		
		glfwSetWindowTitle(window, newTitle.c_str());

		// Resets times and counter
		prevTime = crntTime;
		counter = 0;

		// Use this if you have disabled VSync
		//camera->Inputs(window);
	}


	// Depth testing needed for Shadow Map
	glEnable(GL_DEPTH_TEST);

	// Preparations for the Shadow Map
	/*glViewport(0, 0, shadowMapWidth, shadowMapHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);*/

	// Draw scene for shadow map
	model->Draw(*shadowMapProgram, *camera);


	// Switch back to the default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Switch back to the default viewport
	glViewport(0, 0, 1280, 720);
	// Bind the custom framebuffer
	//glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	// Specify the color of the background
	glClearColor(pow(0.07f, gamma), pow(0.13f, gamma), pow(0.17f, gamma), 1.0f);
	// Clean the back buffer and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Enable depth testing since it's disabled when drawing the framebuffer rectangle
	glEnable(GL_DEPTH_TEST);

	// Handles camera inputs (delete this if you have disabled VSync)
	camera->Inputs(window);
	// Updates and exports the camera matrix to the Vertex Shader
	camera->updateMatrix(45.0f, 0.1f, 100.0f);







	//// Send the light matrix to the shader
	shaderProgram->use();
	shaderProgram->setMat4("lightProjection", lightProjection);
	//glUniformMatrix4fv(glGetUniformLocation(shaderProgram->ID, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));

	//// Bind the Shadow Map
	//glActiveTexture(GL_TEXTURE0 + 2);
	//glBindTexture(GL_TEXTURE_2D, shadowMap);
	//glUniform1i(glGetUniformLocation(shaderProgram.ID, "shadowMap"), 2);

	// Draw the normal model
	model->Draw(*shaderProgram, *camera);

	
}

void ShadowsLayer::OnImGuiRender()
{
}
