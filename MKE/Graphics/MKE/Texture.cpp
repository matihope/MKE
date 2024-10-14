#include "glad/glad.h"
#include "Texture.hpp"

mk::Texture::Texture() { glGenTextures(1, &texture_id); }

mk::Texture::~Texture() { glDeleteTextures(1, &texture_id); }

void mk::Texture::loadFromImage(const Image& image) {
	loadFromMemory(image.getSize().x, image.getSize().y, image.getData());
}

void mk::Texture::loadFromFile(const ResPath& file) { loadFromImage(Image(file)); }

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

u32 mk::Texture::getNativeHandle() const { return texture_id; }

void mk::Texture::bind(const Texture* texture) {
	if (texture)
		glBindTexture(GL_TEXTURE_2D, texture->texture_id);
	else
		glBindTexture(GL_TEXTURE_2D, 0);
}

void mk::Texture::loadFromMemory(usize width, usize height, const void* data, GLenum format) {
	size = { width, height };
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
}
