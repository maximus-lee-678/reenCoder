#include "../headers/reencoder_cp_locale.h"

#define _REENCODER_LOCALE_UTF8 ".UTF-8"
#define _REENCODER_DEFAULT_CODE_PAGE 437

static unsigned int _console_default_code_page = 0;

static void reencoder_set_locale_utf8();
static void reencoder_unset_locale();
static void reencoder_get_default_cp();
static void reencoder_set_cp_utf8();
static void reencoder_unset_cp();

// Quick initialisation function that allows running program to work with UTF-8 related console outputs and file interaction.
void reencoder_init_utf8() {
	reencoder_get_default_cp();
	reencoder_set_cp_utf8();
	reencoder_set_locale_utf8();
}

// Quick deinitialisation function that reverts locale and code page changes made.
void reencoder_deinit() {
	reencoder_unset_cp();
	reencoder_unset_locale();
}

// Sets the locale of the program to UTF-8, 
// overriding default code page usage for certain functions such as mkdir or fopen.
static void reencoder_set_locale_utf8() {
	setlocale(LC_ALL, _REENCODER_LOCALE_UTF8);
}

// Reverts locale to system defaults.
static void reencoder_unset_locale() {
	setlocale(LC_ALL, "");
}

// Gets the default code page of the console. 
static void reencoder_get_default_cp() {
	_console_default_code_page = GetConsoleCP();
}

// Sets the code page of the console to UTF-8, allowing for proper display of UTF-8 characters.
static void reencoder_set_cp_utf8() {
	SetConsoleOutputCP(CP_UTF8);
}

// Reverts to default code page.
static void reencoder_unset_cp() {
	SetConsoleOutputCP(_console_default_code_page != 0 ? _console_default_code_page : _REENCODER_DEFAULT_CODE_PAGE);
}
