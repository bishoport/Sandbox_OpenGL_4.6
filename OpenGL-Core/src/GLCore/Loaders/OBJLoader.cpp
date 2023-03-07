#include "glpch.h"
#include "OBJLoader.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "../vendor/tiny_obj_loader.h"



GLCore::MeshInfo GLCore::Loaders::load_model_from_file(const char* filename, glm::mat4 preTransform) {

	GLCore::MeshInfo meshInfo;

	tinyobj::attrib_t attributes;

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warning, error;


	if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &warning, &error, filename)) {
		std::cout << warning << error << '\n';
	}


	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			meshInfo.vertices.push_back(3 * index.vertex_index);
			meshInfo.vertices.push_back(3 * index.vertex_index + 1);
			meshInfo.vertices.push_back(3 * index.vertex_index + 2);

			glm::vec4 pos = {
				attributes.vertices[3 * index.vertex_index],
				attributes.vertices[3 * index.vertex_index + 1],
				attributes.vertices[3 * index.vertex_index + 2],
				1
			};

			//std::cout << "index.vertex_index-> " << index.vertex_index << std::endl;
			pos = preTransform * pos;
			meshInfo.verticesV4.push_back(pos);

			glm::vec2 texCoord = {
				attributes.texcoords[2 * index.texcoord_index],
				attributes.texcoords[2 * index.texcoord_index + 1],
			};

			meshInfo.texcoords.push_back(2 * index.texcoord_index);
			meshInfo.texcoords.push_back(2 * index.texcoord_index + 1);


			glm::vec3 normal = {
				attributes.normals[3 * index.normal_index],
				attributes.normals[3 * index.normal_index + 1],
				attributes.normals[3 * index.normal_index + 2]
			};

			normal = glm::normalize(glm::mat3(preTransform) * normal);

			meshInfo.normals.push_back(3 * index.normal_index);
			meshInfo.normals.push_back(3 * index.normal_index + 1);
			meshInfo.normals.push_back(3 * index.normal_index + 2);

			meshInfo.allBuffer.push_back(pos.x);
			meshInfo.allBuffer.push_back(pos.y);
			meshInfo.allBuffer.push_back(pos.z);

			meshInfo.allBuffer.push_back(texCoord.x);
			meshInfo.allBuffer.push_back(texCoord.y);

			meshInfo.allBuffer.push_back(normal.x);
			meshInfo.allBuffer.push_back(normal.y);
			meshInfo.allBuffer.push_back(normal.z);

			meshInfo.num_indices += 1;
		}
	}



	//std::cout << "SHAPES EN " << filename << " ->" << shapes.size() << std::endl;
	//std::cout <<"MATERIALES EN " << filename << " ->" << materials.size() << std::endl;
	//std::cout <<"ATTRIBUTES EN " << filename << " ->" << attributes. << std::endl;

	return meshInfo;
}