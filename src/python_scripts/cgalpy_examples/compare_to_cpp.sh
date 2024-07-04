# save failed commands in a table and print them at the end
failed_commands=()
success_commands=()

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
    failed_commands+=($file)
    continue
  fi
  cpp_full_path=$(realpath $cpp_file)
  # include boost, mpfr and gmp
  if ! res=$(g++ -lgmp -lmpfr -lboost_thread -lboost_system -I $cgal_path/include -o $raw_name $cpp_full_path); then
    msg="Error compiling $cpp_file"
    echo ""
    echo $msg
    failed_commands+=($file)
    continue
  fi
  if ! output=$(python3 $file); then
    # echo "Error executing $script"
    # failed_commands+=($file)
    # continue
    msg="Error executing $script"
    echo ""
    echo $msg
    failed_commands+=($file)
    continue
  fi
  if ! cpp_output=$(./$raw_name); then
    msg="Error executing $cpp_file"
    echo ""
    echo $msg
    failed_commands+=($file)
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
    failed_commands+=($file)
  else
    echo "Outputs of $script and $(basename $cpp_file) are the same"
    success_commands+=($file)
  fi
done

echo "Successfull scripts:"
for command in ${success_commands[@]}; do
  echo $command
done

echo "Failed scripts:"
for command in ${failed_commands[@]}; do
  echo $command
done


