if(NOT CGALPY_MACROS_FILE_INCLUDED)
  set(CGALPY_MACROS_FILE_INCLUDED 1)

  macro(hide_variable var)
    set (${var} ${${var}} CACHE INTERNAL "Variable hidden from user" FORCE)
  endmacro()

  function(echo_target_property tgt prop)
    # v for value, d for defined, s for set
    get_property(v TARGET ${tgt} PROPERTY ${prop})
    get_property(d TARGET ${tgt} PROPERTY ${prop} DEFINED)
    get_property(s TARGET ${tgt} PROPERTY ${prop} SET)
    # only produce output for values that are set
    if(s)
      message("tgt='${tgt}' prop='${prop}'")
      message("  value='${v}'")
      message("  defined='${d}'")
      message("  set='${s}'")
      message("")
    endif()
  endfunction()

  function(echo_target tgt)
    if(NOT TARGET ${tgt})
      message("There is no target named '${tgt}'")
      return()
    endif()
    set(props
        DEBUG_OUTPUT_NAME
	DEBUG_POSTFIX
	RELEASE_OUTPUT_NAME
	RELEASE_POSTFIX
	ARCHIVE_OUTPUT_DIRECTORY
	ARCHIVE_OUTPUT_DIRECTORY_DEBUG
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE
	ARCHIVE_OUTPUT_NAME
	ARCHIVE_OUTPUT_NAME_DEBUG
	ARCHIVE_OUTPUT_NAME_RELEASE
	AUTOMOC
	AUTOMOC_MOC_OPTIONS
	BUILD_WITH_INSTALL_RPATH
	BUNDLE
	BUNDLE_EXTENSION
	COMPILE_DEFINITIONS
	COMPILE_DEFINITIONS_DEBUG
	COMPILE_DEFINITIONS_RELEASE
	COMPILE_FLAGS
	DEBUG_POSTFIX
	RELEASE_POSTFIX
	DEFINE_SYMBOL
	ENABLE_EXPORTS
	EXCLUDE_FROM_ALL
	EchoString
	FOLDER
	FRAMEWORK
	Fortran_FORMAT
	Fortran_MODULE_DIRECTORY
	GENERATOR_FILE_NAME
	GNUtoMS
	HAS_CXX
	IMPLICIT_DEPENDS_INCLUDE_TRANSFORM
	IMPORTED
	IMPORTED_CONFIGURATIONS
	IMPORTED_IMPLIB
	IMPORTED_IMPLIB_DEBUG
	IMPORTED_IMPLIB_RELEASE
	IMPORTED_LINK_DEPENDENT_LIBRARIES
	IMPORTED_LINK_DEPENDENT_LIBRARIES_DEBUG
	IMPORTED_LINK_DEPENDENT_LIBRARIES_RELEASE
	IMPORTED_LINK_INTERFACE_LANGUAGES
	IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG
	IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE
	IMPORTED_LINK_INTERFACE_LIBRARIES
	IMPORTED_LINK_INTERFACE_LIBRARIES_DEBUG
	IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE
	IMPORTED_LINK_INTERFACE_MULTIPLICITY
	IMPORTED_LINK_INTERFACE_MULTIPLICITY_DEBUG
	IMPORTED_LINK_INTERFACE_MULTIPLICITY_RELEASE
	IMPORTED_LOCATION
	IMPORTED_LOCATION_DEBUG
	IMPORTED_LOCATION_RELEASE
	IMPORTED_NO_SONAME
	IMPORTED_NO_SONAME_DEBUG
	IMPORTED_NO_SONAME_RELEASE
	IMPORTED_SONAME
	IMPORTED_SONAME_DEBUG
	IMPORTED_SONAME_RELEASE
	IMPORT_PREFIX
	IMPORT_SUFFIX
	INCLUDE_DIRECTORIES
	INSTALL_NAME_DIR
	INSTALL_RPATH
	INSTALL_RPATH_USE_LINK_PATH
	INTERPROCEDURAL_OPTIMIZATION
	INTERPROCEDURAL_OPTIMIZATION_DEBUG
	INTERPROCEDURAL_OPTIMIZATION_RELEASE
	LABELS
	LIBRARY_OUTPUT_DIRECTORY
	LIBRARY_OUTPUT_DIRECTORY_DEBUG
	LIBRARY_OUTPUT_DIRECTORY_RELEASE
	LIBRARY_OUTPUT_NAME
	LIBRARY_OUTPUT_NAME_DEBUG
	LIBRARY_OUTPUT_NAME_RELEASE
	LINKER_LANGUAGE
	LINK_DEPENDS
	LINK_FLAGS
	LINK_FLAGS_DEBUG
	LINK_FLAGS_RELEASE
	LINK_INTERFACE_LIBRARIES
	LINK_INTERFACE_LIBRARIES_DEBUG
	LINK_INTERFACE_LIBRARIES_RELEASE
	LINK_INTERFACE_MULTIPLICITY
	LINK_INTERFACE_MULTIPLICITY_DEBUG
	LINK_INTERFACE_MULTIPLICITY_RELEASE
	LINK_SEARCH_END_STATIC
	LINK_SEARCH_START_STATIC
	LOCATION
	LOCATION_DEBUG
	LOCATION_RELEASE
	MACOSX_BUNDLE
	MACOSX_BUNDLE_INFO_PLIST
	MACOSX_FRAMEWORK_INFO_PLIST
	MAP_IMPORTED_CONFIG_DEBUG
	MAP_IMPORTED_CONFIG_RELEASE
	OSX_ARCHITECTURES
	OSX_ARCHITECTURES_DEBUG
	OSX_ARCHITECTURES_RELEASE
	OUTPUT_NAME
	OUTPUT_NAME_DEBUG
	OUTPUT_NAME_RELEASE
	POST_INSTALL_SCRIPT
	PREFIX
	PRE_INSTALL_SCRIPT
	PRIVATE_HEADER
	PROJECT_LABEL
	PUBLIC_HEADER
	RESOURCE
	RULE_LAUNCH_COMPILE
	RULE_LAUNCH_CUSTOM
	RULE_LAUNCH_LINK
	RUNTIME_OUTPUT_DIRECTORY
	RUNTIME_OUTPUT_DIRECTORY_DEBUG
	RUNTIME_OUTPUT_DIRECTORY_RELEASE
	RUNTIME_OUTPUT_NAME
	RUNTIME_OUTPUT_NAME_DEBUG
	RUNTIME_OUTPUT_NAME_RELEASE
	SKIP_BUILD_RPATH
	SOURCES
	SOVERSION
	STATIC_LIBRARY_FLAGS
	STATIC_LIBRARY_FLAGS_DEBUG
	STATIC_LIBRARY_FLAGS_RELEASE
	SUFFIX
	TYPE
	VERSION
	VS_DOTNET_REFERENCES
	VS_GLOBAL_WHATEVER
	VS_GLOBAL_KEYWORD
	VS_GLOBAL_PROJECT_TYPES
	VS_KEYWORD
	VS_SCC_AUXPATH
	VS_SCC_LOCALPATH
	VS_SCC_PROJECTNAME
	VS_SCC_PROVIDER
	VS_WINRT_EXTENSIONS
	VS_WINRT_REFERENCES
	WIN32_EXECUTABLE
	XCODE_ATTRIBUTE_WHATEVER)

    message("======================== ${tgt} ========================")
    foreach(p ${props})
      echo_target_property("${t}" "${p}")
    endforeach()
    message("")
  endfunction()

  function(echo_targets)
    set(tgts ${ARGV})
    foreach(t ${tgts})
      echo_target("${t}")
    endforeach()
  endfunction()

  # Capitalize first letter (compatible with cmake 2.6)
  function(capitalize_first param)
    set(str ${${param}})
    # Get first letter and capitalize.
    string(SUBSTRING ${str} 0 1 first-letter)
    string(TOUPPER ${first-letter} first-letter)
    # Get the rest of the macro name.
    string(LENGTH ${str} length)
    # Substract one from the length.
    math(EXPR length "${length} - 1")
    string(SUBSTRING ${str} 1 ${length} rest)
    # Compose macro-name with first letter uppercase.
    set(${param} "${first-letter}${rest}" PARENT_SCOPE)
  endfunction()

  function(max a b res)
    if (${a} GREATER ${b})
      set(${res} ${a} PARENT_SCOPE)
    else ()
      set(${res} ${b} PARENT_SCOPE)
    endif ()
  endfunction()

  function(reportWithEmphasis name path relp)
    set(prefix_len "3")
    set(suffix_len "3")

    set(info_name "name: ${name}")
    set(info_path "path: ${path}")
    set(info_relp "relative install path: ${relp}")

    string(LENGTH ${info_name} len_name)
    string(LENGTH ${info_path} len_path)
    string(LENGTH ${info_relp} len_relp)

    max(${len_name} ${len_path} res1)
    max(${len_name} ${len_relp} res2)
    max(${res1} ${res2} res)
    math(EXPR middle_len "${res} + 1" OUTPUT_FORMAT DECIMAL)

    math(EXPR max_len "${prefix_len} + ${suffix_len} + ${middle_len} + 2" OUTPUT_FORMAT DECIMAL)
    math(EXPR reminder_name "${max_len} - (${prefix_len} + ${suffix_len} + ${len_name} + 3)" OUTPUT_FORMAT DECIMAL)
    math(EXPR reminder_path "${max_len} - (${prefix_len} + ${suffix_len} + ${len_path} + 3)" OUTPUT_FORMAT DECIMAL)
    math(EXPR reminder_relp "${max_len} - (${prefix_len} + ${suffix_len} + ${len_relp} + 3)" OUTPUT_FORMAT DECIMAL)

    # 1
    set (line_sep "")
    foreach(i RANGE ${max_len})
      set(line_sep "${line_sep}X")
    endforeach(i)

    # 2
    set (line_empt "")
    foreach(i RANGE ${prefix_len})
      set(line_empt "${line_empt}X")
    endforeach(i)
    foreach(i RANGE ${middle_len})
      set(line_empt "${line_empt} ")
    endforeach(i)
    foreach(i RANGE ${suffix_len})
      set(line_empt "${line_empt}X")
    endforeach(i)

    # 3
    set (line_name "")
    foreach(i RANGE ${prefix_len})
      set(line_name "${line_name}X")
    endforeach(i)
    set(line_name "${line_name} ${info_name}")
    foreach(i RANGE ${reminder_name})
      set(line_name "${line_name} ")
    endforeach(i)
    foreach(i RANGE ${suffix_len})
      set(line_name "${line_name}X")
    endforeach(i)

    # 4
    set (line_path "")
    foreach(i RANGE ${prefix_len})
      set(line_path "${line_path}X")
    endforeach(i)
    set(line_path "${line_path} ${info_path}")
    foreach(i RANGE ${reminder_path})
      set(line_path "${line_path} ")
    endforeach(i)
    foreach(i RANGE ${suffix_len})
      set(line_path "${line_path}X")
    endforeach(i)

    # 5
    set (line_relp "")
    foreach(i RANGE ${prefix_len})
      set(line_relp "${line_relp}X")
    endforeach(i)
    set(line_relp "${line_relp} ${info_relp}")
    foreach(i RANGE ${reminder_relp})
      set(line_relp "${line_relp} ")
    endforeach(i)
    foreach(i RANGE ${suffix_len})
      set(line_relp "${line_relp}X")
    endforeach(i)

    # Print out
    message("${line_sep}")
    message("${line_empt}")
    message("${line_name}")
    message("${line_path}")
    message("${line_relp}")
    message("${line_empt}")
    message("${line_sep}")
  endfunction()
endif()
