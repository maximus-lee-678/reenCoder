#pragma once

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "reencoder_utf_common.h"

#define REENCODER_UTF8_VALID 0
#define REENCODER_UTF8_ERR_INVALID_LEAD 1
#define REENCODER_UTF8_ERR_PREMATURE_END 2
#define REENCODER_UTF8_ERR_INVALID_CONT 3
#define REENCODER_UTF8_ERR_OVERLONG_2BYTE 4
#define REENCODER_UTF8_ERR_OVERLONG_3BYTE 5
#define REENCODER_UTF8_ERR_OVERLONG_4BYTE 6
#define REENCODER_UTF8_ERR_OUT_OF_RANGE 7
#define REENCODER_UTF8_ERR_SURROGATE_PAIR 8
static const char* REENCODER_UTF8_OUTCOME_ARR[] = {
	"Well-formed UTF-8 string",
	"Invalid leading byte",
	"String ended before the sequence completed",
	"Invalid continuation byte",
	"Overlong encoding for 2-byte sequence",
	"Overlong encoding for 3-byte sequence",
	"Overlong encoding for 4-byte sequence",
	"Out of valid Unicode range (U+10FFFF+)",
	"Surrogate pair detected (U+D800–U+DFFF)"
};

/**
 * @brief Parses a given UTF-8 string.
 *
 * Converts a null-terminated UTF-8 string (represented as a uint8_t* or any equivalent size)
 * into a `ReencoderUnicodeStruct` containing detailed information about the string.
 *
 * @param[in] string Input UTF-8 string. Must be null-terminated (0x00).
 *
 * @return Pointer to a `ReencoderUnicodeStruct` containing parsed string data.
 * @retval NULL If memory allocation fails or an invalid `target_endian` is provided.
 *
 * @note The returned `ReencoderUnicodeStruct` must be freed using `reencoder_unicode_struct_free()`.
 */
ReencoderUnicodeStruct* reencoder_utf8_parse(const unsigned char* string);

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
 * String is represented as a uint8_t* or any equivalent size like unsigned char*.
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
uint8_t reencoder_utf8_contains_multibyte(const unsigned char* string);
