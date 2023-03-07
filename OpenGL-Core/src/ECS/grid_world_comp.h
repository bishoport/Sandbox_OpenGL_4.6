#pragma once

#include "glpch.h"
#include "ECSCore.h"

#include <vector>

using namespace std;
using namespace glm;


class GridWorldReferenceComp : public ECS::Component
{
public:

    unsigned int VAO;
    vector<float> vertices;
    GLuint lenght;
    int slices = 10;
    float sizeMultiplicator = 10.2f;


    GLCore::Shader* mainShader;

    void init() override
    {

        std::vector<glm::vec3> vertices;
        std::vector<glm::uvec4> indices;

        for (int j = 0; j <= slices; ++j) {
            for (int i = 0; i <= slices; ++i) {
                float x = (float)i / (float)slices * sizeMultiplicator;
                float y = 0;
                float z = (float)j / (float)slices * sizeMultiplicator;
                vertices.push_back(glm::vec3(x - sizeMultiplicator / 2, y, z - sizeMultiplicator / 2));
            }
        }

        for (int j = 0; j < slices; ++j) {
            for (int i = 0; i < slices; ++i) {

                int row1 = j * (slices + 1);
                int row2 = (j + 1) * (slices + 1);

                indices.push_back(glm::uvec4(row1 + i, row1 + i + 1, row1 + i + 1, row2 + i + 1));
                indices.push_back(glm::uvec4(row2 + i + 1, row2 + i, row2 + i, row1 + i));

            }
        }

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), glm::value_ptr(vertices[0]), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        GLuint ibo;
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glm::uvec4), glm::value_ptr(indices[0]), GL_STATIC_DRAW);

        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        lenght = (GLuint)indices.size() * 4;

    }

    void update() override
    {

    }

    void draw() override
    {
        
        glBindVertexArray(VAO);

        glm::mat4 transform{ glm::mat4(1.0f) };
        /* Apply object's transformation matrix */
        mainShader->setMat4("model", transform);

        glDrawElements(GL_LINES, lenght, GL_UNSIGNED_INT, NULL);
        glBindVertexArray(0);
    }

    void onDestroy() override
    {
        glDeleteVertexArrays(1, &VAO);
    }

};