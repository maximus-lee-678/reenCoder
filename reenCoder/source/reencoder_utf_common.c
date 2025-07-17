#include "../headers/reencoder_utf_common.h"

void reencoder_unicode_struct_free(ReencoderUnicodeStruct** unicode_struct) {
	// [Use Case] End-user Function
	// [End-user Function Tested?] Yes

	if (unicode_struct == NULL || *unicode_struct == NULL) {
		return;
	}

	if ((*unicode_struct)->string_buffer != NULL) {
		free((*unicode_struct)->string_buffer);
	}
	free(*unicode_struct);

	*unicode_struct = NULL;
}

ReencoderUnicodeStruct* reencoder_unicode_struct_duplicate(ReencoderUnicodeStruct* unicode_struct) {
	// [Use Case] End-user Function
	// [End-user Function Tested?] Yes

	if (unicode_struct == NULL) {
		return NULL;
	}

	// cannot use _reencoder_unicode_struct_express_populate, since that expects a uint16_t/uint32_t input for UTF-16/32
	ReencoderUnicodeStruct* new_unicode_struct = _reencoder_unicode_struct_init(unicode_struct->string_type);
	if (new_unicode_struct == NULL) {
		return NULL;
	}
	// accomodate for null-terminator of different sizes
	size_t null_terminator_size = 0;
	if (unicode_struct->string_type == UTF_8) {
		null_terminator_size = sizeof(uint8_t);
	}
	else if (unicode_struct->string_type == UTF_16BE || unicode_struct->string_type == UTF_16LE) {
		null_terminator_size = sizeof(uint16_t);
	}
	else if (unicode_struct->string_type == UTF_32BE || unicode_struct->string_type == UTF_32LE) {
		null_terminator_size = sizeof(uint32_t);
	}
	new_unicode_struct->string_buffer = (uint8_t*)malloc(unicode_struct->num_bytes + null_terminator_size);
	if (new_unicode_struct->string_buffer == NULL) {
		reencoder_unicode_struct_free(&new_unicode_struct);
		return NULL;
	}

	memcpy(new_unicode_struct->string_buffer, unicode_struct->string_buffer, unicode_struct->num_bytes + null_terminator_size);
	new_unicode_struct->num_bytes = unicode_struct->num_bytes;
	new_unicode_struct->string_validity = unicode_struct->string_validity;
	new_unicode_struct->num_chars = unicode_struct->num_chars;

	return new_unicode_struct;
}

const char* reencoder_encode_type_as_str(unsigned int encode_type) {
	// [Use Case] End-user Function
	// [End-user Function Tested?] No (not planned)

	if (encode_type >= (sizeof(_REENCODER_ENCODE_TYPE_ARR) / sizeof(_REENCODER_ENCODE_TYPE_ARR[0]))) {
		return NULL;
	}

	return _REENCODER_ENCODE_TYPE_ARR[encode_type];
}

const char* reencoder_outcome_as_str(unsigned int outcome) {
	// [Use Case] End-user Function
	// [End-user Function Tested?] No (not planned)

	unsigned int outcome_offset = 0;

	outcome_offset = outcome - _REENCODER_UTF8_PARSE_OFFSET;
	if (outcome_offset < (sizeof(_REENCODER_UTF8_OUTCOME_ARR) / sizeof(_REENCODER_UTF8_OUTCOME_ARR[0]))) {
		return _REENCODER_UTF8_OUTCOME_ARR[outcome_offset];
	}

	outcome_offset = outcome - _REENCODER_UTF16_PARSE_OFFSET;
	if (outcome_offset < (sizeof(_REENCODER_UTF16_OUTCOME_ARR) / sizeof(_REENCODER_UTF16_OUTCOME_ARR[0]))) {
		return _REENCODER_UTF16_OUTCOME_ARR[outcome_offset];
	}

	outcome_offset = outcome - _REENCODER_UTF32_PARSE_OFFSET;
	if (outcome_offset < (sizeof(_REENCODER_UTF32_OUTCOME_ARR) / sizeof(_REENCODER_UTF32_OUTCOME_ARR[0]))) {
		return _REENCODER_UTF32_OUTCOME_ARR[outcome_offset];
	}

	return NULL;
}

