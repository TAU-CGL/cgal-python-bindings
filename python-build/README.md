# CGALPY to *.whl

This is the minimal setup needed to compile CGALPY to a *.whl file.

Requirements:

* setuptools
* wheel
* build

Instructions:

1. cd python-build
2. python -m build
3. The whl file will be in dist/CGALPY-1.0-py3-none-any.whl
4. pip install CGALPY-1.0-py3-none-any.whl

See setup.py to see how this works, any included files in the package are added 
inside `package_data`. Currently this setup is extremely minimal.
Annotations work with installed *.whl! :)

Note that much more work is needed in order to make this robust (for example including
any other *.dylib which are platform specific, like gmp and boost-python).
Also the package data contents should probably be dynamic.