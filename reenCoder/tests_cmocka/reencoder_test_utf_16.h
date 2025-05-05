#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "reencoder_test_utf_strings.h"
#include "../headers/reencoder_utf_16.h"

static ReencoderUnicodeStruct _reencoder_test_struct_utf_16_2_byte = {
	.string_type = UTF_16LE,
	.string_validity = REENCODER_UTF16_VALID,
	.num_bytes = 124,
	.num_chars = 62,
	.string_buffer = (uint8_t*)_reencoder_test_string_utf_16_u8le_2_byte
};

static ReencoderUnicodeStruct _reencoder_test_struct_utf_16_4_byte = {
	.string_type = UTF_16LE,
	.string_validity = REENCODER_UTF16_VALID,
	.num_bytes = 220,
	.num_chars = 62,
	.string_buffer = (uint8_t*)_reencoder_test_string_utf_16_u8le_4_byte
};

static ReencoderUnicodeStruct _reencoder_test_struct_utf_16_long_sequence = {
	.string_type = UTF_16LE,
	.string_validity = REENCODER_UTF16_VALID,
	.num_bytes = 3184,
	.num_chars = 1592,
	.string_buffer = (uint8_t*)_reencoder_test_string_utf_16_u8le_long_sequence
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

void _reencoder_test_valid_utf_16_u16_2_byte_sequence(void** state);
void _reencoder_test_valid_utf_16_u16_4_byte_sequence(void** state);
void _reencoder_test_valid_utf_16_u16_long_sequence(void** state);
void _reencoder_test_invalid_utf_16_u16_only_high_surrogate_sequence(void** state);
void _reencoder_test_invalid_utf_16_u16_only_low_surrogate_sequence(void** state);
void _reencoder_test_invalid_utf_16_u16_odd_sequence(void** state);

void _reencoder_test_valid_utf_16_u8le_2_byte_sequence(void** state);
void _reencoder_test_valid_utf_16_u8le_4_byte_sequence(void** state);
void _reencoder_test_valid_utf_16_u8le_long_sequence(void** state);
void _reencoder_test_invalid_utf_16_u8le_only_high_surrogate_sequence(void** state);
void _reencoder_test_invalid_utf_16_u8le_only_low_surrogate_sequence(void** state);

void _reencoder_test_valid_utf_16_u8be_2_byte_sequence(void** state);
void _reencoder_test_valid_utf_16_u8be_4_byte_sequence(void** state);
void _reencoder_test_valid_utf_16_u8be_long_sequence(void** state);
void _reencoder_test_invalid_utf_16_u8be_only_high_surrogate_sequence(void** state);
void _reencoder_test_invalid_utf_16_u8be_only_low_surrogate_sequence(void** state);

static struct CMUnitTest __reencoder_utf_16_test_array[] = {
	// uint16_t
	cmocka_unit_test(_reencoder_test_valid_utf_16_u16_2_byte_sequence),
	cmocka_unit_test(_reencoder_test_valid_utf_16_u16_4_byte_sequence),
	cmocka_unit_test(_reencoder_test_valid_utf_16_u16_long_sequence),
	cmocka_unit_test(_reencoder_test_invalid_utf_16_u16_only_high_surrogate_sequence),
	cmocka_unit_test(_reencoder_test_invalid_utf_16_u16_only_low_surrogate_sequence),
	cmocka_unit_test(_reencoder_test_invalid_utf_16_u16_odd_sequence),
	// uint8_t LE
	cmocka_unit_test(_reencoder_test_valid_utf_16_u8le_2_byte_sequence),
	cmocka_unit_test(_reencoder_test_valid_utf_16_u8le_4_byte_sequence),
	cmocka_unit_test(_reencoder_test_valid_utf_16_u8le_long_sequence),
	cmocka_unit_test(_reencoder_test_invalid_utf_16_u8le_only_high_surrogate_sequence),
	cmocka_unit_test(_reencoder_test_invalid_utf_16_u8le_only_low_surrogate_sequence),
	// uint8_t BE
	cmocka_unit_test(_reencoder_test_valid_utf_16_u8be_2_byte_sequence),
	cmocka_unit_test(_reencoder_test_valid_utf_16_u8be_4_byte_sequence),
	cmocka_unit_test(_reencoder_test_valid_utf_16_u8be_long_sequence),
	cmocka_unit_test(_reencoder_test_invalid_utf_16_u8be_only_high_surrogate_sequence),
	cmocka_unit_test(_reencoder_test_invalid_utf_16_u8be_only_low_surrogate_sequence)
};