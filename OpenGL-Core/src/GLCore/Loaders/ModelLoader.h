#pragma once
#include "glpch.h"

#include "../DataStructs.h"



#include <glad/glad.h>

namespace GLCore::Loaders 
{
    // Función para cargar los datos del modelo 3D utilizando Assimp
    bool loadModelAssimp(const std::string& filePath, const std::string& fileName, std::vector<ModelInfo>& models, glm::mat4 preTransform);

}
