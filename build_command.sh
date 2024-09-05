CIBW_BEFORE_BUILD="pip install conan && conan profile detect && conan install . --build missing -s compiler.cppstd=gnu17 -s compiler.version=14.2" CIBW_BUILD="cp311-* cp310-* cp312-*" cibuildwheel
