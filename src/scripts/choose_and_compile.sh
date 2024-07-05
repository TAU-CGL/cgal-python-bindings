#!/bin/bash
# This script lets the user choose a cmake file, compile it and install the resulting wheel file.
# Usage: ./choose_and_compile.sh [root_dir=../..]
# cmake_files_path is the path to the directory containing the cmake files
# Example: ./choose_and_compile.sh ../../cmake/tests/release/*.cmake

root_dir=${1:-../..}
root_real=$(realpath $root_dir)
cmakes_path=$root_real/cmake/tests/release/*.cmake


# check if the user is in a virtual environment, this is useful to me
if [ -z "$VIRTUAL_ENV" ]; then
  echo "Please activate a virtual environment"
  exit 1
fi

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

# Create a directory for the chosen cmake file and enter it
mkdir -p $dir
cd $dir

# Run the cmake command
if ! cmake -C ../$cmake_file ../../../; then
  echo "CMake command failed"
  exit 1
fi

# Run the make command
if ! make; then
  echo "Make command failed"
  exit 1
fi

# Install the wheel file
if ! pip install src/libs/cgalpy/dist/*.whl; then
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

