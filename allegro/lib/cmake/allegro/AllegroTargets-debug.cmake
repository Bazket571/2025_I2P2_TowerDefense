#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Allegro::allegro_monolith" for configuration "Debug"
set_property(TARGET Allegro::allegro_monolith APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(Allegro::allegro_monolith PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/allegro_monolith-debug.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/allegro_monolith-debug-5.2.dll"
  )

list(APPEND _cmake_import_check_targets Allegro::allegro_monolith )
list(APPEND _cmake_import_check_files_for_Allegro::allegro_monolith "${_IMPORT_PREFIX}/lib/allegro_monolith-debug.lib" "${_IMPORT_PREFIX}/bin/allegro_monolith-debug-5.2.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
