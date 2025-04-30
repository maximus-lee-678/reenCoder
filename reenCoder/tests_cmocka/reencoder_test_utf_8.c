#include "reencoder_test_utf_8.h"

int _reencoder_test_teardown_buffer(void** state) {
    ReencoderUnicodeStruct* s = *state;
    if (s) {
        free(s->string_buffer);
        free(s);
    }

    return 0;
}

void _reencoder_test_struct_equal(ReencoderUnicodeStruct* expected, ReencoderUnicodeStruct* actual) {
    assert_non_null(actual);
    assert_int_equal(expected->string_type, actual->string_type);
    assert_int_equal(expected->string_validity, actual->string_validity);
    assert_int_equal(expected->num_bytes, actual->num_bytes);
    assert_int_equal(expected->num_chars, actual->num_chars);
    assert_memory_equal(expected->string_buffer, actual->string_buffer, expected->num_bytes);
}

void _reencoder_test_valid_utf_8_1_byte_sequence(void** state) {
    (void)state;

    ReencoderUnicodeStruct* struct_actual = reencoder_utf8_parse(_reencoder_test_string_utf_8_1_byte);
	_reencoder_test_struct_equal(&_reencoder_test_struct_utf_8_1_byte, struct_actual);

    *state = struct_actual;
}

void _reencoder_test_valid_utf_8_2_byte_sequence(void** state) {
    (void)state;

    ReencoderUnicodeStruct* struct_actual = reencoder_utf8_parse(_reencoder_test_string_utf_8_2_byte);
    _reencoder_test_struct_equal(&_reencoder_test_struct_utf_8_2_byte, struct_actual);

    *state = struct_actual;
}

void _reencoder_test_valid_utf_8_3_byte_sequence(void** state) {
    (void)state;

    ReencoderUnicodeStruct* struct_actual = reencoder_utf8_parse(_reencoder_test_string_utf_8_3_byte);
    _reencoder_test_struct_equal(&_reencoder_test_struct_utf_8_3_byte, struct_actual);

    *state = struct_actual;
}

void _reencoder_test_valid_utf_8_4_byte_sequence(void** state) {
    (void)state;

    ReencoderUnicodeStruct* struct_actual = reencoder_utf8_parse(_reencoder_test_string_utf_8_4_byte);
    _reencoder_test_struct_equal(&_reencoder_test_struct_utf_8_4_byte, struct_actual);

    *state = struct_actual;
}

void _reencoder_test_valid_utf_8_long_sequence(void** state) {
    (void)state;

    ReencoderUnicodeStruct* struct_actual = reencoder_utf8_parse(_reencoder_test_string_utf_8_long_sequence);
    _reencoder_test_struct_equal(&_reencoder_test_struct_utf_8_long_sequence, struct_actual);
    
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
