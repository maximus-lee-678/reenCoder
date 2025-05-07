#include "../headers/reencoder_utf_8.h"

static const uint8_t _REENCODER_UTF8_REPLACEMENT_CHARACTER[] = { 0xEF, 0xBF, 0xBD };

static inline uint8_t* _reencoder_utf8_grow_buffer(uint8_t* buffer, size_t* current_buffer_size, unsigned int allocate_only_one_unit);

/**
 * @brief Determines the number of UTF-8 characters, not bytes in a string.
 *
 * @param[in] string UTF-8 string to be checked. Should be represented as an array of uint8_t.
 *
 * @return Number of UTF-8 characters in the string.
 */
static size_t reencoder_utf8_determine_num_chars(const uint8_t* string);

static inline unsigned int _reencoder_utf8_char_is_valid(uint8_t code_units[4], unsigned int num_units);
static inline unsigned int _reencoder_utf8_validity_check_1_is_expected_length(uint8_t code_units[4], unsigned int num_units);
static inline unsigned int _reencoder_utf8_validity_check_2_has_valid_continuation_bytes(uint8_t code_units[4], unsigned int num_units);
static inline unsigned int _reencoder_utf8_validity_check_3_is_not_overlong(uint8_t code_units[4], unsigned int num_units);
static inline unsigned int _reencoder_utf8_validity_check_4_is_in_unicode_range(uint8_t code_units[4], unsigned int num_units);
static inline unsigned int _reencoder_utf8_validity_check_5_is_not_surrogate(uint8_t code_units[4], unsigned int num_units);

// ##### //
// https://datatracker.ietf.org/doc/html/rfc3629
// ##### //

ReencoderUnicodeStruct* reencoder_utf8_parse(const uint8_t* string) {
	// REENCODER_UTF_8 USER FUNCTION DEFINITION

	// i've heard NULLs should be supported, i.e. don't use strlen
	// the lowest of low priorities would be to implement length which standardises with utf-16 and utf-32
	// i'll be back, hopefully

	ReencoderUnicodeStruct* struct_utf8_str = _reencoder_unicode_struct_express_populate(
		UTF_8, (const void*)string, strlen(string), _reencoder_utf8_seq_is_valid(string), reencoder_utf8_determine_num_chars(string)
	);

	return struct_utf8_str;
}

ReencoderUnicodeStruct* reencoder_utf8_parse_from_utf16(const uint16_t* string) {
	// REENCODER_UTF_8 USER FUNCTION DEFINITION

	size_t string_length = _reencoder_utf16_strlen(string);
	size_t string_size_bytes = string_length * sizeof(uint16_t);

	// check if utf-16 is valid, if not, return a utf-16 struct
	unsigned int input_utf16_validity = _reencoder_utf16_seq_is_valid(string, string_length);
	if (input_utf16_validity != REENCODER_UTF16_VALID) {
		return _reencoder_unicode_struct_express_populate(
			reencoder_is_system_little_endian() ? UTF_16LE : UTF_16BE, (const void*)string, string_size_bytes, input_utf16_validity, 0
		);
	}

	// begin conversion utf-16 -> utf-8
	size_t utf8_index = 0;
	size_t utf8_buffer_size = 0;
	uint8_t* utf8_output_buffer = NULL;

	// assumes utf-16 is well-formed, since we already checked earlier
	const uint16_t* ptr_read = string;
	while (*ptr_read) {
		// grow buffer if uninitialised or out of space
		if ((utf8_index + 4) * sizeof(uint8_t) > utf8_buffer_size) {
			utf8_output_buffer = _reencoder_utf8_grow_buffer(utf8_output_buffer, &utf8_buffer_size, 0);
			if (utf8_output_buffer == NULL) {
				return NULL;
			}
		}

		unsigned int units_read = 0;
		uint32_t code_point = _reencoder_utf16_decode_to_code_point(ptr_read, &units_read);
		ptr_read += units_read;

		unsigned int units_written = _reencoder_utf8_encode_from_code_point(utf8_output_buffer, utf8_index, code_point);
		utf8_index += units_written;
	}

	// grow buffer if out of space (for null-terminator)
	if (utf8_index + sizeof(uint8_t) > utf8_buffer_size) {
		utf8_output_buffer = _reencoder_utf8_grow_buffer(utf8_output_buffer, &utf8_buffer_size, 1);
		if (utf8_output_buffer == NULL) {
			return NULL;
		}
	}

	// null-terminate utf-8 output
	utf8_output_buffer[utf8_index] = '\0';

	// create utf-8 struct
	ReencoderUnicodeStruct* struct_utf8_str = reencoder_utf8_parse(utf8_output_buffer);

	// clean up other allocated memory
	free(utf8_output_buffer);

	return struct_utf8_str;
}

