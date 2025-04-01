#include "../headers/reencoder_cp_locale.h"

#define _REENCODER_LOCALE_UTF8 ".UTF-8"

/**
 * @brief Sets the locale of the program to UTF-8, overriding default code page usage for certain functions such as mkdir or fopen.
 */
static void reencoder_set_locale_utf8();

/**
 * @brief Reverts locale to system defaults.
 */
static void reencoder_unset_locale();

/**
 * @brief Gets the default code page of the console.
 */
static void reencoder_get_default_cp();

/**
 * @brief Sets the code page of the console to UTF-8, allowing for proper display of UTF-8 characters.
 */
static void reencoder_set_cp_utf8();

/**
 * @brief Reverts to default code page. No-op if default code page is uninitialised.
 */
static void reencoder_unset_cp();

#define CP_UNINITIALISED -1
static int _console_default_code_page = CP_UNINITIALISED;

void reencoder_init_utf8() {
	reencoder_get_default_cp();
	reencoder_set_cp_utf8();
	reencoder_set_locale_utf8();
}

void reencoder_deinit() {
	reencoder_unset_cp();
	reencoder_unset_locale();
}

static void reencoder_set_locale_utf8() {
	setlocale(LC_ALL, _REENCODER_LOCALE_UTF8);
}

static void reencoder_unset_locale() {
	setlocale(LC_ALL, "");
}

static void reencoder_get_default_cp() {
	_console_default_code_page = GetConsoleCP();
}

static void reencoder_set_cp_utf8() {
	SetConsoleOutputCP(CP_UTF8);
}

static void reencoder_unset_cp() {
	if (_console_default_code_page != CP_UNINITIALISED) {
		SetConsoleOutputCP(_console_default_code_page);
	}
}