ReencoderUnicodeStruct* reencoder_convert(enum ReencoderEncodeType source_encoding, enum ReencoderEncodeType target_encoding, const void* source_uint_buffer) {
	if ((source_encoding != UTF_8 && source_encoding != UTF_16BE && source_encoding != UTF_16LE && source_encoding != UTF_32BE && source_encoding != UTF_32LE) ||
		(target_encoding != UTF_8 && target_encoding != UTF_16BE && target_encoding != UTF_16LE && target_encoding != UTF_32BE && target_encoding != UTF_32LE)) {
		return NULL;
	}
	// [Use Case] End-user Function
	// [End-user Function Tested?] Yes

	// gather information about the source string, then check if source_uint_buffer string is valid for specified source_encoding.
	// if not, return a struct with source_encoding.
	size_t string_num_code_units = 0;
	size_t string_size_bytes = 0;
	unsigned int input_buffer_validity = 0;
	if (source_encoding == UTF_8) {
		// okay to cast a uint8_t to a char* for strlen here, since we are only looking for NULLs and don't care about lost data due to the sign bit
		string_num_code_units = strlen((const char*)source_uint_buffer);
		string_size_bytes = string_num_code_units * sizeof(uint8_t);
		input_buffer_validity = _reencoder_utf8_seq_is_valid((const uint8_t*)source_uint_buffer);
		if (input_buffer_validity != REENCODER_UTF8_VALID) {
			return _reencoder_unicode_struct_express_populate(
				source_encoding, (const void*)source_uint_buffer, string_size_bytes, input_buffer_validity, 0
			);
		}
	}
	else if (source_encoding == UTF_16BE || source_encoding == UTF_16LE) {
		string_num_code_units = _reencoder_utf16_strlen((uint16_t*)source_uint_buffer);
		string_size_bytes = string_num_code_units * sizeof(uint16_t);
		input_buffer_validity = _reencoder_utf16_seq_is_valid((const uint16_t*)source_uint_buffer, string_num_code_units);
		if (input_buffer_validity != REENCODER_UTF16_VALID) {
			return _reencoder_unicode_struct_express_populate(
				source_encoding, (const void*)source_uint_buffer, string_size_bytes, input_buffer_validity, 0
			);
		}
	}
	else if (source_encoding == UTF_32BE || source_encoding == UTF_32LE) {
		string_num_code_units = _reencoder_utf32_strlen((uint32_t*)source_uint_buffer);
		string_size_bytes = string_num_code_units * sizeof(uint32_t);
		input_buffer_validity = _reencoder_utf32_seq_is_valid((const uint32_t*)source_uint_buffer, string_num_code_units);
		if (input_buffer_validity != REENCODER_UTF32_VALID) {
			return _reencoder_unicode_struct_express_populate(
				source_encoding, (const void*)source_uint_buffer, string_size_bytes, input_buffer_validity, 0
			);
		}
	}

	// change encoding, assumes input is well-formed, since we already checked earlier
	size_t output_buffer_index = 0;
	size_t output_buffer_size = 0;
	void* output_buffer = NULL;

	if (_reencoder_change_encoding_dynamic(
		source_encoding, target_encoding, string_num_code_units,
		&output_buffer_index, &output_buffer_size, source_uint_buffer, &output_buffer
	) != REENCODER_CONVERT_SUCCESS) {
		// guaranteed to not be null args, output_buffer_index, output_buffer_size, output_buffer addresses have been passed in and they exist on the stack
		return NULL;
	}

	// create struct
	ReencoderUnicodeStruct* output_struct = NULL;
	if (target_encoding == UTF_8) {
		output_struct = reencoder_utf8_parse((uint8_t*)output_buffer);
	}
	else if (target_encoding == UTF_16BE || target_encoding == UTF_16LE) {
		output_struct = reencoder_utf16_parse_uint16((uint16_t*)output_buffer, target_encoding);
	}
	else if (target_encoding == UTF_32BE || target_encoding == UTF_32LE) {
		output_struct = reencoder_utf32_parse_uint32((uint32_t*)output_buffer, target_encoding);
	}

	// clean up other allocated memory
	free(output_buffer);

	return output_struct;
}

