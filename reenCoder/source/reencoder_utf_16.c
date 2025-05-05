#include "../headers/reencoder_utf_16.h"

#define _REENCODER_UTF16_REPLACEMENT_CHARACTER 0xFFFD

static inline uint16_t* _reencoder_utf16_grow_buffer(uint16_t* buffer, size_t* current_buffer_size, unsigned int allocate_only_one_unit);

/**
 * @brief Determines the number of UTF-16 characters, not bytes in a string.
 *
 * @param[in] string UTF-16 string to be checked. Should be represented as an array of uint16_t.
 *
 * @return Number of UTF-16 characters in the string.
 */
static size_t reencoder_utf16_determine_num_chars(const uint16_t* string);

static inline unsigned int _reencoder_utf16_char_is_valid(uint32_t code_unit_1, uint32_t code_unit_2, unsigned int potential_surrogate_pair);
static inline unsigned int _reencoder_utf16_validity_check_1_is_not_surrogate(uint32_t code_unit);
static inline unsigned int _reencoder_utf16_validity_check_2_is_high_surrogate(uint32_t code_unit);
static inline unsigned int _reencoder_utf16_validity_check_3_is_low_surrogate(uint32_t code_unit);

// ##### //
// https://datatracker.ietf.org/doc/html/rfc2781/
// ##### //

ReencoderUnicodeStruct* reencoder_utf16_parse_uint16(const uint16_t* string, enum ReencoderEncodeType target_endian) {
	// REENCODER_UTF_16 USER FUNCTION DEFINITION

	if (target_endian != UTF_16BE && target_endian != UTF_16LE) {
		return NULL;
	}

	size_t string_length_uint16 = _reencoder_utf16_strlen(string);
	size_t string_size_bytes = string_length_uint16 * sizeof(uint16_t);

	return _reencoder_unicode_struct_express_populate(
		target_endian, (const void*)string, string_size_bytes, 
		_reencoder_utf16_seq_is_valid(string, string_length_uint16), reencoder_utf16_determine_num_chars(string)
	);
}

ReencoderUnicodeStruct* reencoder_utf16_parse_uint8(const uint8_t* string, size_t bytes, enum ReencoderEncodeType source_endian, enum ReencoderEncodeType target_endian) {
	// REENCODER_UTF_16 USER FUNCTION DEFINITION

	if (target_endian != UTF_16BE && target_endian != UTF_16LE) {
		return NULL;
	}

	// always malloc a multiple of 2 bytes, going higher if needed
	size_t bytes_adjusted = bytes + (bytes % sizeof(uint16_t));
	uint16_t* string_uint16 = (uint16_t*)malloc(bytes_adjusted + sizeof(uint16_t));
	if (string_uint16 == NULL) {
		return NULL;
	}
	_reencoder_utf16_uint16_from_uint8(string_uint16, string, bytes, source_endian);

	// odd number of bytes is impossible for UTF-16
	if (bytes % 2 != 0) {
		return _reencoder_unicode_struct_express_populate(
			_reencoder_is_system_little_endian() ? UTF_16LE : UTF_16BE, (const void*)string_uint16, bytes, REENCODER_UTF16_ERR_ODD_LENGTH, 0
		);
	}

	ReencoderUnicodeStruct* struct_utf16_str = reencoder_utf16_parse_uint16(string_uint16, target_endian);

	// clean up other allocated memory
	free(string_uint16);

	return struct_utf16_str;
}

