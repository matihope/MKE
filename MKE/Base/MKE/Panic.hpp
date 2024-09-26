#pragma once

#include "StrUtils.hpp"
#include "Exceptions.hpp"
#include <iostream>

#define MK_LOG_ERROR(...) std::cerr << "ERROR: " << mk::strConcat(__VA_ARGS__) << std::endl

// Braces to make it atomic
#define MK_PANIC(...)              \
	{                              \
		MK_LOG_ERROR(__VA_ARGS__); \
		exit(1);                   \
	}


#define MK_ASSERT(cond, ...) \
	if (!(cond)) { throw mk::exceptions::MkException(__VA_ARGS__); }

#define MK_ASSERT_EQUAL(expected, actual, ...) \
	MK_ASSERT(                                 \
		(actual) == (expected),                \
		mk::strConcat(                         \
			__VA_ARGS__,                       \
			"\n\tAt ",                         \
			__FILE__,                          \
			":",                               \
			__LINE__,                          \
			": \n",                            \
			"\t\tValues not equal: ",          \
			#expected,                         \
			" != ",                            \
			#actual                            \
		)                                      \
	)

#define MK_ASSERT_TRUE(expr, ...) MK_ASSERT_EQUAL(true, expr, __VA_ARGS__)
