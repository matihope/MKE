#include "Image.hpp"
#include "MKE/Exceptions.hpp"
#include "MKE/Panic.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

mk::Image::Image(const ResPath& image) { load(image); }

void mk::Image::load(const ResPath& image) {
	destroy();

	auto ext = image.extension();
	MK_ASSERT(ext == ".jpg" || ext == ".png", "Invalid image extension: ", image.getPath());
	MK_ASSERT(image.exists(), "File " + image.getPath().string() + " does not exist!");

	data = stbi_load(image.strPath(), (i32*) &width, (i32*) &height, (i32*) &nrChannels, 4);
	MK_ASSERT(data != nullptr, "Couldn\'t load the image data");
}

bool mk::Image::tryLoad(const ResPath& image) {
	try {
		load(image);
		return true;
	} catch (exceptions::MkException& e) {
		MK_LOG_ERROR("in Image::tryLoad(), continuing after", e.what());
		return false;
	}
}

mk::Image::~Image() { destroy(); }

void mk::Image::destroy() {
	if (data) {
		stbi_image_free(data);
		data = nullptr;
	}
}

const u8* mk::Image::getData() const { return data; }