ReencoderUnicodeStruct* reencoder_utf16_parse_from_utf8(const uint8_t* string, enum ReencoderEncodeType target_endian) {
	// REENCODER_UTF_16 USER FUNCTION DEFINITION

	if (target_endian != UTF_16BE && target_endian != UTF_16LE) {
		return NULL;
	}

	size_t string_length = strlen(string);
	size_t string_size_bytes = string_length;

	// check if utf-8 is valid, if not, return a utf-8 struct
	unsigned int input_utf8_validity = _reencoder_utf8_seq_is_valid(string);
	if (input_utf8_validity != REENCODER_UTF8_VALID) {
		return _reencoder_unicode_struct_express_populate(
			UTF_8, (const void*)string, string_size_bytes, input_utf8_validity, 0
		);
	}

	// begin conversion utf-8 -> utf-16
	size_t utf16_index = 0;
	size_t utf16_buffer_size = 0;
	uint16_t* utf16_output_buffer = NULL;

	// assumes utf8 is well-formed, since we already checked earlier
	const uint8_t* ptr_read = string;
	while (*ptr_read) {
		// grow buffer if uninitialised or out of space
		if ((utf16_index + 2) * sizeof(uint16_t) > utf16_buffer_size) {
			utf16_output_buffer = _reencoder_utf16_grow_buffer(utf16_output_buffer, &utf16_buffer_size, 0);
			if (utf16_output_buffer == NULL) {
				return NULL;
			}
		}

		unsigned int units_read = 0;
		uint32_t code_point = _reencoder_utf8_decode_to_code_point(ptr_read, &units_read);
		ptr_read += units_read;

		unsigned int units_written = _reencoder_utf16_encode_from_code_point(utf16_output_buffer, utf16_index, code_point);
		utf16_index += units_written;
	}

	// grow buffer if out of space (for null-terminator)
	if ((utf16_index * sizeof(uint16_t)) + sizeof(uint16_t) > utf16_buffer_size) {
		utf16_output_buffer = _reencoder_utf16_grow_buffer(utf16_output_buffer, &utf16_buffer_size, 1);
		if (utf16_output_buffer == NULL) {
			return NULL;
		}
	}

	// null-terminate utf-16 output
	utf16_output_buffer[utf16_index] = 0x0000;

	// create utf-16 struct
	ReencoderUnicodeStruct* struct_utf16_str = reencoder_utf16_parse_uint16(utf16_output_buffer, target_endian);

	// clean up other allocated memory
	free(utf16_output_buffer);

	return struct_utf16_str;
}

ReencoderUnicodeStruct* reencoder_utf16_parse_from_utf32(const uint32_t* string, enum ReencoderEncodeType target_endian) {
	// REENCODER_UTF_16 USER FUNCTION DEFINITION

	if (target_endian != UTF_16BE && target_endian != UTF_16LE) {
		return NULL;
	}

	size_t string_length = _reencoder_utf32_strlen(string);
	size_t string_size_bytes = string_length * sizeof(uint32_t);

	// check if utf-32 is valid, if not, return a utf-32 struct
	unsigned int input_utf32_validity = _reencoder_utf32_seq_is_valid(string, string_length);
	if (input_utf32_validity != REENCODER_UTF32_VALID) {
		return _reencoder_unicode_struct_express_populate(
			_reencoder_is_system_little_endian() ? UTF_32LE : UTF_32BE, (const void*)string, string_size_bytes, input_utf32_validity, 0
		);
	}

	// begin conversion utf-32 -> utf-16
	size_t utf16_index = 0;
	size_t utf16_buffer_size = 0;
	uint16_t* utf16_output_buffer = NULL;

	// assumes utf32 is well-formed, since we already checked earlier
	const uint32_t* ptr_read = string;
	while (*ptr_read) {
		// grow buffer if uninitialised or out of space
		if ((utf16_index + 2) * sizeof(uint16_t) > utf16_buffer_size) {
			utf16_output_buffer = _reencoder_utf16_grow_buffer(utf16_output_buffer, &utf16_buffer_size, 0);
			if (utf16_output_buffer == NULL) {
				return NULL;
			}
		}

		unsigned int units_read = 0;
		uint32_t code_point = _reencoder_utf32_decode_to_code_point(ptr_read, &units_read);
		ptr_read += units_read;

		unsigned int units_written = _reencoder_utf16_encode_from_code_point(utf16_output_buffer, utf16_index, code_point);
		utf16_index += units_written;
	}

	// grow buffer if out of space (for null-terminator)
	if ((utf16_index * sizeof(uint16_t)) + sizeof(uint16_t) > utf16_buffer_size) {
		utf16_output_buffer = _reencoder_utf16_grow_buffer(utf16_output_buffer, &utf16_buffer_size, 1);
		if (utf16_output_buffer == NULL) {
			return NULL;
		}
	}

	// null-terminate utf-16 output
	utf16_output_buffer[utf16_index] = 0x0000;

	// create utf-16 struct
	ReencoderUnicodeStruct* struct_utf16_str = reencoder_utf16_parse_uint16(utf16_output_buffer, target_endian);

	// clean up other allocated memory
	free(utf16_output_buffer);

	return struct_utf16_str;
}

