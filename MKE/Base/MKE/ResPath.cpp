#include "ResPath.hpp"
#include "MKE/Panic.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>

namespace mk {
	namespace {
		// Function-local statics: constructed on first use, so setAssetRoot()
		// running during another TU's static init sees a fully-constructed
		// object. Namespace-scope globals here would hit the static
		// initialization order fiasco — the asset-root setter (in the example
		// TU) can run before this TU's globals are constructed, which would
		// then reset the value back to empty.
		std::filesystem::path& assetRootStorage() {
			static std::filesystem::path root;
			return root;
		}

		bool& assetRootSet() {
			static bool set = false;
			return set;
		}
	}  // namespace

	void setAssetRoot(std::filesystem::path root) {
		assetRootStorage() = std::move(root);
		assetRootSet()     = true;
	}

	const std::filesystem::path& assetRoot() {
		if (!assetRootSet()) MK_PANIC("Asset root is not set");
		return assetRootStorage();
	}

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
