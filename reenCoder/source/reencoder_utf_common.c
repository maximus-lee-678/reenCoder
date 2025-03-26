#include "../headers/reencoder_utf_common.h"

ReencoderUnicodeStruct* _reencoder_unicode_struct_init(enum ReencoderEncodeType string_type) {
	ReencoderUnicodeStruct* unicode_struct = (ReencoderUnicodeStruct*)malloc(sizeof(ReencoderUnicodeStruct));
	if (unicode_struct == NULL) {
		return NULL;
	}

	unicode_struct->string_type = string_type;
	unicode_struct->string_buffer = NULL;
	unicode_struct->string_validity = 0;
	unicode_struct->num_chars = 0;
	unicode_struct->num_bytes = 0;

	return unicode_struct;
}

ReencoderUnicodeStruct* _reencoder_unicode_struct_express_populate(
	enum ReencoderEncodeType string_type, const void* string_buffer, size_t string_buffer_bytes,
	unsigned int string_validity, size_t num_chars
) {
	ReencoderUnicodeStruct* unicode_struct = _reencoder_unicode_struct_init(string_type);
	if (unicode_struct == NULL) {
		return NULL;
	}

	// copy to buffer differently based on character type
	switch (string_type) {
	case UTF_8:
		unicode_struct->string_buffer = (uint8_t*)malloc(string_buffer_bytes + sizeof(uint8_t));
		if (unicode_struct->string_buffer == NULL) {
			reencoder_unicode_struct_free(unicode_struct);
			return NULL;
		}

		memcpy(unicode_struct->string_buffer, (const uint8_t*)string_buffer, string_buffer_bytes);
		unicode_struct->string_buffer[string_buffer_bytes] = '\0';

		break;
	case UTF_16BE:
		unicode_struct->string_buffer = (uint8_t*)malloc(string_buffer_bytes + sizeof(uint16_t));
		if (unicode_struct->string_buffer == NULL) {
			reencoder_unicode_struct_free(unicode_struct);
			return NULL;
		}

		if (!_reencoder_is_system_little_endian()) {
			memcpy(unicode_struct->string_buffer, (const uint16_t*)string_buffer, string_buffer_bytes);
		}
		else {
			_reencoder_utf16_write_buffer_swap_endian(unicode_struct->string_buffer, (const uint16_t*)string_buffer, string_buffer_bytes / sizeof(uint16_t));
		}

		unicode_struct->string_buffer[string_buffer_bytes] = '\0';
		unicode_struct->string_buffer[string_buffer_bytes + 1] = '\0';

		break;
	case UTF_16LE:
		unicode_struct->string_buffer = (uint8_t*)malloc(string_buffer_bytes + sizeof(uint16_t));
		if (unicode_struct->string_buffer == NULL) {
			reencoder_unicode_struct_free(unicode_struct);
			return NULL;
		}

		if (_reencoder_is_system_little_endian()) {
			memcpy(unicode_struct->string_buffer, (const uint16_t*)string_buffer, string_buffer_bytes);
		}
		else {
			_reencoder_utf16_write_buffer_swap_endian(unicode_struct->string_buffer, (const uint16_t*)string_buffer, string_buffer_bytes / sizeof(uint16_t));
		}

		unicode_struct->string_buffer[string_buffer_bytes] = '\0';
		unicode_struct->string_buffer[string_buffer_bytes + 1] = '\0';

		break;
	case UTF_32BE:
		break;
	case UTF_32LE:
		break;
	default:
		reencoder_unicode_struct_free(unicode_struct);
		return NULL;
	}

	unicode_struct->string_validity = string_validity;

	// only populate num_chars if the string is valid
	if ((string_type == UTF_8 && string_validity == REENCODER_UTF8_VALID) ||
		((string_type == UTF_16BE || string_type == UTF_16LE) && string_validity == REENCODER_UTF16_VALID)) {
		unicode_struct->num_chars = num_chars;
	}
	unicode_struct->num_bytes = string_buffer_bytes;

	return unicode_struct;
}

void reencoder_unicode_struct_free(ReencoderUnicodeStruct* unicode_struct) {
	if (unicode_struct == NULL) {
		return;
	}

	if (unicode_struct->string_buffer != NULL) {
		free(unicode_struct->string_buffer);
	}
	free(unicode_struct);
}

const char* reencoder_encode_type_as_str(unsigned int encode_type) {
	if (encode_type >= (sizeof(_REENCODER_ENCODE_TYPE_ARR) / sizeof(_REENCODER_ENCODE_TYPE_ARR[0]))) {
		return NULL;
	}

	return _REENCODER_ENCODE_TYPE_ARR[encode_type];
}

uint8_t _reencoder_is_system_little_endian() {
	// BE: 0x0102 -> 0x01 0x02
	// LE: 0x0102 -> 0x02 0x01
	uint16_t determinator = 0x0102;

	// if first byte is 0x02, it's little-endian
	return (*(uint8_t*)&determinator == 0x02);
}
