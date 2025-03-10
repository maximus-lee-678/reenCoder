#include "../headers/reencoder_utf_8.h"

/**
 * @brief Checks if a provided UTF-8 string is valid.
 *
 * Checks for surrogate presence, overlong encoding, invalid bytes, and premature string endings.
 *
 * @param[in] string UTF-16 string to be checked. Should be represented as an array of uint8_t.
 *
 * @return Unsigned integer representing the outcome of the check. Corresponds to index in `REENCODER_UTF8_OUTCOME_ARR`.
 */
static unsigned int reencoder_utf8_is_valid(const unsigned char* string);

/**
 * @brief Given a single UTF-8 starting byte, determines how many bytes this character is.
 *
 * @param[in] first_byte UTF-8 starting byte.
 *
 * @return Unsigned integer 1-4.
 * @retval 0 If leading byte is invalid.
 */
static unsigned int reencoder_utf8_determine_length_from_first_byte(unsigned char first_byte);

/**
 * @brief Determines the number of UTF-8 characters, not bytes in a string.
 *
 * @param[in] string UTF-8 string to be checked. Should be represented as an array of uint8_t.
 *
 * @return Number of UTF-8 characters in the string.
 */
static size_t reencoder_utf8_determine_num_chars(const unsigned char* string);


ReencoderUnicodeStruct* reencoder_utf8_parse(const unsigned char* string) {
	ReencoderUnicodeStruct* struct_utf8_str = _reencoder_unicode_struct_init(UTF8);
	if (struct_utf8_str == NULL) {
		return NULL;
	}

	size_t string_length = strlen(string);
	struct_utf8_str->string_buffer = (uint8_t*)malloc(string_length + 1);
	if (struct_utf8_str->string_buffer == NULL) {
		reencoder_unicode_struct_free(struct_utf8_str);
		return NULL;
	}

	memcpy(struct_utf8_str->string_buffer, string, string_length);
	struct_utf8_str->string_buffer[string_length] = '\0';

	struct_utf8_str->string_validity = reencoder_utf8_is_valid(string);
	if (struct_utf8_str->string_validity != REENCODER_UTF8_VALID) {
		return struct_utf8_str;
	}

	struct_utf8_str->num_chars = reencoder_utf8_determine_num_chars(string);
	struct_utf8_str->num_bytes = string_length;

	return struct_utf8_str;
}

const char* reencoder_utf8_outcome_as_str(unsigned int outcome) {
	if (outcome >= (sizeof(REENCODER_UTF8_OUTCOME_ARR) / sizeof(REENCODER_UTF8_OUTCOME_ARR[0]))) {
		return NULL;
	}

	return REENCODER_UTF8_OUTCOME_ARR[outcome];
}

uint8_t reencoder_utf8_contains_multibyte(const unsigned char* string) {
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

static unsigned int reencoder_utf8_is_valid(const unsigned char* string) {
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

static unsigned int reencoder_utf8_determine_length_from_first_byte(unsigned char first_byte) {
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

static size_t reencoder_utf8_determine_num_chars(const unsigned char* string) {
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
