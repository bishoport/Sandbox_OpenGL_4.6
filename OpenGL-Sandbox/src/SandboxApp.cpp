#include "GLCore.h"

//Layers
#include "SandboxLayer.h"
#include "ShadowsLayer.h"

using namespace GLCore;

class Sandbox : public Application
{
public:
	Sandbox()
	{
		//PushLayer(new SandboxLayer());
		PushLayer(new ShadowsLayer());
	}
};

int main()
{
	std::unique_ptr<Sandbox> app = std::make_unique<Sandbox>();
	app->Run();
}