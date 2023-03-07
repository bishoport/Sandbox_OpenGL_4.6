#pragma once
#include "glpch.h"
#include "ECSCore.h"

class PointLight : public ECS::Component
{
public:

	void init() override{}

	void setLightValues(glm::vec3 position, glm::vec3 color, float strength, unsigned int lightID, GLCore::Shader* mainShader, GLCore::Shader* debugShader)
	{
		this->position = position;
		this->color = color;
		this->strength = strength;
		this->lightID = lightID;
		this->mainShader = mainShader;
		this->debugShader = debugShader;

		//GET LOCATIONS
		location.str("");
		location << "lights[" << lightID << "].color";
		colorLoc = glGetUniformLocation(mainShader->ID, location.str().c_str());
		location.str("");
		location << "lights[" << lightID << "].position";
		positionLoc = glGetUniformLocation(mainShader->ID, location.str().c_str());
		location.str("");
		location << "lights[" << lightID << "].strength";
		strengthLoc = glGetUniformLocation(mainShader->ID, location.str().c_str());




		// Definir los parámetros de la esfera de debug
		const int numSlices = 6;
		const int numStacks = 6;
		const float radius = 1.0f;

		// Crear los vértices de la esfera
		for (int i = 0; i <= numStacks; i++) {
			float phi = glm::pi<float>() * i / numStacks;
			float cosPhi = cos(phi);
			float sinPhi = sin(phi);

			for (int j = 0; j <= numSlices; j++) {
				float theta = 2 * glm::pi<float>() * j / numSlices;
				float cosTheta = cos(theta);
				float sinTheta = sin(theta);

				glm::vec3 vertex(cosTheta * sinPhi, cosPhi, sinTheta * sinPhi);
				sphereVertices.push_back(radius * vertex);
			}
		}

		// Crear los índices de la esfera
		for (int i = 0; i < numStacks; i++) {
			int stackStart = i * (numSlices + 1);
			int nextStackStart = (i + 1) * (numSlices + 1);

			for (int j = 0; j < numSlices; j++) {
				int vertex1 = stackStart + j;
				int vertex2 = stackStart + j + 1;
				int vertex3 = nextStackStart + j + 1;
				int vertex4 = nextStackStart + j;

				sphereIndices.push_back(vertex1);
				sphereIndices.push_back(vertex2);
				sphereIndices.push_back(vertex3);

				sphereIndices.push_back(vertex1);
				sphereIndices.push_back(vertex3);
				sphereIndices.push_back(vertex4);
			}
		}


		// Calcular las normales de la esfera
		for (unsigned int i = 0; i < sphereVertices.size(); i++) {
			glm::vec3 normal = glm::normalize(sphereVertices[i]);
			sphereNormals.push_back(normal);
		}
	}


	void update() override{}

	void draw() override
	{
		mainShader->use();
		glUniform3fv(colorLoc, 1, glm::value_ptr(color));
		glUniform3fv(positionLoc, 1, glm::value_ptr(position));
		glUniform1f(strengthLoc, strength);

		drawDebug();
	}

	void drawDebug()
	{

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// Crear los VBOs (Vertex Buffer Objects) para los vértices y las normales de la esfera
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(glm::vec3), &sphereVertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		GLuint nbo;
		glGenBuffers(1, &nbo);
		glBindBuffer(GL_ARRAY_BUFFER, nbo);
		glBufferData(GL_ARRAY_BUFFER, sphereNormals.size() * sizeof(glm::vec3), &sphereNormals[0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		// Crear el VBO (Vertex Buffer Object) para los índices de la esfera
		GLuint ebo;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereIndices.size() * sizeof(GLuint), &sphereIndices[0], GL_STATIC_DRAW);

		glm::mat4 modelMatrix = glm::mat4(1.0f);
		glm::vec3 size = glm::vec3(0.3f, 0.3f, 0.3f);
		modelMatrix = glm::translate(modelMatrix, position) *glm::scale(glm::mat4(1), size);

		debugShader->use();
		debugShader->setMat4("model", modelMatrix);


		// Dibujar la esfera
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, sphereIndices.size(), GL_UNSIGNED_INT, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	void onDestroy() override
	{
		glDeleteVertexArrays(1, &vao);
	}



private:
	std::stringstream location;
	unsigned int colorLoc, positionLoc, strengthLoc;

	glm::vec3 position, color;
	float strength;
	unsigned int lightID = 0;

	GLCore::Shader* mainShader;
	GLCore::Shader* debugShader;

	GLuint vao;
	std::vector<glm::vec3> sphereVertices;
	std::vector<unsigned int> sphereIndices;
	std::vector<glm::vec3> sphereNormals;
};
