#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "reencoder_test_utf_8.h"
#include "reencoder_test_utf_16.h"
#include "reencoder_test_utf_32.h"

int main(void) {
	int total_tests = 0;
	int total_failed = 0;
	int group_failed = 0;

	group_failed = cmocka_run_group_tests(_reencoder_utf_8_test_array, NULL, NULL);
	total_tests += sizeof(_reencoder_utf_8_test_array) / sizeof(_reencoder_utf_8_test_array[0]);
	total_failed += group_failed;

	group_failed = cmocka_run_group_tests(_reencoder_utf_16_test_array, NULL, NULL);
	total_tests += sizeof(_reencoder_utf_16_test_array) / sizeof(_reencoder_utf_16_test_array[0]);
	total_failed += group_failed;

	group_failed = cmocka_run_group_tests(_reencoder_utf_32_test_array, NULL, NULL);
	total_tests += sizeof(_reencoder_utf_32_test_array) / sizeof(_reencoder_utf_32_test_array[0]);
	total_failed += group_failed;

	printf("\n== TEST SUMMARY ==\n");
	printf("Total tests : %d\n", total_tests);
	printf("Passed      : %d\n", total_tests - total_failed);
	printf("Failed      : %d\n", total_failed);
	printf("==================\n");

	return total_failed;
}
