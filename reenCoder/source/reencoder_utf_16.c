#include "../headers/reencoder_utf_16.h"

/**
 * @brief Checks if a provided UTF-16 string is valid.
 *
 * Checks for surrogate order, overlong encoding, and premature string endings.
 *
 * @param[in] string UTF-16 string to be checked. Should be represented as an array of uint16_t.
 * @param[in] length Length of the provided string. Length is not number of bytes, but number of uint16_t elements.
 *
 * @return Unsigned integer representing the outcome of the check. Corresponds to index in `REENCODER_UTF16_OUTCOME_ARR`.
 */
static unsigned int reencoder_utf16_is_valid(const uint16_t* string, size_t length);

/**
 * @brief Returns the length of a UTF-16 string.
 *
 * Length is not number of bytes, but number of uint16_t elements.
 *
 * @param[in] string UTF-16 string to be checked. Should be represented as an array of uint16_t. Must be null-terminated (0x0000).
 *
 * @return Length of the provided string.
 */
static size_t reencoder_strlen_utf16(const uint16_t* string);

/**
 * @brief Writes a UTF-16 string to a buffer with swapped endianness.
 *
 * Since UTF-16 strings are represented in memory as 16-bit code units, the endianness of the system must be considered.
 * To preserve intended byte order, this function writes to a 1-byte wide buffer (uint8_t).
 *
 * @param[in] src UTF-16 string to be written.
 * @param[out] dest Buffer to be written to.
 * @param[in] length Length of the provided string. Length is not number of bytes, but number of uint16_t elements.
 *
 * @return void
 */
static void reencoder_utf16_write_buffer_swap_endian(const uint16_t* src, uint8_t* dest, size_t length);

ReencoderUnicodeStruct* reencoder_utf16_parse_uint16(const uint16_t* string, enum ReencoderEncodeType target_endian) {
	if (target_endian != UTF16BE && target_endian != UTF16LE) {
		return NULL;
	}

	ReencoderUnicodeStruct* struct_utf16_str = _reencoder_unicode_struct_init(target_endian);
	if (struct_utf16_str == NULL) {
		return NULL;
	}

	size_t string_length_uint16 = reencoder_strlen_utf16(string);
	size_t string_size_bytes = string_length_uint16 * 2;
	struct_utf16_str->string_buffer = (uint8_t*)malloc(string_size_bytes + 2);
	if (struct_utf16_str->string_buffer == NULL) {
		reencoder_unicode_struct_free(struct_utf16_str);
		return NULL;
	}

	uint8_t sys_is_little_endian = _reencoder_is_system_little_endian();
	if ((sys_is_little_endian && target_endian == UTF16BE) || (!sys_is_little_endian && target_endian == UTF16LE)) {
		reencoder_utf16_write_buffer_swap_endian(string, struct_utf16_str->string_buffer, string_length_uint16);
	}
	else {
		memcpy(struct_utf16_str->string_buffer, string, string_size_bytes);
	}

	struct_utf16_str->string_buffer[string_size_bytes] = '\0';
	struct_utf16_str->string_buffer[string_size_bytes + 1] = '\0';

	struct_utf16_str->string_validity = reencoder_utf16_is_valid(string, string_length_uint16);
	if (struct_utf16_str->string_validity != REENCODER_UTF16_VALID) {
		return struct_utf16_str;
	}

	struct_utf16_str->num_chars = string_length_uint16;
	struct_utf16_str->num_bytes = string_size_bytes;

	return struct_utf16_str;
}

ReencoderUnicodeStruct* reencoder_utf16_parse_uint8(const uint8_t* string, size_t bytes, enum ReencoderEncodeType target_endian) {
	uint16_t* string_uint16 = (uint16_t*)malloc((bytes * 2) + 2);
	if (string_uint16 == NULL) {
		return NULL;
	}

	memcpy(string_uint16, string, bytes);
	string_uint16[bytes] = 0x0000;

	ReencoderUnicodeStruct* struct_utf16_str = reencoder_utf16_parse_uint16(string_uint16, target_endian);

	free(string_uint16);

	return struct_utf16_str;
}

const char* reencoder_utf16_outcome_as_str(unsigned int outcome) {
	if (outcome >= (sizeof(REENCODER_UTF16_OUTCOME_ARR) / sizeof(REENCODER_UTF16_OUTCOME_ARR[0]))) {
		return NULL;
	}

	return REENCODER_UTF16_OUTCOME_ARR[outcome];
}

static unsigned int reencoder_utf16_is_valid(const uint16_t* string, size_t length) {
	// https://datatracker.ietf.org/doc/html/rfc2781/

	for (size_t i = 0; i < length; i++) {
		uint16_t code_unit = string[i];

		// high surrogate
		if (code_unit >= 0xD800 && code_unit <= 0xDBFF) {
			// shouldn't be the last code unit
			if (i + 1 >= length) {
				return REENCODER_UTF16_PREMATURE_END;
			}
			// high surrogate without a following valid low surrogate
			if (string[i + 1] < 0xDC00 || string[i + 1] > 0xDFFF) {
				return REENCODER_UTF16_UNPAIRED_HIGH;
			}

			// overlong encoding check
			// https://en.wikipedia.org/wiki/UTF-16#Code_points_from_U+010000_to_U+10FFFF
			// U' = yyyyyyyyyyxxxxxxxxxx  -> U - 0x10000
			// W1 = 110110yyyyyyyyyy      -> 0xD800 + yyyyyyyyyy
			// W2 = 110111xxxxxxxxxx      -> 0xDC00 + xxxxxxxxxx
			// ensure that U' could not have been encoded in a single 16-bit code unit
			uint32_t code_point = (((code_unit - 0xD800) << 10) | (string[i + 1] - 0xDC00));
			if (code_point <= 0b1111111111) {
				return REENCODER_UTF16_OVERLONG_ENCODING;
			}

			i++; // skip the low surrogate for next comparison (valid pair)
		}

		// low surrogate without a preceding high surrogate
		else if (code_unit >= 0xDC00 && code_unit <= 0xDFFF) {
			return REENCODER_UTF16_UNPAIRED_LOW;
		}

		// if it is neither a high nor low surrogate, it's a single 16-bit code unit (U+0000 to U+D7FF)
	}
	return REENCODER_UTF16_VALID;
}

static size_t reencoder_strlen_utf16(const uint16_t* string) {
	const uint16_t* ptr_start = string;
	const uint16_t* ptr_end = ptr_start;

	while (*ptr_end != 0x0000) {
		ptr_end++;
	}

	return ptr_end - ptr_start;
}

static void reencoder_utf16_write_buffer_swap_endian(const uint16_t* src, uint8_t* dest, size_t length) {
	for (size_t i = 0; i < length; i++) {
		uint16_t val = src[i];
		val = (val >> 8) | (val << 8); // swap bytes
		memcpy(dest + (2 * i), &val, 2);
	}
}
