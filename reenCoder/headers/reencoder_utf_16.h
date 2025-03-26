#pragma once

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "reencoder_utf_common.h"

/**
 * @brief Parses a given UTF-16 uint16_t* sequence and loads it into a `ReencoderUnicodeStruct`.
 *
 * Input string must be represented as uint16_t*.
 * The returned `ReencoderUnicodeStruct` will be fully initialised if the string is valid.
 * ReencoderUnicodeStruct->num_chars will be 0 if the string is invalid.
 *
 * The returned `ReencoderUnicodeStruct` must be freed using `reencoder_unicode_struct_free()` once it is no longer needed.
 *
 * @param[in] string Input UTF-16 string. Must be null-terminated (0x0000).
 * @param[in] target_endian Specifies target UTF-16 endianness (UTF_16BE or UTF_16LE).
 *
 * @return Pointer to a `ReencoderUnicodeStruct` containing parsed string data.
 * @retval NULL If memory allocation fails or an invalid `target_endian` is provided.
 */
ReencoderUnicodeStruct* reencoder_utf16_parse_uint16(const uint16_t* string, enum ReencoderEncodeType target_endian);


/**
 * @brief Parses a given UTF-16 uint8_t* sequence and loads it into a `ReencoderUnicodeStruct`.
 *
 * Input string must be represented as uint8_t* and match provided endianness.
 * The returned `ReencoderUnicodeStruct` will be fully initialised if the string is valid.
 * ReencoderUnicodeStruct->num_chars will be 0 if the string is invalid.
 *
 * The returned `ReencoderUnicodeStruct` must be freed using `reencoder_unicode_struct_free()` once it is no longer needed.
 *
 * @param[in] string Input UTF-16 string.
 * @param[in] bytes Number of bytes in the input string.
 * @param[in] source_endian Specifies source UTF-16 endianness (UTF_16BE or UTF_16LE).
 * @param[in] target_endian Specifies target UTF-16 endianness (UTF_16BE or UTF_16LE).
 *
 * @return Pointer to a `ReencoderUnicodeStruct` containing parsed string data.
 * @retval NULL If memory allocation fails or an invalid `target_endian` is provided.
 */
ReencoderUnicodeStruct* reencoder_utf16_parse_uint8(
	const uint8_t* string, size_t bytes, enum ReencoderEncodeType source_endian, enum ReencoderEncodeType target_endian
);


/**
 * @brief Returns a human-readable string for a given UTF-16 parse outcome code.
 *
 * @param[in] outcome Unsigned integer found at `ReencoderUnicodeStruct->string_validity`.
 *
 * @return String representation of the outcome.
 * @retval NULL If provided outcome is out of bounds.
 */
const char* reencoder_utf16_outcome_as_str(unsigned int outcome);


/**
 * @brief Checks if a provided UTF-16 string is valid.
 *
 * Checks for surrogate order, overlong encoding, and premature string endings.
 *
 * @param[in] string UTF-16 string to be checked. Should be represented as an array of uint16_t.
 * @param[in] length Length of the provided string. Length is not number of bytes, but number of uint16_t elements.
 *
 * @return Unsigned integer representing the outcome of the check. Corresponds to index in `REENCODER_UTF16_OUTCOME_ARR`.
 */
unsigned int _reencoder_utf16_is_valid(const uint16_t* string, size_t length);


/**
 * @brief Returns the length of a UTF-16 string.
 *
 * Length in this case is not number of bytes, but number of uint16_t elements.
 *
 * @param[in] string UTF-16 string to be checked. Represented as an array of uint16_t. Must be null-terminated (0x0000).
 *
 * @return Length of the provided string.
 */
size_t _reencoder_strlen_utf16(const uint16_t* string);


/**
 * @brief Converts a UTF-16 string represented in uint8_t to standardised uint16_t.
 *
 * @param[out] dest Output UTF-16 string uint16_t buffer.
 * @param[in] src Input UTF-16 string. Need not be null-terminated since 0x00 is valid in UTF-16 when represented in 1 byte sequences.
 * @param[in] bytes Byte count of input UTF-16 string.
 * @param[in] source_endian Specifies source UTF-16 endianness (UTF_16BE or UTF_16LE).
 */
void _reencoder_utf16_uint16_from_uint8(uint16_t* dest, const uint8_t* src, size_t bytes, enum ReencoderEncodeType source_endian);