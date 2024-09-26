#include "glad/glad.h"
#include "Texture.hpp"

mk::Texture::Texture() { glGenTextures(1, &texture_id); }

mk::Texture::~Texture() { glDeleteTextures(1, &texture_id); }

void mk::Texture::loadFromImage(const Image& image) const {
	glBindTexture(GL_TEXTURE_2D, texture_id);

	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	auto [width, height] = image.getSize();
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.getData()
	);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void mk::Texture::loadFromFile(const ResPath& file) const { loadFromImage(Image(file)); }

void mk::Texture::use() const { glBindTexture(GL_TEXTURE_2D, texture_id); }

mk::math::Vector2u mk::Texture::getSize() const { return size; }

void mk::Texture::setSmooth(bool smooth) const {
	glBindTexture(GL_TEXTURE_2D, texture_id);
	if (smooth)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void mk::Texture::generateMipmaps() const {
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}