unsigned int reencoder_repair_struct(ReencoderUnicodeStruct* unicode_struct) {
	// [Use Case] End-user Function
	// [End-user Function Tested?] Yes

	if (unicode_struct == NULL) {
		return REENCODER_REPAIR_FAILURE_NO_STRUCT;
	}

	if (unicode_struct->string_validity == REENCODER_UTF8_VALID || unicode_struct->string_validity == REENCODER_UTF8_VALID_REPAIRED ||
		unicode_struct->string_validity == REENCODER_UTF16_VALID || unicode_struct->string_validity == REENCODER_UTF16_VALID_REPAIRED ||
		unicode_struct->string_validity == REENCODER_UTF32_VALID || unicode_struct->string_validity == REENCODER_UTF32_VALID_REPAIRED) {
		return REENCODER_REPAIR_FAILURE_NO_OP;
	}

	size_t output_buffer_index = 0;
	size_t output_buffer_size = 0;
	void* output_buffer = NULL;

	void* source_uint_buffer = NULL;
	enum ReencoderEncodeType source_encoding;
	size_t string_num_code_units = 0;
	size_t bytes_adjusted = 0;

	if (unicode_struct->string_type == UTF_8) {
		source_uint_buffer = (uint8_t*)malloc(unicode_struct->num_bytes + sizeof(uint8_t));
		if (source_uint_buffer == NULL) {
			return REENCODER_REPAIR_FAILURE_OOM;
		}
		memcpy((uint8_t*)source_uint_buffer, unicode_struct->string_buffer, unicode_struct->num_bytes);
		source_encoding = unicode_struct->string_type;
		string_num_code_units = unicode_struct->num_bytes / sizeof(uint8_t);
	}
	else if (unicode_struct->string_type == UTF_16BE || unicode_struct->string_type == UTF_16LE) {
		bytes_adjusted = unicode_struct->num_bytes + (unicode_struct->num_bytes % sizeof(uint16_t));
		source_uint_buffer = (uint16_t*)malloc(bytes_adjusted + sizeof(uint16_t));
		if (source_uint_buffer == NULL) {
			return REENCODER_REPAIR_FAILURE_OOM;
		}
		_reencoder_utf16_uint16_from_uint8(
			(uint16_t*)source_uint_buffer, unicode_struct->string_buffer, unicode_struct->num_bytes, unicode_struct->string_type
		);
		source_encoding = reencoder_is_system_little_endian() ? UTF_16LE : UTF_16BE;
		string_num_code_units = unicode_struct->num_bytes / sizeof(uint16_t);
	}
	else if (unicode_struct->string_type == UTF_32BE || unicode_struct->string_type == UTF_32LE) {
		bytes_adjusted = unicode_struct->num_bytes + (unicode_struct->num_bytes % sizeof(uint32_t));
		source_uint_buffer = (uint32_t*)malloc(bytes_adjusted + sizeof(uint32_t));
		if (source_uint_buffer == NULL) {
			return REENCODER_REPAIR_FAILURE_OOM;
		}
		_reencoder_utf32_uint32_from_uint8(
			(uint32_t*)source_uint_buffer, unicode_struct->string_buffer, unicode_struct->num_bytes, unicode_struct->string_type
		);
		source_encoding = reencoder_is_system_little_endian() ? UTF_32LE : UTF_32BE;
		string_num_code_units = unicode_struct->num_bytes / sizeof(uint32_t);
	}

	// change encoding, mistakes will be converted to the replacement character
	if (_reencoder_change_encoding_dynamic(
		source_encoding, source_encoding, string_num_code_units,
		&output_buffer_index, &output_buffer_size, (const void*)source_uint_buffer, &output_buffer
	) != REENCODER_CONVERT_SUCCESS) {
		// guaranteed to not be null args, output_buffer_index, output_buffer_size, output_buffer addresses have been passed in and they exist on the stack
		return REENCODER_REPAIR_FAILURE_OOM;
	}

	free(unicode_struct->string_buffer);

	// assign new string buffer to og struct
	// endianness of original string may have been swapped during the conversion to uint16/32_t
	if (unicode_struct->string_type == UTF_8) {
		unicode_struct->string_buffer = (uint8_t*)output_buffer;
	}
	else if (unicode_struct->string_type == UTF_16BE || unicode_struct->string_type == UTF_16LE) {
		if (source_encoding == unicode_struct->string_type) {
			unicode_struct->string_buffer = (uint8_t*)output_buffer; // can cast to uint8_t* since we are storing to a uint8_t* buffer
		}
		else {
			unicode_struct->string_buffer = (uint8_t*)malloc(bytes_adjusted + sizeof(uint16_t));
			if (unicode_struct->string_buffer == NULL) {
				return REENCODER_REPAIR_FAILURE_OOM;
			}
			_reencoder_utf16_write_buffer_swap_endian(unicode_struct->string_buffer, (const uint16_t*)output_buffer, output_buffer_index);
			free(output_buffer);
		}
	}
	else if (unicode_struct->string_type == UTF_32BE || unicode_struct->string_type == UTF_32LE) {
		if (source_encoding == unicode_struct->string_type) {
			unicode_struct->string_buffer = (uint8_t*)output_buffer; // can cast to uint8_t* since we are storing to a uint8_t* buffer
		}
		else {
			unicode_struct->string_buffer = (uint8_t*)malloc(bytes_adjusted + sizeof(uint32_t));
			if (unicode_struct->string_buffer == NULL) {
				return REENCODER_REPAIR_FAILURE_OOM;
			}
			_reencoder_utf32_write_buffer_swap_endian(unicode_struct->string_buffer, (const uint32_t*)output_buffer, output_buffer_index);
			free(output_buffer);
		}
	}

	// populate remaining unicode_struct fields
	if (unicode_struct->string_type == UTF_8) {
		unicode_struct->num_bytes = output_buffer_index * sizeof(uint8_t);
		unicode_struct->num_chars = _reencoder_utf8_determine_num_chars((const uint8_t*)unicode_struct->string_buffer);
		unicode_struct->string_validity = REENCODER_UTF8_VALID_REPAIRED;
	}
	else if (unicode_struct->string_type == UTF_16BE || unicode_struct->string_type == UTF_16LE) {
		unicode_struct->num_bytes = output_buffer_index * sizeof(uint16_t);
		unicode_struct->num_chars = _reencoder_utf16_determine_num_chars((const uint16_t*)unicode_struct->string_buffer);
		unicode_struct->string_validity = REENCODER_UTF16_VALID_REPAIRED;
	}
	else if (unicode_struct->string_type == UTF_32BE || unicode_struct->string_type == UTF_32LE) {
		unicode_struct->num_bytes = output_buffer_index * sizeof(uint32_t);
		unicode_struct->num_chars = _reencoder_utf32_strlen((const uint32_t*)unicode_struct->string_buffer);
		unicode_struct->string_validity = REENCODER_UTF32_VALID_REPAIRED;
	}

	return REENCODER_REPAIR_SUCCESS;
}

