#pragma once
#include "glpch.h"
#include "ECSCore.h"

#include <GLCore/Core/Render/Shader.h>
#include <GLCore/DataStructs.h>

class MeshFilter : public ECS::Component
{

public:

	unsigned int VBO, VAO, EBO, vertexCount;

	GLCore::Shader* mainShader;
	GLCore::Shader* debugShader;

	GLCore::MeshInfo meshInfo;
	glm::mat4 model_transform_public;

	glm::vec3 center = glm::vec3(0.0f);

	//Bounding box
	unsigned int VBO_BB, VAO_BB, EBO_BB;
	glm::vec3 minPoint, maxPoint;
	

	// Definimos los vértices del bounding box en el espacio del objeto
	GLfloat vertices[24] = {
		-0.5f, -0.5f, -0.5f,  // Vértice 0
		 0.5f, -0.5f, -0.5f,  // Vértice 1
		 0.5f,  0.5f, -0.5f,  // Vértice 2
		-0.5f,  0.5f, -0.5f,  // Vértice 3
		-0.5f, -0.5f,  0.5f,  // Vértice 4
		 0.5f, -0.5f,  0.5f,  // Vértice 5
		 0.5f,  0.5f,  0.5f,  // Vértice 6
		-0.5f,  0.5f,  0.5f   // Vértice 7
	};

	// Definimos los índices de los vértices del bounding box que forman las líneas
	GLuint indices[24] = {
		0, 1, 1, 2, 2, 3, 3, 0,  // Líneas de la cara frontal
		4, 5, 5, 6, 6, 7, 7, 4,  // Líneas de la cara trasera
		0, 4, 1, 5, 2, 6, 3, 7   // Líneas diagonales
	};




	void setMesh(GLCore::MeshInfo _meshInfo)
	{
		this->meshInfo = _meshInfo;

		//vertexCount = int(meshInfo.allBuffer.size()) / 8;
		//glCreateBuffers(1, &VBO);
		//glCreateVertexArrays(1, &VAO);
		//glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 8 * sizeof(float));
		//glNamedBufferStorage(VBO, meshInfo.allBuffer.size() * sizeof(float), meshInfo.allBuffer.data(), GL_DYNAMIC_STORAGE_BIT);

		////pos: 0, texcoord: 1, normal: 2
		//glEnableVertexArrayAttrib(VAO, 0);
		//glEnableVertexArrayAttrib(VAO, 1);
		//glEnableVertexArrayAttrib(VAO, 2);

		//glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
		//glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
		//glVertexArrayAttribFormat(VAO, 2, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float));

		//glVertexArrayAttribBinding(VAO, 0, 0);
		//glVertexArrayAttribBinding(VAO, 1, 0);
		//glVertexArrayAttribBinding(VAO, 2, 0);

		//int numVertices = meshInfo.verticesV4.size();

		//// Suma de las coordenadas de todos los vértices en cada eje
		//for (int i = 0; i < meshInfo.num_indices; i++) {
		//	center.x += meshInfo.verticesV4[i].x;
		//	center.y += meshInfo.verticesV4[i].y;
		//	center.z += meshInfo.verticesV4[i].z;
		//}
		//// División de la suma obtenida en cada eje por el número total de vértices
		//center /= meshInfo.num_indices;


		//CalculateAABB(VAO, numVertices, minPoint, maxPoint);
	}


	void update() override{}

			


	void draw() override
	{
		//draw_bb();
		drawMesh();
	}

	void drawMesh()
	{
		glBindVertexArray(VAO);

		glm::mat4 model_transform{ glm::mat4(1.0f) };
		model_transform = glm::translate(model_transform, entity->getComponent<Transform>().position);
		//model_transform = glm::translate(model_transform, center);

		model_transform =
			model_transform * glm::eulerAngleXYZ(
				entity->getComponent<Transform>().eulers.x,
				entity->getComponent<Transform>().eulers.y,
				entity->getComponent<Transform>().eulers.z
			);

		model_transform_public = model_transform;

		mainShader->use();
		mainShader->setMat4("model", model_transform);
		glDrawArrays(GL_TRIANGLES, 0, meshInfo.num_indices);
		glBindVertexArray(0);
	}


