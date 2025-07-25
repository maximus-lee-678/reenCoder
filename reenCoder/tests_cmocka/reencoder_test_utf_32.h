#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "reencoder_test_utf_definitions.h"
#include "reencoder_test_utf_8.h"
#include "reencoder_test_utf_16.h"
#include "../headers/reencoder_utf_32.h"

static ReencoderUnicodeStruct _reencoder_test_struct_utf_32_valid = {
	.string_type = UTF_32LE,
	.string_validity = REENCODER_UTF32_VALID,
	.num_bytes = 436,
	.num_chars = 109,
	.string_buffer = (uint8_t*)_reencoder_test_string_utf_32_u8le_valid
};

static ReencoderUnicodeStruct _reencoder_test_struct_utf_32_le_valid_long_sequence = {
	.string_type = UTF_32LE,
	.string_validity = REENCODER_UTF32_VALID,
	.num_bytes = 6368,
	.num_chars = 1592,
	.string_buffer = (uint8_t*)_reencoder_test_string_utf_32_u8le_valid_long_sequence
};
static ReencoderUnicodeStruct _reencoder_test_struct_utf_32_be_valid_long_sequence = { // used only in write buffer for BOM checks
	.string_type = UTF_32BE,
	.string_validity = REENCODER_UTF32_VALID,
	.num_bytes = 6368,
	.num_chars = 1592,
	.string_buffer = (uint8_t*)_reencoder_test_string_utf_32_u8be_valid_long_sequence
};
#define _REENCODER_TEST_NUM_BYTES_UTF_32_VALID_LONG_SEQUENCE 6368 // for use in declaring stack buffer size during testing

static ReencoderUnicodeStruct _reencoder_test_struct_utf_32_surrogate = {
	.string_type = UTF_32LE,
	.string_validity = REENCODER_UTF32_ERR_SURROGATE,
	.num_bytes = 160,
	.num_chars = 0,
	.string_buffer = (uint8_t*)_reencoder_test_string_utf_32_u8le_surrogate
};

static ReencoderUnicodeStruct _reencoder_test_struct_utf_32_out_of_range = {
	.string_type = UTF_32LE,
	.string_validity = REENCODER_UTF32_ERR_OUT_OF_RANGE,
	.num_bytes = 184,
	.num_chars = 0,
	.string_buffer = (uint8_t*)_reencoder_test_string_utf_32_u8le_out_of_range
};

static ReencoderUnicodeStruct _reencoder_test_struct_utf_32_repair_fixed = {
	.string_type = UTF_32LE,
	.string_validity = REENCODER_UTF32_VALID_REPAIRED,
	.num_bytes = 288,
	.num_chars = 72,
	.string_buffer = (uint8_t*)_reencoder_test_string_utf_32_repair_fixed
};

// UTF-32 self-checks
void _reencoder_test_valid_utf_32_u32_valid(void** state);
void _reencoder_test_valid_utf_32_u32_valid_long_sequence(void** state);
void _reencoder_test_invalid_utf_32_u32_surrogate(void** state);
void _reencoder_test_invalid_utf_32_u32_out_of_range(void** state);

void _reencoder_test_valid_utf_32_u8le_valid(void** state);
void _reencoder_test_valid_utf_32_u8le_valid_long_sequence(void** state);
void _reencoder_test_invalid_utf_32_u8le_surrogate(void** state);
void _reencoder_test_invalid_utf_32_u8le_out_of_range(void** state);

void _reencoder_test_valid_utf_32_u8be_valid(void** state);
void _reencoder_test_valid_utf_32_u8be_valid_long_sequence(void** state);
void _reencoder_test_invalid_utf_32_u8be_surrogate(void** state);
void _reencoder_test_invalid_utf_32_u8be_out_of_range(void** state);

// Other encodings to UTF-32
void _reencoder_test_valid_utf_32_from_utf_8(void** state);
void _reencoder_test_valid_utf_32_from_utf_16(void** state);
void _reencoder_test_invalid_utf_32_from_utf_8(void** state);
void _reencoder_test_invalid_utf_32_from_utf_16(void** state);

// Repairs
void _reencoder_test_fix_utf_32(void** state);

