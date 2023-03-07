#pragma once
#include "glpch.h"

#include "../DataStructs.h"


namespace GLCore::Loaders {
	GLCore::MeshInfo load_model_from_file(const char* filename, glm::mat4 preTransform);
}
