#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/**
 * @brief Enum containing supported Unicode string types.
 *
 * Contains values UTF-8, UTF-16BE, UTF-16LE, UTF-32BE, and UTF-32LE.
 */
enum ReencoderEncodeType {
	UTF8,
	UTF16BE,
	UTF16LE,
	UTF32BE,
	UTF32LE
};

/**
 * @brief Struct containing information about a parsed Unicode string.
 *
 * Contains the string type (string_type), the string in a 1 byte buffer (string_buffer),
 * validity of the string (string_validity), number of characters (num_chars), and number of bytes (num_bytes).
 */
typedef struct {
	enum ReencoderEncodeType string_type;
	uint8_t* string_buffer;
	unsigned int string_validity;
	size_t num_chars;
	size_t num_bytes;
} ReencoderUnicodeStruct;

/**
 * @brief Initialises a `ReencoderUnicodeStruct` with the provided string type.
 *
 * Intended for internal use by reencoder_utf_* functions.
 * string_buffer is set to NULL, string_validity to 0, num_chars to 0, and num_bytes to 0.
 *
 * @param[in] string_type The type of the string to be parsed. Must be one of the `ReencoderEncodeType` enum values.
 *
 * @return Pointer to a default `ReencoderUnicodeStruct`.
 *
 * @note The returned `ReencoderUnicodeStruct` must be freed using `reencoder_unicode_struct_free()`.
 */
ReencoderUnicodeStruct* _reencoder_unicode_struct_init(enum ReencoderEncodeType string_type);

/**
 * @brief Frees a `ReencoderUnicodeStruct` and its string buffer.
 *
 * @param[in] unicode_struct Pointer to the `ReencoderUnicodeStruct` to be freed.
 *
 * @return void
 */
void reencoder_unicode_struct_free(ReencoderUnicodeStruct* unicode_struct);

/**
 * @brief Determines if the system is little-endian.
 *
 * Intended for internal use by reencoder_utf_* functions.
 *
 * @return 1 if the system is little-endian, 0 if the system is big-endian.
 */
uint8_t _reencoder_is_system_little_endian();
