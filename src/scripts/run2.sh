# cmake -C ../../cmake/tests/release/aos2_epec_fixed_release.cmake ../../
# now execute the command for all files in ../../cmake/tests/release

# cmakes_path=../../cmake/tests/release/*.cmake
cmakes_path=${1:-../../cmake/tests/release/*.cmake}

echo "Using cmake files in $cmakes_path"

# check if the user is in a venv
if [ -z "$VIRTUAL_ENV" ]; then
  echo "Please activate the virtual environment"
  exit 1
fi

# to be sure
python3 -m pip install build

bad_cmake_files=()
bad_make_files=()

for file in $cmakes_path; do
  # make a directory for each file and enter it
  dir=$(basename $file .cmake)
  mkdir -p $dir
  cd $dir
  # run the command
  # cmake -C ../$file ../../../
  if ! cmake -C ../$file ../../../; then
    bad_cmake_files+=($file)
    cd ..
    continue
  fi


  # if make fails add to the list of bad cmake files
  if ! make; then
    bad_make_files+=($file)
  fi
  # install eg. pip install src/libs/cgalpy/dist/CGALPY_kerEpecInt_aos2AlgPl-1.0.0-py3-none-any.whl
  pip install --force-reinstall src/libs/cgalpy/dist/*.whl
  # leave the directory
  cd ..
done


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