ReencoderUnicodeStruct* reencoder_utf8_parse_from_utf32(const uint32_t* string) {
	// REENCODER_UTF_8 USER FUNCTION DEFINITION

	size_t string_length = _reencoder_utf32_strlen(string);
	size_t string_size_bytes = string_length * sizeof(uint32_t);

	// check if utf-32 is valid, if not, return a utf-32 struct
	unsigned int input_utf32_validity = _reencoder_utf32_seq_is_valid(string, string_length);
	if (input_utf32_validity != REENCODER_UTF32_VALID) {
		return _reencoder_unicode_struct_express_populate(
			reencoder_is_system_little_endian() ? UTF_32LE : UTF_32BE, (const void*)string, string_size_bytes, input_utf32_validity, 0
		);
	}

	// begin conversion utf-16 -> utf-32
	size_t utf8_index = 0;
	size_t utf8_buffer_size = 0;
	uint8_t* utf8_output_buffer = NULL;

	// assumes utf-32 is well-formed, since we already checked earlier
	const uint32_t* ptr_read = string;
	while (*ptr_read) {
		// grow buffer if uninitialised or out of space
		if ((utf8_index + 4) * sizeof(uint8_t) > utf8_buffer_size) {
			utf8_output_buffer = _reencoder_utf8_grow_buffer(utf8_output_buffer, &utf8_buffer_size, 0);
			if (utf8_output_buffer == NULL) {
				return NULL;
			}
		}

		unsigned int units_read = 0;
		uint32_t code_point = _reencoder_utf32_decode_to_code_point(ptr_read, &units_read);
		ptr_read += units_read;

		unsigned int units_written = _reencoder_utf8_encode_from_code_point(utf8_output_buffer, utf8_index, code_point);
		utf8_index += units_written;
	}


	// grow buffer if out of space (for null-terminator)
	if (utf8_index + sizeof(uint8_t) > utf8_buffer_size) {
		utf8_output_buffer = _reencoder_utf8_grow_buffer(utf8_output_buffer, &utf8_buffer_size, 1);
		if (utf8_output_buffer == NULL) {
			return NULL;
		}
	}

	// null-terminate utf-8 output
	utf8_output_buffer[utf8_index] = '\0';

	// create utf-8 struct
	ReencoderUnicodeStruct* struct_utf8_str = reencoder_utf8_parse(utf8_output_buffer);

	// clean up other allocated memory
	free(utf8_output_buffer);

	return struct_utf8_str;
}

uint8_t reencoder_utf8_contains_multibyte(const uint8_t* string) {
	// REENCODER_UTF_8 USER FUNCTION DEFINITION

	size_t examined_index = 0;
	while (string[examined_index] != '\0') {
		unsigned int current_utf8_length = _reencoder_utf8_determine_length_from_first_byte(string[examined_index]);
		switch (current_utf8_length) {
		case 0:
			return 0;
		case 1:
			examined_index++;
			break;
		default:
			return 1;
		}
	}

	return 1;
}

