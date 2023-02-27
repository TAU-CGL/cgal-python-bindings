from distutils.core import setup
import os
import sys
import glob

SEARCH_DIR = '.'

files = glob.glob('./*.so') + glob.glob('./*.pyi') + glob.glob('./*/*.pyi')
package_data = {'': files}
print(package_data)

setup(name='CGALPY',
      version='1.0',
      py_modules=['CGALPY'],
      packages=[''],
      package_data=package_data
)
