#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "reencoder_test_utf_definitions.h"

// Struct operations
void _reencoder_test_free_struct(void** state);
void _reencoder_test_duplicate_struct(void** state);

static struct CMUnitTest _reencoder_universal_test_array[] = {
	// Struct operations
	cmocka_unit_test(_reencoder_test_free_struct),
	cmocka_unit_test(_reencoder_test_duplicate_struct)
};
