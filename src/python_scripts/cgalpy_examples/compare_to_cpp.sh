# save failed commands in a table and print them at the end
# failed_commands=()
# success_commands=()
no_cpp=()
compile_error=()
python_error=()
cpp_error=()
different_output=()
successes=()

if [ -z "$1" ]; then
  echo "Usage: $0 <cgal_path>"
  exit 1
fi

cgal_path=$1
# check if the path exists
if [ ! -d "$cgal_path" ]; then
  echo "Path $cgal_path does not exist"
  ehco "Usage: $0 <cgal_path>"
  exit 1
fi

for file in $(find . -name "*.py"); do
  echo ""
  echo "-------------------------------------------------"
  echo "Processing $file"
  echo "-------------------------------------------------"
  echo ""
  directory=$(dirname $file)
  script=$(basename $file)
  # raw_name is the name without the extension
  raw_name=$(basename $file .py)
  cpp_file=$(find $cgal_path -name "$raw_name.cpp")
  if [ -z "$cpp_file" ]; then
    echo "No C++ file found for $file"
    # add the command to the failed commands
    no_cpp+=($file)
    continue
  fi
  cpp_full_path=$(realpath $cpp_file)
  # include boost, mpfr and gmp
  if ! res=$(g++ -lgmp -lmpfr -lboost_thread -lboost_system -I $cgal_path/include -o $raw_name $cpp_full_path); then
    msg="Error compiling $cpp_file"
    echo ""
    echo $msg
    compile_error+=($file)
    continue
  fi
  if ! output=$(python3 $file); then
    # echo "Error executing $script"
    # failed_commands+=($file)
    # continue
    msg="Error executing $script"
    echo ""
    echo $msg
    python_error+=($file)
    continue
  fi
  if ! cpp_output=$(./$raw_name); then
    msg="Error executing $cpp_file"
    echo ""
    echo $msg
    cpp_error+=($file)
    continue
  fi
  rm $raw_name # clean up
  # trim the outputs by piping them to xargs
  output=$(echo $output | xargs)
  cpp_output=$(echo $cpp_output | xargs)
  if [ "$output" != "$cpp_output" ]; then
    echo ""
    echo "-------------------------------------------------"
    echo "Outputs of $script and $cpp_file differ:"
    echo ""
    # print the output of cpp, python and the difference
    echo "-------------------------------------------------"
    echo ""
    echo "Python output:"
    echo $output
    echo "C++ output:"
    echo $cpp_output
    echo "Difference:"
    diff <(echo $output) <(echo $cpp_output)
    echo ""
    echo "-------------------------------------------------"
    different_output+=($file)
  else
    echo "Outputs of $script and $(basename $cpp_file) are the same"
    successes+=($file)
  fi
done

echo ""
echo "-------------------------------------------------"
echo "Summary"
echo "-------------------------------------------------"
echo ""
# check if length of failed commands is 0
if [ ${#no_cpp[@]} -eq 0 ]; then
  echo "No C++ file found for the following Python scripts:"
  for file in ${no_cpp[@]}; do
    echo $file
  done
  echo ""
else
  echo "All Python scripts have corresponding C++ files"
fi

echo ""

if [ ${#compile_error[@]} -eq 0 ]; then
  echo "All C++ files compiled successfully"
else
  echo "Failed to compile the following C++ programs:"
  for file in ${compile_error[@]}; do
    echo $file
  done
fi

echo ""

if [ ${#python_error[@]} -eq 0 ]; then
  echo "All Python scripts executed successfully"
else
  echo "Failed to execute the following Python scripts:"
  for file in ${python_error[@]}; do
    echo $file
  done
fi

echo ""

if [ ${#cpp_error[@]} -eq 0 ]; then
  echo "All C++ files executed successfully"
else
  echo "Failed to execute the following C++ files:"
  for file in ${cpp_error[@]}; do
    echo $file
  done
fi

echo ""

if [ ${#different_output[@]} -eq 0 ]; then
  echo "All outputs are the same"
else
  echo "Outputs of the following scripts differ:"
  for file in ${different_output[@]}; do
    echo $file
  done
fi

echo ""

if [ ${#successes[@]} -eq 0 ]; then
  echo "No scripts executed successfully"
else
  echo "Outputs of the following C++ programs and Python scripts are the same:"
  for file in ${successes[@]}; do
    echo $file
  done
fi

# echo "Failed to compile the following C++ programs:"
# for file in ${compile_error[@]}; do
#   echo $file
# done
# echo ""
# echo "Failed to execute the following Python scripts:"
# for file in ${python_error[@]}; do
#   echo $file
# done
# echo ""
# echo "Failed to execute the following C++ files:"
# for file in ${cpp_error[@]}; do
#   echo $file
# done
# echo ""
# echo "Outputs of the following scripts differ:"
# for file in ${different_output[@]}; do
#   echo $file
# done
# echo ""
# echo "Outputs of the following C++ programs and Python scripts are the same:"
# for file in ${successes[@]}; do
#   echo $file
# done
#