// Write-outs
void _reencoder_test_write_utf_32_le_w_bom_to_buffer(void** state);
void _reencoder_test_write_utf_32_le_wo_bom_to_buffer(void** state);
void _reencoder_test_write_utf_32_le_w_bom_to_file(void** state);
void _reencoder_test_write_utf_32_le_wo_bom_to_file(void** state);
void _reencoder_test_write_utf_32_be_w_bom_to_buffer(void** state);
void _reencoder_test_write_utf_32_be_wo_bom_to_buffer(void** state);
void _reencoder_test_write_utf_32_be_w_bom_to_file(void** state);
void _reencoder_test_write_utf_32_be_wo_bom_to_file(void** state);

static struct CMUnitTest _reencoder_utf_32_test_array[] = {
	// UTF-32 uint32_t
	cmocka_unit_test_teardown(_reencoder_test_valid_utf_32_u32_valid, _reencoder_test_teardown_struct),
	cmocka_unit_test_teardown(_reencoder_test_valid_utf_32_u32_valid_long_sequence, _reencoder_test_teardown_struct),
	cmocka_unit_test_teardown(_reencoder_test_invalid_utf_32_u32_surrogate, _reencoder_test_teardown_struct),
	cmocka_unit_test_teardown(_reencoder_test_invalid_utf_32_u32_out_of_range, _reencoder_test_teardown_struct),
	// UTF-32 uint8_t LE
	cmocka_unit_test_teardown(_reencoder_test_valid_utf_32_u8le_valid, _reencoder_test_teardown_struct),
	cmocka_unit_test_teardown(_reencoder_test_valid_utf_32_u8le_valid_long_sequence, _reencoder_test_teardown_struct),
	cmocka_unit_test_teardown(_reencoder_test_invalid_utf_32_u8le_surrogate, _reencoder_test_teardown_struct),
	cmocka_unit_test_teardown(_reencoder_test_invalid_utf_32_u8le_out_of_range, _reencoder_test_teardown_struct),
	// UTF-32 uint8_t BE
	cmocka_unit_test_teardown(_reencoder_test_valid_utf_32_u8be_valid, _reencoder_test_teardown_struct),
	cmocka_unit_test_teardown(_reencoder_test_valid_utf_32_u8be_valid_long_sequence, _reencoder_test_teardown_struct),
	cmocka_unit_test_teardown(_reencoder_test_invalid_utf_32_u8be_surrogate, _reencoder_test_teardown_struct),
	cmocka_unit_test_teardown(_reencoder_test_invalid_utf_32_u8be_out_of_range, _reencoder_test_teardown_struct),
	// Other encodings to UTF-32
	cmocka_unit_test_teardown(_reencoder_test_valid_utf_32_from_utf_8, _reencoder_test_teardown_struct),
	cmocka_unit_test_teardown(_reencoder_test_valid_utf_32_from_utf_16, _reencoder_test_teardown_struct),
	cmocka_unit_test_teardown(_reencoder_test_invalid_utf_32_from_utf_8, _reencoder_test_teardown_struct),
	cmocka_unit_test_teardown(_reencoder_test_invalid_utf_32_from_utf_16, _reencoder_test_teardown_struct),
	// Repairs
	cmocka_unit_test_teardown(_reencoder_test_fix_utf_32, _reencoder_test_teardown_struct),
	// Write-outs
	cmocka_unit_test(_reencoder_test_write_utf_32_le_w_bom_to_buffer),
	cmocka_unit_test(_reencoder_test_write_utf_32_le_wo_bom_to_buffer),
	cmocka_unit_test(_reencoder_test_write_utf_32_le_w_bom_to_file),
	cmocka_unit_test(_reencoder_test_write_utf_32_le_wo_bom_to_file),
	cmocka_unit_test(_reencoder_test_write_utf_32_be_w_bom_to_buffer),
	cmocka_unit_test(_reencoder_test_write_utf_32_be_wo_bom_to_buffer),
	cmocka_unit_test(_reencoder_test_write_utf_32_be_w_bom_to_file),
	cmocka_unit_test(_reencoder_test_write_utf_32_be_wo_bom_to_file)
};