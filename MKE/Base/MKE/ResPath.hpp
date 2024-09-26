#pragma once

#include "MKE/Panic.hpp"
#include <string>
#include <filesystem>
#include <type_traits>

namespace mk {
	class ResPath {
		friend class std::hash<ResPath>;

	public:
		ResPath() = default;
		ResPath(const std::filesystem::path& path);
		ResPath(const std::string& path);
		ResPath(const char* path);

		[[nodiscard]]
		const std::filesystem::path& getPath() const;

		[[nodiscard]]
		const char* strPath() const;

		[[nodiscard]]
		bool exists() const;

		bool operator()(const ResPath& other) const { return real_path == other.real_path; }

		bool operator<(const ResPath& other) const { return real_path < other.real_path; }

		bool isDirectory() const;

		std::string readContent() const;

		static inline ResPath example([[maybe_unused]] std::string_view path) {
#ifdef CUSTOM_ASSETS_PATH
			return { std::filesystem::path(CUSTOM_ASSETS_PATH) / path };
#else
			MK_PANIC("CUSTOM_ASSETS_PATH is not set");
#endif
		}

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
