#!/bin/bash
# This script installs all .cmake files in the cmake/tests
# Usage: ./run2.sh [cmake_mode=release] [root_dir=../..]

# cmake -C ../../cmake/tests/release/aos2_epec_fixed_release.cmake ../../
root_dir=${1:-../..}
root_real=$(realpath $root_dir)
mode=${2:-release}
cmake_path=$(realpath $root_dir/cmake/tests/$mode)
cmakes_path=$(ls $cmake_path/*.cmake)

# check if the user is in a venv
if [ -z "$VIRTUAL_ENV" ]; then
  echo "Please activate the virtual environment"
  exit 1
fi

# to be sure
python3 -m pip install build

bad_cmake_files=()
bad_make_files=()

mkdir -p build
cd build

for file in $cmakes_path; do
  file_real=$(realpath $file)
  # make a directory for each file and enter it
  dir=$(basename $file .cmake)
  mkdir -p $dir
  cd $dir
  # run the command
  # cmake -C ../$file ../../../
  # if ! cmake -C ../$file ../../../; then
  if ! cmake -C $file_real $root_real; then
    bad_cmake_files+=($file)
    cd ..
    continue
  fi


  # if make fails add to the list of bad cmake files
  if ! make; then
    bad_make_files+=($file)
  fi
  # install eg. pip install src/libs/cgalpy/dist/CGALPY_kerEpecInt_aos2AlgPl-1.0.0-py3-none-any.whl
  pip install src/libs/cgalpy/dist/*.whl
  # leave the directory
  cd ..
done

# exit build
cd ..


echo ""
echo "----------------------------------------"
echo ""
if [ ${#bad_cmake_files[@]} -eq 0 ]; then
  echo "All CMake files passed"
else
  echo "CMake files that failed:"
  for file in ${bad_cmake_files[@]}; do
    echo $file
  done
echo ""
echo "----------------------------------------"
ehco ""
if [ ${#bad_make_files[@]} -eq 0 ]; then
  echo "All Make files passed"
else
  echo "Make files that failed:"
  for file in ${bad_make_files[@]}; do
    echo $file
  done
fi
echo ""
echo "----------------------------------------"
echo "Done"

