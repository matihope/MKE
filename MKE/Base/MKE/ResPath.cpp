#include "ResPath.hpp"
#include "MKE/Panic.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>

namespace mk {
	ResPath::ResPath(const std::filesystem::path& path): real_path("assets" / canonical(path)) {}

	ResPath::ResPath(const std::string& path): mk::ResPath(std::filesystem::path(path)) {}

	ResPath::ResPath(const char* path): mk::ResPath(std::filesystem::path(path)) {}

	const std::filesystem::path& ResPath::getPath() const { return real_path; }

	bool ResPath::exists() const { return std::filesystem::exists(real_path); }

	const char* ResPath::strPath() const { return real_path.c_str(); }

	bool ResPath::isDirectory() const {
		assertExists();
		return std::filesystem::is_directory(getPath());
	}

	std::string ResPath::readContent() const {
		assertExists();
		MK_ASSERT(!isDirectory(), "Cannot read data from directory: ", getPath());
		std::ifstream file(strPath());

		std::stringstream buffer;
		buffer << file.rdbuf();

		file.close();
		return buffer.str();
	}

	void ResPath::assertExists() const { MK_ASSERT(exists(), "File does not exist: ", getPath()); }

	void ResPath::assertExtension(std::string_view desired_extension) const {
		assertExists();

		MK_ASSERT_EQUAL(
			extension(),
			desired_extension,
			"File: ",
			getPath(),
			" does not have the desired extension: ",
			desired_extension
		);
	}

	void ResPath::assertStem(std::string_view desired_stem) const {
		assertExists();
		MK_ASSERT_EQUAL(
			stem(),
			desired_stem,
			"File: ",
			getPath(),
			" does not have the desired stem: ",
			desired_stem
		);
	}

	std::string ResPath::extension() const { return real_path.extension(); }

	std::string ResPath::stem() const { return real_path.stem(); }
}  // mk
