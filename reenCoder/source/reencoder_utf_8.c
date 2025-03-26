#include "../headers/reencoder_utf_8.h"

/**
 * @brief Given a single UTF-8 starting byte, determines how many bytes this character is.
 *
 * @param[in] first_byte UTF-8 starting byte.
 *
 * @return Unsigned integer 1-4.
 * @retval 0 If leading byte is invalid.
 */
static unsigned int reencoder_utf8_determine_length_from_first_byte(uint8_t first_byte);

/**
 * @brief Determines the number of UTF-8 characters, not bytes in a string.
 *
 * @param[in] string UTF-8 string to be checked. Should be represented as an array of uint8_t.
 *
 * @return Number of UTF-8 characters in the string.
 */
static size_t reencoder_utf8_determine_num_chars(const uint8_t* string);


ReencoderUnicodeStruct* reencoder_utf8_parse(const uint8_t* string) {
	size_t string_length = strlen(string);

	ReencoderUnicodeStruct* struct_utf8_str = _reencoder_unicode_struct_express_populate(
		UTF_8,
		(const void*) string,
		string_length,
		_reencoder_utf8_is_valid(string),
		reencoder_utf8_determine_num_chars(string)
	);

	return struct_utf8_str;
}

ReencoderUnicodeStruct* reencoder_utf8_parse_from_utf16(const uint16_t* string) {
	size_t string_length = _reencoder_strlen_utf16(string);
	size_t string_size_bytes = string_length * 2;

	// check if utf16 is valid
	unsigned int input_utf16_validity = _reencoder_utf16_is_valid(string, _reencoder_strlen_utf16(string));

	// invalid utf16
	if (input_utf16_validity != REENCODER_UTF16_VALID) {
		ReencoderUnicodeStruct* struct_utf16_str = _reencoder_unicode_struct_express_populate(
			_reencoder_is_system_little_endian() ? UTF_16LE : UTF_16BE,
			(const void*) string,
			string_size_bytes,
			input_utf16_validity,
			0
		);

		return struct_utf16_str;
	}

	// begin conversion utf-16 -> utf-8
	size_t utf8_index = 0;
	size_t utf8_buffer_size = _REENCODER_BASE_STRING_BYTE_SIZE;
	uint8_t* utf8_output_buffer = (uint8_t*)malloc(utf8_buffer_size);
	if (utf8_output_buffer == NULL) {
		return NULL;
	}

	// assumes utf-16 is well-formed, since we already checked earlier
	for (size_t i = 0; i < string_length; i++) {
		uint16_t code_unit = string[i];

		// grow buffer if out of space
		if (utf8_index + 4 > utf8_buffer_size) {
			uint8_t* temp_ptr_to_utf8_buffer = utf8_output_buffer;
			size_t new_size = utf8_buffer_size * _REENCODER_BASE_STRING_GROW_RATE;
			utf8_output_buffer = (uint8_t*)realloc(utf8_output_buffer, new_size);
			if (utf8_output_buffer == NULL) {
				free(temp_ptr_to_utf8_buffer);
				return NULL;
			}
			utf8_buffer_size = new_size;
		}

		// check for high surrogate (U+D800 to U+DBFF)
		if (code_unit >= 0xD800 && code_unit <= 0xDBFF) {
			uint16_t low_surrogate = string[i + 1];

			// convert surrogate pair to full unicode code point
			uint32_t code_point = 0x10000 + (((code_unit - 0xD800) << 10) | (low_surrogate - 0xDC00));

			i++; // skip the low surrogate

			// encode as UTF-8 (4 bytes)
			// since 3 byte UTF-8 can only encode up to U+FFFF, and our code point is minimum U+10000,
			// 4 bytes are always used for surrogate pairs
			utf8_output_buffer[utf8_index++] = 0b11110000 | ((code_point >> 18) & 0b00000111); // take bits 18-20
			utf8_output_buffer[utf8_index++] = 0b10000000 | ((code_point >> 12) & 0b00111111); // take bits 12-17
			utf8_output_buffer[utf8_index++] = 0b10000000 | ((code_point >> 6) & 0b00111111); // take bits 6-11
			utf8_output_buffer[utf8_index++] = 0b10000000 | (code_point & 0b00111111); // take bits 0-5
		}
		// BMP characters (U+0000 to U+FFFF, excluding surrogates)
		else {
			if (code_unit < 0b10000000) { // room for 7 bits
				utf8_output_buffer[utf8_index++] = code_unit & 0b01111111;
			}
			else if (code_unit < 0b100000000000) { // room for 11 bits
				utf8_output_buffer[utf8_index++] = 0xC0 | ((code_unit >> 6) & 0b00011111);
				utf8_output_buffer[utf8_index++] = 0x80 | (code_unit & 0b00111111);
			}
			else { // room for 16 bits
				utf8_output_buffer[utf8_index++] = 0xE0 | ((code_unit >> 12) & 0b00001111);
				utf8_output_buffer[utf8_index++] = 0x80 | ((code_unit >> 6) & 0b00111111);
				utf8_output_buffer[utf8_index++] = 0x80 | (code_unit & 0b00111111);
			}
		}
	}

	// grow buffer if out of space (for null-terminator)
	if (utf8_index + 1 > utf8_buffer_size) {
		uint8_t* temp_ptr_to_utf8_buffer = utf8_output_buffer;
		size_t new_size = utf8_buffer_size + 1;
		utf8_output_buffer = (uint8_t*)realloc(utf8_output_buffer, new_size);
		if (utf8_output_buffer == NULL) {
			free(temp_ptr_to_utf8_buffer);
			return NULL;
		}
		utf8_buffer_size = new_size;
	}
	// null-terminate UTF-8 output
	utf8_output_buffer[utf8_index] = '\0';

	// create utf-8 struct
	ReencoderUnicodeStruct* struct_utf8_str = reencoder_utf8_parse(utf8_output_buffer);

	// clean up other allocated memory
	free(utf8_output_buffer);

	return struct_utf8_str;
}

