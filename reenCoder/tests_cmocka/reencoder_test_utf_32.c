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
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_32_valid_long_sequence, struct_actual);

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
		_reencoder_test_string_utf_32_u8le_valid_long_sequence, _reencoder_test_struct_utf_32_valid_long_sequence.num_bytes, UTF_32LE, UTF_32LE
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_32_valid_long_sequence, struct_actual);

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
		_reencoder_test_string_utf_32_u8be_valid_long_sequence, _reencoder_test_struct_utf_32_valid_long_sequence.num_bytes, UTF_32BE, UTF_32LE
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_32_valid_long_sequence, struct_actual);

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
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_32_valid_long_sequence, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_valid_utf_32_from_utf_16(void** state) {
	(void)state;
	ReencoderUnicodeStruct* struct_actual = reencoder_convert(
		reencoder_is_system_little_endian ? UTF_16LE : UTF_16BE, UTF_32LE, _reencoder_test_string_utf_16_u16_valid_long_sequence
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_32_valid_long_sequence, struct_actual);
	*state = struct_actual;
}
