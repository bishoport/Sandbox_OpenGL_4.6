#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>
#include <GLCore/Core/Render/Shader.h>
#include <GLCore/Core/Render/Camera.h>
#include <GLCore/Core/Render/Model.h>


class ShadowsLayer :public GLCore::Layer
{

public:
	ShadowsLayer();
	virtual ~ShadowsLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(GLCore::Event& event) override;
	virtual void OnUpdate(GLCore::Timestep ts) override;
	virtual void OnImGuiRender() override;

private:

	// Variables to create periodic event for FPS displaying
	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;
	// Keeps track of the amount of frames in timeDiff
	unsigned int counter = 0;

	// Number of samples per pixel for MSAA
	unsigned int samples = 8;

	// Controls the gamma function
	float gamma = 1.2f;

	// Take care of all the light related things
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 10.5f, 10.5f);

	glm::mat4 orthgonalProjection = glm::ortho(-35.0f, 35.0f, -35.0f, 35.0f, 0.1f, 75.0f);
	glm::mat4 lightView = glm::lookAt(20.0f * lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightProjection = orthgonalProjection * lightView;

	

	GLCore::Shader* shaderProgram;
	GLCore::Shader* framebufferProgram;
	GLCore::Shader* shadowMapProgram;

	GLFWwindow* window;
	Camera* camera;

	Model* model;
};

