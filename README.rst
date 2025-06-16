� reenCoder
===========
A re-implementation of the Unicode standard in C, originally created to deal with UTF-16 in id3v2.3.0 tags.

- Allows for processing of, replacement of malformed characters, and conversion between UTF-8, UTF-16, and UTF-32 string encodings.
- Being a Windows utility library, also comes with some helper functions to display UTF-8 instead of mojibake in the CMD prompt.

  Have you ever seen the true face of πÇÄ ≡ƒç╝ ≡ƒç« ≡ƒç│ ≡ƒç⌐ ≡ƒç┤ ≡ƒç╝ ≡ƒç╕ πÇÅ, exile?

🏭 Usage
---------
- Get the condensed header/source files from the `Releases <https://github.com/maximus-lee-678/reenCoder/releases>`_ tab. (🛠️)
- Add these files to your C project.

1. Functionality is exposed through the ReencoderUnicodeStruct struct.
2. To initialise a struct, use the following:

.. code-block:: c

  ReencoderUnicodeStruct* reencoder_utf8_parse(const uint8_t* string);
  ReencoderUnicodeStruct* reencoder_utf16_parse_uint16(const uint16_t* string, enum ReencoderEncodeType target_endian);
  ReencoderUnicodeStruct* reencoder_utf16_parse_uint8(const uint8_t* string, size_t bytes, enum ReencoderEncodeType source_endian, enum ReencoderEncodeType target_endian);
  ReencoderUnicodeStruct* reencoder_utf32_parse_uint32(const uint32_t* string, enum ReencoderEncodeType target_endian);
  ReencoderUnicodeStruct* reencoder_utf32_parse_uint8(const uint8_t* string, size_t bytes, enum ReencoderEncodeType source_endian, enum ReencoderEncodeType target_endian);

3. To repair a struct containing a malformed string, use:

.. code-block:: c

  unsigned int reencoder_repair_struct(ReencoderUnicodeStruct* unicode_struct);

4. To initialise a struct with a different encode type, use:

.. code-block:: c

  ReencoderUnicodeStruct* reencoder_convert(enum ReencoderEncodeType source_encoding, enum ReencoderEncodeType target_encoding, const void* source_uint_buffer);

5. To get more details about a struct's contents, use the following:

.. code-block:: c

  const char* reencoder_encode_type_as_str(unsigned int encode_type);
  const char* reencoder_outcome_as_str(unsigned int outcome);

6. To make use of the string contents in a struct, use the following:

.. code-block:: c

  size_t reencoder_write_to_buffer(ReencoderUnicodeStruct* unicode_struct, uint8_t* target_buffer, unsigned int write_bom);
  size_t reencoder_write_to_file(ReencoderUnicodeStruct* unicode_struct, FILE* file_pointer, unsigned int write_bom);

7. To duplicate or free a struct, use the following:

.. code-block:: c

  ReencoderUnicodeStruct* reencoder_unicode_struct_duplicate(ReencoderUnicodeStruct* unicode_struct);
  void reencoder_unicode_struct_free(ReencoderUnicodeStruct** unicode_struct);
  
8. To prevent Windows mojibake, use the following:

.. code-block:: c

  void reencoder_locale_init_utf8();
  void reencoder_locale_deinit();

More detailed usage instructions can be found in the header file or the online docs [which doesn't exist yet... :( ].

📕 Documentation
-----------------
(🛠️) website docs soon?

🖥️ VS2022 Setup
---------------
| This program was developed in VS2022 for Windows and tested using cmocka.
| cmocka was installed using `vcpkg <https://github.com/microsoft/vcpkg>`_.

1. Get `latest <https://github.com/microsoft/vcpkg/releases>`_ vcpkg zip file and extract it.
2. Open **Developer Command Prompt for VS** and cd to wherever vckpg has been extracted to.
3. Run:

.. code-block:: console

  bootstrap-vcpkg.bat

4. Run:

.. code-block:: console

  vcpkg.exe integrate install

5. Run:

.. code-block:: console

  vcpkg.exe install cmocka

OR

.. code-block:: console

  vcpkg.exe install cmocka --triplet=x64

6. After installation, in the project: ensure solution configuration (next to debug buttons) is targeting right triplet (x86 or x64).
