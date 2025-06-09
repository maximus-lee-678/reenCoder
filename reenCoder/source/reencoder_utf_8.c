#include "../headers/reencoder_utf_8.h"

/**
 * @brief Checks a single UTF-8 character for validity. A UTF-8 character is defined as a sequence of 1-4 code unit.
 *
 * @param[in] code_units Array of UTF-8 code units.
 * @param[in] units_expected Number of code units in the array.
 * @param[out] units_actual Number of actual code units. This may be less than units_expected if the sequence prematurely ends or contain invalid continuation bytes. NULL can be passed if this information is not needed.
 *
 * @return REENCODER_UTF8_VALID.
 * @retval REENCODER_UTF8_ERR_INVALID_LEAD if code unit contains an invalid leading byte.
 * @retval REENCODER_UTF8_ERR_PREMATURE_END if the character ended before the sequence completed.
 * @retval REENCODER_UTF8_ERR_INVALID_CONT if the character contains a code unit with an invalid continuation byte.
 * @retval REENCODER_UTF8_ERR_OVERLONG_2BYTE if the character is an overlong encoding for a 2-byte sequence.
 * @retval REENCODER_UTF8_ERR_OVERLONG_3BYTE if the character is an overlong encoding for a 3-byte sequence.
 * @retval REENCODER_UTF8_ERR_OVERLONG_4BYTE if the character is an overlong encoding for a 4-byte sequence.
 * @retval REENCODER_UTF8_ERR_OUT_OF_RANGE if character is out of valid Unicode range (>U+10FFFF).
 * @retval REENCODER_UTF8_ERR_SURROGATE_PAIR if character is a high or low surrogate character (U+D800-U+DFFF).
 */
static inline unsigned int _reencoder_utf8_char_is_valid(uint8_t code_units[4], unsigned int units_expected, unsigned int* units_actual);

/**
 * @brief Checks if a UTF-8 code unit sequence contains the expected number of bytes based on the first byte.
 *
 * @param[in] code_units Array of UTF-8 code units.
 * @param[in] units_expected Number of code units in the array.
 *
 * @return _REENCODER_UTF8_VALIDATION_HAS_VALID_LENGTH if the sequence has the expected number of bytes.
 * @retval number of actual bytes available if the sequence does not have the expected number of bytes. (contains NULLs before the end of the sequence)
 */
static inline unsigned int _reencoder_utf8_validity_check_1_is_expected_length(uint8_t code_units[4], unsigned int num_units);

/**
 * @brief Checks if a UTF-8 code unit sequence contains only continuation bytes following the first byte.
 *
 * @param[in] code_units Array of UTF-8 code units.
 * @param[in] units_expected Number of code units in the array.
 *
 * @return _REENCODER_UTF8_VALIDATION_HAS_VALID_CONTINUATION_BYTES if the sequence is properly formed.
 * @retval number of bytes the sequence ran for before an invalid byte was encountered.
 */
static inline unsigned int _reencoder_utf8_validity_check_2_has_valid_continuation_bytes(uint8_t code_units[4], unsigned int num_units);

/**
 * @brief Checks if a UTF-8 code unit sequence is not overlong.
 *
 * @param[in] code_units Array of UTF-8 code units.
 * @param[in] units_expected Number of code units in the array.
 *
 * @return 1 if the sequence is not overlong.
 * @retval 0 if the sequence is overlong.
 */
static inline unsigned int _reencoder_utf8_validity_check_3_is_not_overlong(uint8_t code_units[4], unsigned int num_units);

/**
 * @brief Checks if a UTF-8 code unit sequence is in valid Unicode range.
 *
 * @param[in] code_units Array of UTF-8 code units.
 * @param[in] units_expected Number of code units in the array.
 *
 * @return 1 if the sequence is in valid Unicode range.
 * @retval 0 if the sequence is out of valid Unicode range (>U+10FFFF).
 */
static inline unsigned int _reencoder_utf8_validity_check_4_is_in_unicode_range(uint8_t code_units[4], unsigned int num_units);

/**
 * @brief Checks if a UTF-8 code unit sequence is not a surrogate pair.
 *
 * @param[in] code_units Array of UTF-8 code units.
 * @param[in] units_expected Number of code units in the array.
 *
 * @return 1 if the sequence is not a surrogate pair.
 * @retval 0 if the sequence is a surrogate pair (U+D800-U+DFFF).
 */
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
		UTF_8, (const void*)string, strlen(string), _reencoder_utf8_seq_is_valid(string), _reencoder_utf8_determine_num_chars(string)
	);

	return struct_utf8_str;
}

