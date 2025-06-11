#include "reencoder_test_utf_16.h"

void _reencoder_test_valid_utf_16_u16_valid_2_byte(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf16_parse_uint16(_reencoder_test_string_utf_16_u16_valid_2_byte, UTF_16LE);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_16_valid_2_byte, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_valid_utf_16_u16_valid_4_byte(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf16_parse_uint16(_reencoder_test_string_utf_16_u16_valid_4_byte, UTF_16LE);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_16_valid_4_byte, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_valid_utf_16_u16_valid_long_sequence(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf16_parse_uint16(_reencoder_test_string_utf_16_u16_valid_long_sequence, UTF_16LE);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_16_le_valid_long_sequence, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_invalid_utf_16_u16_only_high_surrogate_sequence(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf16_parse_uint16(_reencoder_test_string_utf_16_u16_only_high_surrogate, UTF_16LE);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_16_only_high_surrogate, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_invalid_utf_16_u16_only_low_surrogate_sequence(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf16_parse_uint16(_reencoder_test_string_utf_16_u16_only_low_surrogate, UTF_16LE);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_16_only_low_surrogate, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_invalid_utf_16_u16_odd_sequence(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf16_parse_uint8(
		_reencoder_test_string_utf_16_u8le_odd_broken, _reencoder_test_struct_utf_16_odd_padded.num_bytes - _reencoder_test_added_bytes_utf_16_u8le_odd_padded, UTF_16LE, UTF_16LE
	);

	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_16_odd_padded, struct_actual);
	*state = struct_actual;
}

void _reencoder_test_valid_utf_16_u8le_valid_2_byte(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf16_parse_uint8(
		_reencoder_test_string_utf_16_u8le_valid_2_byte, _reencoder_test_struct_utf_16_valid_2_byte.num_bytes, UTF_16LE, UTF_16LE
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_16_valid_2_byte, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_valid_utf_16_u8le_valid_4_byte(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf16_parse_uint8(
		_reencoder_test_string_utf_16_u8le_valid_4_byte, _reencoder_test_struct_utf_16_valid_4_byte.num_bytes, UTF_16LE, UTF_16LE
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_16_valid_4_byte, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_valid_utf_16_u8le_valid_long_sequence(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf16_parse_uint8(
		_reencoder_test_string_utf_16_u8le_valid_long_sequence, _reencoder_test_struct_utf_16_le_valid_long_sequence.num_bytes, UTF_16LE, UTF_16LE
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_16_le_valid_long_sequence, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_invalid_utf_16_u8le_only_high_surrogate_sequence(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf16_parse_uint8(
		_reencoder_test_string_utf_16_u8le_only_high_surrogate, _reencoder_test_struct_utf_16_only_high_surrogate.num_bytes, UTF_16LE, UTF_16LE
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_16_only_high_surrogate, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_invalid_utf_16_u8le_only_low_surrogate_sequence(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf16_parse_uint8(
		_reencoder_test_string_utf_16_u8le_only_low_surrogate, _reencoder_test_struct_utf_16_only_low_surrogate.num_bytes, UTF_16LE, UTF_16LE
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_16_only_low_surrogate, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_valid_utf_16_u8be_valid_2_byte(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf16_parse_uint8(
		_reencoder_test_string_utf_16_u8be_valid_2_byte, _reencoder_test_struct_utf_16_valid_2_byte.num_bytes, UTF_16BE, UTF_16LE
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_16_valid_2_byte, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_valid_utf_16_u8be_valid_4_byte(void** state) {
	(void)state;
	ReencoderUnicodeStruct* struct_actual = reencoder_utf16_parse_uint8(
		_reencoder_test_string_utf_16_u8be_valid_4_byte, _reencoder_test_struct_utf_16_valid_4_byte.num_bytes, UTF_16BE, UTF_16LE
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_16_valid_4_byte, struct_actual);
	*state = struct_actual;
}

void _reencoder_test_valid_utf_16_u8be_valid_long_sequence(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf16_parse_uint8(
		_reencoder_test_string_utf_16_u8be_valid_long_sequence, _reencoder_test_struct_utf_16_le_valid_long_sequence.num_bytes, UTF_16BE, UTF_16LE
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_16_le_valid_long_sequence, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_invalid_utf_16_u8be_only_high_surrogate_sequence(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf16_parse_uint8(
		_reencoder_test_string_utf_16_u8be_only_high_surrogate, _reencoder_test_struct_utf_16_only_high_surrogate.num_bytes, UTF_16BE, UTF_16LE
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_16_only_high_surrogate, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_invalid_utf_16_u8be_only_low_surrogate_sequence(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf16_parse_uint8(
		_reencoder_test_string_utf_16_u8be_only_low_surrogate, _reencoder_test_struct_utf_16_only_low_surrogate.num_bytes, UTF_16BE, UTF_16LE
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_16_only_low_surrogate, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_valid_utf_16_from_utf_8(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_convert(
		UTF_8, UTF_16LE, _reencoder_test_string_utf_8_valid_long_sequence
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_16_le_valid_long_sequence, struct_actual);
	*state = struct_actual;
}

void _reencoder_test_valid_utf_16_from_utf_32(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_convert(
		reencoder_is_system_little_endian() ? UTF_32LE : UTF_32BE, UTF_16LE, _reencoder_test_string_utf_32_u32_valid_long_sequence
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_16_le_valid_long_sequence, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_invalid_utf_16_from_utf_8(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_convert(
		UTF_8, UTF_16LE, _reencoder_test_string_utf_8_invalid_lead
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_8_invalid_lead, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_invalid_utf_16_from_utf_32(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_convert(
		reencoder_is_system_little_endian() ? UTF_32LE : UTF_32BE, UTF_16LE, _reencoder_test_string_utf_32_u32_surrogate
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_32_surrogate, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_fix_utf_16(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf16_parse_uint8(
		_reencoder_test_string_utf_16_repair_broken, _reencoder_test_struct_utf_16_repair_fixed.num_bytes - _reencoder_test_added_bytes_utf_16_u8le_repair_fixed, UTF_16LE, UTF_16LE
	);
	reencoder_repair_struct(struct_actual);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_16_repair_fixed, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_write_utf_16_le_w_bom_to_buffer(void** state) {
	(void)state;

	ReencoderUnicodeStruct struct_actual = _reencoder_test_struct_utf_16_le_valid_long_sequence;
	uint8_t write_buffer[_REENCODER_TEST_NUM_BYTES_UTF_16_VALID_LONG_SEQUENCE + sizeof(_REENCODER_UTF16LE_BOM)] = { 0x00 };

	size_t bytes_written = reencoder_write_to_buffer(&struct_actual, write_buffer, 1);

	_reencoder_test_buffer_equal(&struct_actual, write_buffer, bytes_written, 1);
}

void _reencoder_test_write_utf_16_le_wo_bom_to_buffer(void** state) {
	(void)state;

	ReencoderUnicodeStruct struct_actual = _reencoder_test_struct_utf_16_le_valid_long_sequence;
	uint8_t write_buffer[_REENCODER_TEST_NUM_BYTES_UTF_16_VALID_LONG_SEQUENCE + sizeof(_REENCODER_UTF16LE_BOM)] = { 0x00 };

	size_t bytes_written = reencoder_write_to_buffer(&struct_actual, write_buffer, 0);

	_reencoder_test_buffer_equal(&struct_actual, write_buffer, bytes_written, 0);
}

void _reencoder_test_write_utf_16_le_w_bom_to_file(void** state) {
	(void)state;

	ReencoderUnicodeStruct struct_actual = _reencoder_test_struct_utf_16_le_valid_long_sequence;
	FILE* fp_tmp = tmpfile();
	if (fp_tmp == NULL) {
		fail_msg("%s", _REENCODER_TEST_FAIL_STRINGS[_REENCODER_TEST_FAIL_TEMP_FILE]);
	}

	size_t bytes_written = reencoder_write_to_file(&struct_actual, fp_tmp, 1);

	_reencoder_test_file_equal(&struct_actual, fp_tmp, bytes_written, 1);

	fclose(fp_tmp);
}

void _reencoder_test_write_utf_16_le_wo_bom_to_file(void** state) {
	(void)state;

	ReencoderUnicodeStruct struct_actual = _reencoder_test_struct_utf_16_le_valid_long_sequence;
	FILE* fp_tmp = tmpfile();
	if (fp_tmp == NULL) {
		fail_msg("%s", _REENCODER_TEST_FAIL_STRINGS[_REENCODER_TEST_FAIL_TEMP_FILE]);
	}

	size_t bytes_written = reencoder_write_to_file(&struct_actual, fp_tmp, 0);

	_reencoder_test_file_equal(&struct_actual, fp_tmp, bytes_written, 0);

	fclose(fp_tmp);
}

void _reencoder_test_write_utf_16_be_w_bom_to_buffer(void** state) {
	(void)state;

	ReencoderUnicodeStruct struct_actual = _reencoder_test_struct_utf_16_be_valid_long_sequence;
	uint8_t write_buffer[_REENCODER_TEST_NUM_BYTES_UTF_16_VALID_LONG_SEQUENCE + sizeof(_REENCODER_UTF16BE_BOM)] = { 0x00 };

	size_t bytes_written = reencoder_write_to_buffer(&struct_actual, write_buffer, 1);

	_reencoder_test_buffer_equal(&struct_actual, write_buffer, bytes_written, 1);
}

void _reencoder_test_write_utf_16_be_wo_bom_to_buffer(void** state) {
	(void)state;

	ReencoderUnicodeStruct struct_actual = _reencoder_test_struct_utf_16_be_valid_long_sequence;
	uint8_t write_buffer[_REENCODER_TEST_NUM_BYTES_UTF_16_VALID_LONG_SEQUENCE + sizeof(_REENCODER_UTF16BE_BOM)] = { 0x00 };

	size_t bytes_written = reencoder_write_to_buffer(&struct_actual, write_buffer, 0);

	_reencoder_test_buffer_equal(&struct_actual, write_buffer, bytes_written, 0);
}

void _reencoder_test_write_utf_16_be_w_bom_to_file(void** state) {
	(void)state;

	ReencoderUnicodeStruct struct_actual = _reencoder_test_struct_utf_16_be_valid_long_sequence;
	FILE* fp_tmp = tmpfile();
	if (fp_tmp == NULL) {
		fail_msg("%s", _REENCODER_TEST_FAIL_STRINGS[_REENCODER_TEST_FAIL_TEMP_FILE]);
	}

	size_t bytes_written = reencoder_write_to_file(&struct_actual, fp_tmp, 1);

	_reencoder_test_file_equal(&struct_actual, fp_tmp, bytes_written, 1);

	fclose(fp_tmp);
}

void _reencoder_test_write_utf_16_be_wo_bom_to_file(void** state) {
	(void)state;

	ReencoderUnicodeStruct struct_actual = _reencoder_test_struct_utf_16_be_valid_long_sequence;
	FILE* fp_tmp = tmpfile();
	if (fp_tmp == NULL) {
		fail_msg("%s", _REENCODER_TEST_FAIL_STRINGS[_REENCODER_TEST_FAIL_TEMP_FILE]);
	}

	size_t bytes_written = reencoder_write_to_file(&struct_actual, fp_tmp, 0);

	_reencoder_test_file_equal(&struct_actual, fp_tmp, bytes_written, 0);

	fclose(fp_tmp);
}