unsigned int _reencoder_utf16_seq_is_valid(const uint16_t* string, size_t length) {
	// REENCODER_UTF_16 INTERNAL FUNCTION DEFINITION
	// ALSO DECLARED AS EXTERN IN reencoder_utf_common.h

	for (size_t i = 0; i < length; i++) {
		uint16_t code_unit_1 = string[i];
		unsigned int is_potential_surrogate_pair = !_reencoder_utf16_validity_check_1_is_not_surrogate(code_unit_1);
		uint16_t code_unit_2 = is_potential_surrogate_pair && i + 1 < length ? string[i + 1] : 0x0000;

		unsigned int return_code = _reencoder_utf16_char_is_valid(code_unit_1, code_unit_2, is_potential_surrogate_pair);
		if (return_code != REENCODER_UTF16_VALID) {
			return return_code;
		}

		// skip the low surrogate for next comparison (valid pair)
		if (is_potential_surrogate_pair) {
			i++;
		}
	}

	return REENCODER_UTF16_VALID;
}

size_t _reencoder_utf16_strlen(const uint16_t* string) {
	// REENCODER_UTF_16 INTERNAL FUNCTION DEFINITION
	// ALSO DECLARED AS EXTERN IN reencoder_utf_common.h

	const uint16_t* ptr_start = string;
	const uint16_t* ptr_end = ptr_start;

	while (*ptr_end != 0x0000) {
		ptr_end++;
	}

	return ptr_end - ptr_start;
}

void _reencoder_utf16_uint16_from_uint8(uint16_t* dest, const uint8_t* src, size_t bytes, enum ReencoderEncodeType source_endian) {
	// REENCODER_UTF_16 INTERNAL FUNCTION DEFINITION

	if (source_endian != UTF_16BE && source_endian != UTF_16LE) {
		return;
	}

	size_t bytes_adjusted = bytes + (bytes % sizeof(uint16_t));
	size_t code_units = bytes_adjusted / sizeof(uint16_t);

	if (_reencoder_is_system_little_endian() == (source_endian == UTF_16LE)) {
		memcpy(dest, src, bytes);
	}
	else {
		for (size_t i = 0; i < code_units - 1; i++) {
			uint8_t high = src[i * sizeof(uint16_t)]; // MSB in BE, LSB in LE
			uint8_t low = src[(i * sizeof(uint16_t)) + 1]; // LSB in BE, MSB in LE

			dest[i] = (uint16_t)((high << 8) | low);   // swap bytes
		}

		// last unit is well-formed here
		if (bytes == bytes_adjusted) {
			uint8_t high = src[(code_units - 1) * sizeof(uint16_t)]; // MSB in BE, LSB in LE
			uint8_t low = src[((code_units - 1) * sizeof(uint16_t)) + 1]; // LSB in BE, MSB in LE

			dest[code_units - 1] = (uint16_t)((high << 8) | low);   // swap bytes
		}
	}

	// if odd number of bytes, promote the lone byte to the last code unit with msb padding
	if (bytes != bytes_adjusted) {
		uint8_t high = 0x00;
		uint8_t low = src[bytes - 1];

		dest[code_units - 1] = (uint16_t)((high << 8) | low);
	}

	dest[code_units] = 0x0000;
}

uint32_t _reencoder_utf16_decode_to_code_point(const uint16_t* ptr, unsigned int* char_units) {
	// REENCODER_UTF_16 INTERNAL FUNCTION DEFINITION
	// ALSO DECLARED AS EXTERN IN reencoder_utf_common.h

	uint16_t first = ptr[0];

	// (INVALID) low surrogate without a leading high surrogate
	if (first >= 0xDC00 && first <= 0xDFFF) {
		*char_units = 1;
		return _REENCODER_UNICODE_REPLACEMENT_CHARACTER;
	}

	// single-unit BMP character (not a surrogate)
	if (first < 0xD800 || first > 0xDFFF) {
		*char_units = 1;
		return (uint32_t)first;
	}

	// high surrogate (must be followed by a low surrogate)
	uint16_t second = ptr[1];
	if (second >= 0xDC00 && second <= 0xDFFF) {
		*char_units = 2;
		uint32_t high = first - 0xD800;
		uint32_t low = second - 0xDC00;
		return 0x10000 + ((high << 10) | low);
	}

	// (INVALID) high surrogate without a following low surrogate
	*char_units = 1;
	return _REENCODER_UNICODE_REPLACEMENT_CHARACTER;
}