	void CalculateAABB(GLuint vao, int count, glm::vec3& minPoint, glm::vec3& maxPoint) 
	{

		minPoint.x = meshInfo.allBuffer[0];
		minPoint.y = meshInfo.allBuffer[1];
		minPoint.z = meshInfo.allBuffer[2];
		maxPoint.x = meshInfo.allBuffer[0];
		maxPoint.y = meshInfo.allBuffer[1];
		maxPoint.z = meshInfo.allBuffer[2];

		for (int i = 0; i < count; i++) {
			glm::vec3 vertex;
			vertex.x = meshInfo.allBuffer[i * 8];
			vertex.y = meshInfo.allBuffer[i * 8 + 1];
			vertex.z = meshInfo.allBuffer[i * 8 + 2];
			if (vertex.x < minPoint.x) {
				minPoint.x = vertex.x;
			}
			if (vertex.y < minPoint.y) {
				minPoint.y = vertex.y;
			}
			if (vertex.z < minPoint.z) {
				minPoint.z = vertex.z;
			}
			if (vertex.x > maxPoint.x) {
				maxPoint.x = vertex.x;
			}
			if (vertex.y > maxPoint.y) {
				maxPoint.y = vertex.y;
			}
			if (vertex.z > maxPoint.z) {
				maxPoint.z = vertex.z;
			}
		}
	}
	void draw_bb()
	{
		// Creamos un VAO y lo vinculamos
		glGenVertexArrays(1, &VAO_BB);
		glBindVertexArray(VAO_BB);

		// Creamos un VBO y lo vinculamos al VAO
		glGenBuffers(1, &VBO_BB);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_BB);

		// Almacenamos los datos de los vértices en el VBO
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


		// Especificamos los atributos de los vértices
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

		// Habilitamos el atributo de posición
		glEnableVertexAttribArray(0);

		// Creamos un EBO y lo vinculamos al VAO

		glGenBuffers(1, &EBO_BB);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_BB);

		// Almacenamos los índices de los vértices en el EBO
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		GLfloat
			min_x, max_x,
			min_y, max_y,
			min_z, max_z;

		min_x = max_x = meshInfo.verticesV4[0].x;
		min_y = max_y = meshInfo.verticesV4[0].y;
		min_z = max_z = meshInfo.verticesV4[0].z;

		for (int i = 0; i < meshInfo.vertices.size(); i++) {
			if (meshInfo.verticesV4[i].x < min_x) min_x = meshInfo.verticesV4[i].x;
			if (meshInfo.verticesV4[i].x > max_x) max_x = meshInfo.verticesV4[i].x;
			if (meshInfo.verticesV4[i].y < min_y) min_y = meshInfo.verticesV4[i].y;
			if (meshInfo.verticesV4[i].y > max_y) max_y = meshInfo.verticesV4[i].y;
			if (meshInfo.verticesV4[i].z < min_z) min_z = meshInfo.verticesV4[i].z;
			if (meshInfo.verticesV4[i].z > max_z) max_z = meshInfo.verticesV4[i].z;
		}

		glm::vec3 size = glm::vec3((max_x - min_x) / 1, (max_y - min_y) / 1, (max_z - min_z) / 1);
		//glm::vec3 center = glm::vec3((min_x + max_x) / 2, (min_y + max_y) / 2, (min_z + max_z) / 2);

		glm::mat4 transform{ glm::mat4(1.0f) };


		transform = glm::translate(transform, center);
		transform = glm::translate(transform, entity->getComponent<Transform>().position) * glm::scale(glm::mat4(1), size);


		transform =
			transform * glm::eulerAngleXYZ(
				entity->getComponent<Transform>().eulers.x,
				entity->getComponent<Transform>().eulers.y,
				entity->getComponent<Transform>().eulers.z
			);


		/* Apply object's transformation matrix */
		debugShader->use();
		debugShader->setMat4("model", transform);

		// Dibujamos el bounding box con líneas utilizando los índices del EBO
		glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);

		// Desvinculamos el VAO
		glBindVertexArray(0);
	}



	void onDestroy() override {
		glDeleteBuffers(1, &VBO);
		glDeleteVertexArrays(1, &VAO);
	}
};
