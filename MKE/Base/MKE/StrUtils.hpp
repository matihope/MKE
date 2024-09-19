
#pragma once

#include <string>
#include <type_traits>

namespace mk {
	class StrID;

	namespace detail {
		inline void strConcat(std::string& out, const std::string& str)  {
			out += str;
		}

		inline void strConcat(std::string& out, std::string_view str)  {
			out += str;
		}

		template<class T> requires std::is_arithmetic_v<T>
		inline void strConcat(std::string& out, T v)  {
			out.append(std::to_string(v));
		}

		inline void strConcat(std::string& out, bool v) { out.append(v ? "true" : "false"); }

		inline void strConcat(std::string& out, const char* v) {
			if (v == nullptr) throw std::domain_error("strConcat called with nullptr");
			out.append(std::string(v));
		}

		template<typename U, typename V>
		requires(std::is_trivially_copyable_v<U> && std::is_trivially_copyable_v<V>)
		inline void strConcat(std::string& out, std::pair<U, V> pair) {
			out += "<";
			strConcat(out, pair.first);
			out += ", ";
			strConcat(out, pair.second);
			out += ">";
		}
	}

	/**
	 * @brief Creates std::string from elements as if by executing the following pseudo code:
	 * ```
	 * std::string out;
	 * out.append(element_0)
	 * out.append(element_1)
	 * ...
	 * ```
	 *
	 * Specialization are provided for:
	 *
	 * - integer types - uses std::to_string,
	 * - RawView - uses RawView.str(), and
	 * - std::tuple - uses std::make_from_tuple<std::string>.
	 *
	 * Throws std::domain_error on nullptr argument.
	 */
	template<typename... T>
	std::string strConcat(T&&... elements) {
		std::string out;
		(detail::strConcat(out, std::forward<T>(elements)), ...);
		return out;
	}

	/**
	 * Replaces all occurrences of `from` with `to`.
	 * @param str The source string.
	 * @param from The pattern to be erased.
	 * @param to The pattern to be put instead of `from`.
	 */
	void strReplaceAll(std::string& str, const std::string& from, const std::string& to);

	/**
	 * Splits a string by a delimiter.
	 * @param str The string to split.
	 * @param delimiter A string, that is used to separate the substrings.
	 * @return A vector of separated strings.
	 */
	std::vector<std::string>
		strSplit(const std::string_view str, const std::string& delimiter = " ");
}
