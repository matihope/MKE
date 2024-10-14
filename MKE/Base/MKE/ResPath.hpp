#pragma once

#include "MKE/Panic.hpp"
#include <string>
#include <filesystem>

namespace mk {
	class ResPath {
		friend class std::hash<ResPath>;

	public:
		ResPath() = default;

		inline ResPath([[maybe_unused]] const std::filesystem::path& path) {
#ifdef CUSTOM_ASSETS_PATH
			real_path = std::filesystem::path(CUSTOM_ASSETS_PATH) / path;
#else
			MK_PANIC("CUSTOM_ASSETS_PATH is not set");
#endif
		}

		inline ResPath(const char* path): mk::ResPath(std::filesystem::path(path)) {}

		inline ResPath(const std::string& path): mk::ResPath(std::filesystem::path(path)) {}

		[[nodiscard]]
		const std::filesystem::path& getPath() const;

		[[nodiscard]]
		const char* strPath() const;

		[[nodiscard]]
		bool exists() const;

		bool operator==(const ResPath&) const = default;

		bool operator()(const ResPath& other) const { return real_path == other.real_path; }

		bool operator<(const ResPath& other) const { return real_path < other.real_path; }

		bool isDirectory() const;

		std::string readContent() const;

		std::string extension() const;

		std::string stem() const;

		void assertExists() const;
		void assertExtension(std::string_view desired_extension) const;
		void assertStem(std::string_view desired_stem) const;

	private:
		std::filesystem::path real_path;
	};
}  // mk

template<>
struct std::hash<mk::ResPath> {
	std::size_t operator()(const mk::ResPath& k) const {
		return std::hash<std::filesystem::path>()(k.real_path);
	}
};
