#pragma once

#include <Windows.h>
#include <locale.h>

/**
 * @brief Express initialisation function that allows running program to work with UTF-8 related console outputs and file interaction.
 * 
 * @return void
 */
void reencoder_locale_init_utf8();

/**
 * @brief Express deinitialisation function that reverts locale and code page changes made by _reencoder_locale_set_locale_utf8().
 * 
 * Running this without calling reencoder_locale_init_utf8() first will cause no changes to be made to the active code page.
 * 
 * @return void
 */
void reencoder_locale_deinit();