ReencoderUnicodeStruct* reencoder_utf8_parse_from_utf16_uint8(const uint8_t* string, size_t bytes, enum ReencoderEncodeType source_endian) {
	// it's ok to malloc first even if number of bytes is odd,
	// no risk of buffer overflow as logic of copying will skip the last byte
	// odd is incorrect behaviour anyways
	uint16_t* string_uint16 = (uint16_t*)malloc(bytes + sizeof(uint16_t));
	if (string_uint16 == NULL) {
		return NULL;
	}
	_reencoder_utf16_uint16_from_uint8(string_uint16, string, bytes, source_endian);

	// odd number of bytes is impossible for UTF-16
	if (bytes % 2 != 0) {
		ReencoderUnicodeStruct* struct_utf16_str = _reencoder_unicode_struct_express_populate(
			_reencoder_is_system_little_endian() ? UTF_16LE : UTF_16BE,
			(const void*)string_uint16,
			bytes,
			REENCODER_UTF16_ODD_LENGTH,
			0
		);

		return struct_utf16_str;
	}

	ReencoderUnicodeStruct* struct_utf16_str = reencoder_utf8_parse_from_utf16(string_uint16);

	// clean up other allocated memory
	free(string_uint16);

	return struct_utf16_str;
}

const char* reencoder_utf8_outcome_as_str(unsigned int outcome) {
	unsigned int outcome_offset = outcome - _REENCODER_UTF8_PARSE_OFFSET;
	if (outcome_offset >= (sizeof(_REENCODER_UTF8_OUTCOME_ARR) / sizeof(_REENCODER_UTF8_OUTCOME_ARR[0]))) {
		return NULL;
	}

	return _REENCODER_UTF8_OUTCOME_ARR[outcome_offset];
}

