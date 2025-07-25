#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "reencoder_test_utf_definitions.h"
#include "reencoder_test_utf_8.h"
#include "reencoder_test_utf_32.h"
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

static ReencoderUnicodeStruct _reencoder_test_struct_utf_16_le_valid_long_sequence = {
	.string_type = UTF_16LE,
	.string_validity = REENCODER_UTF16_VALID,
	.num_bytes = 3184,
	.num_chars = 1592,
	.string_buffer = (uint8_t*)_reencoder_test_string_utf_16_u8le_valid_long_sequence
};
static ReencoderUnicodeStruct _reencoder_test_struct_utf_16_be_valid_long_sequence = { // used only in write buffer for BOM checks
	.string_type = UTF_16BE,
	.string_validity = REENCODER_UTF16_VALID,
	.num_bytes = 3184,
	.num_chars = 1592,
	.string_buffer = (uint8_t*)_reencoder_test_string_utf_16_u8be_valid_long_sequence
};
#define _REENCODER_TEST_NUM_BYTES_UTF_16_VALID_LONG_SEQUENCE 3184 // for use in declaring stack buffer size during testing

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

static ReencoderUnicodeStruct _reencoder_test_struct_utf_16_odd_padded = {
	.string_type = UTF_16LE,
	.string_validity = REENCODER_UTF16_ERR_ODD_LENGTH,
	.num_bytes = 56,
	.num_chars = 0,
	.string_buffer = (uint8_t*)_reencoder_test_string_utf_16_u8le_odd_padded
};
static unsigned int _reencoder_test_added_bytes_utf_16_u8le_odd_padded = 1; // padding from odd sequence auto-correction (1 byte -> 2 bytes)

static ReencoderUnicodeStruct _reencoder_test_struct_utf_16_repair_fixed = {
	.string_type = UTF_16LE,
	.string_validity = REENCODER_UTF16_VALID_REPAIRED,
	.num_bytes = 196,
	.num_chars = 98,
	.string_buffer = (uint8_t*)_reencoder_test_string_utf_16_repair_fixed
};
static unsigned int _reencoder_test_added_bytes_utf_16_u8le_repair_fixed = 1; // padding from odd sequence auto-correction (1 byte -> 2 bytes)

// UTF-16 self-checks
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

// Other encodings to UTF-16
void _reencoder_test_valid_utf_16_from_utf_8(void** state);
void _reencoder_test_valid_utf_16_from_utf_32(void** state);
void _reencoder_test_invalid_utf_16_from_utf_8(void** state);
void _reencoder_test_invalid_utf_16_from_utf_32(void** state);

// Repairs
void _reencoder_test_fix_utf_16(void** state);

// Write-outs
void _reencoder_test_write_utf_16_le_w_bom_to_buffer(void** state);
void _reencoder_test_write_utf_16_le_wo_bom_to_buffer(void** state);
void _reencoder_test_write_utf_16_le_w_bom_to_file(void** state);
void _reencoder_test_write_utf_16_le_wo_bom_to_file(void** state);
void _reencoder_test_write_utf_16_be_w_bom_to_buffer(void** state);
void _reencoder_test_write_utf_16_be_wo_bom_to_buffer(void** state);
void _reencoder_test_write_utf_16_be_w_bom_to_file(void** state);
void _reencoder_test_write_utf_16_be_wo_bom_to_file(void** state);

static struct CMUnitTest _reencoder_utf_16_test_array[] = {
	// UTF-16 uint16_t
	cmocka_unit_test_teardown(_reencoder_test_valid_utf_16_u16_valid_2_byte, _reencoder_test_teardown_struct),
	cmocka_unit_test_teardown(_reencoder_test_valid_utf_16_u16_valid_4_byte, _reencoder_test_teardown_struct),
	cmocka_unit_test_teardown(_reencoder_test_valid_utf_16_u16_valid_long_sequence, _reencoder_test_teardown_struct),
	cmocka_unit_test_teardown(_reencoder_test_invalid_utf_16_u16_only_high_surrogate_sequence, _reencoder_test_teardown_struct),
	cmocka_unit_test_teardown(_reencoder_test_invalid_utf_16_u16_only_low_surrogate_sequence, _reencoder_test_teardown_struct),
	cmocka_unit_test_teardown(_reencoder_test_invalid_utf_16_u16_odd_sequence, _reencoder_test_teardown_struct),
	// UTF-16 uint8_t LE
	cmocka_unit_test_teardown(_reencoder_test_valid_utf_16_u8le_valid_2_byte, _reencoder_test_teardown_struct),
	cmocka_unit_test_teardown(_reencoder_test_valid_utf_16_u8le_valid_4_byte, _reencoder_test_teardown_struct),
	cmocka_unit_test_teardown(_reencoder_test_valid_utf_16_u8le_valid_long_sequence, _reencoder_test_teardown_struct),
	cmocka_unit_test_teardown(_reencoder_test_invalid_utf_16_u8le_only_high_surrogate_sequence, _reencoder_test_teardown_struct),
	cmocka_unit_test_teardown(_reencoder_test_invalid_utf_16_u8le_only_low_surrogate_sequence, _reencoder_test_teardown_struct),
	// UTF-16 uint8_t BE
	cmocka_unit_test_teardown(_reencoder_test_valid_utf_16_u8be_valid_2_byte, _reencoder_test_teardown_struct),
	cmocka_unit_test_teardown(_reencoder_test_valid_utf_16_u8be_valid_4_byte, _reencoder_test_teardown_struct),
	cmocka_unit_test_teardown(_reencoder_test_valid_utf_16_u8be_valid_long_sequence, _reencoder_test_teardown_struct),
	cmocka_unit_test_teardown(_reencoder_test_invalid_utf_16_u8be_only_high_surrogate_sequence, _reencoder_test_teardown_struct),
	cmocka_unit_test_teardown(_reencoder_test_invalid_utf_16_u8be_only_low_surrogate_sequence, _reencoder_test_teardown_struct),
	// Other encodings to UTF-16
	cmocka_unit_test_teardown(_reencoder_test_valid_utf_16_from_utf_8, _reencoder_test_teardown_struct),
	cmocka_unit_test_teardown(_reencoder_test_valid_utf_16_from_utf_32, _reencoder_test_teardown_struct),
	cmocka_unit_test_teardown(_reencoder_test_invalid_utf_16_from_utf_8, _reencoder_test_teardown_struct),
	cmocka_unit_test_teardown(_reencoder_test_invalid_utf_16_from_utf_32, _reencoder_test_teardown_struct),
	// Repairs
	cmocka_unit_test_teardown(_reencoder_test_fix_utf_16, _reencoder_test_teardown_struct),
	// Write-outs
	cmocka_unit_test(_reencoder_test_write_utf_16_le_w_bom_to_buffer),
	cmocka_unit_test(_reencoder_test_write_utf_16_le_wo_bom_to_buffer),
	cmocka_unit_test(_reencoder_test_write_utf_16_le_w_bom_to_file),
	cmocka_unit_test(_reencoder_test_write_utf_16_le_wo_bom_to_file),
	cmocka_unit_test(_reencoder_test_write_utf_16_be_w_bom_to_buffer),
	cmocka_unit_test(_reencoder_test_write_utf_16_be_wo_bom_to_buffer),
	cmocka_unit_test(_reencoder_test_write_utf_16_be_w_bom_to_file),
	cmocka_unit_test(_reencoder_test_write_utf_16_be_wo_bom_to_file)
};