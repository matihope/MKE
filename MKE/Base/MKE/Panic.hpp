#pragma once

#include "StrUtils.hpp"
#include <iostream>

#define MKE_PANIC(...)                                            \
	std::cerr << "ERROR: " << mk::strConcat(__VA_ARGS__) << '\n'; \
	exit(1)

#define MKE_ASSERT(cond, ...) \
	if (!(cond)) MKE_PANIC(__VA_ARGS__)

#define MKE_ASSERT_EQUAL(expected, actual, ...) \
	MKE_ASSERT(                                 \
		(actual) == (expected),                 \
		mk::strConcat(                          \
			__VA_ARGS__,                        \
			"\n\tAt ",                          \
			__FILE__,                           \
			":",                                \
			__LINE__,                           \
			": \n",                             \
			"\t\tValues not equal: ",           \
			#expected,                          \
			" != ",                             \
			#actual                             \
		)                                       \
	)

#define MKE_ASSERT_TRUE(expr, message) MKE_ASSERT_EQUAL(true, expr, message)
