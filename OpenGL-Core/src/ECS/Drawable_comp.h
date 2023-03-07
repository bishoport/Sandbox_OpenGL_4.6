#pragma once
#include "glpch.h"
#include "ECSCore.h"


class Drawable : public ECS::Component
{
public:

	GLCore::ModelInfo modelInfo;

	GLCore::Shader* mainShader;
	GLCore::Shader* debugShader;

	float pi = 3.1415926535;

	void init() override 
	{
		//By default add a transform component
		entity->addComponent<Transform>();
		entity->getComponent<Transform>().position.x = 0;
		entity->getComponent<Transform>().position.y = 0;
		entity->getComponent<Transform>().position.z = 0;

		//Correccion de rotacion
		entity->getComponent<Transform>().eulers.x = (0 * pi) / 180;
		entity->getComponent<Transform>().eulers.y = (0 * pi) / 180;
		entity->getComponent<Transform>().eulers.z = (0 * pi) / 180;
	}


	void setModelInfo(GLCore::ModelInfo _modelInfo, GLCore::Shader* _mainShader, GLCore::Shader* _debugShader)
	{
		this->mainShader = _mainShader;
		this->debugShader = _debugShader;
		this->modelInfo = _modelInfo;

		if (modelInfo.meshes.size() > 0)
		{
			//int i = 0;
			for (int i = 0; i < modelInfo.meshes.size(); i++)
			{
				modelInfo.meshes[i].vertexCount = int(modelInfo.meshes[i].allBuffer.size()) / 8;
				glCreateBuffers(1, &modelInfo.meshes[i].VBO);
				glCreateVertexArrays(1, &modelInfo.meshes[i].VAO);
				glVertexArrayVertexBuffer(modelInfo.meshes[i].VAO, 0, modelInfo.meshes[i].VBO, 0, 8 * sizeof(float));
				glNamedBufferStorage(modelInfo.meshes[i].VBO, modelInfo.meshes[i].allBuffer.size() * sizeof(float), modelInfo.meshes[i].allBuffer.data(), GL_DYNAMIC_STORAGE_BIT);

				//pos: 0, texcoord: 1, normal: 2
				glEnableVertexArrayAttrib(modelInfo.meshes[i].VAO, 0);
				glEnableVertexArrayAttrib(modelInfo.meshes[i].VAO, 1);
				glEnableVertexArrayAttrib(modelInfo.meshes[i].VAO, 2);

				glVertexArrayAttribFormat(modelInfo.meshes[i].VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
				glVertexArrayAttribFormat(modelInfo.meshes[i].VAO, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
				glVertexArrayAttribFormat(modelInfo.meshes[i].VAO, 2, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float));

				glVertexArrayAttribBinding(modelInfo.meshes[i].VAO, 0, 0);
				glVertexArrayAttribBinding(modelInfo.meshes[i].VAO, 1, 0);
				glVertexArrayAttribBinding(modelInfo.meshes[i].VAO, 2, 0);
			}
		}

		if (modelInfo.materials.size() > 0)
		{
			for (int i = 0; i < modelInfo.materials.size(); i++)
			{
				//if (modelInfo.materials[i].diffuseMap.textureID != 0)
				//{
					glGenTextures(1, &modelInfo.materials[i].diffuseMap.textureID);
					glBindTexture(GL_TEXTURE_2D, modelInfo.materials[i].diffuseMap.textureID);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

					unsigned char* data = modelInfo.materials[i].diffuseMap.image.pixels;
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, modelInfo.materials[i].diffuseMap.image.width, modelInfo.materials[i].diffuseMap.image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
					glGenerateMipmap(GL_TEXTURE_2D);

					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, modelInfo.materials[i].diffuseMap.textureID);
				//}
				
			}
		}
	}


	void update() override 
	{
	}

	void draw() override
	{
		if (modelInfo.meshes.size() > 0)
		{


			for (int i = 0; i < modelInfo.meshes.size(); i++)
			{
				glBindVertexArray(modelInfo.meshes[i].VAO);

				glm::mat4 model_transform{ glm::mat4(1.0f) };
				model_transform = glm::translate(model_transform, entity->getComponent<Transform>().position);

				model_transform =
					model_transform * glm::eulerAngleXYZ(
						entity->getComponent<Transform>().eulers.x,
						entity->getComponent<Transform>().eulers.y,
						entity->getComponent<Transform>().eulers.z
					);

				//model_transform_public = model_transform;

				mainShader->use();
				mainShader->setMat4("model", model_transform);

				//if (modelInfo.materials[i].diffuseMap.textureID != 0)
				//{
					glBindTextureUnit(0, modelInfo.materials[i].diffuseMap.textureID);
				//}
				

				glDrawArrays(GL_TRIANGLES, 0, modelInfo.meshes[i].num_indices);
				glBindVertexArray(0);
			}

			draw_bb();
		}
	}

	void onDestroy() override
	{

	}

private:
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


			glm::vec3 size = glm::vec3((modelInfo.meshes[0].boundingBoxMax.x - modelInfo.meshes[0].boundingBoxMin.x) / 10, 
									   (modelInfo.meshes[0].boundingBoxMax.y - modelInfo.meshes[0].boundingBoxMin.y) / 10, 
									   (modelInfo.meshes[0].boundingBoxMax.z - modelInfo.meshes[0].boundingBoxMin.z) / 10);

			glm::mat4 transform{ glm::mat4(1.0f) };

			transform = glm::translate(transform, entity->getComponent<Transform>().position) * glm::scale(glm::mat4(1.0), size);


			transform =
				transform * glm::eulerAngleXYZ(
					entity->getComponent<Transform>().eulers.x,
					entity->getComponent<Transform>().eulers.y,
					entity->getComponent<Transform>().eulers.z
				);


			glm::vec3 center = glm::vec3(entity->getComponent<Transform>().position.x + 0.5f,
				entity->getComponent<Transform>().position.y, // -((std::abs(modelInfo.meshes[0].boundingBoxMax.y) - std::abs(modelInfo.meshes[0].boundingBoxMin.y)) / 1),
										 entity->getComponent<Transform>().position.z + 0.5f);

			//glm::mat4 inverseModel = glm::inverse(transform);
			//glm::vec4 transformedCenter = inverseModel * glm::vec4(center, 1.0f);
			//glm::vec3 objectCenter = glm::vec3(transformedCenter.x, transformedCenter.y, transformedCenter.z);

			transform = glm::translate(transform, center);

			/* Apply object's transformation matrix */
			debugShader->use();
			debugShader->setMat4("model", transform);

			// Dibujamos el bounding box con líneas utilizando los índices del EBO
			glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);

			// Desvinculamos el VAO
			glBindVertexArray(0);
		}


};