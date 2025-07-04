#include "reencoder_test_utf_32.h"

void _reencoder_test_valid_utf_32_u32_valid(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf32_parse_uint32(_reencoder_test_string_utf_32_u32_valid, UTF_32LE);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_32_valid, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_valid_utf_32_u32_valid_long_sequence(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf32_parse_uint32(_reencoder_test_string_utf_32_u32_valid_long_sequence, UTF_32LE);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_32_le_valid_long_sequence, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_invalid_utf_32_u32_surrogate(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf32_parse_uint32(_reencoder_test_string_utf_32_u32_surrogate, UTF_32LE);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_32_surrogate, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_invalid_utf_32_u32_out_of_range(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf32_parse_uint32(_reencoder_test_string_utf_32_u32_out_of_range, UTF_32LE);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_32_out_of_range, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_valid_utf_32_u8le_valid(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf32_parse_uint8(
		_reencoder_test_string_utf_32_u8le_valid, _reencoder_test_struct_utf_32_valid.num_bytes, UTF_32LE, UTF_32LE
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_32_valid, struct_actual);

	*state = struct_actual;
}
void _reencoder_test_valid_utf_32_u8le_valid_long_sequence(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf32_parse_uint8(
		_reencoder_test_string_utf_32_u8le_valid_long_sequence, _reencoder_test_struct_utf_32_le_valid_long_sequence.num_bytes, UTF_32LE, UTF_32LE
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_32_le_valid_long_sequence, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_invalid_utf_32_u8le_surrogate(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf32_parse_uint8(
		_reencoder_test_string_utf_32_u8le_surrogate, _reencoder_test_struct_utf_32_surrogate.num_bytes, UTF_32LE, UTF_32LE
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_32_surrogate, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_invalid_utf_32_u8le_out_of_range(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf32_parse_uint8(
		_reencoder_test_string_utf_32_u8le_out_of_range, _reencoder_test_struct_utf_32_out_of_range.num_bytes, UTF_32LE, UTF_32LE
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_32_out_of_range, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_valid_utf_32_u8be_valid(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf32_parse_uint8(
		_reencoder_test_string_utf_32_u8be_valid, _reencoder_test_struct_utf_32_valid.num_bytes, UTF_32BE, UTF_32LE
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_32_valid, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_valid_utf_32_u8be_valid_long_sequence(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf32_parse_uint8(
		_reencoder_test_string_utf_32_u8be_valid_long_sequence, _reencoder_test_struct_utf_32_le_valid_long_sequence.num_bytes, UTF_32BE, UTF_32LE
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_32_le_valid_long_sequence, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_invalid_utf_32_u8be_surrogate(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf32_parse_uint8(
		_reencoder_test_string_utf_32_u8be_surrogate, _reencoder_test_struct_utf_32_surrogate.num_bytes, UTF_32BE, UTF_32LE
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_32_surrogate, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_invalid_utf_32_u8be_out_of_range(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf32_parse_uint8(
		_reencoder_test_string_utf_32_u8be_out_of_range, _reencoder_test_struct_utf_32_out_of_range.num_bytes, UTF_32BE, UTF_32LE
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_32_out_of_range, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_valid_utf_32_from_utf_8(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_convert(
		UTF_8, UTF_32LE, _reencoder_test_string_utf_8_valid_long_sequence
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_32_le_valid_long_sequence, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_valid_utf_32_from_utf_16(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_convert(
		reencoder_is_system_little_endian() ? UTF_16LE : UTF_16BE, UTF_32LE, _reencoder_test_string_utf_16_u16_valid_long_sequence
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_32_le_valid_long_sequence, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_invalid_utf_32_from_utf_8(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_convert(
		UTF_8, UTF_32LE, _reencoder_test_string_utf_8_invalid_lead
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_8_invalid_lead, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_invalid_utf_32_from_utf_16(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_convert(
		reencoder_is_system_little_endian() ? UTF_16LE : UTF_16BE, UTF_32LE, _reencoder_test_string_utf_16_u16_only_high_surrogate
	);

	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_16_only_high_surrogate, struct_actual);
	*state = struct_actual;
}

void _reencoder_test_fix_utf_32(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf32_parse_uint8(
		_reencoder_test_string_utf_32_repair_broken, _reencoder_test_struct_utf_32_repair_fixed.num_bytes, UTF_32LE, UTF_32LE
	);
	reencoder_repair_struct(struct_actual);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_32_repair_fixed, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_write_utf_32_le_w_bom_to_buffer(void** state) {
	(void)state;

	ReencoderUnicodeStruct struct_actual = _reencoder_test_struct_utf_32_le_valid_long_sequence;
	uint8_t write_buffer[_REENCODER_TEST_NUM_BYTES_UTF_32_VALID_LONG_SEQUENCE + sizeof(_REENCODER_UTF32LE_BOM)] = { 0x00 };

	size_t bytes_written = reencoder_write_to_buffer(&struct_actual, write_buffer, 1);

	_reencoder_test_buffer_equal(&struct_actual, write_buffer, bytes_written, 1);
}

void _reencoder_test_write_utf_32_le_wo_bom_to_buffer(void** state) {
	(void)state;

	ReencoderUnicodeStruct struct_actual = _reencoder_test_struct_utf_32_le_valid_long_sequence;
	uint8_t write_buffer[_REENCODER_TEST_NUM_BYTES_UTF_32_VALID_LONG_SEQUENCE + sizeof(_REENCODER_UTF32LE_BOM)] = { 0x00 };

	size_t bytes_written = reencoder_write_to_buffer(&struct_actual, write_buffer, 0);

	_reencoder_test_buffer_equal(&struct_actual, write_buffer, bytes_written, 0);
}

void _reencoder_test_write_utf_32_le_w_bom_to_file(void** state) {
	(void)state;

	ReencoderUnicodeStruct struct_actual = _reencoder_test_struct_utf_32_le_valid_long_sequence;
	FILE* fp_tmp = tmpfile();
	if (fp_tmp == NULL) {
		fail_msg("%s", _REENCODER_TEST_FAIL_STRINGS[_REENCODER_TEST_FAIL_TEMP_FILE]);
	}

	size_t bytes_written = reencoder_write_to_file(&struct_actual, fp_tmp, 1);

	_reencoder_test_file_equal(&struct_actual, fp_tmp, bytes_written, 1);

	fclose(fp_tmp);
}

void _reencoder_test_write_utf_32_le_wo_bom_to_file(void** state) {
	(void)state;

	ReencoderUnicodeStruct struct_actual = _reencoder_test_struct_utf_32_le_valid_long_sequence;
	FILE* fp_tmp = tmpfile();
	if (fp_tmp == NULL) {
		fail_msg("%s", _REENCODER_TEST_FAIL_STRINGS[_REENCODER_TEST_FAIL_TEMP_FILE]);
	}

	size_t bytes_written = reencoder_write_to_file(&struct_actual, fp_tmp, 0);

	_reencoder_test_file_equal(&struct_actual, fp_tmp, bytes_written, 0);

	fclose(fp_tmp);
}

void _reencoder_test_write_utf_32_be_w_bom_to_buffer(void** state) {
	(void)state;

	ReencoderUnicodeStruct struct_actual = _reencoder_test_struct_utf_32_be_valid_long_sequence;
	uint8_t write_buffer[_REENCODER_TEST_NUM_BYTES_UTF_32_VALID_LONG_SEQUENCE + sizeof(_REENCODER_UTF32BE_BOM)] = { 0x00 };

	size_t bytes_written = reencoder_write_to_buffer(&struct_actual, write_buffer, 1);

	_reencoder_test_buffer_equal(&struct_actual, write_buffer, bytes_written, 1);
}

void _reencoder_test_write_utf_32_be_wo_bom_to_buffer(void** state) {
	(void)state;

	ReencoderUnicodeStruct struct_actual = _reencoder_test_struct_utf_32_be_valid_long_sequence;
	uint8_t write_buffer[_REENCODER_TEST_NUM_BYTES_UTF_32_VALID_LONG_SEQUENCE + sizeof(_REENCODER_UTF32BE_BOM)] = { 0x00 };

	size_t bytes_written = reencoder_write_to_buffer(&struct_actual, write_buffer, 0);

	_reencoder_test_buffer_equal(&struct_actual, write_buffer, bytes_written, 0);
}

void _reencoder_test_write_utf_32_be_w_bom_to_file(void** state) {
	(void)state;

	ReencoderUnicodeStruct struct_actual = _reencoder_test_struct_utf_32_be_valid_long_sequence;
	FILE* fp_tmp = tmpfile();
	if (fp_tmp == NULL) {
		fail_msg("%s", _REENCODER_TEST_FAIL_STRINGS[_REENCODER_TEST_FAIL_TEMP_FILE]);
	}

	size_t bytes_written = reencoder_write_to_file(&struct_actual, fp_tmp, 1);

	_reencoder_test_file_equal(&struct_actual, fp_tmp, bytes_written, 1);

	fclose(fp_tmp);
}

void _reencoder_test_write_utf_32_be_wo_bom_to_file(void** state) {
	(void)state;

	ReencoderUnicodeStruct struct_actual = _reencoder_test_struct_utf_32_be_valid_long_sequence;
	FILE* fp_tmp = tmpfile();
	if (fp_tmp == NULL) {
		fail_msg("%s", _REENCODER_TEST_FAIL_STRINGS[_REENCODER_TEST_FAIL_TEMP_FILE]);
	}

	size_t bytes_written = reencoder_write_to_file(&struct_actual, fp_tmp, 0);

	_reencoder_test_file_equal(&struct_actual, fp_tmp, bytes_written, 0);

	fclose(fp_tmp);
}
