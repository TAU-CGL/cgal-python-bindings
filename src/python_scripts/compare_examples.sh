#!/bin/bash
# This script compares the output of all examples passed as arguments to the output of C++ programs
# Usage: ./compare_examples.sh <cgal_path> <example1> <example2> ...
# cgal_path is the path to the root of the CGAL C++ library
# example1, example2, ... are the paths to the Python examples

# check if the user is in a virtual environment, this is useful to me
if [ -z "$VIRTUAL_ENV" ]; then
  echo "Please activate a virtual environment before running this script"
  exit 1
fi

not_found=()
no_cpp=()
compile_error=()
python_error=()
cpp_error=()
different_output=()
successes=()

if [ -z "$1" ]; then
  echo "Usage: $0 <cgal_path> <example1> <example2> ..."
  exit 1
fi
cgal_path=$(realpath $1)
echo "CGAL_DIR: $cgal_path"
# check if the path exists
if [ ! -d "$cgal_path" ]; then
  echo "Path $cgal_path does not exist"
  echo "Usage: $0 <cgal_path> <example1> <example2> ..."
  exit 1
fi

if [ "$#" -lt 2 ]; then
  echo "Usage: $0 <cgal_path> <example1> <example2> ..."
  exit 1
fi

# save the current directory
current_dir=$(pwd)

for file in "${@:2}"; do
  echo ""
  echo "-------------------------------------------------"
  echo "Processing $file"
  echo "-------------------------------------------------"
  echo ""
  # directory=$(dirname $file)
  # script=$(basename $file)
  # look for the file recursively in the current directory
  real_file=$(find . -name $file)
  if [ -z "$real_file" ]; then
    echo "Example $file was not found in the current directory"
    not_found+=($file)
    continue
  fi
  directory=$(dirname $real_file)
  raw_name=$(basename $file .py)
  cpp_file=$(find $cgal_path -name "$raw_name.cpp")
  if [ -z "$cpp_file" ]; then
    echo "No C++ file found for $file"
    no_cpp+=($file)
    continue
  fi
  cd $directory # enter the directory to
  if ! output=$(python3 $raw_name.py); then
    msg="Error executing $file"
    echo ""
    echo $msg
    python_error+=($file)
    cd $current_dir
    continue
  fi
  cpp_full_path=$(realpath $cpp_file)
  if ! res=$(g++ -lgmp -lmpfr -lboost_thread -lboost_system -I $cgal_path/include -o $raw_name $cpp_full_path); then
    msg="Error compiling $cpp_file"
    echo ""
    echo $msg
    compile_error+=($file)
    cd $current_dir
    continue
  fi
  if ! cpp_output=$(./$raw_name); then
    msg="Error executing $raw_name"
    echo ""
    echo $msg
    cpp_error+=($file)
    cd $current_dir
    continue
  fi
  if [ "$output" != "$cpp_output" ]; then
    msg="Different output for $file:"

    echo "Python output:"
    echo $output
    echo "C++ output:"
    echo $cpp_output

    echo ""
    echo $msg
    different_output+=($file)
    cd $current_dir
    continue
  fi
  successes+=($file)
  cd $current_dir
done

echo ""
echo "-------------------------------------------------"
echo "Summary"
echo "-------------------------------------------------"
echo ""
echo "Examples not found:"
for file in "${not_found[@]}"; do
  echo $file
done
echo ""
echo "No C++ file found for:"
for file in "${no_cpp[@]}"; do
  echo $file
done
echo ""
echo "Error executing Python script for:"
for file in "${python_error[@]}"; do
  echo $file
done
echo ""
echo "Error compiling C++ file for:"
for file in "${compile_error[@]}"; do
  echo $file
done
echo ""
echo "Error executing C++ file for:"
for file in "${cpp_error[@]}"; do
  echo $file
done
echo ""
echo "Different output for:"
for file in "${different_output[@]}"; do
  echo $file
done
echo ""
echo "Successes:"
for file in "${successes[@]}"; do
  echo $file
done
echo ""
echo "-------------------------------------------------"
echo ""

