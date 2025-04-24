#include "../headers/reencoder_utf_32.h"

#define _REENCODER_UTF32_REPLACEMENT_CHARACTER 0x0000FFFD

static inline uint32_t* _reencoder_utf32_grow_buffer(uint32_t* buffer, size_t* current_buffer_size, unsigned int allocate_only_one_unit);
static inline unsigned int _reencoder_utf32_char_is_valid(uint32_t code_unit);
static inline unsigned int _reencoder_utf32_validity_check_1_is_valid_range(uint32_t code_unit);
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
			_reencoder_is_system_little_endian() ? UTF_32LE : UTF_32BE,
			(const void*)string_uint32,
			bytes,
			REENCODER_UTF32_ODD_LENGTH,
			0
		);

		return struct_utf32_str;
	}

	ReencoderUnicodeStruct* struct_utf32_str = reencoder_utf32_parse_uint32(string_uint32, target_endian);

	// clean up other allocated memory
	free(string_uint32);

	return struct_utf32_str;
}

ReencoderUnicodeStruct* reencoder_utf32_parse_from_utf8(const uint8_t* string, enum ReencoderEncodeType target_endian) {
	// REENCODER_UTF_32 USER FUNCTION DEFINITION

	if (target_endian != UTF_32BE && target_endian != UTF_32LE) {
		return NULL;
	}

	size_t string_length = strlen(string);
	size_t string_size_bytes = string_length * sizeof(uint8_t);

	// check if utf-8 is valid, if not, return a utf-8 struct
	unsigned int input_utf8_validity = _reencoder_utf8_seq_is_valid(string);
	if (input_utf8_validity != REENCODER_UTF8_VALID) {
		return _reencoder_unicode_struct_express_populate(
			UTF_8, (const void*)string, string_size_bytes, input_utf8_validity, 0
		);
	}

	// begin conversion utf-8 -> utf-32
	size_t utf32_index = 0;
	size_t utf32_buffer_size = 0;
	uint32_t* utf32_output_buffer = NULL;

	// assumes utf-8 is well-formed, since we already checked earlier
	const uint8_t* ptr_read = string;
	while (*ptr_read) {
		// grow buffer if uninitialised or out of space
		// maximum a utf-8 character can be is 1 utf-32 character (4 bytes)
		if ((utf32_index + 1) * sizeof(uint32_t) > utf32_buffer_size) {
			utf32_output_buffer = _reencoder_utf32_grow_buffer(utf32_output_buffer, &utf32_buffer_size, 0);
			if (utf32_output_buffer == NULL) {
				return NULL;
			}
		}

		unsigned int units_read = 0;
		uint32_t code_point = _reencoder_utf8_decode_to_code_point(ptr_read, &units_read);
		ptr_read += units_read;

		unsigned int units_written = _reencoder_utf32_encode_from_code_point(utf32_output_buffer, utf32_index, code_point);
		utf32_index += units_written;
	}

	// grow buffer if out of space (for null-terminator)
	if (utf32_index + sizeof(uint32_t) > utf32_buffer_size) {
		utf32_output_buffer = _reencoder_utf32_grow_buffer(utf32_output_buffer, &utf32_buffer_size, 1);
		if (utf32_output_buffer == NULL) {
			return NULL;
		}
	}

	// null-terminate utf-32 output
	utf32_output_buffer[utf32_index] = '\0';

	// create utf-32 struct
	ReencoderUnicodeStruct* struct_utf32_str = reencoder_utf32_parse_uint32(utf32_output_buffer, target_endian);

	// clean up other allocated memory
	free(utf32_output_buffer);

	return struct_utf32_str;
}

ReencoderUnicodeStruct* reencoder_utf32_parse_from_utf16(const uint16_t* string, enum ReencoderEncodeType target_endian) {
	// REENCODER_UTF_32 USER FUNCTION DEFINITION

	if (target_endian != UTF_32BE && target_endian != UTF_32LE) {
		return NULL;
	}

	size_t string_length = _reencoder_utf16_strlen(string);
	size_t string_size_bytes = string_length * sizeof(uint16_t);

	// check if utf-16 is valid, if not, return a utf-16 struct
	unsigned int input_utf16_validity = _reencoder_utf16_seq_is_valid(string, string_length);
	if (input_utf16_validity != REENCODER_UTF16_VALID) {
		return _reencoder_unicode_struct_express_populate(
			_reencoder_is_system_little_endian() ? UTF_16LE : UTF_16BE, (const void*)string, string_size_bytes, input_utf16_validity, 0
		);
	}

	// begin conversion utf-16 -> utf-32
	size_t utf32_index = 0;
	size_t utf32_buffer_size = 0;
	uint32_t* utf32_output_buffer = NULL;

	// assumes utf-16 is well-formed, since we already checked earlier
	const uint16_t* ptr_read = string;
	while (*ptr_read) {
		// grow buffer if uninitialised or out of space
		if ((utf32_index + 1) * sizeof(uint32_t) > utf32_buffer_size) {
			utf32_output_buffer = _reencoder_utf32_grow_buffer(utf32_output_buffer, &utf32_buffer_size, 0);
			if (utf32_output_buffer == NULL) {
				return NULL;
			}
		}

		unsigned int units_read = 0;
		uint32_t code_point = _reencoder_utf16_decode_to_code_point(ptr_read, &units_read);
		ptr_read += units_read;

		unsigned int units_written = _reencoder_utf32_encode_from_code_point(utf32_output_buffer, utf32_index, code_point);
		utf32_index += units_written;
	}

	// grow buffer if out of space (for null-terminator)
	if (utf32_index + sizeof(uint32_t) > utf32_buffer_size) {
		utf32_output_buffer = _reencoder_utf32_grow_buffer(utf32_output_buffer, &utf32_buffer_size, 1);
		if (utf32_output_buffer == NULL) {
			return NULL;
		}
	}

	// null-terminate utf-32 output
	utf32_output_buffer[utf32_index] = '\0';

	// create utf-32 struct
	ReencoderUnicodeStruct* struct_utf32_str = reencoder_utf32_parse_uint32(utf32_output_buffer, target_endian);

	// clean up other allocated memory
	free(utf32_output_buffer);

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

unsigned int _reencoder_utf32_seq_is_valid(const uint32_t* string, size_t length) {
	// REENCODER_UTF_32 INTERNAL FUNCTION DEFINITION
	// ALSO DECLARED AS EXTERN IN reencoder_utf_common.h

	for (size_t i = 0; i < length; i++) {
		uint32_t code_point = string[i];

		unsigned int return_code = _reencoder_utf32_char_is_valid(code_point);
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

	if (_reencoder_is_system_little_endian() == (source_endian == UTF_32LE)) {
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

	// if bytes<4, promote the lone byte to the last code unit with msb padding
	if (extra_bytes) {
		uint32_t last_unit = 0x00000000;

		if (extra_bytes == 1) {
			last_unit = (uint32_t)src[bytes - 1];
		}
		else if (extra_bytes == 2) {
			last_unit = ((uint32_t)src[bytes - 2]) | ((uint32_t)src[bytes - 1] << 8);
		}
		else if (extra_bytes == 3) {
			last_unit = ((uint32_t)src[bytes - 3]) | ((uint32_t)src[bytes - 2] << 8) | ((uint32_t)src[bytes - 1] << 16);
		}

		dest[code_units - 1] = last_unit;
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
		return REENCODER_UTF32_OUT_OF_RANGE;
	}
	if (!_reencoder_utf32_validity_check_2_is_not_surrogate(code_unit)) {
		return REENCODER_UTF32_SURROGATE_PAIR;
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
