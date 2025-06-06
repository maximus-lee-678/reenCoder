#include "../headers/reencoder_utf_32.h"

#define _REENCODER_UTF32_REPLACEMENT_CHARACTER 0x0000FFFD

/**
 * @brief Checks a single UTF-32 code unit for validity.
 *
 * @param[in] code_unit Input UTF-32 code unit.
 *
 * @return REENCODER_UTF32_VALID.
 * @retval REENCODER_UTF32_ERR_OUT_OF_RANGE if code unit is out of valid Unicode range (>U+10FFFF).
 * @retval REENCODER_UTF32_ERR_SURROGATE if code unit is a surrogate (U+D800-U+DFFF).
 */
static inline unsigned int _reencoder_utf32_char_is_valid(uint32_t code_unit);

/**
 * @brief Checks if a single UTF-32 code unit is in valid Unicode range (<U+10FFFF).
 *
 * @param[in] code_unit Input UTF-32 code unit.
 *
 * @return 1 if code unit is valid.
 * @retval 0 if code unit is invalid
 */
static inline unsigned int _reencoder_utf32_validity_check_1_is_valid_range(uint32_t code_unit);

/**
 * @brief Checks if a single UTF-32 code unit is a surrogate (U+D800-U+DFFF).
 *
 * @param[in] code_unit Input UTF-32 code unit.
 *
 * @return 1 if code unit is a surrogate.
 * @retval 0 if code unit is not a surrogate.
 */
static unsigned int _reencoder_utf32_validity_check_2_is_not_surrogate(uint32_t code_unit);

// ##### //
// No IETF lol
// ##### //

ReencoderUnicodeStruct* reencoder_utf32_parse_uint32(const uint32_t* string, enum ReencoderEncodeType target_endian) {
	// REENCODER_UTF_32 USER FUNCTION DEFINITION

	if (target_endian != UTF_32BE && target_endian != UTF_32LE) {
		return NULL;
	}

	size_t string_length_uint32 = _reencoder_utf32_strlen(string);
	size_t string_size_bytes = string_length_uint32 * sizeof(uint32_t);

	ReencoderUnicodeStruct* struct_utf32_str = _reencoder_unicode_struct_express_populate(
		target_endian,
		(const void*)string,
		string_size_bytes,
		_reencoder_utf32_seq_is_valid(string, string_length_uint32),
		string_length_uint32
	);

	return struct_utf32_str;
}

ReencoderUnicodeStruct* reencoder_utf32_parse_uint8(
	const uint8_t* string, size_t bytes, enum ReencoderEncodeType source_endian, enum ReencoderEncodeType target_endian
) {
	// REENCODER_UTF_32 USER FUNCTION DEFINITION

	if (target_endian != UTF_32BE && target_endian != UTF_32LE) {
		return NULL;
	}

	// always malloc a multiple of 2 bytes, going higher if needed
	size_t bytes_adjusted = bytes + (bytes % sizeof(uint32_t));
	uint32_t* string_uint32 = (uint32_t*)malloc(bytes_adjusted + sizeof(uint32_t));
	if (string_uint32 == NULL) {
		return NULL;
	}
	_reencoder_utf32_uint32_from_uint8(string_uint32, string, bytes, source_endian);

	// bytes not in multiples of 4 is impossible for UTF-32
	if (bytes % sizeof(uint32_t) != 0) {
		ReencoderUnicodeStruct* struct_utf32_str = _reencoder_unicode_struct_express_populate(
			reencoder_is_system_little_endian() ? UTF_32LE : UTF_32BE,
			(const void*)string_uint32,
			bytes_adjusted,
			REENCODER_UTF32_ERR_ODD_LENGTH,
			0
		);

		return struct_utf32_str;
	}

	ReencoderUnicodeStruct* struct_utf32_str = reencoder_utf32_parse_uint32(string_uint32, target_endian);

	// clean up other allocated memory
	free(string_uint32);

	return struct_utf32_str;
}

size_t _reencoder_utf32_strlen(const uint32_t* string) {
	// REENCODER_UTF_32 INTERNAL FUNCTION DEFINITION
	// ALSO DECLARED AS EXTERN IN reencoder_utf_common.h

	const uint32_t* ptr_start = string;
	const uint32_t* ptr_end = ptr_start;

	while (*ptr_end != 0x00000000) {
		ptr_end++;
	}

	return ptr_end - ptr_start;
}

unsigned int _reencoder_utf32_buffer_idx0_is_valid(const uint32_t* ptr) {
	return _reencoder_utf32_char_is_valid(ptr[0]);
}

unsigned int _reencoder_utf32_seq_is_valid(const uint32_t* string, size_t length) {
	// REENCODER_UTF_32 INTERNAL FUNCTION DEFINITION
	// ALSO DECLARED AS EXTERN IN reencoder_utf_common.h

	for (size_t i = 0; i < length; i++) {
		unsigned int return_code = _reencoder_utf32_buffer_idx0_is_valid(string + i);
		if (return_code != REENCODER_UTF32_VALID) {
			return return_code;
		}
	}

	return REENCODER_UTF32_VALID;
}

