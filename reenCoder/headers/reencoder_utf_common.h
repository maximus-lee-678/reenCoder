#pragma once

#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#define _REENCODER_BASE_STRING_BYTE_SIZE 256
#define _REENCODER_BASE_STRING_GROW_RATE 4

/**
 * @brief Enum containing supported Unicode string types.
 *
 * Contains values UTF-8, UTF-16BE, UTF-16LE, UTF-32BE, and UTF-32LE.
 */
enum ReencoderEncodeType {
	UTF_8,
	UTF_16BE,
	UTF_16LE,
	UTF_32BE,
	UTF_32LE
};
static const char* _REENCODER_ENCODE_TYPE_ARR[] = {
	"UTF-8",
	"UTF-16BE",
	"UTF-16LE",
	"UTF-32BE",
	"UTF-32LE"
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

#define _REENCODER_UTF8_PARSE_OFFSET 800
#define REENCODER_UTF8_VALID 800
#define REENCODER_UTF8_ERR_INVALID_LEAD 801
#define REENCODER_UTF8_ERR_PREMATURE_END 802
#define REENCODER_UTF8_ERR_INVALID_CONT 803
#define REENCODER_UTF8_ERR_OVERLONG_2BYTE 804
#define REENCODER_UTF8_ERR_OVERLONG_3BYTE 805
#define REENCODER_UTF8_ERR_OVERLONG_4BYTE 806
#define REENCODER_UTF8_ERR_OUT_OF_RANGE 807
#define REENCODER_UTF8_ERR_SURROGATE_PAIR 808
static const char* _REENCODER_UTF8_OUTCOME_ARR[] = {
	"[UTF-8: Valid] Well-formed UTF-8 string",
	"[UTF-8: Invalid] Invalid leading byte",
	"[UTF-8: Invalid] String ended before the sequence completed",
	"[UTF-8: Invalid] Invalid continuation byte",
	"[UTF-8: Invalid] Overlong encoding for 2-byte sequence",
	"[UTF-8: Invalid] Overlong encoding for 3-byte sequence",
	"[UTF-8: Invalid] Overlong encoding for 4-byte sequence",
	"[UTF-8: Invalid] Out of valid Unicode range (>U+10FFFF)",
	"[UTF-8: Invalid] Surrogate pair detected (U+D800-U+DFFF)"
};

#define _REENCODER_UTF16_PARSE_OFFSET 1600
#define REENCODER_UTF16_VALID 1600
#define REENCODER_UTF16_PREMATURE_END 1601
#define REENCODER_UTF16_UNPAIRED_HIGH 1602
#define REENCODER_UTF16_UNPAIRED_LOW 1603
#define REENCODER_UTF16_OVERLONG_ENCODING 1604
#define REENCODER_UTF16_ODD_LENGTH 1605
static const char* _REENCODER_UTF16_OUTCOME_ARR[] = {
	"[UTF-16: Valid] Well-formed UTF-16 string",
	"[UTF-16: Invalid] String ended before the sequence completed",
	"[UTF-16: Invalid] Unpaired high surrogate",
	"[UTF-16: Invalid] Unpaired low surrogate",
	"[UTF-16: Invalid] Overlong encoding for 4 byte sequence",
	"[UTF-16: Invalid] String byte length not a multiple of 2"
};

#define _REENCODER_UTF32_PARSE_OFFSET 3200
#define REENCODER_UTF32_VALID 3200
#define REENCODER_UTF32_OUT_OF_RANGE 3201
#define REENCODER_UTF32_SURROGATE_PAIR 3202
#define REENCODER_UTF32_ODD_LENGTH 3203
static const char* _REENCODER_UTF32_OUTCOME_ARR[] = {
	"[UTF-32: Valid] Well-formed UTF-32 string",
	"[UTF-32: Invalid] Out of valid Unicode range (>U+10FFFF)",
	"[UTF-32: Invalid] Surrogate pair detected (U+D800-U+DFFF)",
	"[UTF-32: Invalid] String byte length not a multiple of 4"
};

/**
 * @brief Initialises a `ReencoderUnicodeStruct` with the provided string type.
 *
 * Intended for internal use by reencoder_utf_* functions which require a base.
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
 * @brief Initialises a `ReencoderUnicodeStruct` dynamically based on provided parameters.
 *
 * Intended for internal use by reencoder_utf_* functions.
 * string_type is copied as-is.
 * string_buffer is updated dynamically based on string_type; UTF_8 is copied directly while UTF_16 and UTF_32 undergo any needed endianness conversion.
 * string_validity is copied as-is.
 * num_chars is populated to the provided value IF the string is valid, otherwise it is left as default 0.
 * num_bytes is copied as-is (from string_buffer_bytes).
 *
 * @param[in] string_type The type of the string to be parsed. Must be one of the `ReencoderEncodeType` enum values.
 * @param[in] string_buffer Provided string buffer to be copied into the `ReencoderUnicodeStruct`. Data type should match the provided string_type.
 * @param[in] string_buffer_bytes Byte size of string buffer.
 * @param[in] string_validity String validity parsed value.
 * @param[in] num_chars Number of characters present in string buffer. Only populated if string_validity is valid.
 *
 * @return Pointer to a default `ReencoderUnicodeStruct`.
 *
 * @note The returned `ReencoderUnicodeStruct` must be freed using `reencoder_unicode_struct_free()`.
 */
ReencoderUnicodeStruct* _reencoder_unicode_struct_express_populate(
	enum ReencoderEncodeType string_type, const void* string_buffer, size_t string_buffer_bytes,
	unsigned int string_validity, size_t num_chars
);


/**
 * @brief Frees a `ReencoderUnicodeStruct` and its string buffer.
 *
 * @param[in] unicode_struct Pointer to the `ReencoderUnicodeStruct` to be freed.
 *
 * @return void
 */
void reencoder_unicode_struct_free(ReencoderUnicodeStruct* unicode_struct);


/**
 * @brief Returns a human-readable string for a given ReencoderEncodeType.
 *
 * @param[in] Encode enum found at `ReencoderUnicodeStruct->string_type`.
 *
 * @return String representation of encode type of a string in a ReencoderUnicodeStruct struct.
 * @retval NULL If provided type is out of bounds.
 */
const char* reencoder_encode_type_as_str(unsigned int encode_type);


/**
 * @brief Determines if the system is little-endian.
 *
 * Intended for internal use by reencoder_utf_* functions.
 *
 * @return 1 if the system is little-endian, 0 if the system is big-endian.
 */
uint8_t _reencoder_is_system_little_endian();

/**
 * @brief Returns a human-readable string for a given parse outcome code.
 *
 * @param[in] outcome Unsigned integer found at `ReencoderUnicodeStruct->string_validity`.
 * @param[in] error_for_string_type The type of the string that was parsed. Must be one of the `ReencoderEncodeType` enum values.
 *
 * @return String representation of the outcome.
 * @retval NULL If provided outcome is out of bounds.
 * @retval NULL If an invalid enum is provided for error_for_string_type.
 */
const char* reencoder_outcome_as_str(unsigned int outcome, enum ReencoderEncodeType error_for_string_type);

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
extern void _reencoder_utf16_write_buffer_swap_endian(uint8_t* dest, const uint16_t* src, size_t length);

/**
 * @brief Writes a UTF-32 uint32_t string to a buffer with swapped endianness.
 *
 * Officially declared in reencoder_utf_32.c.
 * Since UTF-32 strings are represented in memory as 32-bit code units, the endianness of the system must be considered.
 * To preserve intended byte order, this function writes to a 1-byte wide buffer (uint8_t).
 *
 * @param[in] src UTF-32 string to be written.
 * @param[out] dest Buffer to be written to.
 * @param[in] length Length of the provided string. Length is not number of bytes, but number of uint32_t elements.
 *
 * @return void
 */
extern void _reencoder_utf32_write_buffer_swap_endian(uint8_t* dest, const uint32_t* src, size_t length);
