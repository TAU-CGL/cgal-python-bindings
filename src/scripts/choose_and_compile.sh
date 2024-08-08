#!/bin/bash
# This script lets the user choose a cmake file, compile it and install the resulting wheel file.
# Usage: ./choose_and_compile.sh [root_dir=../..]
# cmake_files_path is the path to the directory containing the cmake files
# Example: ./choose_and_compile.sh ../../cmake/tests/test/*.cmake

root_dir=${1:-../..}
root_real=$(realpath $root_dir)
cmakes_path=$root_real/cmake/tests/*.cmake


# check if the user is in a virtual environment, this is useful to me
if [ -z "$VIRTUAL_ENV" ]; then
  echo "Please activate a virtual environment"
  exit 1
fi

python3 -m pip install build

echo "Using CMake files in $cmakes_path"
echo ""
echo "----------------------------------------"
echo ""
echo "Choose a CMake file to compile:"
echo ""

# List all cmake files in the directory
cmake_files=($cmakes_path)
cmake_file_basenames=()
for file in "${cmake_files[@]}"; do
  cmake_file_basenames+=($(basename $file))
done

for i in "${!cmake_file_basenames[@]}"; do
  echo "$i -> ${cmake_file_basenames[$i]}"
done
echo ""

# Ask the user to choose a cmake file
read -p "Enter the number of the cmake file you want to compile: " cmake_file_number

while ! [[ "$cmake_file_number" =~ ^[0-9]+$ ]] || [ "$cmake_file_number" -ge "${#cmake_files[@]}" ]; do
  echo "Invalid input. Please enter a number between 0 and $(( ${#cmake_files[@]} - 1 ))."
  read -p "Enter the number of the cmake file you want to compile: " cmake_file_number
done
# Get the chosen cmake file
cmake_file=${cmake_files[$cmake_file_number]}
dir=$(basename $cmake_file .cmake)

# Ask the user if they want a fixed library name
read -p "Do you want to use a fixed library name? (Y/n): " fixed_lib_name
# Default is yes
if [ -z "$fixed_lib_name" ] || [ "$fixed_lib_name" == "Y" ] || [ "$fixed_lib_name" == "y" ]; then
  fixed_lib_name="set(CGALPY_FIXED_LIBRARY_NAME ON CACHE BOOL \"\" FORCE)"
  lib_name="fixed"
else
  fixed_lib_name="set(CGALPY_FIXED_LIBRARY_NAME OFF CACHE BOOL \"\" FORCE)"
  lib_name="not_fixed"
fi

# Ask the user for build type
read -p "Enter the build type (Release/debug): " build_type
# Default is Release, check if starts with R or r
if [ -z "$build_type" ] || [[ "$build_type" == [Rr]* ]]; then
  build_type="set(CMAKE_BUILD_TYPE \"Release\" CACHE STRING \"\" FORCE)"
  build_str="release"
else
  build_type="set(CMAKE_BUILD_TYPE \"Debug\" CACHE STRING \"\" FORCE)"
  build_str="debug"
fi

# Create a directory for the chosen cmake file and enter it
dirname="$(basename $cmake_file .cmake)_$lib_name"
dirname="$dirname"_"$build_str"
mkdir -p $dirname
cd $dirname

# create the cmake file with user choices
# Write all lines from the original cmake file to the new file
new_filename="$(basename $cmake_file .cmake)_user_choices.cmake"
cat $cmake_file > $new_filename
echo $fixed_lib_name >> $new_filename
echo $build_type >> $new_filename

# Run the cmake command with the new cmake file
if ! cmake -C $new_filename $root_real; then
  echo "CMake command failed"
  exit 1
fi

# Run the make command
if ! make; then
  echo "Make command failed"
  exit 1
fi

# Install the wheel file
if ! pip install --force-reinstall src/libs/cgalpy/dist/*.whl; then
  echo "pip Installation failed"
  exit 1
fi

# Leave the directory
cd ..

echo ""
echo "----------------------------------------"
echo ""
echo "Compilation and installation successful"
echo ""
echo "----------------------------------------"
echo ""

