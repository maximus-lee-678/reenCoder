#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "reencoder_utf_common.h"
#include "reencoder_utf_8.h"
#include "reencoder_utf_32.h"

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
ReencoderUnicodeStruct* reencoder_utf16_parse_uint8(const uint8_t* string, size_t bytes, enum ReencoderEncodeType source_endian, enum ReencoderEncodeType target_endian);

/**
 * @brief Returns the length of a UTF-16 string.
 *
 * Length in this case is not number of bytes, but number of uint16_t elements.
 *
 * @param[in] string UTF-16 string to be checked. Represented as an array of uint16_t. Must be null-terminated (0x0000).
 *
 * @return Length of the provided string.
 */
size_t _reencoder_utf16_strlen(const uint16_t* string);

/**
 * @brief Determines the number of UTF-16 characters, not bytes in a string.
 *
 * @param[in] string UTF-16 string to be checked. Should be represented as an array of uint16_t.
 *
 * @return Number of UTF-16 characters in the string.
 */
size_t _reencoder_utf16_determine_num_chars(const uint16_t* string);

/**
 * @brief Checks if the UTF-16 character present at ptr is valid.
 *
 * Checks for surrogate order, overlong encoding, and premature string endings.
 * Ensures no segmentation fault occurs by always reading at most units_left bytes from ptr.
 * Updates units_actual with the number of code units actually read.
 *
 * @param[in] ptr Pointer to the start of the UTF-16 character in a uint16_t buffer to be checked.
 * @param[in] units_left Number of uint16_t units left in the buffer starting from ptr.
 * @param[out] units_actual Pointer to an unsigned integer where the actual number of code units will be stored. Can be NULL if not needed.
 *
 * @return Unsigned integer representing the outcome of the check. Corresponds to index in `REENCODER_UTF16_OUTCOME_ARR` after offsets.
 */
unsigned int _reencoder_utf16_buffer_idx0_is_valid(const uint16_t* ptr, size_t units_left, unsigned int* units_actual);

/**
 * @brief Checks if a provided UTF-16 string is valid.
 *
 * Checks for surrogate order, overlong encoding, and premature string endings.
 *
 * @param[in] string UTF-16 string to be checked. Should be represented as an array of uint16_t.
 * @param[in] length Length of the provided string. Length is not number of bytes, but number of uint16_t elements.
 *
 * @return Unsigned integer representing the outcome of the check. Corresponds to index in `REENCODER_UTF16_OUTCOME_ARR` after offsets.
 */
unsigned int _reencoder_utf16_seq_is_valid(const uint16_t* string, size_t length);

/**
 * @brief Converts a UTF-16 string represented in uint8_t to standardised uint16_t.
 *
 * If an odd number of bytes is provided, the function will change the last code point to a replacement character (U+FFFD).
 *
 * @param[out] dest Output UTF-16 string buffer.
 * @param[in] src Input UTF-16 string. Need not be null-terminated since 0x00 is valid in UTF-16 when represented in 1 byte sequences.
 * @param[in] bytes Byte count of input UTF-16 string.
 * @param[in] source_endian Specifies source UTF-16 endianness (UTF_16BE or UTF_16LE).
 */
void _reencoder_utf16_uint16_from_uint8(uint16_t* dest, const uint8_t* src, size_t bytes, enum ReencoderEncodeType source_endian);

/**
 * @brief Given a pointer to a UTF-16 buffer, extracts one character to a code point. Also retrieves original character size.
 *
 * @param[in] ptr Pointer to UTF-16 buffer where the character starts.
 * @param[out] char_units Pointer to an unsigned integer where the original character unit count (no. of 2 byte units) will be stored.
 *
 * @return Single Unicode code point.
 * @retval U+FFFD (REPLACEMENT CHARACTER) if the character is invalid.
 */
uint32_t _reencoder_utf16_decode_to_code_point(const uint16_t* ptr, unsigned int* char_units);

/**
 * @brief Encodes a Unicode codepoint to UTF-16 and writes it to a buffer.
 *
 * Writes a REPLACEMENT CHARACTER (0xFFFD) if the code point is invalid.
 *
 * @param[out] buffer Pointer to the buffer where the encoded UTF-16 character will be written.
 * @param[in] index Index in the buffer where the character will be written.
 * @param[in] code_point Unicode code point to be encoded.
 *
 * @return Unsigned integer representing the unit count (no. of 2 byte units) written to the buffer.
 */
unsigned int _reencoder_utf16_encode_from_code_point(uint16_t* buffer, size_t index, uint32_t code_point);

/**
 * @brief Writes a UTF-16 uint16_t string to a buffer with swapped endianness.
 *
 * Officially declared in reencoder_utf_16.c.
 * Since UTF-16 strings are represented in memory as 16-bit code units, the endianness of the system must be considered.
 * To preserve intended byte order, this function writes to a 1-byte wide buffer (uint8_t).
 *
 * @param[in] src UTF-16 string to be written.
 * @param[out] dest Buffer to be written to.
 * @param[in] length Length of the provided string. Length is not number of bytes, but number of uint16_t elements.
 *
 * @return void
 */
void _reencoder_utf16_write_buffer_swap_endian(uint8_t* dest, const uint16_t* src, size_t length);
