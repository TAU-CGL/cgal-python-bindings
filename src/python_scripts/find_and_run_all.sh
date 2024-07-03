# execute all python scripts in all directories recursively and each time echo: "Executing {script_name} in {directory_name}"

# save failed commands in a table and print them at the end
failed_commands=()
success_commands=()

for file in $(find . -name "*.py"); do
  directory=$(dirname $file)
  script=$(basename $file)
  echo "Executing $script in $directory"
  # python3 $file
  if ! python3 $file; then
    failed_commands+=("$file")
  else
    success_commands+=("$file")
  fi
done

echo "Successfull examples:"
for success_command in "${success_commands[@]}"; do
  echo $success_command
done

echo "Failed examples:"
for failed_command in "${failed_commands[@]}"; do
  echo $failed_command
done