unsigned int _reencoder_utf8_seq_is_valid(const uint8_t* string) {
	// REENCODER_UTF_8 INTERNAL FUNCTION DEFINITION
	// ALSO DECLARED AS EXTERN IN reencoder_utf_common.h

	size_t examined_index = 0;

	while (string[examined_index] != 0x00) {
		unsigned int utf8_char_len = _reencoder_utf8_determine_length_from_first_byte(string[examined_index]);

		uint8_t char_bytes[4] = {
			utf8_char_len > 0 ? string[examined_index] : 0x00,
			utf8_char_len > 1 ? string[examined_index + 1] : 0x00,
			utf8_char_len > 2 ? string[examined_index + 2] : 0x00,
			utf8_char_len > 3 ? string[examined_index + 3] : 0x00
		};

		unsigned int return_code = _reencoder_utf8_char_is_valid(char_bytes, utf8_char_len);
		if (return_code != REENCODER_UTF8_VALID) {
			return return_code;
		}

		examined_index += utf8_char_len;
	}

	return REENCODER_UTF8_VALID;
}

unsigned int _reencoder_utf8_determine_length_from_first_byte(uint8_t first_byte) {
	// REENCODER_UTF_8 INTERNAL FUNCTION DEFINITION

	if ((first_byte & 0b10000000) == 0b00000000) // 0xxxxxxx ~ 1-byte ASCII
		return 1;
	else if ((first_byte & 0b11100000) == 0b11000000) // 110xxxxx ~ 2-byte sequence
		return 2;
	else if ((first_byte & 0b11110000) == 0b11100000) // 1110xxxx ~ 3-byte sequence
		return 3;
	else if ((first_byte & 0b11111000) == 0b11110000) // 11110xxx ~ 4-byte sequence
		return 4;
	else
		return 0; // invalid leading byte
}

uint32_t _reencoder_utf8_decode_to_code_point(const uint8_t* ptr, unsigned int* units_read) {
	// REENCODER_UTF_8 INTERNAL FUNCTION DEFINITION
	// ALSO DECLARED AS EXTERN IN reencoder_utf_common.h

	uint32_t code_point = 0;
	unsigned int byte_length = _reencoder_utf8_determine_length_from_first_byte(*ptr);

	*units_read = byte_length;

	switch (byte_length) {
	case 1:
		code_point = *ptr;
		break;
	case 2:
		// 110xxxyy 10yyzzzz
		code_point =
			((ptr[0] & 0b00011111) << 6) |
			(ptr[1] & 0b00111111);
		break;
	case 3:
		// 1110wwww 10xxxxyy 10yyzzzz
		code_point =
			((ptr[0] & 0b00001111) << 12) |
			((ptr[1] & 0b00111111) << 6) |
			(ptr[2] & 0b00111111);
		break;
	case 4:
		// 11110uvv 10vvwwww 10xxxxyy 10yyzzzz
		code_point =
			((ptr[0] & 0b00000111) << 18) |
			((ptr[1] & 0b00111111) << 12) |
			((ptr[2] & 0b00111111) << 6) |
			(ptr[3] & 0b00111111);
		break;
	default:
		// (INVALID) invalid byte length
		*units_read = 1;
		code_point = _REENCODER_UNICODE_REPLACEMENT_CHARACTER;
		break;
	}

	return code_point;
}

