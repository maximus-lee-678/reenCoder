#pragma once

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "reencoder_utf_common.h"
#include "reencoder_utf_8.h"
#include "reencoder_utf_16.h"

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

/**
 * @brief Parses a given UTF-32 uint8_t* sequence and loads it into a `ReencoderUnicodeStruct`.
 *
 * Input string must be represented as uint8_t* and match provided endianness.
 * The returned `ReencoderUnicodeStruct` will be fully initialised if the string is valid.
 * ReencoderUnicodeStruct->num_chars will be 0 if the string is invalid.
 *
 * The returned `ReencoderUnicodeStruct` must be freed using `reencoder_unicode_struct_free()` once it is no longer needed.
 *
 * @param[in] string Input UTF-32 string.
 * @param[in] bytes Number of bytes in the input string.
 * @param[in] source_endian Specifies source UTF-32 endianness (UTF_32BE or UTF_32LE).
 * @param[in] target_endian Specifies target UTF-32 endianness (UTF_32BE or UTF_32LE).
 *
 * @return Pointer to a `ReencoderUnicodeStruct` containing parsed string data.
 * @retval NULL If memory allocation fails or an invalid `target_endian` is provided.
 */
ReencoderUnicodeStruct* reencoder_utf32_parse_uint8(
	const uint8_t* string, size_t bytes, enum ReencoderEncodeType source_endian, enum ReencoderEncodeType target_endian
);

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

/**
 * @brief Checks if the UTF-32 character present at ptr is valid.
 *
 * Checks for surrogate presence, and out of range values.
 * Checks for surrogate order, overlong encoding, and premature string endings.
 * 
 * @param[in] ptr Pointer to the start of the UTF-32 character in a uint32_t buffer to be checked.
 *
 * @return Unsigned integer representing the outcome of the check. Corresponds to index in `REENCODER_UTF32_OUTCOME_ARR` after offsets.
 */
unsigned int _reencoder_utf32_buffer_idx0_is_valid(const uint32_t* ptr);

/**
 * @brief Checks if a provided UTF-32 string is valid.
 *
 * Checks for surrogate presence, and out of range values.
 *
 * @param[in] string UTF-32 string to be checked. Should be represented as an array of uint32_t.
 * @param[in] length Length of the provided string. Length is not number of bytes, but number of uint32_t elements.
 *
 * @return Unsigned integer representing the outcome of the check. Corresponds to index in `REENCODER_UTF32_OUTCOME_ARR` after offsets.
 */
unsigned int _reencoder_utf32_seq_is_valid(const uint32_t* string, size_t length);

/**
 * @brief Converts a UTF-32 string represented in uint8_t to standardised uint32_t.
 *
 * If number of bytes is not a multiple of 4, the function will change the last code point to a replacement character (U+FFFD).
 *
 * @param[out] dest Output UTF-32 string buffer.
 * @param[in] src Input UTF-32 string. Need not be null-terminated since 0x00 is valid in UTF-32 when represented in 1 byte sequences.
 * @param[in] bytes Byte count of input UTF-32 string.
 * @param[in] source_endian Specifies source UTF-32 endianness (UTF_32BE or UTF_32LE).
 */
void _reencoder_utf32_uint32_from_uint8(uint32_t* dest, const uint8_t* src, size_t bytes, enum ReencoderEncodeType source_endian);

/**
 * @brief Given a pointer to a UTF-32 buffer, extracts one character to a code point. Also retrieves original character size.
 *
 * @param[in] ptr Pointer to UTF-32 buffer where the character starts.
 * @param[out] units_read Pointer to an unsigned integer where the original character unit count (no. of 4 byte units) will be stored.
 *
 * @return Single Unicode code point.
 * @retval U+FFFD (REPLACEMENT CHARACTER) if the character is invalid.
 */
uint32_t _reencoder_utf32_decode_to_code_point(const uint32_t* ptr, unsigned int* units_read);

/**
 * @brief Encodes a Unicode codepoint to UTF-32 and writes it to a buffer.
 *
 * Writes a REPLACEMENT CHARACTER (0x0000FFFD) if the code point is invalid.
 *
 * @param[out] buffer Pointer to the buffer where the encoded UTF-32 character will be written.
 * @param[in] index Index in the buffer where the character will be written.
 * @param[in] code_point Unicode code point to be encoded.
 *
 * @return Unsigned integer representing the unit count (no. of 4 byte units) written to the buffer.
 */
unsigned int _reencoder_utf32_encode_from_code_point(uint32_t* buffer, size_t index, uint32_t code_point);


/**
 * @brief Writes a UTF-32 uint32_t string to a buffer with swapped endianness.
 *
 * Since UTF-32 strings are represented in memory as 32-bit code units, the endianness of the system must be considered.
 * To preserve intended byte order, this function writes to a 1-byte wide buffer (uint8_t).
 *
 * @param[in] src UTF-32 string to be written.
 * @param[out] dest Buffer to be written to.
 * @param[in] length Length of the provided string. Length is not number of bytes, but number of uint32_t elements.
 *
 * @return void
 */
void _reencoder_utf32_write_buffer_swap_endian(uint8_t* dest, const uint32_t* src, size_t length);
