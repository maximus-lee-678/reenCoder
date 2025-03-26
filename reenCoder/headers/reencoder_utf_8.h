#pragma once

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "reencoder_utf_common.h"
#include "reencoder_utf_16.h"

/**
 * @brief Parses a given UTF-8 uint8_t* sequence and loads it into a `ReencoderUnicodeStruct`.
 *
 * Input string must be represented as uint8_t*.
 * The returned `ReencoderUnicodeStruct` will be fully initialised if the string is valid.
 * ReencoderUnicodeStruct->num_chars will be 0 if the string is invalid.
 *
 * The returned `ReencoderUnicodeStruct` must be freed using `reencoder_unicode_struct_free()` once it is no longer needed.
 *
 * @param[in] string Input UTF-8 string. Must be null-terminated (0x00).
 *
 * @return Pointer to a `ReencoderUnicodeStruct` containing parsed string data for a UTF-8 string.
 * @retval NULL If memory allocation fails or an invalid `target_endian` is provided.
 */
ReencoderUnicodeStruct* reencoder_utf8_parse(const uint8_t* string);


/**
 * @brief Parses a given UTF-16 uint16_t* sequence and converts it to UTF-8 before loading it into a `ReencoderUnicodeStruct`.
 *
 * Input string must be represented as uint16_t*.
 * The returned `ReencoderUnicodeStruct` will be fully initialised if the string is valid.
 * If the provided UTF-16 string is invalid, a ReencoderUnicodeStruct handling the UTF-16 string directly will be returned.
 * ReencoderUnicodeStruct->num_chars will be 0 if the string is invalid (both UTF-8 and UTF-16).
 *
 * The returned `ReencoderUnicodeStruct` must be freed using `reencoder_unicode_struct_free()` once it is no longer needed
 *
 * @param[in] string Input UTF-16 string. Must be null-terminated (0x0000).
 *
 * @return Pointer to a `ReencoderUnicodeStruct` containing parsed string data for a UTF-8 string.
 * @retval Pointer to a `ReencoderUnicodeStruct` containing parsed string data for a UTF-16 string if the provided UTF-16 string is invalid.
 * @retval NULL If memory allocation fails.
 */
ReencoderUnicodeStruct* reencoder_utf8_parse_from_utf16(const uint16_t* string);


/**
 * @brief Parses a given UTF-16 uint8_t* sequence and converts it to UTF-8 before loading it into a ReencoderUnicodeStruct.
 *
 * Input string must be represented as uint8_t*.
 * The returned `ReencoderUnicodeStruct` will be fully initialised if the string is valid.
 * If the provided UTF-16 string is invalid, a ReencoderUnicodeStruct handling the UTF-16 string directly will be returned.
 * ReencoderUnicodeStruct->num_chars will be 0 if the string is invalid (both UTF-8 and UTF-16).
 *
 * The returned `ReencoderUnicodeStruct` must be freed using `reencoder_unicode_struct_free()` once it is no longer needed.
 *
 * @param[in] string Input UTF-16 string. Need not be null-terminated since 0x00 is valid in UTF-16 when represented in 1 byte sequences.
 * @param[in] bytes Data size of input UTF-16 string.
 * @param[in] source_endian Specifies source UTF-16 endianness (UTF_16BE or UTF_16LE).
 *
 * @return Pointer to a `ReencoderUnicodeStruct` containing parsed string data for a UTF-8 string.
 * @retval Pointer to a `ReencoderUnicodeStruct` containing parsed string data for a UTF-16 string if the provided UTF-16 string is invalid.
 * @retval NULL If memory allocation fails.
 */
ReencoderUnicodeStruct* reencoder_utf8_parse_from_utf16_uint8(const uint8_t* string, size_t bytes, enum ReencoderEncodeType source_endian);


/**
 * @brief Returns a human-readable string for a given UTF-8 parse outcome code.
 *
 * @param[in] outcome Unsigned integer found at `ReencoderUnicodeStruct->string_validity`.
 *
 * @return String representation of the outcome.
 * @retval NULL If provided outcome is out of bounds.
 */
const char* reencoder_utf8_outcome_as_str(unsigned int outcome);


/**
 * @brief Checks if a given UTF-8 string contains multibyte sequences.
 *
 * String is represented as a uint8_t*.
 * It is best to check if the string is valid first by using reencoder_utf8_parse().
 *
 * Alternatively, you could also compare the values of
 * `ReencoderUnicodeStruct->num_chars` and `ReencoderUnicodeStruct->num_bytes` and see if they
 * match. If they do, the string does not contain multibyte sequences.
 *
 * @param[in] string UTF-8 string.
 *
 * @return 1 if string contains multibyte sequences, 0 if it does not.
 * @retval 0 if an invalid leading byte is detected.
 */
uint8_t reencoder_utf8_contains_multibyte(const uint8_t* string);


/**
 * @brief Checks if a provided UTF-8 string is valid.
 *
 * Checks for surrogate presence, overlong encoding, invalid bytes, and premature string endings.
 *
 * @param[in] string UTF-16 string to be checked. Should be represented as an array of uint8_t.
 *
 * @return Unsigned integer representing the outcome of the check. Corresponds to index in `REENCODER_UTF8_OUTCOME_ARR` after offsets.
 */
static unsigned int _reencoder_utf8_is_valid(const uint8_t* string);
