#pragma once

#include <glm/glm.hpp>
#include <GLCoreUtils.h>
#include <vector>

namespace GLCore::Utils {


	class GridWorld
	{
	public:
		GridWorld();
		~GridWorld();

		void draw();
		
	private:
		unsigned int VAO;
		std::vector<float> vertices;
		GLuint lenght;
		int slices = 10;
		float sizeMultiplicator = 10.2f;
		
	};
}