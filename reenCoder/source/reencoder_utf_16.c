#include "../headers/reencoder_utf_16.h"

/**
 * @brief Checks a single UTF-16 code unit for validity.
 *
 * @param[in] code_unit_1 Input UTF-16 code unit.
 * @param[in] code_unit_2 Input UTF-16 code unit (optional, can be NULL or 0x0000).
 * @param[in] units_expected Expected number of code units in the sequence (exactly 1 or 2, no validation is done on this argument!).
 * @param[out] units_actual Pointer to an unsigned integer where the actual number of code units will be stored. Can be NULL if not needed.
 *
 * @return REENCODER_UTF16_VALID.
 * @retval REENCODER_UTF16_ERR_UNPAIRED_LOW if the first code unit is an unpaired low surrogate.
 * @retval REENCODER_UTF16_ERR_UNPAIRED_HIGH if the second code unit is an unpaired high surrogate.
 */
static inline unsigned int _reencoder_utf16_char_is_valid(uint32_t code_unit_1, uint32_t code_unit_2, unsigned int units_expected, unsigned int* units_actual);

/**
 * @brief Checks if a single UTF-16 code unit is not a high or low surrogate.
 *
 * @param[in] code_unit Input UTF-16 code unit.
 *
 * @return 1 if code unit is not a surrogate.
 * @retval 0 if code unit is a surrogate.
 */
static inline unsigned int _reencoder_utf16_validity_check_1_is_not_surrogate(uint32_t code_unit);

/**
 * @brief Checks if a single UTF-16 code unit is a high surrogate.
 *
 * @param[in] code_unit Input UTF-16 code unit.
 *
 * @return 1 if code unit is a high surrogate.
 * @retval 0 if code unit is not a high surrogate.
 */
static inline unsigned int _reencoder_utf16_validity_check_2_is_high_surrogate(uint32_t code_unit);

/**
 * @brief Checks if a single UTF-16 code unit is a low surrogate.
 *
 * @param[in] code_unit Input UTF-16 code unit.
 *
 * @return 1 if code unit is a low surrogate.
 * @retval 0 if code unit is not a low surrogate.
 */
static inline unsigned int _reencoder_utf16_validity_check_3_is_low_surrogate(uint32_t code_unit);

// ##### //
// https://datatracker.ietf.org/doc/html/rfc2781/
// ##### //

ReencoderUnicodeStruct* reencoder_utf16_parse_uint16(const uint16_t* string, enum ReencoderEncodeType target_endian) {
	// [Use Case] End-user Function
	// [End-user Function Tested?] Yes

	if (target_endian != UTF_16BE && target_endian != UTF_16LE) {
		return NULL;
	}

	size_t string_length_uint16 = _reencoder_utf16_strlen(string);
	size_t string_size_bytes = string_length_uint16 * sizeof(uint16_t);

	return _reencoder_unicode_struct_express_populate(
		target_endian, (const void*)string, string_size_bytes,
		_reencoder_utf16_seq_is_valid(string, string_length_uint16), _reencoder_utf16_determine_num_chars(string)
	);
}

ReencoderUnicodeStruct* reencoder_utf16_parse_uint8(const uint8_t* string, size_t bytes, enum ReencoderEncodeType source_endian, enum ReencoderEncodeType target_endian) {
	// [Use Case] End-user Function
	// [End-user Function Tested?] Yes

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
			reencoder_is_system_little_endian() ? UTF_16LE : UTF_16BE, (const void*)string_uint16, bytes_adjusted, REENCODER_UTF16_ERR_ODD_LENGTH, 0
		);
	}

	ReencoderUnicodeStruct* struct_utf16_str = reencoder_utf16_parse_uint16(string_uint16, target_endian);

	// clean up other allocated memory
	free(string_uint16);

	return struct_utf16_str;
}

size_t _reencoder_utf16_strlen(const uint16_t* string) {
	// [Use Case] Internal Function (Non-static, Extern @ _common)
	// [End-user Function Tested?] NA

	const uint16_t* ptr_start = string;
	const uint16_t* ptr_end = ptr_start;

	while (*ptr_end != 0x0000) {
		ptr_end++;
	}

	return ptr_end - ptr_start;
}

size_t _reencoder_utf16_determine_num_chars(const uint16_t* string) {
	// [Use Case] Internal Function (Non-static, Extern @ _common)
	// [End-user Function Tested?] NA

	size_t examined_index = 0;
	size_t num_utf16_chars = 0;

	while (string[examined_index] != 0x0000) {
		unsigned int is_surrogate_half = !_reencoder_utf16_validity_check_1_is_not_surrogate(string[examined_index]);

		if (is_surrogate_half) {
			examined_index += 2;
		}
		else {
			examined_index += 1;
		}
		num_utf16_chars++;
	}

	return num_utf16_chars;
}