size_t reencoder_write_to_buffer(ReencoderUnicodeStruct* unicode_struct, uint8_t* target_buffer, unsigned int write_bom) {
	// [Use Case] End-user Function
	// [End-user Function Tested?] Yes

	if (target_buffer == NULL || unicode_struct == NULL || unicode_struct->string_buffer == NULL) {
		return 0;

	}
	size_t offset_bytes = 0;
	if (write_bom) {
		switch (unicode_struct->string_type) {
		case UTF_8:
			offset_bytes = sizeof(_REENCODER_UTF8_BOM);
			memcpy(target_buffer, _REENCODER_UTF8_BOM, offset_bytes);
			break;
		case UTF_16BE:
			offset_bytes = sizeof(_REENCODER_UTF16BE_BOM);
			memcpy(target_buffer, _REENCODER_UTF16BE_BOM, offset_bytes);
			break;
		case UTF_16LE:
			offset_bytes = sizeof(_REENCODER_UTF16LE_BOM);
			memcpy(target_buffer, _REENCODER_UTF16LE_BOM, offset_bytes);
			break;
		case UTF_32BE:
			offset_bytes = sizeof(_REENCODER_UTF32BE_BOM);
			memcpy(target_buffer, _REENCODER_UTF32BE_BOM, offset_bytes);
			break;
		case UTF_32LE:
			offset_bytes = sizeof(_REENCODER_UTF32LE_BOM);
			memcpy(target_buffer, _REENCODER_UTF32LE_BOM, offset_bytes);
			break;
		}
	}

	memcpy(target_buffer + offset_bytes, unicode_struct->string_buffer, unicode_struct->num_bytes);

	return offset_bytes + unicode_struct->num_bytes;
}

