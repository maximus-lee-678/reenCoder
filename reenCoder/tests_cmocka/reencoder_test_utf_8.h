#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "reencoder_test_utf_common.h"
#include "../headers/reencoder_utf_8.h"

static ReencoderUnicodeStruct _reencoder_test_struct_utf_8_valid_1_byte = {	
	.string_type = UTF_8,
	.string_validity = REENCODER_UTF8_VALID,
	.num_bytes = 61,
	.num_chars = 61,
	.string_buffer = (uint8_t*)_reencoder_test_string_utf_8_valid_1_byte
};

static ReencoderUnicodeStruct _reencoder_test_struct_utf_8_valid_2_byte = {
	.string_type = UTF_8,
	.string_validity = REENCODER_UTF8_VALID,
	.num_bytes = 122,
	.num_chars = 68,
	.string_buffer = (uint8_t*)_reencoder_test_string_utf_8_valid_2_byte
};

static ReencoderUnicodeStruct _reencoder_test_struct_utf_8_valid_3_byte = {
	.string_type = UTF_8,
	.string_validity = REENCODER_UTF8_VALID,
	.num_bytes = 55,
	.num_chars = 25,
	.string_buffer = (uint8_t*)_reencoder_test_string_utf_8_valid_3_byte
};

static ReencoderUnicodeStruct _reencoder_test_struct_utf_8_valid_4_byte = {
	.string_type = UTF_8,
	.string_validity = REENCODER_UTF8_VALID,
	.num_bytes = 205,
	.num_chars = 61,
	.string_buffer = (uint8_t*)_reencoder_test_string_utf_8_valid_4_byte
};

static ReencoderUnicodeStruct _reencoder_test_struct_utf_8_valid_long_sequence = {
	.string_type = UTF_8,
	.string_validity = REENCODER_UTF8_VALID,
	.num_bytes = 1592,
	.num_chars = 1592,
	.string_buffer = (uint8_t*)_reencoder_test_string_utf_8_valid_long_sequence
};

static ReencoderUnicodeStruct _reencoder_test_struct_utf_8_invalid_lead = {
	.string_type = UTF_8,
	.string_validity = REENCODER_UTF8_ERR_INVALID_LEAD,
	.num_bytes = 43,
	.num_chars = 0,
	.string_buffer = (uint8_t*)_reencoder_test_string_utf_8_invalid_lead
};

static ReencoderUnicodeStruct _reencoder_test_struct_utf_8_truncated = {
	.string_type = UTF_8,
	.string_validity = REENCODER_UTF8_ERR_PREMATURE_END,
	.num_bytes = 44,
	.num_chars = 0,
	.string_buffer = (uint8_t*)_reencoder_test_string_utf_8_truncated
};

static ReencoderUnicodeStruct _reencoder_test_struct_utf_8_invalid_cont = {
	.string_type = UTF_8,
	.string_validity = REENCODER_UTF8_ERR_INVALID_CONT,
	.num_bytes = 53,
	.num_chars = 0,
	.string_buffer = (uint8_t*)_reencoder_test_string_utf_8_invalid_cont
};

static ReencoderUnicodeStruct _reencoder_test_struct_utf_8_overlong_2 = {
	.string_type = UTF_8,
	.string_validity = REENCODER_UTF8_ERR_OVERLONG_2BYTE,
	.num_bytes = 58,
	.num_chars = 0,
	.string_buffer = (uint8_t*)_reencoder_test_string_utf_8_overlong_2
};

static ReencoderUnicodeStruct _reencoder_test_struct_utf_8_overlong_3 = {
	.string_type = UTF_8,
	.string_validity = REENCODER_UTF8_ERR_OVERLONG_3BYTE,
	.num_bytes = 62,
	.num_chars = 0,
	.string_buffer = (uint8_t*)_reencoder_test_string_utf_8_overlong_3
};

