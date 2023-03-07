#pragma once

#include "glpch.h"
#include "ECSCore.h"

class Transform : public ECS::Component
{
public :
	glm::vec3 position, eulers;

};
