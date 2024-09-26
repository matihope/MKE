#include "ResPath.hpp"
#include "MKE/Panic.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>

#define ASSERT_EXISTS() \
	MK_ASSERT(exists(), "Called ", __FUNCTION__, "(), but file does not exist: ", getPath())

namespace mk {
	ResPath::ResPath(const std::string& path):
		  real_path(canonical(std::filesystem::path("assets") / path)) {}

	ResPath::ResPath(const char* path): ResPath(std::string(path)) {}

	const std::filesystem::path& ResPath::getPath() const { return real_path; }

	bool ResPath::exists() const { return std::filesystem::exists(real_path); }

	const char* ResPath::strPath() const { return real_path.c_str(); }

	bool ResPath::isDirectory() const {
		ASSERT_EXISTS();
		return std::filesystem::is_directory(getPath());
	}

	std::string ResPath::readContent() const {
		ASSERT_EXISTS();
		MK_ASSERT(!isDirectory(), "Cannot read content from directory: ", getPath());
		std::ifstream file(strPath());

		std::stringstream buffer;
		buffer << file.rdbuf();

		file.close();
		return buffer.str();
	}
}  // mk
