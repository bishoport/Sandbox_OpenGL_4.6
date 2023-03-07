#pragma once

#include <glm/glm.hpp>

namespace GLCore::Utils {

	class PerspectiveCamera
	{
	public:
		PerspectiveCamera();

		void SetProjection();

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		float GetRotation() const { return m_Rotation; }
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }
		void SetFront(glm::vec3 _m_Front) { m_Front = _m_Front; RecalculateViewMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
		
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Front = { 0.0f, 0.0f, -1.0f };
		glm::vec3 m_Up = { 0.0f, 1.0f, 0.0f };
		glm::vec3 m_Right = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_WorldUp = { 0.0f, 0.0f, 0.0f };

	private:
		void RecalculateViewMatrix();
	private:
		/*glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;*/

		/*glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Front = { 0.0f, 0.0f, -1.0f };
		glm::vec3 m_Up = { 0.0f, 1.0f, 0.0f };
		glm::vec3 m_Right = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_WorldUp = { 0.0f, 0.0f, 0.0f };*/

		// euler Angles
		float m_Yaw = 0.0f;
		float m_Pitch = 0.0f;
		float m_Rotation = 0.0f;
		float m_Fov = 45.0f;
	};

}
