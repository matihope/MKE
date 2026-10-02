#pragma once
#include <MKE/Panic.hpp>
#include <MKE/Ints.hpp>

#define variant_match(value)                                                                    \
	switch (auto&& internal_value = (value); [&] {                                              \
		MK_ASSERT(not value.valueless_by_exception(), "Variant in variant_match is valueless"); \
		return internal_value.index();                                                          \
	}())

#define variant_case(type, name)                                       \
	break;                                                             \
	case (::base::variantTypeIndex<decltype(internal_value), type>()): \
		if ([[maybe_unused]]                                           \
		    auto&& name = std::get<type>(internal_value);              \
		    true)

namespace base {
	namespace internal {
		template<typename...>
		inline constexpr bool DEPENDENT_FALSE_V = false;

		template<typename>
		struct Tag {};

		template<typename Variant, typename T>
		struct VariantTypeIndexAux {
			static_assert(
				DEPENDENT_FALSE_V<Variant>, "variantTypeIndex() can be used only for variant"
			);
		};

		template<typename T, typename... Types>
		struct VariantTypeIndexAux<std::variant<Types...>, T> {
			static constexpr usize findIndex() {
				return std::variant<Tag<Types>...>(Tag<T>{}).index();
			}
		};
	}

	/**
	 * @brief Returns the index of `T` in a `std::variant` at compile time.
	 */
	template<typename VariantT, typename T>
	constexpr usize variantTypeIndex() {
		using ClearedVariantT = std::remove_cvref_t<VariantT>;
		return internal::VariantTypeIndexAux<ClearedVariantT, T>::findIndex();
	}
}
