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
#define REENCODER_UTF16_ERR_UNPAIRED_HIGH 1601
#define REENCODER_UTF16_ERR_UNPAIRED_LOW 1602
#define REENCODER_UTF16_ERR_ODD_LENGTH 1603
static const char* _REENCODER_UTF16_OUTCOME_ARR[] = {
	"[UTF-16: Valid] Well-formed UTF-16 string",
	"[UTF-16: Invalid] Unpaired high surrogate",
	"[UTF-16: Invalid] Unpaired low surrogate",
	"[UTF-16: Invalid] String byte length not a multiple of 2"
};

#define _REENCODER_UTF32_PARSE_OFFSET 3200
#define REENCODER_UTF32_VALID 3200
#define REENCODER_UTF32_ERR_OUT_OF_RANGE 3201
#define REENCODER_UTF32_ERR_SURROGATE 3202
#define REENCODER_UTF32_ERR_ODD_LENGTH 3203
static const char* _REENCODER_UTF32_OUTCOME_ARR[] = {
	"[UTF-32: Valid] Well-formed UTF-32 string",
	"[UTF-32: Invalid] Out of valid Unicode range (>U+10FFFF)",
	"[UTF-32: Invalid] Surrogate code unit detected (U+D800-U+DFFF)",
	"[UTF-32: Invalid] String byte length not a multiple of 4"
};

#define _REENCODER_UNICODE_REPLACEMENT_CHARACTER 0xFFFD

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
 * @brief Parses a given UTF sequence and converts it to a UTF sequence of different encoding before loading it into a `ReencoderUnicodeStruct`.
 *
 * Input string (source_uint_buffer) must be represented as: uint8_t* (UTF-8), uint16_t* (UTF-16), or uint32_t* (UTF-32) and cast to void*.
 * The returned `ReencoderUnicodeStruct` will be fully initialised if the string is valid.
 * If the provided UTF string is invalid, a ReencoderUnicodeStruct handling that string directly will be returned.
 * ReencoderUnicodeStruct->num_chars will be 0 if any string is invalid (both provided and converted strings).
 *
 * The returned `ReencoderUnicodeStruct` must be freed using `reencoder_unicode_struct_free()` once it is no longer needed.
 *
 * @param[in] source_encoding Specifies source encoding type (UTF-8, UTF_16BE, UTF_16LE, UTF_32BE, or UTF_32LE). Source endian should follow system endianness, obtainable using `_reencoder_is_system_little_endian()`.
 * @param[in] target_encoding Specifies target encoding type (UTF-8, UTF_16BE, UTF_16LE, UTF_32BE, or UTF_32LE).
 * @param[in] source_uint_buffer Input UTF string. Must be represented as a uint8_t* (UTF-8), uint16_t* (UTF-16), or uint32_t* (UTF-32) and cast to const void*. Must be null-terminated: 0x00 (UTF-8), 0x0000 (UTF-16), and 0x00000000 (UTF-32).
 *
 * @return Pointer to a `ReencoderUnicodeStruct` containing data for a string encoded in provided target encoding type.
 * @retval Pointer to a `ReencoderUnicodeStruct` containing data for a string encoded in provided source encoding type if the provided string is invalid.
 * @retval NULL If memory allocation fails or an invalid `target_endian` is provided.
 */
ReencoderUnicodeStruct* reencoder_convert(enum ReencoderEncodeType source_encoding, enum ReencoderEncodeType target_encoding, const void* source_uint_buffer);

/**
 * @brief Determines if the system is little-endian.
 *
 * Intended for internal use by reencoder_utf_* functions.
 *
 * @return 1 if the system is little-endian, 0 if the system is big-endian.
 */
uint8_t reencoder_is_system_little_endian();

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
 * @brief Initialises or grows a buffer for UTF-8/16/32 encoding.
 *
 * If the buffer is NULL, it will be allocated with the base size (_REENCODER_BASE_STRING_BYTE_SIZE).
 * The buffer either grows by a defined factor (_REENCODER_BASE_STRING_GROW_RATE) when allocate_only_one_unit is 0,
 * and by element_size bytes when allocate_only_one_unit is 1.
 *
 * @param[in] buffer Buffer to expand. Can be NULL.
 * @param[in/out] current_buffer_size Current size of the buffer. Is updated to the new size during function call.
 * @param[in] allocate_only_one_unit Whether or not to allocate only one unit of memory, particularly just for the null-terminator.
 * @param[in] element_size Size of one unit in caller's context. Use sizeof(uint8_t), sizeof(uint16_t), or sizeof(uint32_t) as appropriate.
 *
 * @return Pointer to the new buffer. Recommended to cast to appropriate type (uint8_t*, uint16_t*, or uint32_t*).
 * @retval NULL If memory allocation fails.
 */
void* _reencoder_grow_buffer(void* buffer, size_t* current_buffer_size, unsigned int allocate_only_one_unit, size_t element_size);


/**
 * @brief Checks if a given Unicode code point is valid.
 * 
 * Performs range and surrogate pair checks.
 *
 * @param[in] code_point Unicode code point.
 *
 * @return 1 if the code point is valid, 0 if it is not.
 */
unsigned int _reencoder_code_point_is_valid(const uint32_t code_point);

// Below are declared extern functions present in reencoder_utf_8.h, reencoder_utf_16.h, and reencoder_utf_32.h.
// Separated by file for clarity.

extern ReencoderUnicodeStruct* reencoder_utf8_parse(const uint8_t* string);
extern unsigned int _reencoder_utf8_seq_is_valid(const uint8_t* string);
extern uint32_t _reencoder_utf8_decode_to_code_point(const uint8_t* ptr, unsigned int* units_read);
extern unsigned int _reencoder_utf8_encode_from_code_point(uint8_t* buffer, size_t index, uint32_t code_point);

extern ReencoderUnicodeStruct* reencoder_utf16_parse_uint16(const uint16_t* string, enum ReencoderEncodeType target_endian);
extern size_t _reencoder_utf16_strlen(const uint16_t* string);
unsigned int _reencoder_utf16_seq_is_valid(const uint16_t* string, size_t length);
extern uint32_t _reencoder_utf16_decode_to_code_point(const uint16_t* ptr, unsigned int* char_units);
extern unsigned int _reencoder_utf16_encode_from_code_point(uint16_t* buffer, size_t index, uint32_t code_point);
extern void _reencoder_utf16_write_buffer_swap_endian(uint8_t* dest, const uint16_t* src, size_t length);

extern ReencoderUnicodeStruct* reencoder_utf32_parse_uint32(const uint32_t* string, enum ReencoderEncodeType target_endian);
extern size_t _reencoder_utf32_strlen(const uint32_t* string);
extern unsigned int _reencoder_utf32_seq_is_valid(const uint32_t* string, size_t length);
extern uint32_t _reencoder_utf32_decode_to_code_point(const uint32_t* ptr, unsigned int* units_read);
extern unsigned int _reencoder_utf32_encode_from_code_point(uint32_t* buffer, size_t index, uint32_t code_point);
extern void _reencoder_utf32_write_buffer_swap_endian(uint8_t* dest, const uint32_t* src, size_t length);
