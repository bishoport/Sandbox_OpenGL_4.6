#include "glpch.h"
#include "IMGLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


GLCore::Image  GLCore::Loaders::load_from_file(const char* filename) {
	Image image;
	stbi_set_flip_vertically_on_load(1);
	image.pixels = stbi_load(filename, &(image.width), &(image.height), &(image.channels), STBI_rgb_alpha);

	if (image.pixels)
	{
		std::cout << "Image loaded successfully" << std::endl;
	}
	else
	{
		std::cout << "Image failed to load at path: " << filename << std::endl;
	}

	return image;
}

void GLCore::Loaders::free_image_memory(GLCore::Image oldImage) {
	stbi_image_free(oldImage.pixels);
}



float GLCore::Loaders::rand_FloatRange(float a, float b)
{
	return ((b - a) * ((float)rand() / RAND_MAX)) + a;
}