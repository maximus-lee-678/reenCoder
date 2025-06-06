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
unsigned int reencoder_utf8_contains_multibyte(const uint8_t* string);

/**
 * @brief Determines the number of UTF-8 characters, not bytes in a string.
 *
 * @param[in] string UTF-8 string to be checked. Should be represented as an array of uint8_t.
 *
 * @return Number of UTF-8 characters in the string.
 */
size_t _reencoder_utf8_determine_num_chars(const uint8_t* string);

/**
 * @brief Checks if the UTF-8 character present at ptr is valid.
 * 
 * Checks for surrogate presence, overlong encoding, invalid bytes, and premature string endings.
 * Ensures no segmentation fault occurs by always reading at most units_left bytes from ptr.
 * Updates units_actual with the number of code units actually read.
 *
 * @param[in] ptr Pointer to the start of the UTF-8 character in a uint8_t buffer to be checked.
 * @param[in] units_left Number of uint8_t units left in the buffer starting from ptr.
 * @param[out] units_actual Pointer to an unsigned integer where the actual number of code units will be stored. Can be NULL if not needed.
 *
 * @return Unsigned integer representing the outcome of the check. Corresponds to index in `REENCODER_UTF8_OUTCOME_ARR` after offsets.
 */
unsigned int _reencoder_utf8_buffer_idx0_is_valid(const uint8_t* ptr, size_t units_left, unsigned int* units_actual);

/**
 * @brief Checks if a provided UTF-8 string is valid.
 *
 * Checks for surrogate presence, overlong encoding, invalid bytes, and premature string endings.
 *
 * @param[in] string UTF-16 string to be checked. Should be represented as an array of uint8_t.
 *
 * @return Unsigned integer representing the outcome of the check. Corresponds to index in `REENCODER_UTF8_OUTCOME_ARR` after offsets.
 */
unsigned int _reencoder_utf8_seq_is_valid(const uint8_t* string);

/**
 * @brief Given a single UTF-8 starting byte, determines how many bytes this character is.
 *
 * @param[in] first_byte UTF-8 starting byte.
 *
 * @return Unsigned integer 1-4.
 * @retval 0 If leading byte is invalid.
 */
unsigned int _reencoder_utf8_determine_length_from_first_byte(uint8_t first_byte);

/**
 * @brief Given a pointer to a UTF-8 buffer, extracts one character to a code point. Also retrieves original character size.
 *
 * @param[in] ptr Pointer to UTF-8 buffer where the character starts.
 * @param[out] units_read Pointer to an unsigned integer where the original character unit count (no. of 1 byte units) will be stored.
 *
 * @return Single Unicode codepoint.
 * @retval U+FFFD (REPLACEMENT CHARACTER) if the character is invalid.
 */
uint32_t _reencoder_utf8_decode_to_code_point(const uint8_t* ptr, unsigned int* units_read);

/**
 * @brief Encodes a Unicode code point to UTF-8 and writes it to a buffer.
 *
 * Writes a REPLACEMENT CHARACTER (0xEF, 0xBF, 0xBD) if the codepoint is invalid.
 *
 * @param[out] buffer Pointer to the buffer where the encoded UTF-8 character will be written.
 * @param[in] index Index in the buffer where the character will be written.
 * @param[in] code_point Unicode code point to be encoded.
 *
 * @return Unsigned integer representing the unit count (no. of 1 byte units) written to the buffer.
 */
unsigned int _reencoder_utf8_encode_from_code_point(uint8_t* buffer, size_t index, uint32_t code_point);
