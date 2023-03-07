#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>
#include <GLCoreLoaders.h>
#include <ECSCore.h>


class SandboxLayer : public GLCore::Layer
{
public:
	SandboxLayer();
	virtual ~SandboxLayer();

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnEvent(GLCore::Event& event) override;
	virtual void OnUpdate(GLCore::Timestep ts) override;
	virtual void OnImGuiRender() override;

private:
	GLCore::Shader* main_shader;
	GLCore::Shader* debug_shader;

	GLCore::Utils::PerspectiveCameraController m_PerspectiveCameraController;

	ECS::Entity* gridWorldReference = nullptr;
};