unsigned int reencoder_utf8_contains_multibyte(const uint8_t* string) {
	// REENCODER_UTF_8 USER FUNCTION DEFINITION

	size_t examined_index = 0;
	while (string[examined_index] != '\0') {
		switch (_reencoder_utf8_determine_length_from_first_byte(string[examined_index])) {
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

size_t _reencoder_utf8_determine_num_chars(const uint8_t* string) {
	// REENCODER_UTF_8 INTERNAL FUNCTION DEFINITION

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

unsigned int _reencoder_utf8_buffer_idx0_is_valid(const uint8_t* ptr, size_t units_left, unsigned int* units_actual) {
	unsigned int units_expected = _reencoder_utf8_determine_length_from_first_byte(ptr[0]);

	// segfault moment
	uint8_t char_bytes[4] = {
		(units_expected > 0 && units_left > 0) ? ptr[0] : 0x00,
		(units_expected > 1 && units_left > 1) ? ptr[1] : 0x00,
		(units_expected > 2 && units_left > 2) ? ptr[2] : 0x00,
		(units_expected > 3 && units_left > 3) ? ptr[3] : 0x00
	};

	return _reencoder_utf8_char_is_valid(char_bytes, units_expected, units_actual);
}

unsigned int _reencoder_utf8_seq_is_valid(const uint8_t* string) {
	// REENCODER_UTF_8 INTERNAL FUNCTION DEFINITION
	// ALSO DECLARED AS EXTERN IN reencoder_utf_common.h

	size_t input_string_len = strlen(string);

	for (size_t i = 0; i < input_string_len;) {
		unsigned int units_actual = 0;

		unsigned int return_code = _reencoder_utf8_buffer_idx0_is_valid(string + i, input_string_len - i, &units_actual);
		if (return_code != REENCODER_UTF8_VALID) {
			return return_code;
		}

		i += units_actual;
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
		memcpy(buffer + index, _REENCODER_UTF8_REPLACEMENT_CHARACTER, sizeof(_REENCODER_UTF8_REPLACEMENT_CHARACTER));
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
		memcpy(buffer + index, _REENCODER_UTF8_REPLACEMENT_CHARACTER, sizeof(_REENCODER_UTF8_REPLACEMENT_CHARACTER));
		return sizeof(_REENCODER_UTF8_REPLACEMENT_CHARACTER);
	}
}

static inline unsigned int _reencoder_utf8_char_is_valid(uint8_t code_units[4], unsigned int units_expected, unsigned int* units_actual) {
	// REENCODER_UTF_8 STATIC FUNCTION DEFINITION

	unsigned int measured_units = 0;
	if (units_actual != NULL) {
		*units_actual = units_expected; // update later if errors found
	}

	if (units_expected == 1) {
		return REENCODER_UTF8_VALID;
	}
	if (units_expected == 0) {
		if (units_actual != NULL) {
			*units_actual = 1; // always read at least 1 or infinite loop will happen, since we will be stuck processing the same unit over and over
		}
		return REENCODER_UTF8_ERR_INVALID_LEAD;
	}
	measured_units = _reencoder_utf8_validity_check_1_is_expected_length(code_units, units_expected);
	if (measured_units != _REENCODER_UTF8_VALIDATION_HAS_VALID_LENGTH) {
		if (units_actual != NULL) {
			*units_actual = measured_units;
		}
		return REENCODER_UTF8_ERR_PREMATURE_END;
	}
	measured_units = _reencoder_utf8_validity_check_2_has_valid_continuation_bytes(code_units, units_expected);
	if (measured_units != _REENCODER_UTF8_VALIDATION_HAS_VALID_CONTINUATION_BYTES) {
		if (units_actual != NULL) {
			*units_actual = measured_units;
		}
		return REENCODER_UTF8_ERR_INVALID_CONT;
	}
	if (!_reencoder_utf8_validity_check_3_is_not_overlong(code_units, units_expected)) {
		switch (units_expected) {
		case 2:
			return REENCODER_UTF8_ERR_OVERLONG_2BYTE;
		case 3:
			return REENCODER_UTF8_ERR_OVERLONG_3BYTE;
		case 4:
			return REENCODER_UTF8_ERR_OVERLONG_4BYTE;
		}
	}
	if (!_reencoder_utf8_validity_check_4_is_in_unicode_range(code_units, units_expected)) {
		return REENCODER_UTF8_ERR_OUT_OF_RANGE;
	}
	if (!_reencoder_utf8_validity_check_5_is_not_surrogate(code_units, units_expected)) {
		return REENCODER_UTF8_ERR_SURROGATE_PAIR;
	}

	return REENCODER_UTF8_VALID;
}

static inline unsigned int _reencoder_utf8_validity_check_1_is_expected_length(uint8_t code_units[4], unsigned int units_expected) {
	// REENCODER_UTF_8 STATIC FUNCTION DEFINITION

	for (unsigned int i = 1; i < units_expected; i++) {
		if (code_units[i] == 0x00) {
			return i;
		}
	}

	return _REENCODER_UTF8_VALIDATION_HAS_VALID_LENGTH;
}

static inline unsigned int _reencoder_utf8_validity_check_2_has_valid_continuation_bytes(uint8_t code_units[4], unsigned int units_expected) {
	// REENCODER_UTF_8 STATIC FUNCTION DEFINITION

	for (unsigned int i = 1; i < units_expected; i++) {
		if ((code_units[i] & 0b11000000) != 0b10000000) {
			return i;
		}
	}

	return _REENCODER_UTF8_VALIDATION_HAS_VALID_CONTINUATION_BYTES;
}

static inline unsigned int _reencoder_utf8_validity_check_3_is_not_overlong(uint8_t code_units[4], unsigned int units_expected) {
	// REENCODER_UTF_8 STATIC FUNCTION DEFINITION

	// overlong encoding check for 2-byte long characters
	// the first byte of a valid 2-byte UTF-8 sequence must be at least 0xC2.
	// 0xC0 (0b11000000) and 0xC1 (0b11000001) are invalid because they would
	// be encoding values that could fit in a single byte (ASCII range 0x00-0x7F).
	// Example of overlong encodings:
	// [NUL] (U+0000) > 0b11000000 0b10000000 (0xC0 0x80) should be 0b00000000 (0x00)
	// [@]   (U+0040) > 0b11000001 0b10000000 (0xC1 0x80) should be 0b01000000 (0x40)
	// USABLE BITS >         xxxx-     ------                          -------
	if (units_expected == 2 && code_units[0] < 0b11000010) {
		return 0;
	}

	// overlong encoding check for 3-byte long characters
	// since 2-byte long characters contain up to 11 bits (0x07FF) of info, any value smaller than that is overlong.
	// which results in anything smaller than 0b11100000 0b10100000 0b10000000 being invalid.
	// USABLE BITS >                                xxxx     x-----     ------
	if (units_expected == 3 && code_units[0] == 0b11100000 && code_units[1] < 0b10100000) {
		return 0;
	}

	// overlong encoding check for 4-byte long characters
	// since 3-byte long characters contain up to 16 bits of info, any value smaller than that is overlong.
	// which results in anything smaller than 0b11110000 0b10010000 0b10000000 0b10000000 being invalid.
	// USABLE BITS >                                 xxx     xx----     ------     ------
	if (units_expected == 4 && code_units[0] == 0b11110000 && code_units[1] < 0b10010000) {
		return 0;
	}

	return 1;
}

static inline unsigned int _reencoder_utf8_validity_check_4_is_in_unicode_range(uint8_t code_units[4], unsigned int units_expected) {
	// REENCODER_UTF_8 STATIC FUNCTION DEFINITION

	// valid unicode range: U+0000 to U+10FFFF
	// U+10FFFF equates to 0001 0000 1111 1111 1111 1111
	// represented in unicode as 0b11110100 0b10001111 0b10111111 0b10111111 (4-byte character)
	if (units_expected == 4 && (code_units[0] > 0b11110100 || (code_units[0] == 0b11110100 && code_units[1] > 0b10001111))) {
		return 0;
	}

	return 1;
}

static inline unsigned int _reencoder_utf8_validity_check_5_is_not_surrogate(uint8_t code_units[4], unsigned int units_expected) {
	// REENCODER_UTF_8 STATIC FUNCTION DEFINITION

	// surrogate pairs check: U+D800 to U+DFFF
	// equates to 0b11101101 0b10100000 0b10000000 to 
	// ---------- 0b11101101 0b10111111 0b10111111
	if (units_expected == 3 && code_units[0] == 0b11101101 && (code_units[1] & 0b11100000) == 0b10100000) {
		return 0;
	}
	return 1;
}
