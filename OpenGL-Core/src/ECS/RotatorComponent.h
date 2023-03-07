#pragma once
#include "glpch.h"
#include "ECSCore.h"

class CubeRotator : public ECS::Component
{

    void update() override
    {
         entity->getComponent<Transform>().eulers.y += 0.001f;
    }

};