uint8_t reencoder_utf8_contains_multibyte(const uint8_t* string) {
	size_t examined_index = 0;
	while (string[examined_index] != '\0') {
		unsigned int current_utf8_length = reencoder_utf8_determine_length_from_first_byte(string[examined_index]);
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

static unsigned int _reencoder_utf8_is_valid(const uint8_t* string) {
	// https://datatracker.ietf.org/doc/html/rfc3629

	size_t examined_index = 0;

	while (string[examined_index] != '\0') {
		unsigned int utf8_char_len = reencoder_utf8_determine_length_from_first_byte(string[examined_index]);
		if (utf8_char_len == 0) {
			return REENCODER_UTF8_ERR_INVALID_LEAD;
		}

		for (unsigned int i = 1; i < utf8_char_len; i++) {
			if (string[examined_index + i] == '\0') {
				return REENCODER_UTF8_ERR_PREMATURE_END;
			}
			if ((string[examined_index + i] & 0b11000000) != 0b10000000) {
				return REENCODER_UTF8_ERR_INVALID_CONT;
			}
		}

		// overlong encoding check for 2-byte long characters
		// this fulfills: the octet values [C0, C1], F5 to FF never appear.
		// the first byte of a valid 2-byte UTF-8 sequence must be at least 0xC2.
		// 0xC0 (0b11000000) and 0xC1 (0b11000001) are invalid because they would
		// be encoding values that could fit in a single byte (ASCII range 0x00-0x7F).
		// Example of overlong encodings:
		// [NUL] (U+0000) > 0b11000000 0b10000000 (0xC0 0x80) should be 0b00000000 (0x00)
		// [@]   (U+0040) > 0b11000001 0b10000000 (0xC1 0x80) should be 0b01000000 (0x40)
		// USABLE BITS >         xxxx-     ------                          -------
		if (utf8_char_len == 2 && string[examined_index] < 0b11000010) {
			return REENCODER_UTF8_ERR_OVERLONG_2BYTE;
		}

		// overlong encoding check for 3-byte long characters
		// since 2-byte long characters contain up to 11 bits (0x07FF) of info, any value smaller than that is overlong.
		// which results in anything smaller than 0b11100000 0b10100000 0b10000000 being invalid.
		//                                              xxxx     x-----     ------
		if (utf8_char_len == 3 && string[examined_index] == 0b11100000 && string[examined_index + 1] < 0b10100000) {
			return REENCODER_UTF8_ERR_OVERLONG_3BYTE;
		}

		// overlong encoding check for 4-byte long characters
		// since 3-byte long characters contain up to 16 bits of info, any value smaller than that is overlong.
		// which results in anything smaller than 0b11110000 0b10010000 0b10000000 0b10000000 being invalid.
		//                                               xxx     xx----     ------     ------
		if (utf8_char_len == 4 && string[examined_index] == 0b11110000 && string[examined_index + 1] < 0b10010000) {
			return REENCODER_UTF8_ERR_OVERLONG_4BYTE;
		}

		// valid unicode range: U+0000 to U+10FFFF
		// this fulfills: the octet values C0, C1, [F5 to FF] never appear.
		// U+10FFFF equates to 0001 0000 1111 1111 1111 1111
		// represented in unicode as 0b11110100 0b10001111 0b10111111 0b10111111 (4-byte character)
		if (utf8_char_len == 4 && string[examined_index] > 0b11110100) {
			return REENCODER_UTF8_ERR_OUT_OF_RANGE;
		}

		// surrogate pairs check: U+D800 to U+DFFF
		// equates to 0b11101101 0b10100000 0b10000000 to 
		// ---------- 0b11101101 0b10111111 0b10111111
		if (utf8_char_len == 3 && string[examined_index] == 0b11101101 && (string[examined_index + 1] & 0b11100000) == 0b10100000) {
			return REENCODER_UTF8_ERR_SURROGATE_PAIR;
		}

		examined_index += utf8_char_len;
	}

	return REENCODER_UTF8_VALID; // valid UTF-8
}

static unsigned int reencoder_utf8_determine_length_from_first_byte(uint8_t first_byte) {
	if ((first_byte & 0b10000000) == 0b00000000) // 0xxxxxxx ~ 1-byte ASCII
		return 1;
	else if ((first_byte & 0x11100000) == 0b11000000) // 110xxxxx ~ 2-byte sequence
		return 2;
	else if ((first_byte & 0b11110000) == 0b11100000) // 1110xxxx ~ 3-byte sequence
		return 3;
	else if ((first_byte & 0b11111000) == 0b11110000) // 11110xxx ~ 4-byte sequence
		return 4;
	else
		return 0; // invalid leading byte
}

static size_t reencoder_utf8_determine_num_chars(const uint8_t* string) {
	size_t examined_index = 0;
	size_t num_utf8_chars = 0;

	while (string[examined_index] != '\0') {
		unsigned int utf8_char_len = reencoder_utf8_determine_length_from_first_byte(string[examined_index]);
		if (utf8_char_len == 0) {
			return 0;
		}
		examined_index += utf8_char_len;
		num_utf8_chars++;
	}

	return num_utf8_chars;
}