static ReencoderUnicodeStruct _reencoder_test_struct_utf_8_overlong_4 = {
	.string_type = UTF_8,
	.string_validity = REENCODER_UTF8_ERR_OVERLONG_4BYTE,
	.num_bytes = 63,
	.num_chars = 0,
	.string_buffer = (uint8_t*)_reencoder_test_string_utf_8_overlong_4
};

static ReencoderUnicodeStruct _reencoder_test_struct_utf_8_surrogate_pair = {
	.string_type = UTF_8,
	.string_validity = REENCODER_UTF8_ERR_SURROGATE_PAIR,
	.num_bytes = 42,
	.num_chars = 0,
	.string_buffer = (uint8_t*)_reencoder_test_string_utf_8_surrogate_pair
};

static ReencoderUnicodeStruct _reencoder_test_struct_utf_8_out_of_range = {
	.string_type = UTF_8,
	.string_validity = REENCODER_UTF8_ERR_OUT_OF_RANGE,
	.num_bytes = 50,
	.num_chars = 0,
	.string_buffer = (uint8_t*)_reencoder_test_string_utf_8_out_of_range
};

void _reencoder_test_valid_utf_8_valid_1_byte(void** state);
void _reencoder_test_valid_utf_8_valid_2_byte(void** state);
void _reencoder_test_valid_utf_8_valid_3_byte(void** state);
void _reencoder_test_valid_utf_8_valid_4_byte(void** state);
void _reencoder_test_valid_utf_8_valid_long_sequence(void** state);
void _reencoder_test_invalid_utf_8_invalid_lead(void** state);
void _reencoder_test_invalid_utf_8_truncated(void** state);
void _reencoder_test_invalid_utf_8_invalid_cont(void** state);
void _reencoder_test_invalid_utf_8_overlong_2(void** state);
void _reencoder_test_invalid_utf_8_overlong_3(void** state);
void _reencoder_test_invalid_utf_8_overlong_4(void** state);
void _reencoder_test_invalid_utf_8_surrogate_pair(void** state);
void _reencoder_test_invalid_utf_8_out_of_range(void** state);

static struct CMUnitTest __reencoder_utf_8_test_array[] = {
	cmocka_unit_test_teardown(_reencoder_test_valid_utf_8_valid_1_byte, _reencoder_test_teardown_buffer),
	cmocka_unit_test_teardown(_reencoder_test_valid_utf_8_valid_2_byte, _reencoder_test_teardown_buffer),
	cmocka_unit_test_teardown(_reencoder_test_valid_utf_8_valid_3_byte, _reencoder_test_teardown_buffer),
	cmocka_unit_test_teardown(_reencoder_test_valid_utf_8_valid_4_byte, _reencoder_test_teardown_buffer),
	cmocka_unit_test_teardown(_reencoder_test_valid_utf_8_valid_long_sequence, _reencoder_test_teardown_buffer),
	cmocka_unit_test_teardown(_reencoder_test_invalid_utf_8_invalid_lead, _reencoder_test_teardown_buffer),
	cmocka_unit_test_teardown(_reencoder_test_invalid_utf_8_truncated, _reencoder_test_teardown_buffer),
	cmocka_unit_test_teardown(_reencoder_test_invalid_utf_8_invalid_cont, _reencoder_test_teardown_buffer),
	cmocka_unit_test_teardown(_reencoder_test_invalid_utf_8_overlong_2, _reencoder_test_teardown_buffer),
	cmocka_unit_test_teardown(_reencoder_test_invalid_utf_8_overlong_3, _reencoder_test_teardown_buffer),
	cmocka_unit_test_teardown(_reencoder_test_invalid_utf_8_overlong_4, _reencoder_test_teardown_buffer),
	cmocka_unit_test_teardown(_reencoder_test_invalid_utf_8_surrogate_pair, _reencoder_test_teardown_buffer),
	cmocka_unit_test_teardown(_reencoder_test_invalid_utf_8_out_of_range, _reencoder_test_teardown_buffer)
};
