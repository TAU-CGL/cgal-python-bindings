# # cmake -C ../../cmake/tests/release/aos2_epec_fixed_release.cmake ../../
# # now execute the command for all files in ../../cmake/tests/release
#
# # cmakes_path=../../cmake/tests/release/*.cmake
# cmakes_path=${1:-../../cmake/tests/release/*.cmake}
#
# echo "Using cmake files in $cmakes_path"
#
# # check if the user is in a venv
# if [ -z "$VIRTUAL_ENV" ]; then
#   echo "Please activate the virtual environment"
#   exit 1
# fi
#
# # to be sure
# python3 -m pip install build
#
# bad_cmake_files=()
# bad_make_files=()
#
# for file in $cmakes_path; do
#   # make a directory for each file and enter it
#   dir=$(basename $file .cmake)
#   mkdir -p $dir
#   cd $dir
#   # run the command
#   # cmake -C ../$file ../../../
#   if ! cmake -C ../$file ../../../; then
#     bad_cmake_files+=($file)
#     cd ..
#     continue
#   fi
#
#
#   # if make fails add to the list of bad cmake files
#   if ! make; then
#     bad_make_files+=($file)
#   fi
#   # install eg. pip install src/libs/cgalpy/dist/CGALPY_kerEpecInt_aos2AlgPl-1.0.0-py3-none-any.whl
#   pip install src/libs/cgalpy/dist/*.whl
#   # leave the directory
#   cd ..
# done


# This script lets the user choose a cmake file, compile it and install the resulting wheel file.

# The script takes an optional argument, which is the path to the cmake files. If no argument is given, the default path is used.

cmakes_path=${1:-../../cmake/tests/release/*.cmake}

if [ -z "$VIRTUAL_ENV" ]; then
  echo "Please activate the virtual environment"
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

