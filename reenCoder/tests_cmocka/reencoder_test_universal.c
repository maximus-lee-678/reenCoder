#include "reencoder_test_universal.h"

void _reencoder_test_free_struct(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf8_parse(_reencoder_test_string_utf_8_valid_1_byte);
	assert_non_null(struct_actual);

	reencoder_unicode_struct_free(&struct_actual);
	assert_null(struct_actual);
}

void _reencoder_test_duplicate_struct(void** state) {
	(void)state;

	ReencoderUnicodeStruct* struct_actual = reencoder_utf8_parse(_reencoder_test_string_utf_8_valid_1_byte);
	assert_non_null(struct_actual);

	ReencoderUnicodeStruct* struct_duplicate = reencoder_unicode_struct_duplicate(struct_actual);
	_reencoder_test_struct_equal(struct_actual, struct_duplicate);

	reencoder_unicode_struct_free(&struct_actual);
	reencoder_unicode_struct_free(&struct_duplicate);
}
