#include "GLCore.h"
#include "ExampleLayer.h"

using namespace GLCore;

class Example : public Application
{
public:
	Example()
		: Application("OpenGL Examples",1280,720)
	{
		PushLayer(new ExampleLayer());
	}
};

int main()
{
	std::unique_ptr<Example> app = std::make_unique<Example>();
	app->Run();
}