size_t reencoder_write_to_file(ReencoderUnicodeStruct* unicode_struct, FILE* fp_write_binary, unsigned int write_bom) {
	// [Use Case] End-user Function
	// [End-user Function Tested?] Yes

	if (fp_write_binary == NULL || unicode_struct == NULL || unicode_struct->string_buffer == NULL) {
		return 0;
	}

	size_t offset_bytes = 0;
	size_t num_bytes_written_bom = 0;
	if (write_bom) {
		switch (unicode_struct->string_type) {
		case UTF_8:
			offset_bytes = sizeof(_REENCODER_UTF8_BOM);
			num_bytes_written_bom = fwrite(_REENCODER_UTF8_BOM, sizeof(uint8_t), offset_bytes, fp_write_binary);
			break;
		case UTF_16BE:
			offset_bytes = sizeof(_REENCODER_UTF16BE_BOM);
			num_bytes_written_bom = fwrite(_REENCODER_UTF16BE_BOM, sizeof(uint8_t), offset_bytes, fp_write_binary);
			break;
		case UTF_16LE:
			offset_bytes = sizeof(_REENCODER_UTF16LE_BOM);
			num_bytes_written_bom = fwrite(_REENCODER_UTF16LE_BOM, sizeof(uint8_t), offset_bytes, fp_write_binary);
			break;
		case UTF_32BE:
			offset_bytes = sizeof(_REENCODER_UTF32BE_BOM);
			num_bytes_written_bom = fwrite(_REENCODER_UTF32BE_BOM, sizeof(uint8_t), offset_bytes, fp_write_binary);
			break;
		case UTF_32LE:
			offset_bytes = sizeof(_REENCODER_UTF32LE_BOM);
			num_bytes_written_bom = fwrite(_REENCODER_UTF32LE_BOM, sizeof(uint8_t), offset_bytes, fp_write_binary);
			break;
		}
	}
	if (num_bytes_written_bom != offset_bytes) {
		return 0; // failed to write BOM
	}

	size_t num_bytes_written = 0;
	num_bytes_written = fwrite(unicode_struct->string_buffer, sizeof(uint8_t), unicode_struct->num_bytes, fp_write_binary);
	if (num_bytes_written != unicode_struct->num_bytes) {
		return 0; // failed to write string buffer
	}

	return num_bytes_written_bom + num_bytes_written;
}

uint8_t reencoder_is_system_little_endian() {
	// [Use Case] End-user Function
	// [End-user Function Tested?] No (not planned)

	// BE: 0x0102 -> 0x01 0x02
	// LE: 0x0102 -> 0x02 0x01
	uint16_t determinator = 0x0102;

	// if first byte is 0x02, it's little-endian
	return (*(uint8_t*)&determinator == 0x02);
}

