#pragma once

#include <string>
#include <filesystem>

namespace mk {
	// Runtime root that all relative resource paths resolve against.
	// Set once at startup (see AutoAssetRoot.hpp / setAssetRoot); reading it
	// before it is set panics. Kept as a runtime value so it lives in exactly
	// one translation unit: baking it in per-target via a macro caused an ODR
	// violation, because ResPath's constructor is inline and resolved
	// differently in the example TU (macro set) vs the engine libraries (macro
	// unset) — which optimized builds tripped over.
	void                         setAssetRoot(std::filesystem::path root);
	const std::filesystem::path& assetRoot();

	class ResPath {
		friend class std::hash<ResPath>;

	public:
		ResPath() = default;

		inline ResPath(const std::filesystem::path& path) { real_path = assetRoot() / path; }

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
