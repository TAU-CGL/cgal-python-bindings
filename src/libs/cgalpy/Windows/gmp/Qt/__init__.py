import os
#os.add_dll_directory("@Boost_LIBRARY_DIRS@")
os.add_dll_directory("@GMP_BINARY_DIR@")
os.add_dll_directory("@MPFR_BINARY_DIR@")
os.add_dll_directory("@Qt6_BINARY_DIR@")
from .@CGALPY_LIB_NAME@ import *