unsigned int _reencoder_utf16_buffer_idx0_is_valid(const uint16_t* ptr, size_t units_left, unsigned int* units_actual) {
	// [Use Case] Internal Function (Non-static, Extern @ _common)
	// [End-user Function Tested?] NA

	uint16_t code_unit_1 = ptr[0];
	unsigned int is_potential_surrogate_pair = !_reencoder_utf16_validity_check_1_is_not_surrogate(code_unit_1);
	uint16_t code_unit_2 = is_potential_surrogate_pair && units_left > 1 ? ptr[1] : 0x0000;

	return _reencoder_utf16_char_is_valid(code_unit_1, code_unit_2, is_potential_surrogate_pair ? 2 : 1, units_actual);
}

unsigned int _reencoder_utf16_seq_is_valid(const uint16_t* string, size_t length) {
	// [Use Case] Internal Function (Non-static, Extern @ _common)
	// [End-user Function Tested?] NA

	for (size_t i = 0; i < length;) {
		unsigned int units_actual = 0;

		unsigned int return_code = _reencoder_utf16_buffer_idx0_is_valid(string + i, length - i, &units_actual);
		if (return_code != REENCODER_UTF16_VALID) {
			return return_code;
		}

		i += units_actual;
	}

	return REENCODER_UTF16_VALID;
}

void _reencoder_utf16_uint16_from_uint8(uint16_t* dest, const uint8_t* src, size_t bytes, enum ReencoderEncodeType source_endian) {
	// [Use Case] Internal Function (Non-static, Extern @ _common)
	// [End-user Function Tested?] NA

	if (source_endian != UTF_16BE && source_endian != UTF_16LE) {
		return;
	}

	size_t bytes_adjusted = bytes + (bytes % sizeof(uint16_t));
	size_t code_units = bytes_adjusted / sizeof(uint16_t);

	if (reencoder_is_system_little_endian() == (source_endian == UTF_16LE)) {
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

	// if odd number of bytes, replace lone byte with replacement character
	if (bytes != bytes_adjusted) {
		dest[code_units - 1] = _REENCODER_UTF16_REPLACEMENT_CHARACTER;
	}

	dest[code_units] = 0x0000;
}

uint32_t _reencoder_utf16_decode_to_code_point(const uint16_t* ptr, unsigned int* char_units) {
	// [Use Case] Internal Function (Extern @ _common ONLY)
	// [End-user Function Tested?] NA

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
	// [Use Case] Internal Function (Extern @ _common ONLY)
	// [End-user Function Tested?] NA

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
	// [Use Case] Internal Function (Extern @ _common ONLY)
	// [End-user Function Tested?] NA

	for (size_t i = 0; i < length; i++) {
		uint16_t val = src[i];
		val = (val >> 8) | (val << 8); // swap bytes
		memcpy(dest + (2 * i), &val, sizeof(uint16_t));
	}
}

static inline unsigned int _reencoder_utf16_char_is_valid(uint32_t code_unit_1, uint32_t code_unit_2, unsigned int units_expected, unsigned int* units_actual) {
	// [Use Case] Internal Function (Static)
	// [End-user Function Tested?] NA

	if (units_actual != NULL) {
		*units_actual = units_expected; // update later if errors found
	}

	if (units_expected == 1 && _reencoder_utf16_validity_check_1_is_not_surrogate(code_unit_1)) {
		return REENCODER_UTF16_VALID;
	}
	if (!_reencoder_utf16_validity_check_2_is_high_surrogate(code_unit_1)) {
		if (units_expected == 2 && units_actual != NULL) {
			*units_actual = 1;
		}
		return REENCODER_UTF16_ERR_UNPAIRED_LOW;
	}
	if (units_expected == 2 && !_reencoder_utf16_validity_check_3_is_low_surrogate(code_unit_2)) {
		if (units_actual != NULL) {
			*units_actual = 1;
		}
		return REENCODER_UTF16_ERR_UNPAIRED_HIGH;
	}

	return REENCODER_UTF16_VALID;
}

static inline unsigned int _reencoder_utf16_validity_check_1_is_not_surrogate(uint32_t code_unit) {
	// [Use Case] Internal Function (Static)
	// [End-user Function Tested?] NA

	return code_unit < 0xD800 || code_unit > 0xDFFF; // valid BMP characters (0x0000-0xD7FF, 0xE000-0xFFFF)
}

static inline unsigned int _reencoder_utf16_validity_check_2_is_high_surrogate(uint32_t code_unit) {
	// [Use Case] Internal Function (Static)
	// [End-user Function Tested?] NA

	return code_unit >= 0xD800 && code_unit <= 0xDBFF; // high surrogates (0xD800-0xDBFF)
}

static inline unsigned int _reencoder_utf16_validity_check_3_is_low_surrogate(uint32_t code_unit) {
	// [Use Case] Internal Function (Static)
	// [End-user Function Tested?] NA

	return code_unit >= 0xDC00 && code_unit <= 0xDFFF; // low surrogates (0xDC00-0xDFFF)
}
