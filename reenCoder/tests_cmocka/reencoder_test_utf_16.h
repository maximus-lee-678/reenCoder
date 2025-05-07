#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "reencoder_test_utf_common.h"
#include "../headers/reencoder_utf_16.h"

static ReencoderUnicodeStruct _reencoder_test_struct_utf_16_valid_2_byte = {
	.string_type = UTF_16LE,
	.string_validity = REENCODER_UTF16_VALID,
	.num_bytes = 124,
	.num_chars = 62,
	.string_buffer = (uint8_t*)_reencoder_test_string_utf_16_u8le_valid_2_byte
};

static ReencoderUnicodeStruct _reencoder_test_struct_utf_16_valid_4_byte = {
	.string_type = UTF_16LE,
	.string_validity = REENCODER_UTF16_VALID,
	.num_bytes = 220,
	.num_chars = 62,
	.string_buffer = (uint8_t*)_reencoder_test_string_utf_16_u8le_valid_4_byte
};

static ReencoderUnicodeStruct _reencoder_test_struct_utf_16_valid_long_sequence = {
	.string_type = UTF_16LE,
	.string_validity = REENCODER_UTF16_VALID,
	.num_bytes = 3184,
	.num_chars = 1592,
	.string_buffer = (uint8_t*)_reencoder_test_string_utf_16_u8le_valid_long_sequence
};

static ReencoderUnicodeStruct _reencoder_test_struct_utf_16_only_high_surrogate = {
	.string_type = UTF_16LE,
	.string_validity = REENCODER_UTF16_ERR_UNPAIRED_HIGH,
	.num_bytes = 80,
	.num_chars = 0,
	.string_buffer = (uint8_t*)_reencoder_test_string_utf_16_u8le_only_high_surrogate
};

static ReencoderUnicodeStruct _reencoder_test_struct_utf_16_only_low_surrogate = {
	.string_type = UTF_16LE,
	.string_validity = REENCODER_UTF16_ERR_UNPAIRED_LOW,
	.num_bytes = 80,
	.num_chars = 0,
	.string_buffer = (uint8_t*)_reencoder_test_string_utf_16_u8le_only_low_surrogate
};

static ReencoderUnicodeStruct _reencoder_test_struct_utf_16_odd = {
	.string_type = UTF_16LE,
	.string_validity = REENCODER_UTF16_ERR_ODD_LENGTH,
	.num_bytes = 55,
	.num_chars = 0,
	.string_buffer = (uint8_t*)_reencoder_test_string_utf_16_u8le_odd
};

void _reencoder_test_valid_utf_16_u16_valid_2_byte(void** state);
void _reencoder_test_valid_utf_16_u16_valid_4_byte(void** state);
void _reencoder_test_valid_utf_16_u16_valid_long_sequence(void** state);
void _reencoder_test_invalid_utf_16_u16_only_high_surrogate_sequence(void** state);
void _reencoder_test_invalid_utf_16_u16_only_low_surrogate_sequence(void** state);
void _reencoder_test_invalid_utf_16_u16_odd_sequence(void** state);

void _reencoder_test_valid_utf_16_u8le_valid_2_byte(void** state);
void _reencoder_test_valid_utf_16_u8le_valid_4_byte(void** state);
void _reencoder_test_valid_utf_16_u8le_valid_long_sequence(void** state);
void _reencoder_test_invalid_utf_16_u8le_only_high_surrogate_sequence(void** state);
void _reencoder_test_invalid_utf_16_u8le_only_low_surrogate_sequence(void** state);

void _reencoder_test_valid_utf_16_u8be_valid_2_byte(void** state);
void _reencoder_test_valid_utf_16_u8be_valid_4_byte(void** state);
void _reencoder_test_valid_utf_16_u8be_valid_long_sequence(void** state);
void _reencoder_test_invalid_utf_16_u8be_only_high_surrogate_sequence(void** state);
void _reencoder_test_invalid_utf_16_u8be_only_low_surrogate_sequence(void** state);

static struct CMUnitTest __reencoder_utf_16_test_array[] = {
	// uint16_t
	cmocka_unit_test_teardown(_reencoder_test_valid_utf_16_u16_valid_2_byte, _reencoder_test_teardown_buffer),
	cmocka_unit_test_teardown(_reencoder_test_valid_utf_16_u16_valid_4_byte, _reencoder_test_teardown_buffer),
	cmocka_unit_test_teardown(_reencoder_test_valid_utf_16_u16_valid_long_sequence, _reencoder_test_teardown_buffer),
	cmocka_unit_test_teardown(_reencoder_test_invalid_utf_16_u16_only_high_surrogate_sequence, _reencoder_test_teardown_buffer),
	cmocka_unit_test_teardown(_reencoder_test_invalid_utf_16_u16_only_low_surrogate_sequence, _reencoder_test_teardown_buffer),
	cmocka_unit_test_teardown(_reencoder_test_invalid_utf_16_u16_odd_sequence, _reencoder_test_teardown_buffer),
	// uint8_t LE
	cmocka_unit_test_teardown(_reencoder_test_valid_utf_16_u8le_valid_2_byte, _reencoder_test_teardown_buffer),
	cmocka_unit_test_teardown(_reencoder_test_valid_utf_16_u8le_valid_4_byte, _reencoder_test_teardown_buffer),
	cmocka_unit_test_teardown(_reencoder_test_valid_utf_16_u8le_valid_long_sequence, _reencoder_test_teardown_buffer),
	cmocka_unit_test_teardown(_reencoder_test_invalid_utf_16_u8le_only_high_surrogate_sequence, _reencoder_test_teardown_buffer),
	cmocka_unit_test_teardown(_reencoder_test_invalid_utf_16_u8le_only_low_surrogate_sequence, _reencoder_test_teardown_buffer),
	// uint8_t BE
	cmocka_unit_test_teardown(_reencoder_test_valid_utf_16_u8be_valid_2_byte, _reencoder_test_teardown_buffer),
	cmocka_unit_test_teardown(_reencoder_test_valid_utf_16_u8be_valid_4_byte, _reencoder_test_teardown_buffer),
	cmocka_unit_test_teardown(_reencoder_test_valid_utf_16_u8be_valid_long_sequence, _reencoder_test_teardown_buffer),
	cmocka_unit_test_teardown(_reencoder_test_invalid_utf_16_u8be_only_high_surrogate_sequence, _reencoder_test_teardown_buffer),
	cmocka_unit_test_teardown(_reencoder_test_invalid_utf_16_u8be_only_low_surrogate_sequence, _reencoder_test_teardown_buffer)
};