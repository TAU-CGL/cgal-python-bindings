#!/bin/bash
# This script compiles for all examples in the example2cmake file
# Usage: ./compile_for_examples.sh [cmakes_mode=release] [root_path=../../]

if [ -z "$VIRTUAL_ENV" ]; then
  echo "Please activate a virtual environment"
  exit 1
fi
# to be sure
python3 -m pip install --quiet build

mode=${1:-release}

root_path=${2:-../../}
root_dir=$(realpath $root_path)
cmakes_path=$(realpath $root_dir/cmake/tests/$mode)
examples_path=$(realpath $root_dir/src/python_scripts/cgalpy_examples)
cmake2example_path=$(realpath $root_dir/src/scripts/example2cmake/cmake2example.txt)

full_cmakes_path=$cmakes_path
full_examples_path=$examples_path
full_cmake2example_path=$cmake2example_path


echo ""
echo "----------------------------------------"
echo ""
echo "Using CMake files in $full_cmakes_path"
echo "Using examples in $full_examples_path"
echo "Using cmake2example file in $full_cmake2example_path"
echo ""
echo "----------------------------------------"
echo ""

# create a dictonary cmake_file -> example_files
declare -A cmake2example
while IFS= read -r line
do
  IFS=' ' read -r -a array <<< "$line"
  cmake_file=${array[0]}_$mode.cmake
  example_files=${array[@]:1}
  cmake2example[$cmake_file]=$example_files
done < $full_cmake2example_path

mkdir -p build
cd build

cmake_file_missings=()
cmake_fails=()
make_fails=()
pip_fails=()
successes=()

for key in "${!cmake2example[@]}"
do
  if [ ! -f $full_cmakes_path/$key ]; then
    echo "CMake file $key does not exist"
    cmake_file_missings+=($key)
    continue
  fi
  example_files=${cmake2example[$key]}
  cmake_file=$full_cmakes_path/$key
  dir=$(basename $cmake_file .cmake)
  mkdir -p $dir
  cd $dir
  echo "Compiling $(basename $cmake_file)"
  if ! cmake -C $cmake_file $root_dir; then
    echo "CMake command failed"
    cmake_fails+=($key)
    cd ..
    continue
  fi
  # make and install the wheel file
  if ! make; then
    echo "Make command failed"
    make_fails+=($key)
    cd ..
    continue
  fi
  if ! pip install src/libs/cgalpy/dist/*.whl; then
    echo "pip Installation failed"
    pip_fails+=($key)
    cd ..
    continue
  fi
  successes+=($key)
  cd ..
done

# exit build
cd ..

echo ""
echo "----------------------------------------"
echo ""
if [ ${#cmake_file_missings[@]} -eq 0 ]; then
  echo "All cmake2example files exist in $full_cmakes_path"
else
  echo "CMake files that are missing:"
  for file in ${cmake_file_missings[@]}; do
    echo $file
  done
fi

echo ""
echo "----------------------------------------"
echo ""
if [ ${#cmake_fails[@]} -eq 0 ]; then
  echo "All CMake commands passed"
else
  echo "CMakes that failed to create Makefiles:"
  for file in ${cmake_fails[@]}; do
    echo $file
  done
fi

echo ""
echo "----------------------------------------"
echo ""
if [ ${#make_fails[@]} -eq 0 ]; then
  echo "All Make commands passed"
else
  echo "Makes that failed to compile:"
  for file in ${make_fails[@]}; do
    echo $file
  done
fi

echo ""
echo "----------------------------------------"
echo ""
if [ ${#pip_fails[@]} -eq 0 ]; then
  echo "All pip installs passed"
else
  echo "Pip installations that failed:"
  for file in ${pip_fails[@]}; do
    echo $file
  done
fi

echo ""
echo "----------------------------------------"
echo ""
if [ ${#successes[@]} -eq 0 ]; then
  echo "No example CMake files were compiled and installed"
else
  echo "Examples CMake files that were compiled and installed:"
  for file in ${successes[@]}; do
    echo $file
  done
fi
echo ""
echo "----------------------------------------"

