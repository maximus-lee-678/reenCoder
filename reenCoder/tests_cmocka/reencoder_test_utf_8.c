#include "reencoder_test_utf_8.h"

void _reencoder_test_valid_utf_8_valid_1_byte(void** state) {
    (void)state;

    ReencoderUnicodeStruct* struct_actual = reencoder_utf8_parse(_reencoder_test_string_utf_8_valid_1_byte);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_8_valid_1_byte, struct_actual);

    *state = struct_actual;
}

void _reencoder_test_valid_utf_8_valid_2_byte(void** state) {
    (void)state;

    ReencoderUnicodeStruct* struct_actual = reencoder_utf8_parse(_reencoder_test_string_utf_8_valid_2_byte);
    _reencoder_test_struct_equal(&_reencoder_test_struct_utf_8_valid_2_byte, struct_actual);

    *state = struct_actual;
}

void _reencoder_test_valid_utf_8_valid_3_byte(void** state) {
    (void)state;

    ReencoderUnicodeStruct* struct_actual = reencoder_utf8_parse(_reencoder_test_string_utf_8_valid_3_byte);
    _reencoder_test_struct_equal(&_reencoder_test_struct_utf_8_valid_3_byte, struct_actual);

    *state = struct_actual;
}

void _reencoder_test_valid_utf_8_valid_4_byte(void** state) {
    (void)state;

    ReencoderUnicodeStruct* struct_actual = reencoder_utf8_parse(_reencoder_test_string_utf_8_valid_4_byte);
    _reencoder_test_struct_equal(&_reencoder_test_struct_utf_8_valid_4_byte, struct_actual);

    *state = struct_actual;
}

void _reencoder_test_valid_utf_8_valid_long_sequence(void** state) {
    (void)state;

    ReencoderUnicodeStruct* struct_actual = reencoder_utf8_parse(_reencoder_test_string_utf_8_valid_long_sequence);
    _reencoder_test_struct_equal(&_reencoder_test_struct_utf_8_valid_long_sequence, struct_actual);
    
    *state = struct_actual;
}

void _reencoder_test_invalid_utf_8_invalid_lead(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf8_parse(_reencoder_test_string_utf_8_invalid_lead);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_8_invalid_lead, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_invalid_utf_8_truncated(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf8_parse(_reencoder_test_string_utf_8_truncated);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_8_truncated, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_invalid_utf_8_invalid_cont(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf8_parse(_reencoder_test_string_utf_8_invalid_cont);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_8_invalid_cont, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_invalid_utf_8_overlong_2(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf8_parse(_reencoder_test_string_utf_8_overlong_2);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_8_overlong_2, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_invalid_utf_8_overlong_3(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf8_parse(_reencoder_test_string_utf_8_overlong_3);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_8_overlong_3, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_invalid_utf_8_overlong_4(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf8_parse(_reencoder_test_string_utf_8_overlong_4);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_8_overlong_4, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_invalid_utf_8_surrogate_pair(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf8_parse(_reencoder_test_string_utf_8_surrogate_pair);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_8_surrogate_pair, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_invalid_utf_8_out_of_range(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf8_parse(_reencoder_test_string_utf_8_out_of_range);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_8_out_of_range, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_valid_utf_8_from_utf_16(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_convert(
		reencoder_is_system_little_endian ? UTF_16LE : UTF_16BE, UTF_8, _reencoder_test_string_utf_16_u16_valid_long_sequence
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_8_valid_long_sequence, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_valid_utf_8_from_utf_32(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_convert(
		reencoder_is_system_little_endian ? UTF_32LE : UTF_32BE, UTF_8, _reencoder_test_string_utf_32_u32_valid_long_sequence
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_8_valid_long_sequence, struct_actual);

	*state = struct_actual;
}

void _reencoder_test_invalid_utf_8_from_utf_16(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_convert(
		reencoder_is_system_little_endian ? UTF_16LE : UTF_16BE, UTF_8, _reencoder_test_string_utf_16_u16_only_high_surrogate
	);

	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_16_only_high_surrogate, struct_actual);
	*state = struct_actual;
}

void _reencoder_test_invalid_utf_8_from_utf_32(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_convert(
		reencoder_is_system_little_endian ? UTF_32LE : UTF_32BE, UTF_8, _reencoder_test_string_utf_32_u32_surrogate
	);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_32_surrogate, struct_actual);

	*state = struct_actual;
}
