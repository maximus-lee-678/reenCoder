// Shoutout to the mojibake produced by non-ascii strings in C, this one's for you!!

#pragma once

#include <Windows.h>
#include <locale.h>

/**
 * @brief Express initialisation function that allows running program to work with UTF-8 related console outputs and file interaction.
 */
void reencoder_init_utf8();

/**
 * @brief Express deinitialisation function that reverts locale and code page changes made by reencoder_set_locale_utf8().
 * 
 * Running this without calling reencoder_init_utf8() first will cause no changes to be made to the active code page.
 */
void reencoder_deinit();
