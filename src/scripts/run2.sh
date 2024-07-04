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


for file in $cmakes_path; do
  # make a directory for each file and enter it
  dir=$(basename $file .cmake)
  mkdir -p $dir
  cd $dir
  # run the command
  cmake -C ../$file ../../../
  make
  # install eg. pip install src/libs/cgalpy/dist/CGALPY_kerEpecInt_aos2AlgPl-1.0.0-py3-none-any.whl
  pip install src/libs/cgalpy/dist/*.whl
  # leave the directory
  cd ..
done

