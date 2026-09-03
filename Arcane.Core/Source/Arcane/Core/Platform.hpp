#pragma once

namespace Arcane { }

#if defined(_WIN32)
#	define AR_PLATFORM_OS_WINDOWS 1
#elif defined(__linux__)
#    define AR_PLATFORM_OS_LINUX 1
#else
#	error "Unsupported operating system."
#endif

#if defined(__x86_64__)
#	define AR_PLATFORM_SYSTEM_ARCHITECTURE_X86_64 1
#else
#	error "Unsupported system architecture."
#endif

#if defined(__GNUC__)
#	define AR_PLATFORM_COMPILER_GCC 1

#	define AR_PLATFORM_COMPILER_VERSION_MAJOR __GNUC__
#	define AR_PLATFORM_COMPILER_VERSION_MINOR __GNUC_MINOR__
#	define AR_PLATFORM_COMPILER_VERSION_PATCH __GNUC_PATCHLEVEL__

#	define AR_CURRENT_FILENAME __FILE_NAME__
#	define AR_CURRENT_FILE __FILE__
#	define AR_CURRENT_LINE __LINE__
#	define AR_CURRENT_FILE_LAST_MODIFICATION __TIMESTAMP__

#	define AR_COMPILATION_DATE __DATE__
#	define AR_COMPILATION_TIME __TIME__

#	define AR_INT8_TYPE __INT8_TYPE__
#	define AR_INT8_MAX __INT8_MAX__
#	define AR_INT16_TYPE __INT16_TYPE__
#	define AR_INT16_MAX __INT16_MAX__
#	define AR_INT32_TYPE __INT32_TYPE__
#	define AR_INT32_MAX __INT32_MAX__
#	define AR_INT64_TYPE __INT64_TYPE__
#	define AR_INT64_MAX __INT64_MAX__

#	define AR_UINT8_TYPE __UINT8_TYPE__
#	define AR_UINT8_MAX __UINT8_MAX__
#	define AR_UINT16_TYPE __UINT16_TYPE__
#	define AR_UINT16_MAX __UINT16_MAX__
#	define AR_UINT32_TYPE __UINT32_TYPE__
#	define AR_UINT32_MAX __UINT32_MAX__
#	define AR_UINT64_TYPE __UINT64_TYPE__
#	define AR_UINT64_MAX __UINT64_MAX__

#	define AR_FLOAT32_TYPE float
#	define AR_FLOAT32_MAX __FLT_MAX__
#	define AR_FLOAT64_TYPE double
#	define AR_FLOAT64_MAX __DBL_MAX__

#	define AR_CHAR8_TYPE char
#	define AR_CHAR16_TYPE __CHAR16_TYPE__
#	define AR_CHAR32_TYPE __CHAR32_TYPE__

#	define AR_INTPTR_TYPE __INTPTR_TYPE__
#	define AR_INTPTR_MAX __INTPTR_MAX__
#	define AR_UINTPTR_TYPE __UINTPTR_TYPE__
#	define AR_UINTPTR_MAX __UINTPTR_MAX__

#	if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#		define AR_PLATFORM_BYTE_ORDER_LITTLE_ENDIAN 1
#	elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#		define AR_PLATFORM_BYTE_ORDER_BIG_ENDIAN 1
#	else
#		error "Unsupported byte order."
#	endif

#	if __FLOAT_WORD_ORDER__ == __ORDER_LITTLE_ENDIAN__
#		define AR_PLATFORM_FLOAT_WORD_ORDER_LITTLE_ENDIAN 1
#	elif __FLOAT_WORD_ORDER__ == __ORDER_BIG_ENDIAN__
#		define AR_PLATFORM_FLOAT_WORD_ORDER_BIG_ENDIAN 1
#	else
#		error "Unsupported float word order."
#	endif
#else
#	error "Unsupported compiler."
#endif

#if defined(__cplusplus)
#	define AR_PLATFORM_LANGUAGE_CPLUSPLUS 1
#else
#	error "Unsupported programming language."
#endif