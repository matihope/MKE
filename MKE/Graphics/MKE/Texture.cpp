#include "glad/glad.h"
#include "Texture.hpp"

mk::Texture::Texture() { glGenTextures(1, &texture_id); }

mk::Texture::~Texture() { glDeleteTextures(1, &texture_id); }

void mk::Texture::loadFromImage(const Image& image) {
	loadFromMemory(image.getSize().x, image.getSize().y, image.getData());
}

void mk::Texture::loadFromFile(const ResPath& file) { loadFromImage(Image(file)); }

mk::math::Vector2u mk::Texture::getSize() const { return size; }

void mk::Texture::setSmooth(bool smooth) {
	is_smooth = smooth;
	glBindTexture(GL_TEXTURE_2D, texture_id);
	if (smooth) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
	if (has_mipmaps) generateMipmaps();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void mk::Texture::generateMipmaps() {
	glBindTexture(GL_TEXTURE_2D, texture_id);
	if (is_smooth)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	has_mipmaps = true;
}

u32 mk::Texture::getNativeHandle() const { return texture_id; }

void mk::Texture::setWrapMode(GLint mode) {
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void mk::Texture::bind(const Texture* texture) {
	if (texture)
		glBindTexture(GL_TEXTURE_2D, texture->texture_id);
	else
		glBindTexture(GL_TEXTURE_2D, 0);
}

void mk::Texture::bindAt(const Texture* texture, u32 at) {
	glActiveTexture(GL_TEXTURE0 + at);
	bind(texture);
}

void mk::Texture::loadFromMemory(u32 width, u32 height, const void* data, GLenum format) {
	size = { width, height };
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	setWrapMode(GL_CLAMP_TO_BORDER);
	setSmooth(is_smooth);  // this also unbinds the texture.
}