unsigned int _reencoder_utf16_encode_from_code_point(uint16_t* buffer, size_t index, uint32_t code_point) {
	// REENCODER_UTF_16 INTERNAL FUNCTION DEFINITION
	// ALSO DECLARED AS EXTERN IN reencoder_utf_common.h

	if (!_reencoder_code_point_is_valid(code_point)) {
		buffer[index] = _REENCODER_UTF16_REPLACEMENT_CHARACTER;
		return 1;
	}

	if (code_point <= 0xFFFF) {
		buffer[index + 0] = (uint16_t)code_point;
		return 1;
	}
	else {
		// convert to surrogate pair
		// https://en.wikipedia.org/wiki/UTF-16#Code_points_from_U+010000_to_U+10FFFF
		code_point -= 0x10000;
		buffer[index + 0] = 0xD800 | (code_point >> 10);
		buffer[index + 1] = 0xDC00 | (code_point & 0x3FF);
		return 2;
	}
}

void _reencoder_utf16_write_buffer_swap_endian(uint8_t* dest, const uint16_t* src, size_t length) {
	// REENCODER_UTF_16 INTERNAL FUNCTION DEFINITION
	// ALSO DECLARED AS EXTERN IN reencoder_utf_common.h

	for (size_t i = 0; i < length; i++) {
		uint16_t val = src[i];
		val = (val >> 8) | (val << 8); // swap bytes
		memcpy(dest + (2 * i), &val, sizeof(uint16_t));
	}
}

static inline uint16_t* _reencoder_utf16_grow_buffer(uint16_t* buffer, size_t* current_buffer_size, unsigned int allocate_only_one_unit) {
	// REENCODER_UTF_16 STATIC FUNCTION DEFINITION

	return (uint16_t*)_reencoder_grow_buffer(buffer, current_buffer_size, allocate_only_one_unit, sizeof(uint16_t));
}

static size_t reencoder_utf16_determine_num_chars(const uint16_t* string) {
	// REENCODER_UTF_16 STATIC FUNCTION DEFINITION

	size_t examined_index = 0;
	size_t num_utf16_chars = 0;

	while (string[examined_index] != 0x0000) {
		unsigned int is_surrogate_half = !_reencoder_utf16_validity_check_1_is_not_surrogate(string[examined_index]);

		if (is_surrogate_half) {
			num_utf16_chars++;
			examined_index += 2;
		}
		else {
			num_utf16_chars++;
			examined_index += 1;

		}
	}

	return num_utf16_chars;
}

static inline unsigned int _reencoder_utf16_char_is_valid(uint32_t code_unit_1, uint32_t code_unit_2, unsigned int potential_surrogate_pair) {
	// REENCODER_UTF_16 STATIC FUNCTION DEFINITION

	if (_reencoder_utf16_validity_check_1_is_not_surrogate(code_unit_1)) {
		return REENCODER_UTF16_VALID;
	}
	if (!_reencoder_utf16_validity_check_2_is_high_surrogate(code_unit_1)) {
		return REENCODER_UTF16_ERR_UNPAIRED_LOW;
	}
	if (potential_surrogate_pair && !_reencoder_utf16_validity_check_3_is_low_surrogate(code_unit_2)) {
		return REENCODER_UTF16_ERR_UNPAIRED_HIGH;
	}

	return REENCODER_UTF16_VALID;
}

static inline unsigned int _reencoder_utf16_validity_check_1_is_not_surrogate(uint32_t code_unit) {
	// REENCODER_UTF_16 STATIC FUNCTION DEFINITION

	return code_unit < 0xD800 || code_unit > 0xDFFF; // valid BMP characters (0x0000-0xD7FF, 0xE000-0xFFFF)
}

static inline unsigned int _reencoder_utf16_validity_check_2_is_high_surrogate(uint32_t code_unit) {
	// REENCODER_UTF_16 STATIC FUNCTION DEFINITION

	return code_unit >= 0xD800 && code_unit <= 0xDBFF; // high surrogates (0xD800-0xDBFF)
}

static inline unsigned int _reencoder_utf16_validity_check_3_is_low_surrogate(uint32_t code_unit) {
	// REENCODER_UTF_16 STATIC FUNCTION DEFINITION

	return code_unit >= 0xDC00 && code_unit <= 0xDFFF; // low surrogates (0xDC00-0xDFFF)
}
