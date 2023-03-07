#include "glpch.h"
#include "ModelLoader.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "IMGLoader.h"
#include <ECSCore.h>

using namespace GLCore::Loaders;

namespace GLCore::Loaders
{

    bool loadModelAssimp(const std::string& filePath, const std::string& fileName, std::vector<ModelInfo>& models, glm::mat4 preTransform)
    {
        std::string completePathMesh = filePath + fileName;
        std::cout << "File ->" << completePathMesh << std::endl;
        ModelInfo modelInfo;

        Assimp::Importer importer;

        // Configura las opciones de importación
        const aiScene* scene = importer.ReadFile(completePathMesh, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace); //aiProcess_FlipUVs | aiProcess_SortByPType

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
        {
            std::cout << "Error al cargar el modelo: " << importer.GetErrorString() << std::endl;
            return false;
        }

        std::cout << "Meshes->" << scene->mNumMeshes << std::endl;
        

        // Recorre los nodos del árbol de escena y procesa los mallas
        for (unsigned int i = 0; i < scene->mNumMeshes; i++) {

            // Crea una nueva estructura de malla
            MeshInfo meshInfo;

            // Obtén un puntero a la malla actual
            aiMesh* aiMesh = scene->mMeshes[i];

            aiVector3D minVertex(FLT_MAX, FLT_MAX, FLT_MAX);
            aiVector3D maxVertex(-FLT_MAX, -FLT_MAX, -FLT_MAX);

            for (unsigned int j = 0; j < aiMesh->mNumVertices; j++) {
                aiVector3D vertex = aiMesh->mVertices[j];

                if (vertex.x < minVertex.x) minVertex.x = vertex.x;
                if (vertex.y < minVertex.y) minVertex.y = vertex.y;
                if (vertex.z < minVertex.z) minVertex.z = vertex.z;

                if (vertex.x > maxVertex.x) maxVertex.x = vertex.x;
                if (vertex.y > maxVertex.y) maxVertex.y = vertex.y;
                if (vertex.z > maxVertex.z) maxVertex.z = vertex.z;
            }

            meshInfo.boundingBoxMin = { minVertex.x, minVertex.y, minVertex.z };
            meshInfo.boundingBoxMax = { maxVertex.x, maxVertex.y, maxVertex.z };


            // Recorre las caras de la malla y almacena sus índices
            for (unsigned int j = 0; j < aiMesh->mNumFaces; j++) {
                aiFace face = aiMesh->mFaces[j];
                for (unsigned int k = 0; k < face.mNumIndices; k++) {
                    meshInfo.indices.push_back(face.mIndices[k]);
                }
            }

            // Recorre los vértices de la malla y almacena sus coordenadas, normales y coordenadas de textura
            for (unsigned int j = 0; j < meshInfo.indices.size(); j++) {

                //-------------------VERTICES------------------
                glm::vec4 pos = {
                    aiMesh->mVertices[meshInfo.indices[j]].x,
                    aiMesh->mVertices[meshInfo.indices[j]].y,
                    aiMesh->mVertices[meshInfo.indices[j]].z,
                    1
                };

                pos = preTransform * pos;

                meshInfo.vertices.push_back(pos.x);
                meshInfo.vertices.push_back(pos.y);
                meshInfo.vertices.push_back(pos.z);
                meshInfo.verticesV4.push_back(pos);
                //--------------------------------------------------


                glm::vec2 texcoord = {
                    aiMesh->mTextureCoords[0][meshInfo.indices[j]].x,
                    aiMesh->mTextureCoords[0][meshInfo.indices[j]].y
                };

                meshInfo.texcoords.push_back(texcoord.x);
                meshInfo.texcoords.push_back(texcoord.y);


                glm::vec3 normal = {
                    aiMesh->mNormals[meshInfo.indices[j]].x,
                    aiMesh->mNormals[meshInfo.indices[j]].y,
                    aiMesh->mNormals[meshInfo.indices[j]].z
                };

                normal = glm::normalize(glm::mat3(preTransform) * normal);
                meshInfo.normals.push_back(normal.x);
                meshInfo.normals.push_back(normal.y);
                meshInfo.normals.push_back(normal.z);




                meshInfo.allBuffer.push_back(pos.x);
                meshInfo.allBuffer.push_back(pos.y);
                meshInfo.allBuffer.push_back(pos.z);

               meshInfo.allBuffer.push_back(texcoord.x);
                meshInfo.allBuffer.push_back(texcoord.y);

                meshInfo.allBuffer.push_back(normal.x);
                meshInfo.allBuffer.push_back(normal.y);
                meshInfo.allBuffer.push_back(normal.z);

                meshInfo.num_indices += 1;
            }

            modelInfo.meshes.push_back(meshInfo);
        }


        if (scene->HasMaterials())
        {
            Material material;

            std::cout << "Materials->" << scene->mNumMaterials << std::endl;

            for (unsigned int i = 0; i < scene->mNumMaterials; i++)
            {
                const aiMaterial* mat = scene->mMaterials[i];
                
                aiString texturePath;

                unsigned int numTextures = mat->GetTextureCount(aiTextureType_DIFFUSE);   // always 0

                if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0 && mat->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS)
                {
                    /*std::cout << "Texture filePath->" << filePath << std::endl;
                    std::cout << "Texture fileName->" << texturePath.C_Str() << std::endl;*/

                    std::string completePathTexture = filePath + texturePath.C_Str();
                    std::cout << "Diffuse Texture->" << completePathTexture << std::endl;

                    Texture diffuseTexture;
                            diffuseTexture.image = GLCore::Loaders::load_from_file(completePathTexture.c_str());
                            diffuseTexture.image.path = texturePath.C_Str();

                    material.diffuseMap = diffuseTexture;
                    modelInfo.materials.push_back(material);
                }

                /*if (mat->GetTextureCount(aiTextureType_DISPLACEMENT) > 0 && mat->GetTexture(aiTextureType_DISPLACEMENT, 0, &texturePath) == AI_SUCCESS)
                {
                    Texture normalTexture;
                    normalTexture.image = GLCore::Loaders::load_from_file(texturePath.C_Str());
                    normalTexture.path = texturePath.C_Str();

                    material.normalMap = normalTexture;
                    modelInfo.materials.push_back(material);
                }*/
            }

            modelInfo.materials.push_back(material);
        }



        /*std::vector<Texture> textures;
        if (scene->HasTextures()) {

            std::cout << "Textures->" << scene->mNumTextures << std::endl;

            for (unsigned int i = 0; i < scene->mNumTextures; i++) {
                const aiTexture* texture = scene->mTextures[i];

                std::cout << "Texture_"<< i <<"-> " << texture->mFilename.C_Str() << std::endl;
            }
        }*/

        models.push_back(modelInfo);

        return true;
    }



   

}