unsigned int _reencoder_utf8_encode_from_code_point(uint8_t* buffer, size_t index, uint32_t code_point) {
	// REENCODER_UTF_8 INTERNAL FUNCTION DEFINITION
	// ALSO DECLARED AS EXTERN IN reencoder_utf_common.h

	// https://en.wikipedia.org/wiki/UTF-8#Description

	if (!_reencoder_code_point_is_valid(code_point)) {
		memcpy(&buffer[index], _REENCODER_UTF8_REPLACEMENT_CHARACTER, sizeof(_REENCODER_UTF8_REPLACEMENT_CHARACTER));
		return sizeof(_REENCODER_UTF8_REPLACEMENT_CHARACTER);
	}

	if (code_point <= 0x7F) {
		// 1-byte sequence: 0xxxxxxx
		buffer[index + 0] = code_point;
		return 1;
	}
	else if (code_point <= 0x7FF) {
		// 2-byte sequence: 110xxxxx 10xxxxxx
		buffer[index + 0] = 0b11000000 | ((code_point >> 6) & 0b00011111);
		buffer[index + 1] = 0b10000000 | (code_point & 0b00111111);
		return 2;
	}
	else if (code_point <= 0xFFFF) {
		// 3-byte sequence: 1110xxxx 10xxxxxx 10xxxxxx
		buffer[index + 0] = 0b11100000 | ((code_point >> 12) & 0b00001111);
		buffer[index + 1] = 0b10000000 | ((code_point >> 6) & 0b00111111);
		buffer[index + 2] = 0b10000000 | (code_point & 0b00111111);
		return 3;
	}
	else if (code_point <= 0x10FFFF) {
		// 4-byte sequence: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
		buffer[index + 0] = 0b11110000 | ((code_point >> 18) & 0b00000111);
		buffer[index + 1] = 0b10000000 | ((code_point >> 12) & 0b00111111);
		buffer[index + 2] = 0b10000000 | ((code_point >> 6) & 0b00111111);
		buffer[index + 3] = 0b10000000 | (code_point & 0b00111111);
		return 4;
	}
	else {
		// (INVALID) units written was 0 somehow
		memcpy(&buffer[index], _REENCODER_UTF8_REPLACEMENT_CHARACTER, sizeof(_REENCODER_UTF8_REPLACEMENT_CHARACTER));
		return sizeof(_REENCODER_UTF8_REPLACEMENT_CHARACTER);
	}
}

static inline uint8_t* _reencoder_utf8_grow_buffer(uint8_t* buffer, size_t* current_buffer_size, unsigned int allocate_only_one_unit) {
	// REENCODER_UTF_8 STATIC FUNCTION DEFINITION

	return (uint8_t*)_reencoder_grow_buffer(buffer, current_buffer_size, allocate_only_one_unit, sizeof(uint8_t));
}

static inline unsigned int _reencoder_utf8_char_is_valid(uint8_t code_units[4], unsigned int num_units) {
	// REENCODER_UTF_8 STATIC FUNCTION DEFINITION

	if (num_units == 0) {
		return REENCODER_UTF8_ERR_INVALID_LEAD;
	}
	if (num_units == 1) {
		return REENCODER_UTF8_VALID;
	}
	if (!_reencoder_utf8_validity_check_1_is_expected_length(code_units, num_units)) {
		return REENCODER_UTF8_ERR_PREMATURE_END;
	}
	if (!_reencoder_utf8_validity_check_2_has_valid_continuation_bytes(code_units, num_units)) {
		return REENCODER_UTF8_ERR_INVALID_CONT;
	}
	if (!_reencoder_utf8_validity_check_3_is_not_overlong(code_units, num_units)) {
		switch (num_units) {
		case 2:
			return REENCODER_UTF8_ERR_OVERLONG_2BYTE;
		case 3:
			return REENCODER_UTF8_ERR_OVERLONG_3BYTE;
		case 4:
			return REENCODER_UTF8_ERR_OVERLONG_4BYTE;
		}
	}
	if (!_reencoder_utf8_validity_check_4_is_in_unicode_range(code_units, num_units)) {
		return REENCODER_UTF8_ERR_OUT_OF_RANGE;
	}
	if (!_reencoder_utf8_validity_check_5_is_not_surrogate(code_units, num_units)) {
		return REENCODER_UTF8_ERR_SURROGATE_PAIR;
	}

	return REENCODER_UTF8_VALID;
}

static inline unsigned int _reencoder_utf8_validity_check_1_is_expected_length(uint8_t code_units[4], unsigned int num_units) {
	// REENCODER_UTF_8 STATIC FUNCTION DEFINITION

	for (unsigned int i = 1; i < num_units; i++) {
		if (code_units[i] == 0x00) {
			return 0;
		}
	}

	return 1;
}

