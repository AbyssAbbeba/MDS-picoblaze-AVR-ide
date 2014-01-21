# ==============================================================================
#
#
#
# ==============================================================================


# ------------------------------------------------------------------------------
# GENERAL OPTIONS
# ------------------------------------------------------------------------------

# Set project name
project ( boost )

# ------------------------------------------------------------------------------
# COMPILATION OPTIONS
# ------------------------------------------------------------------------------

# List of preprocessor include file search directories.
include_directories ( "${CMAKE_CURRENT_SOURCE_DIR}/.." )

## Add a library to the project using the specified source files,
 # in this case it will procude dynamic C++ library.
add_library ( ${PROJECT_NAME} SHARED
              # Boost system.
              system/error_code.cpp

              # Boost filesystem.
              filesystem/path.cpp
              filesystem/operations.cpp
              filesystem/path_traits.cpp
              filesystem/unique_path.cpp
              filesystem/portability.cpp
              filesystem/utf8_codecvt_facet.cpp
              filesystem/windows_file_codecvt.cpp
              filesystem/codecvt_error_category.cpp

              # Boost regex.
              regex/icu.cpp
              regex/regex.cpp
              regex/cregex.cpp
              regex/fileiter.cpp
              regex/posix_api.cpp
              regex/instances.cpp
              regex/winstances.cpp
              regex/regex_debug.cpp
              regex/usinstances.cpp
              regex/static_mutex.cpp
              regex/wide_posix_api.cpp
              regex/c_regex_traits.cpp
              regex/wc_regex_traits.cpp
              regex/regex_raw_buffer.cpp
              regex/w32_regex_traits.cpp
              regex/cpp_regex_traits.cpp
              regex/regex_traits_defaults.cpp )
