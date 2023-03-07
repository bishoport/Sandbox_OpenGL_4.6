#include "glpch.h"
#include "PerspectiveCamera.h"

#include <glm/gtc/matrix_transform.hpp>

GLCore::Utils::PerspectiveCamera::PerspectiveCamera():m_ViewMatrix(1.0f)
{
	m_ProjectionMatrix = glm::perspective(glm::radians(m_Fov), (float)1280 / (float)720, 0.1f, 100.0f);
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void GLCore::Utils::PerspectiveCamera::SetProjection()
{
	m_ProjectionMatrix = glm::perspective(glm::radians(m_Fov), (float)1280 / (float)720, 0.1f, 100.0f);
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void GLCore::Utils::PerspectiveCamera::RecalculateViewMatrix()
{
    glm::mat4 transform = glm::translate( glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), m_Up);

    m_ViewMatrix = glm::inverse(transform);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}
