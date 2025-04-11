#include "../headers/reencoder_utf_32.h"

ReencoderUnicodeStruct* reencoder_utf32_parse_uint32(const uint32_t* string, enum ReencoderEncodeType target_endian) {
	if (target_endian != UTF_32BE && target_endian != UTF_32LE) {
		return NULL;
	}

	size_t string_length_uint32 = _reencoder_utf32_strlen(string);
	size_t string_size_bytes = string_length_uint32 * sizeof(uint32_t);

	ReencoderUnicodeStruct* struct_utf32_str = _reencoder_unicode_struct_express_populate(
		target_endian,
		(const void*)string,
		string_size_bytes,
		_reencoder_utf32_is_valid(string, string_length_uint32),
		string_length_uint32
	);

	return struct_utf32_str;
}

ReencoderUnicodeStruct* reencoder_utf32_parse_uint8(
	const uint8_t* string, size_t bytes, enum ReencoderEncodeType source_endian, enum ReencoderEncodeType target_endian
) {
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

unsigned int _reencoder_utf32_is_valid(const uint32_t* string, size_t length) {
	for (size_t i = 0; i < length; i++) {
		uint32_t code_point = string[i];

		// max range check: https://scripts.sil.org/cms/scripts/page.php?id=iws-appendixa&site_id=nrsi
		if (code_point > 0x10FFFF) {
			return REENCODER_UTF32_OUT_OF_RANGE;
		}
		// surrogate pair check
		if (code_point >= 0xD800 && code_point <= 0xDFFF) {
			return REENCODER_UTF32_SURROGATE_PAIR;
		}
	}

	return REENCODER_UTF32_VALID;
}

size_t _reencoder_utf32_strlen(const uint32_t* string) {
	const uint32_t* ptr_start = string;
	const uint32_t* ptr_end = ptr_start;

	while (*ptr_end != 0x00000000) {
		ptr_end++;
	}

	return ptr_end - ptr_start;
}

void _reencoder_utf32_uint32_from_uint8(uint32_t* dest, const uint8_t* src, size_t bytes, enum ReencoderEncodeType source_endian) {
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

// extern function as defined in reencoder_utf_common.h
void _reencoder_utf32_write_buffer_swap_endian(uint8_t* dest, const uint32_t* src, size_t length) {
	for (size_t i = 0; i < length; i++) {
		uint32_t val = src[i];
		val = (val >> 24) | ((val & 0x00FF0000) >> 8) | ((val & 0x0000FF00) << 8) | (val << 24); // swap bytes
		memcpy(dest + (4 * i), &val, sizeof(uint32_t));
	}
}
