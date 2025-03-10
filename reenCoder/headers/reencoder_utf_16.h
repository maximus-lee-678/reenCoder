#pragma once

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "reencoder_utf_common.h"

#define REENCODER_UTF16_VALID 0
#define REENCODER_UTF16_PREMATURE_END 1
#define REENCODER_UTF16_UNPAIRED_HIGH 2
#define REENCODER_UTF16_UNPAIRED_LOW 3
#define REENCODER_UTF16_OVERLONG_ENCODING 4
static const char* REENCODER_UTF16_OUTCOME_ARR[] = {
	"Well-formed UTF-16 string",
	"String ended before the sequence completed",
	"Unpaired high surrogate",
	"Unpaired low surrogate",
	"Overlong encoding for 4 byte sequence"
};

/**
 * @brief Parses a given UTF-16 string.
 *
 * Converts a null-terminated UTF-16 string (represented as a uint16_t* or any equivalent size)
 * into a `ReencoderUnicodeStruct` containing detailed information about the string.
 *
 * @param[in] string Input UTF-16 string. Must be null-terminated (0x0000).
 * @param[in] target_endian Specifies target UTF-16 endianness (UTF16BE or UTF16LE).
 *
 * @return Pointer to a `ReencoderUnicodeStruct` containing parsed string data.
 * @retval NULL If memory allocation fails or an invalid `target_endian` is provided.
 *
 * @note The returned `ReencoderUnicodeStruct` must be freed using `reencoder_unicode_struct_free()`.
 */
ReencoderUnicodeStruct* reencoder_utf16_parse_uint16(const uint16_t* string, enum ReencoderEncodeType target_endian);

/**
 * @brief Parses a given UTF-16 string.
 *
 * Converts a null-terminated UTF-16 string (represented as a uint8_t* or any equivalent size)
 * into a `ReencoderUnicodeStruct` containing detailed information about the string.
 *
 * @param[in] string Input UTF-16 string.
 * @param[in] bytes Number of bytes in the input string.
 * @param[in] target_endian Specifies target UTF-16 endianness (UTF16BE or UTF16LE).
 *
 * @return Pointer to a `ReencoderUnicodeStruct` containing parsed string data.
 * @retval NULL If memory allocation fails or an invalid `target_endian` is provided.
 *
 * @note The returned `ReencoderUnicodeStruct` must be freed using `reencoder_unicode_struct_free()`.
 */
ReencoderUnicodeStruct* reencoder_utf16_parse_uint8(const uint8_t* string, size_t bytes, enum ReencoderEncodeType target_endian);

/**
 * @brief Returns a human-readable string for a given UTF-16 parse outcome code.
 *
 * @param[in] outcome Unsigned integer found at `ReencoderUnicodeStruct->string_validity`.
 *
 * @return String representation of the outcome.
 * @retval NULL If provided outcome is out of bounds.
 */
const char* reencoder_utf16_outcome_as_str(unsigned int outcome);
