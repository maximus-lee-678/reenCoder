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

void reencoder_unicode_struct_free(ReencoderUnicodeStruct* unicode_struct) {
	if (unicode_struct == NULL) {
		return;
	}

	if (unicode_struct->string_buffer != NULL) {
		free(unicode_struct->string_buffer);
	}
	free(unicode_struct);
}

uint8_t _reencoder_is_system_little_endian() {
	// BE: 0x0102 -> 0x01 0x02
	// LE: 0x0102 -> 0x02 0x01
	uint16_t determinator = 0x0102;

	// if first byte is 0x02, it's little-endian
	return (*(uint8_t*)&determinator == 0x02);
}
