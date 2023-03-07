#pragma once
#include "glpch.h"
#include "ECSCore.h"

#include "../vendor/glfw/include/GLFW/glfw3.h"
#include "../vendor/Glad/include/glad/glad.h"


class MaterialComp : public ECS::Component
{
public:	
	GLuint texture = 0;

	void init() override{}
	
	
	void update() override{}

	void setImage(GLCore::Image* image)
	{
		int texWidth, texHeight;
			texWidth = image->width;
			texHeight = image->height;

		unsigned char* data = image->pixels;

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		//GLCore::Loaders::free_image_memory(&image->pixels);
	}

	void draw() override
	{
		glBindTextureUnit(0, texture);
	}

	//void use() {
	//	glBindTextureUnit(0, texture);
	//}

	void onDestroy() override
	{
		glDeleteTextures(1, &texture);
	}
};
