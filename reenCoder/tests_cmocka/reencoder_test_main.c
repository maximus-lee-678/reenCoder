#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "reencoder_test_universal.h"
#include "reencoder_test_utf_8.h"
#include "reencoder_test_utf_16.h"
#include "reencoder_test_utf_32.h"
#include "../Consolidator/consolidator.h"

#define REENCODER_OUTPUT_FILE_NAME "reencoder.h"
static const char* REENCODER_FILE_NAMES_ROOT[] = {
	"headers/reencoder_cp_locale.h",
	"headers/reencoder_utf_common.h",
	"headers/reencoder_utf_8.h",
	"headers/reencoder_utf_16.h",
	"headers/reencoder_utf_32.h",
	"source/reencoder_cp_locale.c",
	"source/reencoder_utf_common.c",
	"source/reencoder_utf_8.c",
	"source/reencoder_utf_16.c",
	"source/reencoder_utf_32.c"
};
static const char* REENCODER_FILE_NAMES_FROM_TEST_DIR[] = {
	"../headers/reencoder_cp_locale.h",
	"../headers/reencoder_utf_common.h",
	"../headers/reencoder_utf_8.h",
	"../headers/reencoder_utf_16.h",
	"../headers/reencoder_utf_32.h",
	"../source/reencoder_cp_locale.c",
	"../source/reencoder_utf_common.c",
	"../source/reencoder_utf_8.c",
	"../source/reencoder_utf_16.c",
	"../source/reencoder_utf_32.c"
};
static const char* REENCODER_FILE_NAMES_FROM_DEBUG[] = {
	"../../reenCoder/headers/reencoder_cp_locale.h",
	"../../reenCoder/headers/reencoder_utf_common.h",
	"../../reenCoder/headers/reencoder_utf_8.h",
	"../../reenCoder/headers/reencoder_utf_16.h",
	"../../reenCoder/headers/reencoder_utf_32.h",
	"../../reenCoder/source/reencoder_cp_locale.c",
	"../../reenCoder/source/reencoder_utf_common.c",
	"../../reenCoder/source/reencoder_utf_8.c",
	"../../reenCoder/source/reencoder_utf_16.c",
	"../../reenCoder/source/reencoder_utf_32.c"
};

int main(void) {
	int total_tests = 0;
	int total_failed = 0;
	int group_failed = 0;

	group_failed = cmocka_run_group_tests(_reencoder_universal_test_array, NULL, NULL);
	total_tests += sizeof(_reencoder_universal_test_array) / sizeof(_reencoder_universal_test_array[0]);
	total_failed += group_failed;

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

	if (total_failed == 0) {
		uint8_t buf_cwd[512] = { '\0' };
		consolidator_get_working_dir(buf_cwd, 512);

		if (!consolidator_main(REENCODER_OUTPUT_FILE_NAME, NULL, 10, REENCODER_FILE_NAMES_ROOT, 0, NULL)) {
			printf("Consolidated files written to %s at %s (Root).\n", REENCODER_OUTPUT_FILE_NAME, buf_cwd);
		}
		else if (!consolidator_main(REENCODER_OUTPUT_FILE_NAME, NULL, 10, REENCODER_FILE_NAMES_FROM_TEST_DIR, 0, NULL)) {
			printf("Consolidated files written to %s at %s (Test Dir).\n", REENCODER_OUTPUT_FILE_NAME, buf_cwd);
		}
		else if (!consolidator_main(REENCODER_OUTPUT_FILE_NAME, NULL, 10, REENCODER_FILE_NAMES_FROM_DEBUG, 0, NULL)) {
			printf("Consolidated files written to %s at %s (Debug Folder).\n", REENCODER_OUTPUT_FILE_NAME, buf_cwd);
		}
		else {
			printf("Could not find files to consolidate! Working directory: %s\n", buf_cwd);
		}
	}
	else {
		printf("Did not consolidate files due to %d error(s) present.\n", total_failed);
	}

	return total_failed;
}
