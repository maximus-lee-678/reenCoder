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
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_16_valid_long_sequence, struct_actual);

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
		_reencoder_test_string_utf_16_u8le_odd, _reencoder_test_struct_utf_16_odd.num_bytes, UTF_16LE, UTF_16LE
	);

	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_16_odd, struct_actual);
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
		_reencoder_test_string_utf_16_u8le_valid_long_sequence, _reencoder_test_struct_utf_16_valid_long_sequence.num_bytes, UTF_16LE, UTF_16LE
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_16_valid_long_sequence, struct_actual);

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

void _reencoder_test_invalid_utf_16_u8le_overlong_sequence(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf16_parse_uint8(
		_reencoder_test_string_utf_16_u8le_overlong, _reencoder_test_struct_utf_16_only_high_surrogate.num_bytes, UTF_16LE, UTF_16LE
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_16_only_high_surrogate, struct_actual);

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
		_reencoder_test_string_utf_16_u8be_valid_long_sequence, _reencoder_test_struct_utf_16_valid_long_sequence.num_bytes, UTF_16BE, UTF_16LE
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_16_valid_long_sequence, struct_actual);

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
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_16_valid_long_sequence, struct_actual);
	*state = struct_actual;
}

void _reencoder_test_valid_utf_16_from_utf_32(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_convert(
		reencoder_is_system_little_endian? UTF_32LE: UTF_32BE, UTF_16LE, _reencoder_test_string_utf_32_u32_valid_long_sequence
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_16_valid_long_sequence, struct_actual);

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
		reencoder_is_system_little_endian ? UTF_32LE : UTF_32BE, UTF_16LE, _reencoder_test_string_utf_32_u32_surrogate
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_32_surrogate, struct_actual);

	*state = struct_actual;
}