ReencoderUnicodeStruct* _reencoder_unicode_struct_init(enum ReencoderEncodeType string_type) {
	// [Use Case] Internal Function (Non-static ONLY)
	// [End-user Function Tested?] NA

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

ReencoderUnicodeStruct* _reencoder_unicode_struct_express_populate(enum ReencoderEncodeType string_type, const void* string_buffer, size_t string_buffer_bytes, unsigned int string_validity, size_t num_chars) {
	// [Use Case] Internal Function (Non-static, Used in _8/16/32)
	// [End-user Function Tested?] NA

	ReencoderUnicodeStruct* unicode_struct = _reencoder_unicode_struct_init(string_type);
	if (unicode_struct == NULL) {
		return NULL;
	}

	// copy to buffer differently based on character type
	switch (string_type) {
	case UTF_8:
		unicode_struct->string_buffer = (uint8_t*)malloc(string_buffer_bytes + sizeof(uint8_t));
		if (unicode_struct->string_buffer == NULL) {
			reencoder_unicode_struct_free(&unicode_struct);
			return NULL;
		}

		memcpy(unicode_struct->string_buffer, (const uint8_t*)string_buffer, string_buffer_bytes);
		unicode_struct->string_buffer[string_buffer_bytes] = '\0';

		break;
	case UTF_16BE:
		unicode_struct->string_buffer = (uint8_t*)malloc(string_buffer_bytes + sizeof(uint16_t));
		if (unicode_struct->string_buffer == NULL) {
			reencoder_unicode_struct_free(&unicode_struct);
			return NULL;
		}

		if (!reencoder_is_system_little_endian()) {
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
			reencoder_unicode_struct_free(&unicode_struct);
			return NULL;
		}

		if (reencoder_is_system_little_endian()) {
			memcpy(unicode_struct->string_buffer, (const uint16_t*)string_buffer, string_buffer_bytes);
		}
		else {
			_reencoder_utf16_write_buffer_swap_endian(unicode_struct->string_buffer, (const uint16_t*)string_buffer, string_buffer_bytes / sizeof(uint16_t));
		}

		unicode_struct->string_buffer[string_buffer_bytes] = '\0';
		unicode_struct->string_buffer[string_buffer_bytes + 1] = '\0';

		break;
	case UTF_32BE:
		unicode_struct->string_buffer = (uint8_t*)malloc(string_buffer_bytes + sizeof(uint32_t));
		if (unicode_struct->string_buffer == NULL) {
			reencoder_unicode_struct_free(&unicode_struct);
			return NULL;
		}

		if (!reencoder_is_system_little_endian()) {
			memcpy(unicode_struct->string_buffer, (const uint32_t*)string_buffer, string_buffer_bytes);
		}
		else {
			for (size_t i = 0; i < string_buffer_bytes / sizeof(uint32_t); i++) {
				_reencoder_utf32_write_buffer_swap_endian(unicode_struct->string_buffer, (const uint32_t*)string_buffer, string_buffer_bytes / sizeof(uint32_t));
			}
		}
		break;
	case UTF_32LE:
		unicode_struct->string_buffer = (uint8_t*)malloc(string_buffer_bytes + sizeof(uint32_t));
		if (unicode_struct->string_buffer == NULL) {
			reencoder_unicode_struct_free(&unicode_struct);
			return NULL;
		}

		if (reencoder_is_system_little_endian()) {
			memcpy(unicode_struct->string_buffer, (const uint32_t*)string_buffer, string_buffer_bytes);
		}
		else {
			for (size_t i = 0; i < string_buffer_bytes / sizeof(uint32_t); i++) {
				_reencoder_utf32_write_buffer_swap_endian(unicode_struct->string_buffer, (const uint32_t*)string_buffer, string_buffer_bytes / sizeof(uint32_t));
			}
		}
		break;
	default:
		reencoder_unicode_struct_free(&unicode_struct);
		return NULL;
	}

	unicode_struct->string_validity = string_validity;

	// only populate num_chars if the string is valid
	if ((string_type == UTF_8 && string_validity == REENCODER_UTF8_VALID) ||
		((string_type == UTF_16BE || string_type == UTF_16LE) && string_validity == REENCODER_UTF16_VALID) ||
		((string_type == UTF_32BE || string_type == UTF_32LE) && string_validity == REENCODER_UTF32_VALID)) {
		unicode_struct->num_chars = num_chars;
	}
	unicode_struct->num_bytes = string_buffer_bytes;

	return unicode_struct;
}

void* _reencoder_grow_buffer(void* buffer, size_t* buffer_size_bytes, unsigned int allocate_only_one_unit, size_t element_size) {
	// [Use Case] Internal Function (Non-static ONLY)
	// [End-user Function Tested?] NA

	size_t new_size = 0;

	if (*buffer_size_bytes == 0 && !allocate_only_one_unit) {
		new_size = _REENCODER_BASE_STRING_BYTE_SIZE;
	}
	else if (allocate_only_one_unit) {
		new_size = *buffer_size_bytes + element_size;
	}
	else {
		new_size = *buffer_size_bytes * _REENCODER_BASE_STRING_GROW_RATE;
	}

	void* new_buffer = realloc(buffer, new_size);
	if (new_buffer == NULL) {
		*buffer_size_bytes = 0;
		free(buffer);
		return NULL;
	}

	*buffer_size_bytes = new_size;
	return new_buffer;
}

void* _reencoder_grow_buffer_dynamic(enum ReencoderEncodeType string_type, void* buffer, size_t* buffer_size_bytes, size_t buffer_current_index, unsigned int allocate_only_one_unit) {
	// [Use Case] Internal Function (Non-static ONLY)
	// [End-user Function Tested?] NA

	size_t required_bytes = 0;

	switch (string_type) {
	case UTF_8:
		required_bytes = (buffer_current_index + (allocate_only_one_unit ? 1 : 4)) * sizeof(uint8_t);
		if (required_bytes > *buffer_size_bytes) {
			return _reencoder_grow_buffer((uint8_t*)buffer, buffer_size_bytes, allocate_only_one_unit, sizeof(uint8_t));
		}
		break;
	case UTF_16BE:
	case UTF_16LE:
		required_bytes = (buffer_current_index + (allocate_only_one_unit ? 1 : 2)) * sizeof(uint16_t);
		if (required_bytes > *buffer_size_bytes) {
			return _reencoder_grow_buffer((uint16_t*)buffer, buffer_size_bytes, allocate_only_one_unit, sizeof(uint16_t));
		}
		break;
	case UTF_32BE:
	case UTF_32LE:
		required_bytes = (buffer_current_index + 1) * sizeof(uint32_t);
		if (required_bytes > *buffer_size_bytes) {
			return _reencoder_grow_buffer((uint32_t*)buffer, buffer_size_bytes, allocate_only_one_unit, sizeof(uint32_t));
		}
		break;
	default:
		return NULL;
	}

	return buffer;
}

unsigned int _reencoder_change_encoding_dynamic(enum ReencoderEncodeType source_encoding, enum ReencoderEncodeType target_encoding, size_t string_num_code_units, size_t* output_buffer_index, size_t* output_buffer_size, const void* source_buffer, void** output_buffer) {
	// [Use Case] Internal Function (Non-static ONLY)
	// [End-user Function Tested?] NA

	if (source_buffer == NULL || output_buffer_index == NULL || output_buffer_size == NULL) {
		return REENCODER_CONVERT_FAILURE_NULL_ARGS;
	}

	const void* ptr_read = source_buffer;
	size_t units_processed = 0;
	while (units_processed < string_num_code_units) {
		// grow buffer if uninitialised or out of space
		*output_buffer = _reencoder_grow_buffer_dynamic(target_encoding, *output_buffer, output_buffer_size, *output_buffer_index, 0);
		if (*output_buffer == NULL) {
			return REENCODER_CONVERT_FAILURE_OOM;
		}


		// decode 1x char while ensuring all malformations are handled gracefully
		// decode_to_code_point has SOME error handling, but not enough to catch stuff like truncations
		unsigned int units_read = 0;
		uint32_t code_point = 0x00000000;
		if (source_encoding == UTF_8) {
			if (_reencoder_utf8_buffer_idx0_is_valid((const uint8_t*)ptr_read, string_num_code_units - units_processed, &units_read) == REENCODER_UTF8_VALID) {
				code_point = _reencoder_utf8_decode_to_code_point((const uint8_t*)ptr_read, &units_read);
			}
			else {
				code_point = _REENCODER_UNICODE_REPLACEMENT_CHARACTER;
			}
			ptr_read = (const uint8_t*)ptr_read + units_read;
		}
		else if (source_encoding == UTF_16BE || source_encoding == UTF_16LE) {
			if (_reencoder_utf16_buffer_idx0_is_valid((const uint16_t*)ptr_read, string_num_code_units - units_processed, &units_read) == REENCODER_UTF16_VALID) {
				code_point = _reencoder_utf16_decode_to_code_point((const uint16_t*)ptr_read, &units_read);
			}
			else {
				code_point = _REENCODER_UNICODE_REPLACEMENT_CHARACTER;
			}
			ptr_read = (const uint16_t*)ptr_read + units_read;
		}
		else if (source_encoding == UTF_32BE || source_encoding == UTF_32LE) {
			if (_reencoder_utf32_buffer_idx0_is_valid((const uint32_t*)ptr_read) == REENCODER_UTF32_VALID) {
				code_point = _reencoder_utf32_decode_to_code_point((const uint32_t*)ptr_read, &units_read);
			}
			else {
				// always read at least 1 or infinite loop will happen, since we will be stuck processing the same unit over and over 
				// (units_read isn't updated in the if check for UTF-32)
				units_read = 1;
				code_point = _REENCODER_UNICODE_REPLACEMENT_CHARACTER;
			}
			ptr_read = (const uint32_t*)ptr_read + units_read;
		}

		// encode 1x char
		unsigned int units_written = 0;
		if (target_encoding == UTF_8) {
			units_written = _reencoder_utf8_encode_from_code_point((uint8_t*)*output_buffer, *output_buffer_index, code_point);
		}
		else if (target_encoding == UTF_16BE || target_encoding == UTF_16LE) {
			units_written = _reencoder_utf16_encode_from_code_point((uint16_t*)*output_buffer, *output_buffer_index, code_point);
		}
		else if (target_encoding == UTF_32BE || target_encoding == UTF_32LE) {
			units_written = _reencoder_utf32_encode_from_code_point((uint32_t*)*output_buffer, *output_buffer_index, code_point);
		}
		(*output_buffer_index) += units_written;

		units_processed += units_read;
	}

	// grow buffer if out of space (for null-terminator) 
	*output_buffer = _reencoder_grow_buffer_dynamic(target_encoding, *output_buffer, output_buffer_size, *output_buffer_index, 1);
	if (*output_buffer == NULL) {
		return REENCODER_CONVERT_FAILURE_OOM;
	}

	// null-terminate output
	// DO NOT increment output_buffer_index here, it will be used to be count bytes of actual characters only
	if (target_encoding == UTF_8) {
		((uint8_t*)*output_buffer)[*output_buffer_index] = 0x00;
	}
	else if (target_encoding == UTF_16BE || target_encoding == UTF_16LE) {
		((uint16_t*)*output_buffer)[*output_buffer_index] = 0x0000;
	}
	else if (target_encoding == UTF_32BE || target_encoding == UTF_32LE) {
		((uint32_t*)*output_buffer)[*output_buffer_index] = 0x00000000;
	}

	return REENCODER_CONVERT_SUCCESS;
}

unsigned int _reencoder_code_point_is_valid(const uint32_t code_point) {
	// [Use Case] Internal Function (Used in _8/16/32 ONLY)
	// [End-user Function Tested?] NA

	// (INVALID) unicode out of range
	if (code_point > 0x10FFFF) {
		return 0;
	}
	// (INVALID) surrogate value
	if (code_point >= 0xD800 && code_point <= 0xDFFF) {
		return 0;
	}

	return 1;
}
