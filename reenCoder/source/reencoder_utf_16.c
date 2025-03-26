#include "../headers/reencoder_utf_16.h"

ReencoderUnicodeStruct* reencoder_utf16_parse_uint16(const uint16_t* string, enum ReencoderEncodeType target_endian) {
	if (target_endian != UTF_16BE && target_endian != UTF_16LE) {
		return NULL;
	}

	size_t string_length_uint16 = _reencoder_strlen_utf16(string);
	size_t string_size_bytes = string_length_uint16 * sizeof(uint16_t);

	ReencoderUnicodeStruct* struct_utf16_str = _reencoder_unicode_struct_express_populate(
		target_endian,
		(const void*) string,
		string_size_bytes,
		_reencoder_utf16_is_valid(string, string_length_uint16),
		string_length_uint16
	);

	return struct_utf16_str;
}

ReencoderUnicodeStruct* reencoder_utf16_parse_uint8(
	const uint8_t* string, size_t bytes, enum ReencoderEncodeType source_endian, enum ReencoderEncodeType target_endian
) {
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

	ReencoderUnicodeStruct* struct_utf16_str = reencoder_utf16_parse_uint16(string_uint16, target_endian);

	// clean up other allocated memory
	free(string_uint16);

	return struct_utf16_str;
}

const char* reencoder_utf16_outcome_as_str(unsigned int outcome) {
	unsigned int outcome_offset = outcome - _REENCODER_UTF16_PARSE_OFFSET;
	if (outcome_offset >= (sizeof(_REENCODER_UTF16_OUTCOME_ARR) / sizeof(_REENCODER_UTF16_OUTCOME_ARR[0]))) {
		return NULL;
	}

	return _REENCODER_UTF16_OUTCOME_ARR[outcome_offset];
}

unsigned int _reencoder_utf16_is_valid(const uint16_t* string, size_t length) {
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

size_t _reencoder_strlen_utf16(const uint16_t* string) {
	const uint16_t* ptr_start = string;
	const uint16_t* ptr_end = ptr_start;

	while (*ptr_end != 0x0000) {
		ptr_end++;
	}

	return ptr_end - ptr_start;
}

void _reencoder_utf16_write_buffer_swap_endian(uint8_t* dest, const uint16_t* src, size_t length) {
	for (size_t i = 0; i < length; i++) {
		uint16_t val = src[i];
		val = (val >> 8) | (val << 8); // swap bytes
		memcpy(dest + (2 * i), &val, 2);
	}
}

void _reencoder_utf16_uint16_from_uint8(uint16_t* dest, const uint8_t* src, size_t bytes, enum ReencoderEncodeType source_endian) {
	if (source_endian != UTF_16BE && source_endian != UTF_16LE) {
		return;
	}

	if (_reencoder_is_system_little_endian() == (source_endian == UTF_16LE)) {
		memcpy(dest, src, bytes);
	}
	else {
		for (size_t i = 0; i < bytes / sizeof(uint16_t); i++) {
			uint8_t high = src[i * 2]; // MSB in BE, LSB in LE
			uint8_t low = src[(i * 2) + 1]; // LSB in BE, MSB in LE
			dest[i] = (high << 8) | low;   // swap bytes
		}
	}

	dest[bytes / sizeof(uint16_t)] = 0x0000;
}