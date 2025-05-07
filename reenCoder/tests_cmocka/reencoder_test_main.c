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

	int failed = 0;

	failed = cmocka_run_group_tests(__reencoder_utf_8_test_array, NULL, NULL);
	total_tests += sizeof(__reencoder_utf_8_test_array) / sizeof(__reencoder_utf_8_test_array[0]);
	total_failed += failed;

	failed = cmocka_run_group_tests(__reencoder_utf_16_test_array, NULL, NULL);
	total_tests += sizeof(__reencoder_utf_16_test_array) / sizeof(__reencoder_utf_16_test_array[0]);
	total_failed += failed;

	failed = cmocka_run_group_tests(__reencoder_utf_32_test_array, NULL, NULL);
	total_tests += sizeof(__reencoder_utf_32_test_array) / sizeof(__reencoder_utf_32_test_array[0]);
	total_failed += failed;

	int passed = total_tests - total_failed;
	printf("\n==== TEST SUMMARY ====\n");
	printf("Total tests : %d\n", total_tests);
	printf("Passed      : %d\n", passed);
	printf("Failed      : %d\n", total_failed);
	printf("======================\n");

	return total_failed;
}
