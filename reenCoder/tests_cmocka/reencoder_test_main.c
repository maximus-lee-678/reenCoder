#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "reencoder_test_utf_8.h"
#include "reencoder_test_utf_16.h"
#include "reencoder_test_utf_32.h"

int main(void) {
    int result = 0;

    result |= cmocka_run_group_tests(__reencoder_utf_8_test_array, NULL, NULL);
	result |= cmocka_run_group_tests(__reencoder_utf_16_test_array, NULL, NULL);
    
    return result;
}
