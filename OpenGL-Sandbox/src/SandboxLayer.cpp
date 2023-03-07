#include "SandboxLayer.h"
#include "Glad/glad.h"


using namespace GLCore;
using namespace GLCore::Utils;
using namespace GLCore::Loaders;

ECS::Manager manager;

SandboxLayer::SandboxLayer() : m_PerspectiveCameraController(16.0f / 9.0f,true)
{
}

SandboxLayer::~SandboxLayer()
{
}

void SandboxLayer::OnAttach()
{
	EnableGLDebugging();
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	//CARGA DE SHADERS
	main_shader  = new GLCore::Shader("assets/shaders/complete_shader.vert.glsl","assets/shaders/complete_shader.frag.glsl");
	debug_shader = new GLCore::Shader("assets/shaders/debug_shader.vert.glsl", "assets/shaders/debug_shader.frag.glsl");

	//GRID WORLD REFERENCE
	gridWorldReference = &manager.addEntity();
	gridWorldReference->name = "GridWorld";
	gridWorldReference->addComponent<GridWorldReferenceComp>();
	gridWorldReference->getComponent<GridWorldReferenceComp>().mainShader = main_shader;


	//CARGA DE TEXTURAS CON ASSIMP
	std::vector<ModelInfo> models;
	GLCore::Loaders::loadModelAssimp("assets/meshes/","Meta_Box.fbx", models, glm::mat4(0.1));

	std::cout << "models SIZE: " << models.size() << std::endl;
	for (int i = 0; i < models.size(); i++)
	{
		//ENTITY CREATION
		ECS::Entity* entity= &manager.addEntity();
		entity->name = "TEAPOT";
		entity->addComponent<Drawable>().setModelInfo(models[i], main_shader, debug_shader);
	}


	//LUCES
	ECS::Entity* LightPoint_1 = &manager.addEntity();
	LightPoint_1->addComponent<PointLight>();
	LightPoint_1->getComponent<PointLight>().setLightValues(glm::vec3(0, 2, 0), glm::vec3(0.5, 0.5, 0.5), 10.0f, 0, main_shader,debug_shader);

	ECS::Entity* LightPoint_2 = &manager.addEntity();
	LightPoint_2->addComponent<PointLight>();
	LightPoint_2->getComponent<PointLight>().setLightValues(glm::vec3(-2, 2, 1), glm::vec3(1, 1, 1), 10.0f, 1, main_shader, debug_shader);

}




void SandboxLayer::OnDetach()
{

}

void SandboxLayer::OnEvent(Event& event)
{
	// Events here
	m_PerspectiveCameraController.OnEvent(event);
}

void SandboxLayer::OnUpdate(Timestep ts)
{
	manager.refresh();
	manager.update();

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_PerspectiveCameraController.OnUpdate(ts);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glEnable(GL_POLYGON_OFFSET_FILL);
	//glPolygonOffset(1, 0);


	//DEBUG SHADER
	debug_shader->use();
	debug_shader->setMat4("projection", m_PerspectiveCameraController.GetCamera().GetProjectionMatrix());
	debug_shader->setMat4("view", m_PerspectiveCameraController.GetCamera().GetViewMatrix());

	//COMPLETE SHADER
	main_shader->use();
	main_shader->setMat4("projection", m_PerspectiveCameraController.GetCamera().GetProjectionMatrix());
	main_shader->setMat4("view", m_PerspectiveCameraController.GetCamera().GetViewMatrix());

	


	manager.draw();
	
}

void SandboxLayer::OnImGuiRender()
{
	// ImGui here
}