void _reencoder_utf32_uint32_from_uint8(uint32_t* dest, const uint8_t* src, size_t bytes, enum ReencoderEncodeType source_endian) {
	// REENCODER_UTF_32 INTERNAL FUNCTION DEFINITION

	if (source_endian != UTF_32BE && source_endian != UTF_32LE) {
		return;
	}

	size_t bytes_adjusted = bytes + (bytes % sizeof(uint32_t));
	size_t extra_bytes = bytes_adjusted - bytes;
	size_t code_units = bytes_adjusted / sizeof(uint32_t);

	if (reencoder_is_system_little_endian() == (source_endian == UTF_32LE)) {
		memcpy(dest, src, bytes);
	}
	else {
		for (size_t i = 0; i < code_units - 1; i++) {
			uint8_t msb = src[i * sizeof(uint32_t)];
			uint8_t byte1 = src[(i * sizeof(uint32_t)) + 1];
			uint8_t byte2 = src[(i * sizeof(uint32_t)) + 2];
			uint8_t lsb = src[(i * sizeof(uint32_t)) + 3];

			dest[i] = (lsb << 0) | (byte2 << 8) | (byte1 << 16) | (msb << 24);
		}

		// last unit is well-formed here
		if (bytes == bytes_adjusted) {
			uint8_t msb = src[(code_units - 1) * sizeof(uint32_t)];
			uint8_t byte1 = src[((code_units - 1) * sizeof(uint32_t)) + 1];
			uint8_t byte2 = src[((code_units - 1) * sizeof(uint32_t)) + 2];
			uint8_t lsb = src[((code_units - 1) * sizeof(uint32_t)) + 3];

			dest[(code_units - 1)] = (lsb << 0) | (byte2 << 8) | (byte1 << 16) | (msb << 24);
		}
	}

	// if bytes<4, change last unit to replacement character
	if (extra_bytes) {
		dest[code_units - 1] = _REENCODER_UTF32_REPLACEMENT_CHARACTER;
	}

	dest[code_units] = 0x00000000;
}

uint32_t _reencoder_utf32_decode_to_code_point(const uint32_t* ptr, unsigned int* units_read) {
	// REENCODER_UTF_32 INTERNAL FUNCTION DEFINITION
	// ALSO DECLARED AS EXTERN IN reencoder_utf_common.h

	uint32_t code_point = ptr[0];
	*units_read = 1;

	// can use unicode check since utf-32 is just a unicode sequence
	if (!_reencoder_code_point_is_valid(code_point)) {
		code_point = _REENCODER_UTF32_REPLACEMENT_CHARACTER;
	}

	return code_point;
}

unsigned int _reencoder_utf32_encode_from_code_point(uint32_t* buffer, size_t index, uint32_t code_point) {
	// REENCODER_UTF_32 INTERNAL FUNCTION DEFINITION
	// ALSO DECLARED AS EXTERN IN reencoder_utf_common.h

	if (!_reencoder_code_point_is_valid(code_point)) {
		buffer[index] = _REENCODER_UTF32_REPLACEMENT_CHARACTER;
		return 1;
	}

	buffer[index] = code_point;
	return 1;
}

void _reencoder_utf32_write_buffer_swap_endian(uint8_t* dest, const uint32_t* src, size_t length) {
	// REENCODER_UTF_32 INTERNAL FUNCTION DEFINITION
	// ALSO DECLARED AS EXTERN IN reencoder_utf_common.h

	for (size_t i = 0; i < length; i++) {
		uint32_t val = src[i];
		val = (val >> 24) | ((val & 0x00FF0000) >> 8) | ((val & 0x0000FF00) << 8) | (val << 24); // swap bytes
		memcpy(dest + (4 * i), &val, sizeof(uint32_t));
	}
}

static inline uint32_t* _reencoder_utf32_grow_buffer(uint32_t* buffer, size_t* current_buffer_size, unsigned int allocate_only_one_unit) {
	// REENCODER_UTF_32 STATIC FUNCTION DEFINITION

	return (uint32_t*)_reencoder_grow_buffer(buffer, current_buffer_size, allocate_only_one_unit, sizeof(uint32_t));
}

static inline unsigned int _reencoder_utf32_char_is_valid(uint32_t code_unit) {
	// REENCODER_UTF_32 STATIC FUNCTION DEFINITION

	if (!_reencoder_utf32_validity_check_1_is_valid_range(code_unit)) {
		return REENCODER_UTF32_ERR_OUT_OF_RANGE;
	}
	if (!_reencoder_utf32_validity_check_2_is_not_surrogate(code_unit)) {
		return REENCODER_UTF32_ERR_SURROGATE;
	}

	return REENCODER_UTF32_VALID;
}

static inline unsigned int _reencoder_utf32_validity_check_1_is_valid_range(uint32_t code_unit) {
	// REENCODER_UTF_32 STATIC FUNCTION DEFINITION

	// max range check: https://scripts.sil.org/cms/scripts/page.php?id=iws-appendixa&site_id=nrsi
	return code_unit < 0x10FFFF;
}
static unsigned int _reencoder_utf32_validity_check_2_is_not_surrogate(uint32_t code_unit) {
	// REENCODER_UTF_16 STATIC FUNCTION DEFINITION

	// surrogate pair check
	return code_unit < 0xD800 || code_unit > 0xDFFF;
}
