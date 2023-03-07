#pragma once
#include "glpch.h"


namespace GLCore {

	struct MeshInfo
	{
		unsigned int num_indices = 0;
		std::vector<float> allBuffer;
		std::vector<glm::vec4> verticesV4;

		std::vector<GLfloat> vertices;
        std::vector<GLfloat> normals;
        std::vector<GLfloat> texcoords;
        std::vector<GLuint> indices;

        unsigned int VBO, VAO, EBO, vertexCount;

        glm::vec3 boundingBoxMin;
        glm::vec3 boundingBoxMax;
	};

    struct Image {
        unsigned char* pixels;
        int width, height, channels;
        std::string path;
    };

    struct Texture {
        GLuint textureID = 0;
        std::string type;
        Image image;
    };

    struct Material {
        std::string name;

		Texture diffuseMap;
		Texture normalMap;
        Texture specularMap;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		float shininess;
	};

    


    struct ModelInfo
    {
        std::vector<MeshInfo> meshes;
        std::vector<Material> materials;
    };


}