static inline unsigned int _reencoder_utf8_validity_check_2_has_valid_continuation_bytes(uint8_t code_units[4], unsigned int num_units) {
	// REENCODER_UTF_8 STATIC FUNCTION DEFINITION

	for (unsigned int i = 1; i < num_units; i++) {
		if ((code_units[i] & 0b11000000) != 0b10000000) {
			return 0;
		}
	}

	return 1;
}

static inline unsigned int _reencoder_utf8_validity_check_3_is_not_overlong(uint8_t code_units[4], unsigned int num_units) {
	// REENCODER_UTF_8 STATIC FUNCTION DEFINITION

	// overlong encoding check for 2-byte long characters
	// the first byte of a valid 2-byte UTF-8 sequence must be at least 0xC2.
	// 0xC0 (0b11000000) and 0xC1 (0b11000001) are invalid because they would
	// be encoding values that could fit in a single byte (ASCII range 0x00-0x7F).
	// Example of overlong encodings:
	// [NUL] (U+0000) > 0b11000000 0b10000000 (0xC0 0x80) should be 0b00000000 (0x00)
	// [@]   (U+0040) > 0b11000001 0b10000000 (0xC1 0x80) should be 0b01000000 (0x40)
	// USABLE BITS >         xxxx-     ------                          -------
	if (num_units == 2 && code_units[0] < 0b11000010) {
		return 0;
	}

	// overlong encoding check for 3-byte long characters
	// since 2-byte long characters contain up to 11 bits (0x07FF) of info, any value smaller than that is overlong.
	// which results in anything smaller than 0b11100000 0b10100000 0b10000000 being invalid.
	// USABLE BITS >                                xxxx     x-----     ------
	if (num_units == 3 && code_units[0] == 0b11100000 && code_units[1] < 0b10100000) {
		return 0;
	}

	// overlong encoding check for 4-byte long characters
	// since 3-byte long characters contain up to 16 bits of info, any value smaller than that is overlong.
	// which results in anything smaller than 0b11110000 0b10010000 0b10000000 0b10000000 being invalid.
	// USABLE BITS >                                 xxx     xx----     ------     ------
	if (num_units == 4 && code_units[0] == 0b11110000 && code_units[1] < 0b10010000) {
		return 0;
	}

	return 1;
}

static inline unsigned int _reencoder_utf8_validity_check_4_is_in_unicode_range(uint8_t code_units[4], unsigned int num_units) {
	// REENCODER_UTF_8 STATIC FUNCTION DEFINITION

	// valid unicode range: U+0000 to U+10FFFF
	// U+10FFFF equates to 0001 0000 1111 1111 1111 1111
	// represented in unicode as 0b11110100 0b10001111 0b10111111 0b10111111 (4-byte character)
	if (num_units == 4 && (code_units[0] > 0b11110100 || (code_units[0] == 0b11110100 && code_units[1] > 0b10001111))) {
		return 0;
	}

	return 1;
}

static inline unsigned int _reencoder_utf8_validity_check_5_is_not_surrogate(uint8_t code_units[4], unsigned int num_units) {
	// REENCODER_UTF_8 STATIC FUNCTION DEFINITION

	// surrogate pairs check: U+D800 to U+DFFF
	// equates to 0b11101101 0b10100000 0b10000000 to 
	// ---------- 0b11101101 0b10111111 0b10111111
	if (num_units == 3 && code_units[0] == 0b11101101 && (code_units[1] & 0b11100000) == 0b10100000) {
		return 0;
	}
	return 1;
}

static size_t reencoder_utf8_determine_num_chars(const uint8_t* string) {
	// REENCODER_UTF_8 STATIC FUNCTION DEFINITION

	size_t examined_index = 0;
	size_t num_utf8_chars = 0;

	while (string[examined_index] != 0x00) {
		unsigned int utf8_char_len = _reencoder_utf8_determine_length_from_first_byte(string[examined_index]);
		if (utf8_char_len == 0) {
			return 0;
		}
		examined_index += utf8_char_len;
		num_utf8_chars++;
	}

	return num_utf8_chars;
}
