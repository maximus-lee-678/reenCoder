#pragma once

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "reencoder_utf_common.h"

/**
 * @brief Parses a given UTF-32 uint32_t* sequence and loads it into a `ReencoderUnicodeStruct`.
 *
 * Input string must be represented as uint32_t*.
 * The returned `ReencoderUnicodeStruct` will be fully initialised if the string is valid.
 * ReencoderUnicodeStruct->num_chars will be 0 if the string is invalid.
 *
 * The returned `ReencoderUnicodeStruct` must be freed using `reencoder_unicode_struct_free()` once it is no longer needed.
 *
 * @param[in] string Input UTF-32 string. Must be null-terminated (0x00000000).
 * @param[in] target_endian Specifies target UTF-32 endianness (UTF_32BE or UTF_32LE).
 *
 * @return Pointer to a `ReencoderUnicodeStruct` containing parsed string data.
 * @retval NULL If memory allocation fails or an invalid `target_endian` is provided.
 */
ReencoderUnicodeStruct* reencoder_utf32_parse_uint32(const uint32_t* string, enum ReencoderEncodeType target_endian);


ReencoderUnicodeStruct* reencoder_utf32_parse_uint8(
	const uint8_t* string, size_t bytes, enum ReencoderEncodeType source_endian, enum ReencoderEncodeType target_endian
);

/**
 * @brief Checks if a provided UTF-32 string is valid.
 *
 * Checks for surrogate presence, and out of range values.
 *
 * @param[in] string UTF-32 string to be checked. Should be represented as an array of uint32_t.
 * @param[in] length Length of the provided string. Length is not number of bytes, but number of uint32_t elements.
 *
 * @return Unsigned integer representing the outcome of the check. Corresponds to index in `REENCODER_UTF16_OUTCOME_ARR`.
 */
unsigned int _reencoder_utf32_is_valid(const uint32_t* string, size_t length);

/**
 * @brief Returns the length of a UTF-32 string.
 *
 * Length in this case is not number of bytes, but number of uint32_t elements.
 *
 * @param[in] string UTF-32 string to be checked. Represented as an array of uint32_t. Must be null-terminated (0x00000000).
 *
 * @return Length of the provided string.
 */
size_t _reencoder_utf32_strlen(const uint32_t* string);

void _reencoder_utf32_uint32_from_uint8(uint32_t* dest, const uint8_t* src, size_t bytes, enum ReencoderEncodeType source_endian);