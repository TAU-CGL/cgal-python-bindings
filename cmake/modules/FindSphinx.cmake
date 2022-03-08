set(CGALPY_SPHINX_SCRIPT_DIR ${CMAKE_CURRENT_LIST_DIR})

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
#   CONF_FILE <conf-py-filename>
#   INDEX_FILE <index-rst-filename>
#   [C_API <c-api-header-file>]
#   [SKIP_HTML] [SKIP_PDF]
#   <rst-src-file>...)
#
# Function for creating Sphinx documentation targets.
function(add_sphinx_document TARGET_NAME)
  cmake_parse_arguments(${TARGET_NAME}
    "SKIP_HTML;SKIP_PDF"
    "SRC_DIR;CONF_FILE;INDEX_FILE"
    "MODULES"
    ${ARGN})
  get_filename_component(SRCDIR "${${TARGET_NAME}_CONF_FILE}" DIRECTORY)
  set(INTDIR "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}/source")
  set(OUTDIR "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}/build")
  foreach(line IN LISTS MODULES)
    string(APPEND MULTILINE_MODULES "   ${line}\\\n")
  endforeach()
  string(TIMESTAMP SPHINX_TARGET_YEAR "%Y" UTC)

  # handle fonf.py
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
    "-DSPHINX_TARGET_YEAR=${SPHINX_TARGET_YEAR}"
    "-DSPHINX_MODULE_DIR=${CMAKE_CURRENT_BINARY_DIR}"
    -P "${CGALPY_SPHINX_SCRIPT_DIR}/BuildTimeFile.cmake"
    DEPENDS "${${TARGET_NAME}_CONF_FILE}")

  set(SPHINX_DEPENDS "${INTDIR}/conf.py")

  # handle index.rst
  add_custom_command(
    OUTPUT "${INTDIR}/index.rst"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${INTDIR}"
    COMMAND "${CMAKE_COMMAND}"
    "-DFILE_IN=${${TARGET_NAME}_INDEX_FILE}"
    "-DFILE_OUT=${INTDIR}/index.rst"
    "-DSPHINX_TARGET_NAME=${TARGET_NAME}"
    "-DSPHINX_MULTILINE_MODULES=${MULTILINE_MODULES}"
    -P "${CGALPY_SPHINX_SCRIPT_DIR}/BuildTimeFile.cmake"
    DEPENDS "${${TARGET_NAME}_CONF_FILE}")

  set(SPHINX_DEPENDS ${SPHINX_DEPENDS} "${INTDIR}/index.rst")

  # handle all <module>.rst files
  foreach(MODULE ${MODULES})
    add_custom_command(
      OUTPUT "${INTDIR}/${MODULE}.rst"
      COMMAND "${CMAKE_COMMAND}" -E make_directory "${INTDIR}"
      COMMAND "${CMAKE_COMMAND}"
      "-DFILE_IN=${${TARGET_NAME}_SRC_DIR}/${MODULE}.rst"
      "-DFILE_OUT=${INTDIR}/${MODULE}.rst"
      "-DSPHINX_TARGET_NAME=${TARGET_NAME}"
      -P "${CGALPY_SPHINX_SCRIPT_DIR}/BuildTimeFile.cmake"
      DEPENDS "${${TARGET_NAME}_CONF_FILE}")

    set(SPHINX_DEPENDS ${SPHINX_DEPENDS} "${INTDIR}/${MODULE}.rst")
  endforeach()

  # Copy the Sphinx source files to the intermediate directory
  foreach(DOCFILE ${${TARGET_NAME}_UNPARSED_ARGUMENTS})
    get_filename_component(DOCFILE_INTDIR "${DOCFILE}" DIRECTORY)
    string(REPLACE "${SRCDIR}" "${INTDIR}" DOCFILE_INTDIR "${DOCFILE_INTDIR}")
    get_filename_component(DOCFILE_DEST "${DOCFILE}" NAME)
    set(DOCFILE_DEST "${DOCFILE_INTDIR}/${DOCFILE_DEST}")
    add_custom_command(
      OUTPUT "${DOCFILE_DEST}"
      COMMAND "${CMAKE_COMMAND}" -E make_directory "${DOCFILE_INTDIR}"
      COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${DOCFILE}" "${DOCFILE_DEST}"
      DEPENDS "${DOCFILE}")

    list(APPEND SPHINX_DEPENDS "${DOCFILE_DEST}")
  endforeach()

  # Build the Sphinx HTML output
  set(TARGET_DEPENDS)
  if(NOT ${TARGET_NAME}_SKIP_HTML)
    add_custom_command(OUTPUT "${OUTDIR}/html.stamp"
      # Create the _static directory required by Sphinx in case it
      # wasn't added as one of the source files
      COMMAND "${CMAKE_COMMAND}" -E make_directory "${INTDIR}/_static"
      COMMAND "${SPHINX_EXECUTABLE}" -M html "${INTDIR}" "${OUTDIR}"
      COMMAND "${CMAKE_COMMAND}" -E touch "${OUTDIR}/html.stamp"
      DEPENDS ${SPHINX_DEPENDS})
    list(APPEND TARGET_DEPENDS "${OUTDIR}/html.stamp")
  endif()

  # Build the pdf output
  if(NOT ${TARGET_NAME}_SKIP_PDF)
    find_package(LATEX COMPONENTS PDFLATEX)
    if(LATEX_PDFLATEX_FOUND)
      add_custom_command(
        OUTPUT "${OUTDIR}/latex/${TARGET_NAME}.tex"
        COMMAND "${SPHINX_EXECUTABLE}" -M latex "${INTDIR}" "${OUTDIR}"
        DEPENDS ${SPHINX_DEPENDS})
      add_custom_command(
        OUTPUT "${OUTDIR}/latex/${TARGET_NAME}.pdf"
        # Three times' the charm for PdfLaTeX to get all xrefs right
        COMMAND "${PDFLATEX_COMPILER}" "${TARGET_NAME}.tex"
        COMMAND "${PDFLATEX_COMPILER}" "${TARGET_NAME}.tex"
        COMMAND "${PDFLATEX_COMPILER}" "${TARGET_NAME}.tex"
        WORKING_DIRECTORY "${OUTDIR}/latex"
        DEPENDS "${OUTDIR}/latex/${TARGET_NAME}.tex")
      list(APPEND TARGET_DEPENDS "${OUTDIR}/latex/${TARGET_NAME}.pdf")
    else()
      message(WARNING "No PdfLaTeX found. PDF output not available.")
    endif()
  endif()

  # create the CMake targets for the documentation
  add_custom_target(
    ${TARGET_NAME}
    DEPENDS ${TARGET_DEPENDS})
  if(NOT TARGET doc)
    add_custom_target(doc)
  endif()
  add_dependencies(doc ${TARGET_NAME})
endfunction()
