# cmake -C ../../cmake/tests/release/aos2_epec_fixed_release.cmake ../../
# now execute the command for all files in ../../cmake/tests/release

for file in ../../cmake/tests/release/*.cmake; do
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

