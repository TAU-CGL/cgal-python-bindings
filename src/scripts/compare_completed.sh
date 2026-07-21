#!/bin/bash
# This script compares the output of all examples present in the cmake2example file to the output of C++ programs
# Usage: ./compare_completed.sh <cgal_path> [root_path=../../]
# cgal_path is the path to the root of the CGAL C++ library
# root_path is the path to the root of the project

# check if the user is in a virtual environment, this is useful to me
if [ -z "$VIRTUAL_ENV" ]; then
 echo "Please activate a virtual environment before running this script"
 exit 1
fi

no_cpp=()
compile_error=()
python_error=()
cpp_error=()
different_output=()
successes=()

if [ -z "$1" ]; then
  echo "Usage: $0 <cgal_path> [root_path=../../]"
  exit 1
fi

cgal_path=$(realpath $1)
# check if the path exists
if [ ! -d "$cgal_path" ]; then
  echo "Path $cgal_path does not exist"
  echo "Usage: $0 <cgal_path> [root_path=../../]"
  exit 1
fi

root_path=${2:-../../}
root_dir=$(realpath $root_path)
# check if the path exists
if [ ! -d "$root_dir" ]; then
  echo "Path $root_dir does not exist"
  echo "Usage: $0 <cgal_path> [root_path=../../]"
  exit 1
fi

# save the current directory
current_dir=$(pwd)

# get the cmake2example file
cmake2example_path=$(realpath $root_dir/src/scripts/example2cmake/cmake2example.txt)
# aos2_alg_epec algebraic_curves.py algebraic_segments.py
# aos2_conic_epec conic_multiplicities.py conics.py
# aos2_linear_epec io_unbounded.py
# aos2_seg_ae_epec dcel_extension.py dcel_extension_io.py face_extension.py face_extension_overlay.py overlay_color.py
# aos2_seg_epec batched_point_location.py incremental_insertion.py point_location.py
# bso2_seg_epec sequence.py
# ms2_conic_ccr exact_offset.py
# ms2_cs_epec approx_inset.py approx_offset.py
# ms2_seg_epec sum_by_decomposition.py
# get all lines that end with .py
examples=$(grep -oP '.*\.py' $cmake2example_path)

for example in $examples; do
  echo ""
  echo "-------------------------------------------------"
  echo "Processing $example"
  echo "-------------------------------------------------"
  echo ""
  # look for the file recursively in the current directory
  real_file=$(find . -name $example)
  if [ -z "$real_file" ]; then
    echo "Example $example was not found in the current directory"
    not_found+=($example)
    continue
  fi
  directory=$(dirname $real_file)
  raw_name=$(basename $example .py)
  cpp_file=$(find $cgal_path -name "$raw_name.cpp")
  if [ -z "$cpp_file" ]; then
    echo "C++ file $raw_name.cpp was not found in the CGAL library"
    no_cpp+=($example)
    continue
  fi
  # enter the directory to execute the Python script
  cd $directory
  if ! output=$(python3 $example); then
    msg="Error executing $example"
    echo ""
    echo $msg
    python_error+=($example)
    cd $current_dir
    continue
  fi
  cpp_full_path=$(realpath $cpp_file)
  if ! res=$(g++ -lgmp -lmpfr -lboost_thread -lboost_system -I $cgal_path/include -o $raw_name $cpp_full_path); then
    msg="Error compiling $cpp_file"
    echo ""
    echo $msg
    compile_error+=($example)
    cd $current_dir
    continue
  fi
  if ! cpp_output=$($raw_name); then
    msg="Error executing C++ program $raw_name"
    echo ""
    echo $msg
    cpp_error+=($example)
    cd $current_dir
    continue
  fi
  if [ "$output" != "$cpp_output" ]; then
    msg="Different output for $example"
    echo ""
    echo $msg
    different_output+=($example)
    cd $current_dir
    continue
  fi
  successes+=($example)
  cd $current_dir
done

echo ""
echo "-------------------------------------------------"
echo "Results"
echo "-------------------------------------------------"
echo ""
echo "No C++ file found for the following examples:"
for example in ${no_cpp[@]}; do
  echo $example
done
echo ""
echo "Error executing the Python script for the following examples:"
for example in ${python_error[@]}; do
  echo $example
done
echo ""
echo "Error compiling the following examples:"
for example in ${compile_error[@]}; do
  echo $example
done
echo ""
echo "Error executing the C++ program for the following examples:"
for example in ${cpp_error[@]}; do
  echo $example
done
echo ""
echo "Different output for the following examples:"
for example in ${different_output[@]}; do
  echo $example
done
echo ""
echo "The following examples have the same output for the Python script and the C++ program:"
for example in ${successes[@]}; do
  echo $example
done
