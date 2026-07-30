set(_SPHINX_SCRIPT_DIR ${CMAKE_CURRENT_LIST_DIR})

include(FindPackageHandleStandardArgs)

# We are likely to find Sphinx near the Python interpreter
find_package(PythonInterp)
if(PYTHONINTERP_FOUND)
  get_filename_component(_PYTHON_DIR "${PYTHON_EXECUTABLE}" DIRECTORY)
  set(_PYTHON_PATHS
    "${_PYTHON_DIR}"
    "${_PYTHON_DIR}/bin"
    "${_PYTHON_DIR}/Scripts")
endif()

find_program(SPHINX_EXECUTABLE
  NAMES sphinx-build sphinx-build.exe
  HINTS ${_PYTHON_PATHS})
mark_as_advanced(SPHINX_EXECUTABLE)

find_package_handle_standard_args(Sphinx DEFAULT_MSG SPHINX_EXECUTABLE)

# If finding Sphinx fails, there is no use in defining
# add_sphinx_document, so return early
if(NOT Sphinx_FOUND)
    return()
endif()

# add_sphinx_document(
#   <name>
#   MODULES <module-list>
#   STUB_TARGETS <stub-target-list>
#   AUTHORS <author-list>
#   CONF_FILE <conf-py-filename>
#   INDEX_FILE <index-rst-filename>
#   ROOT_FILE <root-rst-template>
#   MODULE_FILE <module-rst-template>
#   ROOT_STUB <root-stub-file>
#   STUBS_DIR <module-stub-directory>
#   [SKIP_HTML] [SKIP_PDF]
#   [<additional-rst-source-files>...])
#
# Function for creating Sphinx documentation targets.
function(add_sphinx_document TARGET_NAME)
  cmake_parse_arguments(${TARGET_NAME}
    "SKIP_HTML;SKIP_PDF"
    "AUTHORS;CONF_FILE;INDEX_FILE;ROOT_FILE;MODULE_FILE;ROOT_STUB;STUBS_DIR"
    "MODULES;STUB_TARGETS"
    ${ARGN})

  get_filename_component(SRCDIR "${${TARGET_NAME}_CONF_FILE}" DIRECTORY)
  get_filename_component(SPHINX_MODULE_DIR
    "${CMAKE_CURRENT_BINARY_DIR}"
    DIRECTORY)
  set(INTDIR "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}/source")
  set(OUTDIR "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}/build")
  string(TIMESTAMP TARGET_YEAR "%Y" UTC)
  set(PROJECT_AUTHORS "${${TARGET_NAME}_AUTHORS}")

  # Configure conf.py.
  add_custom_command(
    OUTPUT "${INTDIR}/conf.py"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${INTDIR}"
    COMMAND "${CMAKE_COMMAND}"
      "-DFILE_IN=${${TARGET_NAME}_CONF_FILE}"
      "-DFILE_OUT=${INTDIR}/conf.py"
      "-DSPHINX_TARGET_NAME=${TARGET_NAME}"
      "-DSPHINX_TARGET_VERSION=${PROJECT_VERSION}"
      "-DSPHINX_TARGET_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}"
      "-DSPHINX_TARGET_VERSION_MINOR=${PROJECT_VERSION_MINOR}"
      "-DSPHINX_TARGET_YEAR=${TARGET_YEAR}"
      "-DSPHINX_TARGET_AUTHORS=${PROJECT_AUTHORS}"
      "-DSPHINX_MODULE_DIR=${SPHINX_MODULE_DIR}"
      -P "${_SPHINX_SCRIPT_DIR}/BuildTimeFile.cmake"
    DEPENDS "${${TARGET_NAME}_CONF_FILE}"
    VERBATIM)

  set(SPHINX_DEPENDS "${INTDIR}/conf.py")

  # Generate the index for the binding library and all active modules.
  set(INDEX_FILE_IN "${${TARGET_NAME}_INDEX_FILE}")
  set(INDEX_FILE_OUT "${INTDIR}/index.rst")
  set(CONF_INDEX_ARGS
    "${INDEX_FILE_IN}"
    --module
    "${TARGET_NAME}"
    ${${TARGET_NAME}_MODULES}
    --output-file
    "${INDEX_FILE_OUT}")
  set(CONF_INDEX "${CMAKE_SOURCE_DIR}/src/scripts/configure_index.py")

  if(WIN32)
    set(CONF_INDEX_CMD "${Python3_EXECUTABLE}" "${CONF_INDEX}")
  else()
    set(CONF_INDEX_CMD "${CONF_INDEX}")
  endif()

  add_custom_command(
    OUTPUT "${INDEX_FILE_OUT}"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${INTDIR}"
    COMMAND ${CONF_INDEX_CMD} ${CONF_INDEX_ARGS}
    DEPENDS "${INDEX_FILE_IN}" "${CONF_INDEX}"
    VERBATIM)

  list(APPEND SPHINX_DEPENDS "${INDEX_FILE_OUT}")

  # Generate the binding-library page.
  set(ROOT_PAGE_OUT "${INTDIR}/${TARGET_NAME}.rst")

  add_custom_command(
    OUTPUT "${ROOT_PAGE_OUT}"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${INTDIR}"
    COMMAND "${CMAKE_COMMAND}"
      "-DFILE_IN=${${TARGET_NAME}_ROOT_FILE}"
      "-DFILE_OUT=${ROOT_PAGE_OUT}"
      "-DSPHINX_TARGET_NAME=${TARGET_NAME}"
      "-DSPHINX_STUB_FILE=stubs/${TARGET_NAME}.pyi"
      -P "${_SPHINX_SCRIPT_DIR}/BuildTimeFile.cmake"
    DEPENDS "${${TARGET_NAME}_ROOT_FILE}"
    VERBATIM)

  list(APPEND SPHINX_DEPENDS "${ROOT_PAGE_OUT}")

  # Copy the generated binding-library stub into the Sphinx source tree.
  set(ROOT_STUB_OUT "${INTDIR}/stubs/${TARGET_NAME}.pyi")

  add_custom_command(
    OUTPUT "${ROOT_STUB_OUT}"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${INTDIR}/stubs"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
      "${${TARGET_NAME}_ROOT_STUB}"
      "${ROOT_STUB_OUT}"
    DEPENDS "${${TARGET_NAME}_ROOT_STUB}"
    VERBATIM)

  list(APPEND SPHINX_DEPENDS "${ROOT_STUB_OUT}")

  # Generate one page and copy one generated stub for each active module.
  foreach(MODULE IN LISTS ${TARGET_NAME}_MODULES)
    set(MODULE_PAGE_OUT "${INTDIR}/${MODULE}.rst")
    set(MODULE_STUB_IN
      "${${TARGET_NAME}_STUBS_DIR}/${MODULE}/__init__.pyi")
    set(MODULE_STUB_OUT
      "${INTDIR}/stubs/${MODULE}/__init__.pyi")

    add_custom_command(
      OUTPUT "${MODULE_PAGE_OUT}"
      COMMAND "${CMAKE_COMMAND}" -E make_directory "${INTDIR}"
      COMMAND "${CMAKE_COMMAND}"
        "-DFILE_IN=${${TARGET_NAME}_MODULE_FILE}"
        "-DFILE_OUT=${MODULE_PAGE_OUT}"
        "-DSPHINX_TARGET_NAME=${TARGET_NAME}"
        "-DSPHINX_MODULE_NAME=${MODULE}"
        "-DSPHINX_STUB_FILE=stubs/${MODULE}/__init__.pyi"
        -P "${_SPHINX_SCRIPT_DIR}/BuildTimeFile.cmake"
      DEPENDS "${${TARGET_NAME}_MODULE_FILE}"
      VERBATIM)

    add_custom_command(
      OUTPUT "${MODULE_STUB_OUT}"
      COMMAND "${CMAKE_COMMAND}" -E make_directory
        "${INTDIR}/stubs/${MODULE}"
      COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "${MODULE_STUB_IN}"
        "${MODULE_STUB_OUT}"
      DEPENDS "${MODULE_STUB_IN}"
      VERBATIM)

    list(APPEND SPHINX_DEPENDS
      "${MODULE_PAGE_OUT}"
      "${MODULE_STUB_OUT}")
  endforeach()

  # Copy any additional Sphinx source files.
  foreach(DOCFILE IN LISTS ${TARGET_NAME}_UNPARSED_ARGUMENTS)
    get_filename_component(DOCFILE_INTDIR "${DOCFILE}" DIRECTORY)
    string(REPLACE
      "${SRCDIR}"
      "${INTDIR}"
      DOCFILE_INTDIR
      "${DOCFILE_INTDIR}")
    get_filename_component(DOCFILE_DEST "${DOCFILE}" NAME)
    set(DOCFILE_DEST "${DOCFILE_INTDIR}/${DOCFILE_DEST}")

    add_custom_command(
      OUTPUT "${DOCFILE_DEST}"
      COMMAND "${CMAKE_COMMAND}" -E make_directory "${DOCFILE_INTDIR}"
      COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "${DOCFILE}"
        "${DOCFILE_DEST}"
      DEPENDS "${DOCFILE}"
      VERBATIM)

    list(APPEND SPHINX_DEPENDS "${DOCFILE_DEST}")
  endforeach()

  set(TARGET_DEPENDS)

  # Build the Sphinx HTML output.
  if(NOT ${TARGET_NAME}_SKIP_HTML)
    add_custom_command(
      OUTPUT "${OUTDIR}/html.stamp"
      COMMAND "${CMAKE_COMMAND}" -E make_directory "${INTDIR}/_static"
      COMMAND "${CMAKE_COMMAND}" -E env
        "SPHINX_AUTODOC_IGNORE_NATIVE_MODULE_TYPE_STUBS=1"
        "${SPHINX_EXECUTABLE}" -M html "${INTDIR}" "${OUTDIR}"
      COMMAND "${CMAKE_COMMAND}" -E touch "${OUTDIR}/html.stamp"
      DEPENDS ${SPHINX_DEPENDS}
      VERBATIM)

    list(APPEND TARGET_DEPENDS "${OUTDIR}/html.stamp")
  endif()

  # Build the PDF output when PdfLaTeX is available.
  if(NOT ${TARGET_NAME}_SKIP_PDF)
    find_package(LATEX COMPONENTS PDFLATEX)

    if(LATEX_PDFLATEX_FOUND)
      add_custom_command(
        OUTPUT "${OUTDIR}/latex/${TARGET_NAME}.tex"
        COMMAND "${CMAKE_COMMAND}" -E env
          "SPHINX_AUTODOC_IGNORE_NATIVE_MODULE_TYPE_STUBS=1"
          "${SPHINX_EXECUTABLE}" -M latex "${INTDIR}" "${OUTDIR}"
        DEPENDS ${SPHINX_DEPENDS}
        VERBATIM)

      add_custom_command(
        OUTPUT "${OUTDIR}/latex/${TARGET_NAME}.pdf"
        COMMAND "${PDFLATEX_COMPILER}" "${TARGET_NAME}.tex"
        COMMAND "${PDFLATEX_COMPILER}" "${TARGET_NAME}.tex"
        COMMAND "${PDFLATEX_COMPILER}" "${TARGET_NAME}.tex"
        WORKING_DIRECTORY "${OUTDIR}/latex"
        DEPENDS "${OUTDIR}/latex/${TARGET_NAME}.tex"
        VERBATIM)

      list(APPEND TARGET_DEPENDS
        "${OUTDIR}/latex/${TARGET_NAME}.pdf")
    else()
      message(WARNING "No PdfLaTeX found. PDF output not available.")
    endif()
  endif()

  add_custom_target(
    ${TARGET_NAME}_DOC
    DEPENDS ${TARGET_DEPENDS})

  if(DEFINED ${TARGET_NAME}_STUB_TARGETS
      AND NOT "${${TARGET_NAME}_STUB_TARGETS}" STREQUAL "")
    add_dependencies(
      ${TARGET_NAME}_DOC
      ${${TARGET_NAME}_STUB_TARGETS})
  endif()

  if(NOT TARGET doc)
    add_custom_target(doc)
  endif()

  add_dependencies(doc ${TARGET_NAME}_DOC)
endfunction()
